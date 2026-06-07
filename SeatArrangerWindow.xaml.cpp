#include "pch.h"
#include "SeatArrangerWindow.xaml.h"
#if __has_include("SeatArrangerWindow.g.cpp")
#include "SeatArrangerWindow.g.cpp"
#endif
#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <algorithm>
#include <random>
#include "NamelistHelper.h"
#include "SeatArrangerErrorPrompt.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Windowing;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Xaml::Input;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Microsoft::UI;


// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LuckyStudentPicker::implementation
{
    int32_t SeatArrangerWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void SeatArrangerWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    //windowStartup: Initialize seat arranger window
    void SeatArrangerWindow::windowStartup() {
        this->ExtendsContentIntoTitleBar(true);
        this->SetTitleBar(SeatArrangerWindowTitleBar());
        this->AppWindow().SetIcon(L"Assets/program.ico");
        this->AppWindow().TitleBar().PreferredHeightOption(winrt::Microsoft::UI::Windowing::TitleBarHeightOption::Tall);

        // namelist should be read from a file
        //m_studentNames = NamelistHelper::Instance().GetNamelist();
    }

    int seatCount = 0;
    //InitializeSeatGrid: Initialize seat grid (written mostly by Kimi)
    void SeatArrangerWindow::InitializeSeatGrid()
    {
        SeatGrid().Children().Clear();
        m_seats.clear();

        int seatIndex = 0;
        for (int row = 0; row < m_seatLayout.size(); row++) {
            for (int col = 0; col < m_seatLayout[row].size(); col++) {
                bool isEmpty = !m_seatLayout[row][col];
                auto seatControl = CreateSeatControl(row, col, isEmpty, L"");

                // Grid placement
                Grid::SetRow(seatControl, row);
                Grid::SetColumn(seatControl, col);
                SeatGrid().Children().Append(seatControl);

                if (!isEmpty) {
                    m_seats.push_back({ seatControl, row, col, L"", false });
                    seatCount++;
                }
                else {
                    m_seats.push_back({ seatControl, row, col, L"", true });
                }

                seatIndex++;
            }
        }
    }

    //Grid_Loaded: Execute InitializeSeatGrid() when SeatGrid() is ready to avoid crash
    void winrt::LuckyStudentPicker::implementation::SeatArrangerWindow::Grid_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (SeatGrid()) {
            InitializeSeatGrid();
        }
    }

    //CreateSeatControl: Create a single seat
    winrt::Microsoft::UI::Xaml::Controls::Grid SeatArrangerWindow::CreateSeatControl(int row, int col, bool isEmpty, hstring sname)
    {
        //Basic settings
        auto seat = Grid();
        seat.Width(150);
        seat.Height(100);
        seat.Margin(ThicknessHelper::FromLengths(12, 12, 12, 12));
        seat.CornerRadius(Microsoft::UI::Xaml::CornerRadiusHelper::FromUniformRadius(12.0));
        seat.Padding(ThicknessHelper::FromLengths(8, 8, 8, 8));
        seat.BorderThickness(ThicknessHelper::FromUniformLength(4.0));

        if (isEmpty) {

            //Settings specified for empty seats

            auto emptyText = TextBlock();
            emptyText.Text(L"Empty");
            emptyText.FontSize(16);
            emptyText.FontWeight(winrt::Microsoft::UI::Text::FontWeights::SemiBold());
            emptyText.Foreground(SolidColorBrush(ColorHelper::FromArgb(255, 160, 174, 192)));
            emptyText.HorizontalAlignment(HorizontalAlignment::Center);
            emptyText.VerticalAlignment(VerticalAlignment::Center);
            emptyText.TextWrapping(TextWrapping::Wrap);
            seat.Children().Append(emptyText);
            auto brush = Application::Current().Resources().Lookup(box_value(L"AccentAAFillColorDisabledBrush")).as<Media::Brush>();
            seat.BorderBrush(brush);


        }
        else {

            //Settings for normal seats

            // Numbering
            auto seatNumber = TextBlock();
            seatNumber.Text(to_hstring(row + 1) + L"-" + to_hstring(col + 1));
            seatNumber.FontSize(11);
            seatNumber.HorizontalAlignment(HorizontalAlignment::Left);
            seatNumber.VerticalAlignment(VerticalAlignment::Top);
            seatNumber.Margin(ThicknessHelper::FromLengths(4, 2, 0, 0));

            // Student name
            auto studentName = TextBlock();
            studentName.Name(L"StudentNameText");
            studentName.Text(sname);
            studentName.FontSize(20);
            studentName.FontWeight(winrt::Microsoft::UI::Text::FontWeights::Bold());
            studentName.HorizontalAlignment(HorizontalAlignment::Center);
            studentName.VerticalAlignment(VerticalAlignment::Center);
            studentName.TextWrapping(TextWrapping::Wrap);
            studentName.TextAlignment(TextAlignment::Center);
            seat.Children().Append(seatNumber);
            seat.Children().Append(studentName);
            auto brush = Application::Current().Resources().Lookup(box_value(L"AccentAAFillColorDefaultBrush")).as<Media::Brush>();
            seat.BorderBrush(brush);

        }

        return seat;
    }

    // RandomizeButton_Click: Randomly arrange seats (written partly by Kimi)
    winrt::Windows::Foundation::IAsyncAction SeatArrangerWindow::RandomizeButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        //Handle Exception
        if (seatCount != NamelistHelper::Instance().GetNamelistLength()) {
            auto dialog = winrt::LuckyStudentPicker::SeatArrangerErrorPrompt();
            dialog.XamlRoot(this->Content().XamlRoot());
            co_await dialog.ShowAsync();
            co_return;
        }

        // Copy + sort randomly
        auto shuffledStudents = NamelistHelper::Instance().GetNamelist();
        std::shuffle(shuffledStudents.begin(), shuffledStudents.end(), std::mt19937(std::random_device{}()));

        // Assign students to seats
        int currentEmptyCount = 0;
        for (int i = 0; i < m_seats.size(); i++) {
            if (!(m_seats[i].isEmpty))
            {
                m_seats[i].studentName = shuffledStudents[i - currentEmptyCount];
                m_seats[i].seatElement = CreateSeatControl(m_seats[i].row, m_seats[i].col, false, m_seats[i].studentName);
            }
            else
            {
                m_seats[i].seatElement = CreateSeatControl(m_seats[i].row, m_seats[i].col, true, L"");
                currentEmptyCount++;
            }
            Grid::SetRow(m_seats[i].seatElement, m_seats[i].row);
            Grid::SetColumn(m_seats[i].seatElement, m_seats[i].col);
            SeatGrid().Children().SetAt(i, m_seats[i].seatElement); 
        }

    }

    /*
    IAsyncOperation<SoftwareBitmap> SeatArrangerWindow::CaptureWindowAsync(HWND targetHwnd) {
        // 1. 创建 GraphicsCaptureItem
        auto interop = get_activation_factory<GraphicsCaptureItem, IGraphicsCaptureItemInterop>();
        GraphicsCaptureItem item{ nullptr };
        winrt::check_hresult(interop->CreateForWindow(
            targetHwnd,
            winrt::guid_of<ABI::Windows::Graphics::Capture::IGraphicsCaptureItem>(),
            put_abi(item)
        ));

        // 2. 获取窗口尺寸
        RECT rect;
        GetWindowRect(targetHwnd, &rect);
        auto width = rect.right - rect.left;
        auto height = rect.bottom - rect.top;

        // 3. 创建 D3D 设备
        UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
        wil::com_ptr<ID3D11Device> d3dDevice;
        wil::com_ptr<ID3D11DeviceContext> context;
        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels, 1,
            D3D11_SDK_VERSION, &d3dDevice, nullptr, &context);

        // 4. 创建 WinRT Direct3D 设备
        wil::com_ptr<IDXGIDevice> dxgiDevice;
        d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
        com_ptr<::IInspectable> d3dInspectable;
        CreateDirect3D11DeviceFromDXGIDevice(dxgiDevice.get(), reinterpret_cast<::IInspectable**>(d3dInspectable.put()));
        auto device = d3dInspectable.as<IDirect3DDevice>();

        // 5. 创建帧池和会话
        auto framePool = Direct3D11CaptureFramePool::Create(
            device,
            DirectXPixelFormat::B8G8R8A8UIntNormalized,
            1,
            { width, height }
        );
        auto session = framePool.CreateCaptureSession(item);
        session.IsCursorCaptureEnabled(false);

        // 6. 等待捕获帧（使用 event 同步）
        struct CaptureResult {
            SoftwareBitmap bitmap{ nullptr };
            winrt::event_token token;
            event closed;
        };
        auto result = std::make_shared<CaptureResult>();

        result->token = framePool.FrameArrived([result, d3dDevice, width, height](auto& sender, auto&) {
            auto frame = sender.TryGetNextFrame();
            if (!frame) return;

            // 获取纹理
            auto surface = frame.Surface();
            auto access = surface.as<::Windows::Graphics::DirectX::Direct3D11::IDirect3DDxgiInterfaceAccess>();
            wil::com_ptr<ID3D11Texture2D> sourceTexture;
            access->GetInterface(IID_PPV_ARGS(&sourceTexture));

            // 创建 CPU 可读纹理
            D3D11_TEXTURE2D_DESC desc = {};
            desc.Width = width;
            desc.Height = height;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.Usage = D3D11_USAGE_STAGING;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

            wil::com_ptr<ID3D11Texture2D> cpuTexture;
            d3dDevice->CreateTexture2D(&desc, nullptr, &cpuTexture);

            // 复制并映射
            wil::com_ptr<ID3D11DeviceContext> ctx;
            d3dDevice->GetImmediateContext(&ctx);
            ctx->CopyResource(cpuTexture.get(), sourceTexture.get());

            D3D11_MAPPED_SUBRESOURCE mapped = {};
            ctx->Map(cpuTexture.get(), 0, D3D11_MAP_READ, 0, &mapped);

            // 创建 SoftwareBitmap
            result->bitmap = SoftwareBitmap::CreateCopyFromBuffer(
                array_view<uint8_t>(static_cast<uint8_t*>(mapped.pData), height * mapped.RowPitch),
                BitmapPixelFormat::Bgra8,
                width,
                height,
                BitmapAlphaMode::Premultiplied
            );

            ctx->Unmap(cpuTexture.get(), 0);
            result->closed.set();
            });

        // 7. 开始捕获并等待
        session.StartCapture();
        co_await result->closed;

        // 清理
        framePool.FrameArrived(result->token);
        session.Close();
        framePool.Close();

        co_return result->bitmap;
    }

    HWND SeatArrangerWindow::GetWindowHandle() {
        auto windowNative = this->try_as<::IWindowNative>();
        HWND hWnd{ nullptr };
        if (windowNative) windowNative->get_WindowHandle(&hWnd);
        return hWnd;
    }

    */
    // CaptureButtonClick: (a function to be realized in the near future) Captures the window and save the image
    void winrt::LuckyStudentPicker::implementation::SeatArrangerWindow::CaptureButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        return;
        /*HWND hwnd = GetWindowHandle();
        auto bitmap = co_await CaptureWindowAsync(hwnd);

        auto file = co_await StorageFile::GetFileFromPathAsync(L"D:\\");
        IRandomAccessStream stream = co_await file.OpenAsync(FileAccessMode::ReadWrite);

        auto encoder = co_await BitmapEncoder::CreateAsync(BitmapEncoder::PngEncoderId(), stream);
        encoder.SetSoftwareBitmap(bitmap);
        co_await encoder.FlushAsync();*/
    }

    // InformationButton_Click: Opens or closes the information panel
    void winrt::LuckyStudentPicker::implementation::SeatArrangerWindow::InformationButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        auto status = SeatArrangerSplitView().IsPaneOpen();
        if (status == true) SeatArrangerSplitView().IsPaneOpen(false);
        else SeatArrangerSplitView().IsPaneOpen(true);
    }
}

    // main code ends here

    // To-do: chaos (swap two students' seats when dragged)
    /*
    void SeatArrangerWindow::Seat_DragStarting(IInspectable const& sender, DragStartingEventArgs const& args)
    {
        m_draggedSeat = sender.as<Grid>();
        args.Data().SetText(L"seat");
        args.DragUI().SetContentFromDataPackage();
    }


    void SeatArrangerWindow::Seat_Drop(IInspectable const& sender, DragEventArgs const& args)
    {
        if (m_draggedSeat && sender != m_draggedSeat) {
            // 查找源座位和目标座位的索引
            int sourceIndex = -1;
            int targetIndex = -1;

            for (int i = 0; i < m_seats.size(); i++) {
                if (m_seats[i].seatElement == m_draggedSeat) {
                    sourceIndex = i;
                }
                if (m_seats[i].seatElement == sender) {
                    targetIndex = i;
                }
            }

            if (sourceIndex >= 0 && targetIndex >= 0) {
                SwapSeats(sourceIndex, targetIndex);
            }
        }

        m_draggedSeat = nullptr;
    }

    void SeatArrangerWindow::Seat_DragOver(IInspectable const& sender, DragEventArgs const& args)
    {
        args.AcceptedOperation(DataPackageOperation::Move);
    }

    void SeatArrangerWindow::SwapSeats(int sourceIndex, int targetIndex)
    {
        if (sourceIndex < 0 || sourceIndex >= m_seats.size() ||
            targetIndex < 0 || targetIndex >= m_seats.size()) {
            return;
        }

        // 交换学生姓名
        hstring temp = m_seats[sourceIndex].studentName;
        m_seats[sourceIndex].studentName = m_seats[targetIndex].studentName;
        m_seats[targetIndex].studentName = temp;
        int tr = m_seats[sourceIndex].row, tc = m_seats[sourceIndex].col;
        m_seats[sourceIndex].row = m_seats[targetIndex].row, m_seats[sourceIndex].col = m_seats[targetIndex].col;
        m_seats[targetIndex].row = tr, m_seats[targetIndex].col = tc;

        // 更新显示
        //AnimateSeatChange(, m_seats[sourceIndex].studentName);
        //AnimateSeatChange(m_seats[targetIndex].seatElement, m_seats[targetIndex].studentName);

        /*                    int r = m_seats[i].row, c = m_seats[i].col;
                    m_seats[i].seatElement.Children().Clear();
                    m_seats[i].seatElement.Padding(ThicknessHelper::FromLengths(-12, -12, -12, -12));
                    m_seats[i].seatElement.Children().Append(CreateSeatControl(r, c, false, shuffledStudents[i]));

        .Children().Clear();
        m_seats[sourceIndex].seatElement.Padding(ThicknessHelper::FromLengths(-16, -16, -16, -16));
        m_seats[sourceIndex].seatElement.Children().Append(CreateSeatControl();
        m_seats[targetIndex].seatElement.Children().Clear();
        m_seats[targetIndex].seatElement.Padding(ThicknessHelper::FromLengths(-16, -16, -16, -16));
        m_seats[targetIndex].seatElement.Children().Append();

        m_seats[sourceIndex].seatElement = CreateSeatControl(m_seats[sourceIndex].row, m_seats[sourceIndex].col, false, m_seats[sourceIndex].studentName);
        m_seats[targetIndex].seatElement = CreateSeatControl(m_seats[targetIndex].row, m_seats[targetIndex].col, false, m_seats[targetIndex].studentName);
        Grid::SetRow(m_seats[sourceIndex].seatElement, m_seats[sourceIndex].row);
        Grid::SetColumn(m_seats[sourceIndex].seatElement, m_seats[sourceIndex].col);
        Grid::SetRow(m_seats[targetIndex].seatElement, m_seats[targetIndex].row);
        Grid::SetColumn(m_seats[targetIndex].seatElement, m_seats[targetIndex].col);
        SeatGrid().Children().SetAt((sourceIndex < 19 ? sourceIndex : sourceIndex + 1), m_seats[sourceIndex].seatElement);
        SeatGrid().Children().SetAt((targetIndex < 19 ? targetIndex : targetIndex + 1), m_seats[targetIndex].seatElement);
            }
        }
        */

    // Also to-dos :)
    /*
    void SeatArrangerWindow::seatPointerEntered(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
        auto grid = sender.as<Grid>();
        //grid.SetValue(Control::BackgroundProperty(), box_value(L"{ThemeResource CardBackgroundFillColorSecondaryBrush}"));
        auto brush = Application::Current().Resources().Lookup(box_value(L"CardBackgroundFillColorSecondaryBrush")).as<Media::Brush>();
        grid.Background(brush);

    }

    void SeatArrangerWindow::seatPointerExited(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
        auto grid = sender.as<Grid>();
        grid.Background(SolidColorBrush(ColorHelper::FromArgb(0, 7, 7, 7)));
    }
    */

    // To-do: animation
    //AnimateSeatChange: a weird animation (written by Kimi)
    /*
    void SeatArrangerWindow::AnimateSeatChange(Grid const& seat, hstring const& newName)
    {
        auto studentNameText = seat.FindName(L"StudentNameText").as<TextBlock>();
        if (studentNameText) {
            // 淡出动画
            auto fadeOut = Media::Animation::DoubleAnimation();
            fadeOut.To(0.0);
            //fadeOut.Duration(TimeSpan(std::chrono::milliseconds(200)));

            auto storyboard = Media::Animation::Storyboard();
            storyboard.Children().Append(fadeOut);
            Media::Animation::Storyboard::SetTarget(fadeOut, studentNameText);
            Media::Animation::Storyboard::SetTargetProperty(fadeOut, L"Opacity");

            storyboard.Completed([studentNameText, newName](IInspectable const&, IInspectable const&) {
                studentNameText.Text(newName);

                // 淡入动画
                auto fadeIn = Media::Animation::DoubleAnimation();
                fadeIn.To(1.0);
                //fadeIn.Duration(TimeSpan(std::chrono::milliseconds(200)));

                auto storyboard2 = Media::Animation::Storyboard();
                storyboard2.Children().Append(fadeIn);
                Media::Animation::Storyboard::SetTarget(fadeIn, studentNameText);
                Media::Animation::Storyboard::SetTargetProperty(fadeIn, L"Opacity");
                storyboard2.Begin();
            });

            storyboard.Begin();
        }
    }
    */
    // To-dos: in CreateSeatControl(), line 147
    /* 添加悬停效果
    /*seat.PointerEntered([](IInspectable const& sender, PointerRoutedEventArgs const& args) {
        auto grid = sender.as<Grid>();
        grid.SetValue(Control::BackgroundProperty(), box_value(L"{ThemeResource CardBackgroundFillColorSecondaryBrush}"));
        //grid.BorderBrush(SolidColorBrush(ColorHelper::FromArgb(255, 102, 126, 234)));
    });

    seat.PointerExited([](IInspectable const& sender, PointerRoutedEventArgs const& args) {
        auto grid = sender.as<Grid>();
        //grid.Background(SolidColorBrush(ColorHelper::FromArgb(255, 247, 250, 252)));
        //grid.BorderBrush(SolidColorBrush(ColorHelper::FromArgb(255, 226, 232, 240)));
        grid.Background(SolidColorBrush(ColorHelper::FromArgb(0, 7, 7, 7)));
    });

    seat.PointerEntered({ this, &SeatArrangerWindow::seatPointerEntered });
    seat.PointerExited({ this, &SeatArrangerWindow::seatPointerExited });

    // 添加拖拽事件
    seat.CanDrag(true);
    seat.DragStarting({ this, &SeatArrangerWindow::Seat_DragStarting });
    seat.Drop({ this, &SeatArrangerWindow::Seat_Drop });
    seat.DragOver({ this, &SeatArrangerWindow::Seat_DragOver });
    seat.AllowDrop(true);*/