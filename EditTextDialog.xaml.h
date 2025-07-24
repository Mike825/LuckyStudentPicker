//
// EditTextDialog.xaml.h
// EditTextDialog 类的声明
//

#pragma once

#include "EditTextDialog.g.h"

using namespace winrt::LuckyStudentPicker::implementation;

namespace winrt::LuckyStudentPicker::implementation
{
	struct EditTextDialog : EditTextDialogT<EditTextDialog> {
	public: 
		EditTextDialog();

		int32_t MyProperty();
		void MyProperty(int32_t value);

		void ContentDialog_PrimaryButtonClick(winrt::Microsoft::UI::Xaml::Controls::ContentDialog const&, winrt::Microsoft::UI::Xaml::Controls::ContentDialogButtonClickEventArgs const&);
		//void EditTextDialog::ContentDialog_SecondaryButtonClick(winrt::Microsoft::UI::Xaml::Controls::ContentDialog const&, winrt::Microsoft::UI::Xaml::Controls::ContentDialogButtonClickEventArgs const&);

		winrt::hstring InitialText();
		void InitialText(winrt::hstring const& value);
		winrt::hstring ResultText() const noexcept;
		
	private:
		winrt::hstring m_initialText{};
		winrt::hstring m_resultText{};
	};
}

namespace winrt::LuckyStudentPicker::factory_implementation
{
	struct EditTextDialog : EditTextDialogT<EditTextDialog, implementation::EditTextDialog>
	{
		//EditTextDialog();
	};
}


