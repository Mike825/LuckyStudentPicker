#pragma once

#include "SeatArrangerWindow.g.h"
#include <vector>
#include <string>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

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
        
        
        // Event handlers
        void RandomizeButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        
        winrt::Microsoft::UI::Xaml::Controls::Grid CreateSeatControl(int row, int col, bool isEmpty, hstring sname);
        
        //IAsyncOperation<SoftwareBitmap> SeatArrangerWindow::CaptureWindowAsync(HWND targetHwnd);

        //HWND SeatArrangerWindow::GetWindowHandle();

        /*void Seat_DragStarting(winrt::Windows::Foundation::IInspectable const& sender,
                               winrt::Microsoft::UI::Xaml::DragStartingEventArgs const& args);
        
        void Seat_Drop(winrt::Windows::Foundation::IInspectable const& sender,
                       winrt::Microsoft::UI::Xaml::DragEventArgs const& args);

        void Seat_DragOver(winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::DragEventArgs const& args);
                
        void seatPointerEntered(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&);
        void seatPointerExited(IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&);*/
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
        void RandomAssignSeats();
        
        /*void SwapSeats(int sourceIndex, int targetIndex);
        void AnimateSeatChange(winrt::Microsoft::UI::Xaml::Controls::Grid const& seat,
                               hstring const& newName);*/
    public:
        void Grid_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void CaptureButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void InformationButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
    };
}

namespace winrt::LuckyStudentPicker::factory_implementation
{
    struct SeatArrangerWindow : SeatArrangerWindowT<SeatArrangerWindow, implementation::SeatArrangerWindow>
    {
    };
}
