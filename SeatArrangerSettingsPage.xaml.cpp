#include "pch.h"
#include "SeatArrangerSettingsPage.xaml.h"
#if __has_include("SeatArrangerSettingsPage.g.cpp")
#include "SeatArrangerSettingsPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LuckyStudentPicker::implementation
{
    int32_t SeatArrangerSettingsPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void SeatArrangerSettingsPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}
