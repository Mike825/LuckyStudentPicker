#pragma once

#include "AboutPage.g.h"

namespace winrt::LuckyStudentPicker::implementation
{
    struct AboutPage : AboutPageT<AboutPage>
    {
        AboutPage()
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
    struct AboutPage : AboutPageT<AboutPage, implementation::AboutPage>
    {
    };
}
