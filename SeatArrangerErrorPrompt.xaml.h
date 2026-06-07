//
// SeatArrangerErrorPrompt.xaml.h
// SeatArrangerErrorPrompt 类的声明
//

#pragma once

#include "SeatArrangerErrorPrompt.g.h"

using namespace winrt::LuckyStudentPicker::implementation;

namespace winrt::LuckyStudentPicker::implementation
{
	struct SeatArrangerErrorPrompt : SeatArrangerErrorPromptT<SeatArrangerErrorPrompt> {
	public:
		SeatArrangerErrorPrompt();

		int32_t MyProperty();
		void MyProperty(int32_t);

		void SeatArrangerErrorPrompt_PrimaryButtonClick(winrt::Microsoft::UI::Xaml::Controls::ContentDialog const&, winrt::Microsoft::UI::Xaml::Controls::ContentDialogButtonClickEventArgs const&);
	};
}

namespace winrt::LuckyStudentPicker::factory_implementation
{
	struct SeatArrangerErrorPrompt : SeatArrangerErrorPromptT<SeatArrangerErrorPrompt, implementation::SeatArrangerErrorPrompt>
	{
	};

}
