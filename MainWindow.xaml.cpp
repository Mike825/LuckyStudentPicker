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
#include <vector>
#include <numeric>
#include <winrt/base.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Media.SpeechSynthesis.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include "EditTextDialog.xaml.h"
#include "nlohmann/json.hpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::Media::SpeechSynthesis;
using namespace Windows::Storage::Streams;
using namespace Windows::Media::Core;
using namespace Windows::Media::Playback;
using namespace Microsoft::UI::Dispatching;
using namespace Windows::UI::Xaml::Controls;
using json = nlohmann::json;

namespace nlohmann {
    template <>
    struct adl_serializer<winrt::hstring> {
        static winrt::hstring from_json(const json& j) {
            return winrt::to_hstring(j.get<std::string>());
        }
    };
}

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
    

    bool isNamelistWrong = false; 
    winrt::Windows::Storage::StorageFolder appDataFolder = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
    hstring folderName = L"LuckyStudentPicker";
    winrt::Windows::Storage::StorageFolder newFolder = appDataFolder.CreateFolderAsync(folderName, winrt::Windows::Storage::CreationCollisionOption::OpenIfExists).get();
    hstring fileName = L"\\UnluckyStudentLog.log";
    hstring fLocation = newFolder.Path() + fileName;
    std::ofstream log(winrt::to_string(fLocation).c_str(), std::ios::out | std::ios::app);
    hstring nameListFileName = L"\\UnluckyStudentsNameList.json";
    hstring nameListFileLocation = newFolder.Path() + nameListFileName;
    std::vector<hstring> load_hstring(hstring fpath) {
        try {
            std::ifstream f(to_string(fpath));
            json j;
            f >> j;
            return j.get<std::vector<winrt::hstring>>();
        }
        catch (...) {
            isNamelistWrong = true;
            std::vector<hstring> r = { L"" };
            return r;
        }
    }

    std::string getTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
        char dt[66];
        ctime_s(dt, sizeof(dt), &timeNow);
        std::string rtime = dt;
        rtime.erase(rtime.end() - 1);
        return rtime;
    }

    /* ====== Defining variables ======*/
    std::vector<hstring> orgNameList = load_hstring(nameListFileLocation);
    int int_count = 0, int_rnum = 0, int_rpCount = 0, int_rpCount_progressing = 0, int_rp_random_index = 0, arrlen = orgNameList.size();
    std::vector<hstring> newNameList, newNameList_rp;
    long long int_count_repeat = 0;

    static std::mt19937 gen(std::random_device{}());
    std::vector<int> rnumList, l1;

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
    int CurrentWindowMode = 1;

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

    void MainWindow::windowSizeChange(int mode) {
        getScreenResolution();
        switch (mode) {
        case (1): {//normal mode
            int W = ((int)pw / 4), H = (int)ph / 3;
            int windowWidth = (W >= 1080) ? 1080 : W, windowHeight = (W >= 720) ? 720 : H;
            int wCenter = (w - windowWidth) / 2, hCenter = (h - windowHeight) / 2;
            winrt::Windows::Graphics::RectInt32 windowPos{ wCenter, hCenter, windowWidth, windowHeight };
            this->AppWindow().MoveAndResize(windowPos);
            this->AppWindow().SetPresenter(Microsoft::UI::Windowing::AppWindowPresenterKind::Overlapped);
            if (CurrentWindowMode != 1) this->AppWindow().TitleBar().PreferredHeightOption(Microsoft::UI::Windowing::TitleBarHeightOption::Standard);
            CurrentWindowMode = 1;
            log << "Window width: " << w << "/" << pw << "/" << windowWidth << ", Window height: " << h << "/" << ph << "/" << windowHeight << ".\n";
            break;
        }
        case (2): {//small & topmost
            Microsoft::UI::Windowing::CompactOverlayPresenter COPer = Microsoft::UI::Windowing::CompactOverlayPresenter::Create();
            COPer.InitialSize(Microsoft::UI::Windowing::CompactOverlaySize::Medium);
            this->AppWindow().SetPresenter(COPer);
            Windows::Graphics::SizeInt32 wsize = this->AppWindow().Size();
            Windows::Graphics::PointInt32 wpos = { w - wsize.Width - 50, 50 };
            this->AppWindow().Move(wpos);
            CurrentWindowMode = 2;
            break;
        }
        }
    }

    void MainWindow::windowStartup() {
        windowSizeChange(1);
        this->ExtendsContentIntoTitleBar(true);
        this->SetTitleBar(AppTitleBar());
        this->AppWindow().SetIcon(L"Assets/program.ico");
        SpeakTextAsync(L"");
        std::string timeNow = getTime();
        log << "This message is from windowStartup(), current time: " << timeNow << '\n';
        log << "LuckyStudentPicker initialized successfully. \n\n";
    }

    /* ====== Controlling sound & repeatition ======*/

    bool sound = true, repeated = false;
    void MainWindow::IsSoundOn(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { sound = true; }
    void MainWindow::IsRepeated(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { repeated = true; NumLeft().Text(L" " + to_hstring(int_count_repeat)); }
    void MainWindow::IsSoundOff(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { sound = false; }
    void MainWindow::IsNotRepeated(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) { repeated = false; NumLeft().Text(L" " + to_hstring(arrlen - int_count)); }
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
    
    void getRandList(std::vector<int> & arr) {
        arr.resize(arrlen);
        std::iota(arr.begin(), arr.end(), 0);
        for (int i = static_cast<int>(arr.size()) - 1; i > 0; --i) {
            std::uniform_int_distribution<int> dist(0, i);
            std::swap(arr[i], arr[dist(gen)]);
        }
    }

    void initializeNameList() {
        getRandList(rnumList);
        getRandList(l1);
        newNameList.resize(arrlen);
        newNameList_rp.resize(arrlen);
        for (int i = 0; i < arrlen; i++) {
            if (!repeated) newNameList[i] = orgNameList[rnumList[i]];
            else newNameList_rp[i] = orgNameList[rnumList[i]];
        }
        //writing logs
        log << "This message is from initializeNameList(), current method: " << (repeated ? "Repeat" : "nonRepeat") << ".\n";
        log << "Generated namelist: ";
        for (int i = 0; i < arrlen; i++) {
            if (repeated) log << to_string(newNameList_rp[i]) << " ";
            else log << to_string(newNameList[i]) << " ";
        }
        log << "\nNamelist initialized successfully.\n\n";
    }

    hstring randName() {
        hstring hs;
        if (repeated == false) {
            if (int_count == 0) initializeNameList();
            if (int_count <= arrlen - 1) hs = newNameList[int_count], int_count++;
            else hs = L"点名结束";
        }
        else {
            if (int_rpCount == 0) {
                log << "This message is from randName(), with method Repeat.\n";
                int_rpCount = randNum(10, 18, 1);
                int_rp_random_index = randNum(0, arrlen - 1, 0); 
                log << "Reconstruct namelist after: " << int_rpCount << ", index offset: " << int_rp_random_index << ".\nNow calling initializeNameList()...\n";
                initializeNameList();
            }
            if (int_rpCount_progressing < int_rpCount) {
                hs = newNameList_rp[(int_rpCount_progressing + int_rp_random_index) % arrlen];
                int_rpCount_progressing++;
            }
            else {
                hs = newNameList_rp[int_rpCount_progressing % arrlen];
                int_rpCount = 0, int_rpCount_progressing = 0; 
            }
            int_count_repeat++;
        }
        return hs;
    }
    
    /* ====== Controlling UI elements ======*/

    void MainWindow::reset(IInspectable const&, RoutedEventArgs const&) {
        int_count = 0;
        if (repeated) int_count_repeat = 0;
        if (sound) SpeakTextAsync(L"重置");
        
        text().Text(L"");
        if (repeated) NumLeft().Text(L" " + to_hstring(int_count_repeat));
        else NumLeft().Text(L" " + to_hstring(arrlen - 1));
        buttonText().Text(L"  开始");
        buttonGlyph().Glyph(L"\uF5B0");

        log << "LuckyStudentPicker has been reset.\n";
    }

    void MainWindow::updRandName(IInspectable const& sender, RoutedEventArgs const& r) {
        if (isNamelistWrong) {
            DispatcherQueue::GetForCurrentThread().TryEnqueue([]() {
                auto namelistTroubleshootingWindow = winrt::LuckyStudentPicker::NamelistTroubleshootingWindow();
                namelistTroubleshootingWindow.Activate(); });
        }
        else {
            if (!repeated) {
                hstring rn = randName();
                if (rn != L"点名结束") {
                    if (buttonText().Text() == L"  继续" || buttonText().Text() == L"  开始") buttonText().Text(L"  下一个");
                    buttonGlyph().Glyph(L"\uE72A");
                    text().Text(rn);
                    text().TextAlignment(winrt::Microsoft::UI::Xaml::TextAlignment::Center);
                    NumLeft().Text(L" " + to_hstring(arrlen - int_count));
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
                if (buttonText().Text() == L"  继续" || buttonText().Text() == L"  开始") buttonText().Text(L"  下一个");
                buttonGlyph().Glyph(L"\uE72A");
                text().Text(randName());
                text().TextAlignment(winrt::Microsoft::UI::Xaml::TextAlignment::Center);
                NumLeft().Text(L" " + to_hstring(int_count_repeat));
            }
            if (sound) SpeakTextAsync(text().Text());
        }
    }
    
    winrt::Windows::Foundation::IAsyncAction MainWindow::EditClick(IInspectable const&, RoutedEventArgs const&) {
        auto dialog = winrt::LuckyStudentPicker::EditTextDialog();
        dialog.XamlRoot(this->Content().XamlRoot());
        dialog.InitialText(text().Text());
        auto result = co_await dialog.ShowAsync();
        if (result == Microsoft::UI::Xaml::Controls::ContentDialogResult::Primary) {
            hstring editedText = dialog.ResultText();
            text().Text(editedText);
            text().TextAlignment(winrt::Microsoft::UI::Xaml::TextAlignment::Center);
            if (sound) SpeakTextAsync(editedText);
            buttonText().Text(L"  继续");
            buttonGlyph().Glyph(L"\uF5B0");
            log << "Displayed text has been edited: " << to_string(editedText) << ".\n";
        }
    }

    void MainWindow::Window_Closed(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::WindowEventArgs const&) {
        log << "LuckyStudentPicker.MainWindow is closed, time: " << getTime() << ".\n\n";
    }

    void MainWindow::SettingsClick(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) {
        auto settingsWindow = winrt::LuckyStudentPicker::SettingsWindow();
        settingsWindow.Activate();
    }

    void MainWindow::WindowCompactOverlayMode(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) {
        if (CurrentWindowMode == 2) {
            windowSizeChange(1);
            CompactOverlay().Label(L"Compact Overlay Mode");
            CompactOverlayIcon().Glyph(L"\uEE49");
            text().FontSize(48);
            text().Margin(Microsoft::UI::Xaml::ThicknessHelper::FromLengths(0.0, 12.0, 0.0, 18.0));
            button().Width(230);
            button().Height(85);
            buttonText().FontSize(24);
            buttonGlyph().FontSize(20);
            log << "LuckyStudentPicker has changed to normal mode. \n";
        }
        else {
            windowSizeChange(2);
            CompactOverlay().Label(L"Normal Mode");
            CompactOverlayIcon().Glyph(L"\uEE47");
            text().FontSize(32);
            text().Margin(Microsoft::UI::Xaml::ThicknessHelper::FromLengths(0.0, 12.0, 0.0, 12.0));
            button().Width(200);
            button().Height(70);
            buttonText().FontSize(20);
            buttonGlyph().FontSize(18);
            log << "LuckyStudentPicker has changed to compact overlay mode. \n";
        }
    }

    void MainWindow::SeatArrangerClick(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) {
        auto seatArrangerWindow = winrt::LuckyStudentPicker::SeatArrangerWindow();
        seatArrangerWindow.Activate();

    }
    
    void winrt::LuckyStudentPicker::implementation::MainWindow::ReloadNamelist_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        isNamelistWrong = false;
        orgNameList = load_hstring(nameListFileLocation);
        reset(sender, e);
    }

}
