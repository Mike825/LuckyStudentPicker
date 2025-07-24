//
// EditTextDialog.xaml.cpp
// EditTextDialog 类的实现
//

#include "pch.h"
#include "EditTextDialog.xaml.h"
#include "EditTextDialog.g.cpp"
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

namespace winrt::LuckyStudentPicker::implementation
{
    EditTextDialog::EditTextDialog() {
    }

    int32_t EditTextDialog::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void EditTextDialog::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }


    void EditTextDialog::ContentDialog_PrimaryButtonClick(winrt::Microsoft::UI::Xaml::Controls::ContentDialog const&, winrt::Microsoft::UI::Xaml::Controls::ContentDialogButtonClickEventArgs const&) {
        m_resultText = EditBox().Text();
    }


    winrt::hstring EditTextDialog::InitialText() { return m_initialText; }
    void EditTextDialog::InitialText(winrt::hstring const& value) { m_initialText = value; }

    winrt::hstring EditTextDialog::ResultText() const noexcept
    {
        return m_resultText;
    }
    
    /*void EditTextDialog::ContentDialog_SecondaryButtonClick(winrt::Microsoft::UI::Xaml::Controls::ContentDialog const&, winrt::Microsoft::UI::Xaml::Controls::ContentDialogButtonClickEventArgs const&) {

    }*/
}

