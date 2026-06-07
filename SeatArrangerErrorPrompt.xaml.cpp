//
// SeatArrangerErrorPrompt.xaml.cpp
// SeatArrangerErrorPrompt 类的实现
//

#include "pch.h"
#include "SeatArrangerErrorPrompt.xaml.h"
#include "SeatArrangerErrorPrompt.g.cpp"
#include <winrt/base.h>
#pragma comment(lib, "windowsapp")

using namespace winrt;
using namespace winrt::LuckyStudentPicker;
using namespace winrt::LuckyStudentPicker::implementation;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Controls::Primitives;
using namespace winrt::Microsoft::UI::Xaml::Data;
using namespace winrt::Microsoft::UI::Xaml::Input;
using namespace winrt::Microsoft::UI::Xaml::Media;
using namespace winrt::Microsoft::UI::Xaml::Navigation;

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“内容对话框”项模板

/*LuckyStudentPicker::SeatArrangerErrorPrompt::SeatArrangerErrorPrompt()
{
	InitializeComponent();
}

void LuckyStudentPicker::SeatArrangerErrorPrompt::ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}

void LuckyStudentPicker::SeatArrangerErrorPrompt::ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}
*/
namespace winrt::LuckyStudentPicker::implementation
{
	SeatArrangerErrorPrompt::SeatArrangerErrorPrompt() {

	}

	int32_t SeatArrangerErrorPrompt::MyProperty()
	{
		throw hresult_not_implemented();
	}

	void SeatArrangerErrorPrompt::MyProperty(int32_t)
	{
		throw hresult_not_implemented();
	}

	void SeatArrangerErrorPrompt::SeatArrangerErrorPrompt_PrimaryButtonClick(winrt::Microsoft::UI::Xaml::Controls::ContentDialog const&, winrt::Microsoft::UI::Xaml::Controls::ContentDialogButtonClickEventArgs const&) {
		return;
	}

}
