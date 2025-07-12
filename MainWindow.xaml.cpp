#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#pragma comment(lib, "shell32.lib")
#include <string>
#include <Windows.h>
#include <random>
#include <ctime>
#include <fstream>
#include <chrono>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Media.SpeechSynthesis.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::Media::SpeechSynthesis;
using namespace Windows::Storage::Streams;
using namespace Windows::Media::Core;
using namespace Windows::Media::Playback;
using namespace Microsoft::UI::Dispatching;
using namespace Windows::UI::Xaml::Controls;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LuckyStudentPicker::implementation
{
    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
    
    /* ====== Writing Logs & get time ======*/
    
    winrt::Windows::Storage::StorageFolder appDataFolder = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
    hstring folderName = L"LuckyStudentPicker";
    winrt::Windows::Storage::StorageFolder newFolder = appDataFolder.CreateFolderAsync(folderName, winrt::Windows::Storage::CreationCollisionOption::OpenIfExists).get();
    hstring fileName = L"\\UnluckyStudentLog.log";
    hstring fLocation = newFolder.Path() + fileName;
    std::ofstream log(winrt::to_string(fLocation).c_str(), std::ios::out | std::ios::app);
    std::time_t getTime() {
        auto now = std::chrono::system_clock::now();
        return std::chrono::system_clock::to_time_t(now);
    }

    /* ====== Synthesizing sounds ======*/

    SpeechSynthesizer synthesizer{ nullptr };
    MediaPlayer mediaPlayer{ nullptr };

    winrt::Windows::Foundation::IAsyncAction MainWindow::SpeakTextAsync(hstring text) {
        synthesizer = SpeechSynthesizer();
        if (!synthesizer) {
            OutputDebugString(L"synthesizer is null!");
            co_return;
        }
        try {
            SpeechSynthesisStream stream = co_await synthesizer.SynthesizeTextToStreamAsync(text);
            mediaPlayer = MediaPlayer();
            mediaPlayer.AutoPlay(true);
            mediaPlayer.Source(MediaSource::CreateFromStream(stream, stream.ContentType()));
            mediaPlayer.Play();

        }
        catch (const hresult_error& ex) {
            OutputDebugString(ex.message().c_str());
        }
    }

    /* ====== Automatically change window size & position ======*/

    HWND MainWindow::GetWindowHandle() {
        auto windowNative = this->try_as<::IWindowNative>();
        HWND hWnd{ nullptr };
        if (windowNative) windowNative->get_WindowHandle(&hWnd);
        return hWnd;
    }

    int32_t w = 1920, h = 1080, pw = 1920, ph = 1080;

    void MainWindow::getScreenResolution() {
        HWND hWnd = GetWindowHandle();
        winrt::Microsoft::UI::WindowId windowId = winrt::Microsoft::UI::GetWindowIdFromWindow(hWnd);
        winrt::Microsoft::UI::Windowing::DisplayAreaFallback dpfb{};
        winrt::Microsoft::UI::Windowing::DisplayArea displayArea = winrt::Microsoft::UI::Windowing::DisplayArea::GetFromWindowId(windowId, dpfb);
        int32_t logicalWidth = displayArea.WorkArea().Width;
        int32_t logicalHeight = displayArea.WorkArea().Height;
        double dpi = GetDpiForWindow(hWnd) / 96.0;
        int32_t physicalWidth = static_cast<int32_t>(logicalWidth * dpi);
        int32_t physicalHeight = static_cast<int32_t>(logicalHeight * dpi);
        w = logicalWidth;
        h = logicalHeight;
        pw = physicalWidth;
        ph = physicalHeight;
    }

    void MainWindow::windowStartup() {
        getScreenResolution();
        int windowWidth = (int)pw / 4, windowHeight = (int)ph / 3;
        int wCenter = (w - windowWidth) / 2, hCenter = (h - windowHeight) / 2;
        winrt::Windows::Graphics::RectInt32 windowPos{ wCenter, hCenter, windowWidth, windowHeight };
        this->AppWindow().MoveAndResize(windowPos);
        this->ExtendsContentIntoTitleBar(true);
        this->SetTitleBar(AppTitleBar());
        SpeakTextAsync(L"");
        std::time_t now = getTime();
        char dt[66];
        ctime_s(dt, sizeof(dt), &now);
        log << "This message is from windowStartup(), current time: " << dt;
        log << "Window width: " << w << "/" << pw << "/" << windowWidth << ", Window height: " << h << "/" << ph << "/" << windowHeight << ".\n";
        log << "LuckyStudentPicker initialized successfully. \n\n";
    }

    /* ====== Controlling sound & repeatition ======*/

    bool sound = true, repeated = false;
    void MainWindow::IsSoundOn(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { sound = true; }
    void MainWindow::IsRepeated(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { repeated = true; }
    void MainWindow::IsSoundOff(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { sound = false; }
    void MainWindow::IsNotRepeated(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { repeated = false; }
    void MainWindow::LockDownText(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { button().IsEnabled(false); }
    void MainWindow::UnlockText(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { button().IsEnabled(true); }
    void MainWindow::Showlogs(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) {
        log.close();
        ShellExecuteA(nullptr, "open", winrt::to_string(fLocation).c_str(), 0, nullptr, SW_SHOWNORMAL);
        log.open(winrt::to_string(fLocation).c_str(), std::ios::out | std::ios::app);
    }
    void MainWindow::Clearlogs(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) {
        log.close();
        log.open(winrt::to_string(fLocation).c_str(), std::ios::out);
    }

    /* ====== Generating randomized names ======*/

    hstring orgNameList[22] = { L"昝禹丞", L"雷坚", L"张艺馨", L"马璇", L"刘海延", L"郑梓萱", L"赵轩毅", L"刘忠灏", L"赵崇旭", L"郭劭晗", L"李岳", L"马浩凯", L"王舒瑶", L"王杨冰清", L"韩雨贝", L"李佳阳", L"刘雨卓", L"郭超", L"林诗睿", L"何禛", L"孟奕州", L"李昀檀" };
    int int_count = 0, int_rnum = 0, int_rpCount = 0, int_rpCount_progressing = 0, int_rp_random_index = 0;
    hstring newNameList[22] = { L"" }, newNameList_rp[22] = { L"" };

    static std::mt19937 gen(std::random_device{}());
    int rnumList[22] = { 0 }, l1[22] = { 0 };

    int randNum(int start, int end, bool trueRand) {
        switch (trueRand) {
            case (0): {
                std::uniform_int_distribution<int> u(start, end);
                return u(gen);
            }
            case (1): {
                std::shuffle_order_engine<std::mt19937, 8> rd(std::random_device{}());
                std::uniform_int_distribution<int> dis(start, end);
                return dis(rd);
            }
        }
        return 0;
    }
    
    void getRandList(int* arr, int size) {
        for (int i = 0; i < size; ++i) {
            arr[i] = i;
        }
        for (int i = size - 1; i > 0; --i) {
            std::uniform_int_distribution<int> dist(0, i);
            int j = dist(gen);
            std::swap(arr[i], arr[j]);
        }
    }

    void initializeNameList() {
        getRandList(rnumList, 22);
        getRandList(l1, 22);
        for (int i = 0; i < 22; i++) {
            if (!repeated) newNameList[i] = orgNameList[rnumList[i]];
            else newNameList_rp[i] = orgNameList[rnumList[i]];
        }
        //writing logs
        log << "This message is from initializeNameList(), current method: " << (repeated ? "Repeat" : "nonRepeat") << ".\n";
        log << "Generated namelist: ";
        for (int i = 0; i <= 21; i++) {
            if (repeated) log << to_string(newNameList_rp[i]) << " ";
            else log << to_string(newNameList[i]) << " ";
        }
        log << "\nNamelist initialized successfully.\n\n";
    }

    hstring randName() {
        hstring hs;
        if (repeated == false) {
            if (int_count == 0) initializeNameList();
            if (int_count <= 21) hs = newNameList[int_count], int_count++;
            else hs = L"点名结束";
        }
        else {
            if (int_rpCount == 0) {
                log << "This message is from randName(), with method Repeat.\n";
                int_rpCount = randNum(10, 18, 1);
                int_rp_random_index = randNum(0, 21, 0); 
                log << "Reconstruct namelist after: " << int_rpCount << ", index offset: " << int_rp_random_index << ".\nNow calling initializeNameList()...\n";
                initializeNameList();
            }
            if (int_rpCount_progressing < int_rpCount) {
                hs = newNameList_rp[(int_rpCount_progressing + int_rp_random_index) % 22];
                int_rpCount_progressing++;
            }
            else {
                hs = newNameList_rp[int_rpCount_progressing];
                int_rpCount = 0, int_rpCount_progressing = 0; 
            }
        }
        return hs;
    }
    
    /* ====== Controlling UI elements ======*/

    void MainWindow::reset(IInspectable const&, RoutedEventArgs const&) {
        int_count = 0;
        if (sound) SpeakTextAsync(L"重置");
        text().Text(L"");
        NumLeft().Text(L" " + to_hstring(21));
        buttonText().Text(L"  开始");
        buttonGlyph().Glyph(L"\uF5B0");
        log << "LuckyStudentPicker has been reset.\n";
    }

    void MainWindow::updRandName(IInspectable const& sender, RoutedEventArgs const& r) {
        if (!repeated) {
            hstring rn = randName();
            if (rn != L"点名结束") {
                if (buttonText().Text() == L"  继续" || buttonText().Text() == L"  开始") buttonText().Text(L"  下一个");
                text().Text(rn);
                text().TextAlignment(winrt::Microsoft::UI::Xaml::TextAlignment::Center);
                NumLeft().Text(L" " + to_hstring(22 - int_count));
            }
            else {
                if (buttonText().Text() == L"  下一个") {
                    text().Text(L"点名结束");
                    text().TextAlignment(winrt::Microsoft::UI::Xaml::TextAlignment::Center);
                    buttonText().Text(L"  重置");
                    buttonGlyph().Glyph(L"\uE72c");
                }
                else reset(sender, r);
            }
        }
        else {
            if (buttonText().Text() == L"  继续" || buttonText().Text() ==L"  开始") buttonText().Text(L"  下一个");
            text().Text(randName());
            text().TextAlignment(winrt::Microsoft::UI::Xaml::TextAlignment::Center);
        }
        if (sound) SpeakTextAsync(text().Text());
    }
    
    void MainWindow::editTextApply(IInspectable const&, RoutedEventArgs const&) {
        text().Text(editText().Text());
        text().TextAlignment(winrt::Microsoft::UI::Xaml::TextAlignment::Center);
        if (sound) SpeakTextAsync(editText().Text());
        buttonText().Text(L"  继续");
        log << "Displayed text has been edited: " << to_string(editText().Text()) << ".\n";
    }
}