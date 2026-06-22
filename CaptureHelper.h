#pragma once

#include "pch.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.UI.Dispatching.h>

// Helper class that captures a window using the Windows GraphicsCapture API
// and saves the result as a PNG image.
class CaptureHelper
{
public:
    static CaptureHelper& Instance();

    // Captures the specified window and writes it as a PNG to outputFile.
    // The dispatcherQueue is used to marshal back to the UI thread when
    // writing the image file (StorageFile has thread affinity).
    // Returns true if the image was captured and saved successfully.
    winrt::Windows::Foundation::IAsyncOperation<bool> CaptureWindowToFileAsync(
        HWND targetHwnd,
        winrt::Windows::Storage::StorageFile const& outputFile,
        winrt::Microsoft::UI::Dispatching::DispatcherQueue const& dispatcherQueue);

private:
    CaptureHelper() = default;
    ~CaptureHelper() = default;
    CaptureHelper(const CaptureHelper&) = delete;
    CaptureHelper& operator=(const CaptureHelper&) = delete;
};
