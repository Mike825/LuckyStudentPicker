#pragma once

#include "SeatArrangerWindow.g.h"

namespace winrt::LuckyStudentPicker::implementation
{
    struct SeatArrangerWindow : SeatArrangerWindowT<SeatArrangerWindow>
    {
        SeatArrangerWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
            windowStartup();
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void windowStartup();
        void SeatArrangerNV_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args);
        void SeatArrangerNV_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::LuckyStudentPicker::factory_implementation
{
    struct SeatArrangerWindow : SeatArrangerWindowT<SeatArrangerWindow, implementation::SeatArrangerWindow>
    {
    };
}
