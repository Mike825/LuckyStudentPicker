#include "pch.h"
#include "NamelistTroubleshootingWindow.xaml.h"
#if __has_include("NamelistTroubleshootingWindow.g.cpp")
#include "NamelistTroubleshootingWindow.g.cpp"
#endif
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <fstream>
#include "nlohmann/json.hpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Windowing;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace winrt::Windows::ApplicationModel::DataTransfer;
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
    int32_t NamelistTroubleshootingWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void NamelistTroubleshootingWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void NamelistTroubleshootingWindow::windowStartup() {
        this->ExtendsContentIntoTitleBar(true);
        this->SetTitleBar(NamelistTroubleshootingWindowTitleBar());
        this->AppWindow().SetIcon(L"Assets/program.ico");
        this->AppWindow().TitleBar().PreferredHeightOption(winrt::Microsoft::UI::Windowing::TitleBarHeightOption::Tall);
    }
}

fire_and_forget winrt::LuckyStudentPicker::implementation::NamelistTroubleshootingWindow::OpenDataFolderButton_Click(winrt::Windows::Foundation::IInspectable const& , winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
    winrt::Windows::Storage::StorageFolder a = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
    hstring fn = L"LuckyStudentPicker";
    winrt::Windows::Storage::StorageFolder df = co_await a.CreateFolderAsync(fn, winrt::Windows::Storage::CreationCollisionOption::OpenIfExists);
    ShellExecuteA(nullptr, "open", to_string(df.Path()).c_str(), 0, nullptr, SW_SHOWNORMAL);
}

fire_and_forget winrt::LuckyStudentPicker::implementation::NamelistTroubleshootingWindow::EditNamelistButton_Click(winrt::Windows::Foundation::IInspectable const& , winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
    winrt::Windows::Storage::StorageFolder a = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
    hstring fn = L"LuckyStudentPicker";
    winrt::Windows::Storage::StorageFolder df = co_await a.CreateFolderAsync(fn, winrt::Windows::Storage::CreationCollisionOption::OpenIfExists);
    winrt::hstring namelist = df.Path() + L"\\UnluckyStudentsNameList.json";
    ShellExecuteA(nullptr, "open", to_string(namelist).c_str(), 0, nullptr, SW_SHOWNORMAL);
}

void winrt::LuckyStudentPicker::implementation::NamelistTroubleshootingWindow::CopyTemplateButton_Click(winrt::Windows::Foundation::IInspectable const& , winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
    auto dp = DataPackage();
    dp.SetText(L"[\"A\", \"B\", \"C\"]");
    winrt::Windows::ApplicationModel::DataTransfer::Clipboard::SetContent(dp);
    winrt::Windows::ApplicationModel::DataTransfer::Clipboard::Flush();
    CopyGlyph().Glyph(L"\uE8FB");
}

fire_and_forget winrt::LuckyStudentPicker::implementation::NamelistTroubleshootingWindow::TestNamelistButton_Click(winrt::Windows::Foundation::IInspectable const& , winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
    bool success = true;
    try {
        winrt::Windows::Storage::StorageFolder a = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
        hstring fn = L"LuckyStudentPicker";
        winrt::Windows::Storage::StorageFolder df = co_await a.CreateFolderAsync(fn, winrt::Windows::Storage::CreationCollisionOption::OpenIfExists);
        winrt::hstring namelist = df.Path() + L"\\UnluckyStudentsNameList.json";
        std::ifstream f(to_string(namelist));
        json j;
        f >> j;
        j.get<std::vector<winrt::hstring>>();
    }
    catch (...) {
        success = false;
    }
    if (success) {
        TestGlyph().Glyph(L"\uE8FB");
        auto successBrush = Application::Current().Resources().Lookup(box_value(L"SystemFillColorSuccessBackgroundBrush")).as<Media::Brush>();
        CloseButton().Background(successBrush);
        CloseButton().IsEnabled(true);
    }
    else {
        TestGlyph().Glyph(L"\uE711");
        auto failBrush = Application::Current().Resources().Lookup(box_value(L"SystemFillColorCriticalBackgroundBrush")).as<Media::Brush>();
        CloseButton().Background(failBrush);
    }
}

void winrt::LuckyStudentPicker::implementation::NamelistTroubleshootingWindow::Close_Click(winrt::Windows::Foundation::IInspectable const& , winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
    this->AppWindow().Destroy();
}
