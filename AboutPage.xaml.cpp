#include "pch.h"
#include "AboutPage.xaml.h"
#if __has_include("AboutPage.g.cpp")
#include "AboutPage.g.cpp"
#endif
#include <winrt/Windows.Storage.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LuckyStudentPicker::implementation
{
    int32_t AboutPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void AboutPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }



    fire_and_forget AboutPage::OpenDataFolder(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) {
        winrt::Windows::Storage::StorageFolder a = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
        hstring fn = L"LuckyStudentPicker";
        winrt::Windows::Storage::StorageFolder df = co_await a.CreateFolderAsync(fn, winrt::Windows::Storage::CreationCollisionOption::OpenIfExists);
        ShellExecuteA(nullptr, "open", to_string(df.Path()).c_str(), 0, nullptr, SW_SHOWNORMAL);
    }

    fire_and_forget AboutPage::EditNamelist(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&) {
        winrt::Windows::Storage::StorageFolder a = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
        hstring fn = L"LuckyStudentPicker";
        winrt::Windows::Storage::StorageFolder df = co_await a.CreateFolderAsync(fn, winrt::Windows::Storage::CreationCollisionOption::OpenIfExists);
        winrt::hstring namelist = df.Path() + L"\\UnluckyStudentsNameList.json";
        ShellExecuteA(nullptr, "open", to_string(namelist).c_str(), 0, nullptr, SW_SHOWNORMAL);
    }
}
