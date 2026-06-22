#pragma once

#include "SeatArrangerPage.g.h"
#include <vector>
#include <string>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

namespace winrt::LuckyStudentPicker::implementation
{
    struct SeatArrangerPage : SeatArrangerPageT<SeatArrangerPage>
    {
        SeatArrangerPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
    
    public: 
        // Event handlers
        void OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);
        winrt::Windows::Foundation::IAsyncAction RandomizeButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        winrt::Microsoft::UI::Xaml::Controls::Grid CreateSeatControl(int row, int col, bool isEmpty, hstring sname);
        winrt::Windows::Foundation::IAsyncAction OpenCaptureFolderButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void Grid_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        winrt::Windows::Foundation::IAsyncAction CaptureButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        // Data structures
        struct SeatInfo
        {
            winrt::Microsoft::UI::Xaml::Controls::Grid seatElement{ nullptr };
            int row{ 0 };
            int col{ 0 };
            hstring studentName;
            bool isEmpty{ false };
        };

        std::vector<hstring> m_studentNames;
        std::vector<SeatInfo> m_seats;
        winrt::Microsoft::UI::Xaml::Controls::Grid m_draggedSeat{ nullptr };
        
        // Layout configuration
        const std::vector<std::vector<bool>> m_seatLayout = {
            {true, true, true, true},   
            {true, true, true, true},   
            {true, true, true, true},   
            {true, true, true, true},   
            {true, true, true, true},   
            {false, false, true, false}  
        };

        // Helper methods
        void InitializeSeatGrid();
        static HWND s_hostHwnd;
    };
}

namespace winrt::LuckyStudentPicker::factory_implementation
{
    struct SeatArrangerPage : SeatArrangerPageT<SeatArrangerPage, implementation::SeatArrangerPage>
    {
    };
}
