#include "pch.h"
#include "SeatArrangerWindow.xaml.h"
#if __has_include("SeatArrangerWindow.g.cpp")
#include "SeatArrangerWindow.g.cpp"
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
    int32_t SeatArrangerWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void SeatArrangerWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    HWND hWnd = nullptr;

    void SeatArrangerWindow::windowStartup() {
        this->ExtendsContentIntoTitleBar(true);
        this->SetTitleBar(SeatArrangerWindowTitleBar());
        this->AppWindow().SetIcon(L"Assets/program.ico");
        this->AppWindow().TitleBar().PreferredHeightOption(winrt::Microsoft::UI::Windowing::TitleBarHeightOption::Tall);
        auto windowNative = this->try_as<::IWindowNative>();
        if (windowNative) windowNative->get_WindowHandle(&hWnd);
    }

    void SeatArrangerWindow::SeatArrangerNV_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
    {
        if (auto item = args.InvokedItemContainer())
        {
            auto tag = item.Tag().try_as<hstring>();
            if (tag) {
                if (*tag == L"SeatArrangerPage") {
                    SeatArrangerContentFrame().Navigate(xaml_typename<SeatArrangerPage>(), box_value(reinterpret_cast<int64_t>(hWnd)));
                }
                else if (*tag == L"SeatArrangerInformationPage") {
                    SeatArrangerContentFrame().Navigate(xaml_typename<SeatArrangerInformationPage>());
                }
                else if (*tag == L"SeatArrangerSettingsPage") {
                    SeatArrangerContentFrame().Navigate(xaml_typename<SeatArrangerSettingsPage>());
                }
            }
        }
    }

    void SeatArrangerWindow::SeatArrangerNV_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        SeatArrangerNV().SelectedItem(SeatArrangerNV().MenuItems().GetAt(0));
        SeatArrangerContentFrame().Navigate(xaml_typename<SeatArrangerPage>(), box_value(reinterpret_cast<int64_t>(hWnd)));
    }
}
