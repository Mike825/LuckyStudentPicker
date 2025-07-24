//
// EditTextContentDialog.xaml.cpp
// EditTextContentDialog 类的实现
//

#include "pch.h"
#include "EditTextContentDialog.xaml.h"

using namespace LuckyStudentPicker;
/*
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;*/

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“内容对话框”项模板

void LuckyStudentPicker::EditTextContentDialog::EditTextContentDialog()
{
	InitializeComponent();
}

void LuckyStudentPicker::EditTextContentDialog::ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}

void LuckyStudentPicker::EditTextContentDialog::ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}
