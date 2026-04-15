#pragma once

#include "NamelistTroubleshootingWindow.g.h"
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

namespace winrt::LuckyStudentPicker::implementation
{
    struct NamelistTroubleshootingWindow : NamelistTroubleshootingWindowT<NamelistTroubleshootingWindow>
    {
        NamelistTroubleshootingWindow()
        {
            windowStartup();
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void windowStartup();
        fire_and_forget OpenDataFolderButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& );
        fire_and_forget EditNamelistButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& );
        void CopyTemplateButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& );
        fire_and_forget TestNamelistButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& );
        void Close_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& );
    };
}

namespace winrt::LuckyStudentPicker::factory_implementation
{
    struct NamelistTroubleshootingWindow : NamelistTroubleshootingWindowT<NamelistTroubleshootingWindow, implementation::NamelistTroubleshootingWindow>
    {
    };
}
