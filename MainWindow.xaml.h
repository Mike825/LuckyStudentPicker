#pragma once

#include "MainWindow.g.h"
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

namespace winrt::LuckyStudentPicker::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

            winrt::Windows::Graphics::RectInt32 windowPos{ 100, 100, 300, 200 };
            this->AppWindow().MoveAndResize(windowPos);
            this->ExtendsContentIntoTitleBar(true);
            this->SetTitleBar(AppTitleBar());
            //button().Click([this](IInspectable const& a, RoutedEventArgs const& b) { updRandName(a, b); });
            //winrt::Microsoft::UI::Xaml::Application::Current().RequestedTheme(winrt::Microsoft::UI::Xaml::ApplicationTheme::Dark);
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        //RoutedEventHandler reset(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        //RoutedEventHandler updRandName(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void reset(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void updRandName(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        winrt::Windows::Foundation::IAsyncAction SpeakTextAsync(hstring text);
        //winrt::Windows::UI::Xaml::Controls::MediaElement media;
    };
}

namespace winrt::LuckyStudentPicker::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
