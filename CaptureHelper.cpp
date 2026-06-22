#include "pch.h"
#include "CaptureHelper.h"

#include <d3d11.h>
#include <dxgi.h>
#include <Windows.Graphics.Capture.Interop.h>
#include <Windows.Graphics.DirectX.Direct3D11.Interop.h>

#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>

#include <wil/resource.h>
#include <winrt/Windows.UI.Core.h>

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Graphics::Capture;
using namespace winrt::Windows::Graphics::DirectX;
using namespace winrt::Windows::Graphics::DirectX::Direct3D11;
using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;

CaptureHelper& CaptureHelper::Instance()
{
    static CaptureHelper instance;
    return instance;
}

IAsyncOperation<bool> CaptureHelper::CaptureWindowToFileAsync(
    HWND targetHwnd,
    StorageFile const& outputFile,
    winrt::Microsoft::UI::Dispatching::DispatcherQueue const& dispatcherQueue)
{
    if (!targetHwnd || !IsWindow(targetHwnd) || !outputFile)
    {
        co_return false;
    }

    // Window rectangle in physical pixels.
    RECT windowRect{};
    GetWindowRect(targetHwnd, &windowRect);
    const int32_t width = windowRect.right - windowRect.left;
    const int32_t height = windowRect.bottom - windowRect.top;
    if (width <= 0 || height <= 0)
    {
        co_return false;
    }

    try
    {
        // Create a GraphicsCaptureItem for the target HWND.
        auto captureFactory = get_activation_factory<GraphicsCaptureItem, IGraphicsCaptureItemInterop>();
        GraphicsCaptureItem captureItem{ nullptr };
        check_hresult(captureFactory->CreateForWindow(
            targetHwnd,
            winrt::guid_of<GraphicsCaptureItem>(),
            put_abi(captureItem)));

        // Create a D3D11 device that can be used with the capture API.
        UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        winrt::com_ptr<ID3D11Device> d3dDevice;
        winrt::com_ptr<ID3D11DeviceContext> d3dContext;
        check_hresult(D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            d3dDevice.put(),
            nullptr,
            d3dContext.put()));

        // Wrap the D3D device for WinRT consumption.
        winrt::com_ptr<IDXGIDevice> dxgiDevice;
        check_hresult(d3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgiDevice.put())));
        com_ptr<::IInspectable> d3dInspectable;
        check_hresult(CreateDirect3D11DeviceFromDXGIDevice(dxgiDevice.get(), d3dInspectable.put()));
        IDirect3DDevice device = d3dInspectable.as<IDirect3DDevice>();

        // Frame pool that receives captured frames.
        auto framePool = Direct3D11CaptureFramePool::Create(
            device,
            DirectXPixelFormat::B8G8R8A8UIntNormalized,
            1,
            winrt::Windows::Graphics::SizeInt32{ width, height });

        auto session = framePool.CreateCaptureSession(captureItem);
        session.IsCursorCaptureEnabled(false);

        // Shared state used by the frame-arrived callback.
        struct FrameContext
        {
            SoftwareBitmap bitmap{ nullptr };
            winrt::event_token token{};
        };
        auto frameContext = std::make_shared<FrameContext>();
        wil::unique_event frameEvent{ wil::EventOptions::None };

        frameContext->token = framePool.FrameArrived(
            [frameContext, d3dDevice, width, height, &frameEvent](auto& sender, auto&)
            {
                auto frame = sender.TryGetNextFrame();
                if (!frame)
                {
                    frameEvent.SetEvent();
                    return;
                }

                auto surface = frame.Surface();
                auto access = surface.as<::Windows::Graphics::DirectX::Direct3D11::IDirect3DDxgiInterfaceAccess>();
                winrt::com_ptr<ID3D11Texture2D> sourceTexture;
                check_hresult(access->GetInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(sourceTexture.put())));

                // Create a staging texture that the CPU can read.
                D3D11_TEXTURE2D_DESC desc{};
                desc.Width = static_cast<UINT>(width);
                desc.Height = static_cast<UINT>(height);
                desc.MipLevels = 1;
                desc.ArraySize = 1;
                desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
                desc.SampleDesc.Count = 1;
                desc.Usage = D3D11_USAGE_STAGING;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

                winrt::com_ptr<ID3D11Texture2D> cpuTexture;
                check_hresult(d3dDevice->CreateTexture2D(&desc, nullptr, cpuTexture.put()));

                winrt::com_ptr<ID3D11DeviceContext> ctx;
                d3dDevice->GetImmediateContext(ctx.put());
                ctx->CopyResource(cpuTexture.get(), sourceTexture.get());

                D3D11_MAPPED_SUBRESOURCE mapped{};
                check_hresult(ctx->Map(cpuTexture.get(), 0, D3D11_MAP_READ, 0, &mapped));

                UINT dataSize = static_cast<UINT>(height) * mapped.RowPitch;

                Buffer buffer(dataSize);
                buffer.Length(dataSize);

                memcpy(buffer.data(), mapped.pData, dataSize);
                ctx->Unmap(cpuTexture.get(), 0);

                frameContext->bitmap = SoftwareBitmap::CreateCopyFromBuffer(
                    buffer,
                    BitmapPixelFormat::Bgra8,
                    width,
                    height,
                    BitmapAlphaMode::Premultiplied);

                
                frameEvent.SetEvent();
            });

        session.StartCapture();
        co_await resume_on_signal(frameEvent.get());

        // Stop capturing and detach the event handler.
        framePool.FrameArrived(frameContext->token);
        session.Close();
        framePool.Close();

        if (!frameContext->bitmap)
        {
            co_return false;
        }

        // Marshal back to the UI thread because StorageFile has thread affinity.
        co_await wil::resume_foreground(dispatcherQueue);

        // Encode and save the captured bitmap as a PNG file.
        auto stream = co_await outputFile.OpenAsync(FileAccessMode::ReadWrite);
        auto encoder = co_await BitmapEncoder::CreateAsync(BitmapEncoder::PngEncoderId(), stream);
        encoder.SetSoftwareBitmap(frameContext->bitmap);
        co_await encoder.FlushAsync();

        co_return true;
    }
    catch (...)
    {
        co_return false;
    }
}
