#pragma once

#include "SettingsHomePage.g.h"

namespace winrt::LuckyStudentPicker::implementation
{
    struct SettingsHomePage : SettingsHomePageT<SettingsHomePage>
    {
        SettingsHomePage()
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
    struct SettingsHomePage : SettingsHomePageT<SettingsHomePage, implementation::SettingsHomePage>
    {
    };
}
