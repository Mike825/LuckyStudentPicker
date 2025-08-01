#include "pch.h"
#include "SettingsWindow.xaml.h"
#if __has_include("SettingsWindow.g.cpp")
#include "SettingsWindow.g.cpp"
#endif
#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Windowing;
using namespace Microsoft::UI::Xaml::Controls;


// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LuckyStudentPicker::implementation
{
    int32_t SettingsWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }
    
    void SettingsWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    HWND SettingsWindow::GetWindowHandle() {
        auto windowNative = this->try_as<::IWindowNative>();
        HWND hWnd{ nullptr };
        if (windowNative) windowNative->get_WindowHandle(&hWnd);
        return hWnd;
    }

    void SettingsWindow::windowStartup() {
        this->ExtendsContentIntoTitleBar(true);
        this->SetTitleBar(SettingsTitleBar());
        this->AppWindow().TitleBar().PreferredHeightOption(winrt::Microsoft::UI::Windowing::TitleBarHeightOption::Tall);
    }

    void winrt::LuckyStudentPicker::implementation::SettingsWindow::SettingsNV_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args) {
        if (auto item = args.InvokedItemContainer())
        {
            auto tag = item.Tag().try_as<hstring>();
            if (tag) {
                if (*tag == L"HomePage") {
                    ContentFrame().Navigate(xaml_typename<SettingsHomePage>());
                }
                if (*tag == L"AboutPage") {
                    ContentFrame().Navigate(xaml_typename<AboutPage>());
                }
            }
        }
    }

    void winrt::LuckyStudentPicker::implementation::SettingsWindow::SettingsNV_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e) {
        ContentFrame().Navigate(xaml_typename<SettingsHomePage>());
    }


}


