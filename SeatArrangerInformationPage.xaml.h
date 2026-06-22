#pragma once

#include "SeatArrangerInformationPage.g.h"

namespace winrt::LuckyStudentPicker::implementation
{
    struct SeatArrangerInformationPage : SeatArrangerInformationPageT<SeatArrangerInformationPage>
    {
        SeatArrangerInformationPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::LuckyStudentPicker::factory_implementation
{
    struct SeatArrangerInformationPage : SeatArrangerInformationPageT<SeatArrangerInformationPage, implementation::SeatArrangerInformationPage>
    {
    };
}
