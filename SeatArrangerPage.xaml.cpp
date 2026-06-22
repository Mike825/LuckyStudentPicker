#include "pch.h"
#include "SeatArrangerPage.xaml.h"
#if __has_include("SeatArrangerPage.g.cpp")
#include "SeatArrangerPage.g.cpp"
#endif

#pragma comment(lib, "shell32.lib")

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Windows.Storage.h>

#include <algorithm>
#include <random>
#include <sstream>
#include <chrono>
#include <iomanip>

#include "NamelistHelper.h"
#include "SeatArrangerErrorPrompt.xaml.h"
#include "CaptureHelper.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Windowing;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Xaml::Input;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Microsoft::UI;
using namespace Windows::Storage;


// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LuckyStudentPicker::implementation
{

    int32_t SeatArrangerPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void SeatArrangerPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    // InitializeSeatGrid: Initialize seat grid (written mostly by Kimi)
    int seatCount = 0;
    void SeatArrangerPage::InitializeSeatGrid()
    {
        SeatGrid().Children().Clear();
        m_seats.clear();

        seatCount = 0;
        int seatIndex = 0;
        for (int row = 0; row < m_seatLayout.size(); row++) {
            for (int col = 0; col < m_seatLayout[row].size(); col++) {
                bool isEmpty = !m_seatLayout[row][col];
                auto seatControl = CreateSeatControl(row, col, isEmpty, L"");

                // Grid placement
                Grid::SetRow(seatControl, row);
                Grid::SetColumn(seatControl, col);
                SeatGrid().Children().Append(seatControl);

                if (!isEmpty) {
                    m_seats.push_back({ seatControl, row, col, L"", false });
                    seatCount++;
                }
                else {
                    m_seats.push_back({ seatControl, row, col, L"", true });
                }

                seatIndex++;
            }
        }
    }

    // Grid_Loaded: Execute InitializeSeatGrid() when SeatGrid() is ready to avoid crash
    std::vector<hstring> studentsSeatsCurrent = { };
    void winrt::LuckyStudentPicker::implementation::SeatArrangerPage::Grid_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (SeatGrid()) 
        {
            InitializeSeatGrid();
        }
    }

    // OnNavigatedTo: get SeatArrangerWindow's HWND when the page is loaded
    HWND SeatArrangerPage::s_hostHwnd = nullptr;
    void winrt::LuckyStudentPicker::implementation::SeatArrangerPage::OnNavigatedTo(winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        auto hwnd = reinterpret_cast<HWND>(unbox_value<int64_t>(e.Parameter()));
        s_hostHwnd = hwnd;
        this->NavigationCacheMode(winrt::Microsoft::UI::Xaml::Navigation::NavigationCacheMode::Enabled);
    }

    //CreateSeatControl: Create a single seat
    winrt::Microsoft::UI::Xaml::Controls::Grid SeatArrangerPage::CreateSeatControl(int row, int col, bool isEmpty, hstring sname)
    {
        //Basic settings
        auto seat = Grid();
        seat.Width(150);
        seat.Height(100);
        seat.Margin(ThicknessHelper::FromLengths(12, 12, 12, 12));
        seat.CornerRadius(Microsoft::UI::Xaml::CornerRadiusHelper::FromUniformRadius(12.0));
        seat.Padding(ThicknessHelper::FromLengths(8, 8, 8, 8));
        seat.BorderThickness(ThicknessHelper::FromUniformLength(4.0));

        if (isEmpty) {

            //Settings specified for empty seats

            auto emptyText = TextBlock();
            emptyText.Text(L"Empty");
            emptyText.FontSize(16);
            emptyText.FontWeight(winrt::Microsoft::UI::Text::FontWeights::SemiBold());
            emptyText.Foreground(SolidColorBrush(ColorHelper::FromArgb(255, 160, 174, 192)));
            emptyText.HorizontalAlignment(HorizontalAlignment::Center);
            emptyText.VerticalAlignment(VerticalAlignment::Center);
            emptyText.TextWrapping(TextWrapping::Wrap);
            seat.Children().Append(emptyText);
            auto brush = Application::Current().Resources().Lookup(box_value(L"AccentAAFillColorDisabledBrush")).as<Media::Brush>();
            seat.BorderBrush(brush);


        }
        else {

            //Settings for normal seats

            // Numbering
            auto seatNumber = TextBlock();
            seatNumber.Text(to_hstring(row + 1) + L"-" + to_hstring(col + 1));
            seatNumber.FontSize(11);
            seatNumber.HorizontalAlignment(HorizontalAlignment::Left);
            seatNumber.VerticalAlignment(VerticalAlignment::Top);
            seatNumber.Margin(ThicknessHelper::FromLengths(4, 2, 0, 0));

            // Student name
            auto studentName = TextBlock();
            studentName.Name(L"StudentNameText");
            studentName.Text(sname);
            studentName.FontSize(20);
            studentName.FontWeight(winrt::Microsoft::UI::Text::FontWeights::Bold());
            studentName.HorizontalAlignment(HorizontalAlignment::Center);
            studentName.VerticalAlignment(VerticalAlignment::Center);
            studentName.TextWrapping(TextWrapping::Wrap);
            studentName.TextAlignment(TextAlignment::Center);
            seat.Children().Append(seatNumber);
            seat.Children().Append(studentName);
            auto brush = Application::Current().Resources().Lookup(box_value(L"AccentAAFillColorDefaultBrush")).as<Media::Brush>();
            seat.BorderBrush(brush);

        }

        return seat;
    }

    // RandomizeButton_Click: Randomly arrange seats (written partly by Kimi)
    winrt::Windows::Foundation::IAsyncAction SeatArrangerPage::RandomizeButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        // Handle Exception (When the number of students doesn't match the number of seats)
        if (seatCount != NamelistHelper::Instance().GetNamelistLength()) {
            auto dialog = winrt::LuckyStudentPicker::SeatArrangerErrorPrompt();
            dialog.XamlRoot(this->Content().XamlRoot());
            co_await dialog.ShowAsync();
            co_return;
        }

        // Sort students randomly
        auto shuffledStudents = NamelistHelper::Instance().GetNamelist();
        std::shuffle(shuffledStudents.begin(), shuffledStudents.end(), std::mt19937(std::random_device{}()));

        // Assign students to seats
        int currentEmptyCount = 0;
        for (int i = 0; i < m_seats.size(); i++) {
            if (!(m_seats[i].isEmpty))
            {
                m_seats[i].studentName = shuffledStudents[i - currentEmptyCount];
                m_seats[i].seatElement = CreateSeatControl(m_seats[i].row, m_seats[i].col, false, m_seats[i].studentName);
            }
            else
            {
                m_seats[i].seatElement = CreateSeatControl(m_seats[i].row, m_seats[i].col, true, L"");
                currentEmptyCount++;
            }
            Grid::SetRow(m_seats[i].seatElement, m_seats[i].row);
            Grid::SetColumn(m_seats[i].seatElement, m_seats[i].col);
            SeatGrid().Children().SetAt(i, m_seats[i].seatElement); 
        }

        studentsSeatsCurrent = shuffledStudents;
    }

    // CaptureButton_Click: Captures the host window and saves it as a PNG image.
    winrt::Windows::Foundation::IAsyncAction SeatArrangerPage::CaptureButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        if (!s_hostHwnd)
        {
            co_return;
        }
        
        // Get time
        auto now = std::chrono::system_clock::now();
        std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
        std::tm timeInfo;
        localtime_s(&timeInfo, &timeNow);
        std::stringstream rtime;
        rtime << std::put_time(&timeInfo, "%Y%m%d%H%M%S");
        hstring fileName = L"SeatArrangerCapture_" + to_hstring(rtime.str()) + L".png";

        auto localFolder = ApplicationData::Current().LocalFolder();
        auto capturesFolder = co_await localFolder.CreateFolderAsync(L"LuckyStudentPicker\\Captures", CreationCollisionOption::OpenIfExists);
        auto file = co_await capturesFolder.CreateFileAsync(fileName, CreationCollisionOption::GenerateUniqueName);

        co_await CaptureHelper::Instance().CaptureWindowToFileAsync(s_hostHwnd, file, this->DispatcherQueue());
    }

    winrt::Windows::Foundation::IAsyncAction winrt::LuckyStudentPicker::implementation::SeatArrangerPage::OpenCaptureFolderButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        auto localFolder = ApplicationData::Current().LocalFolder();
        auto capturesFolder = co_await localFolder.CreateFolderAsync(L"LuckyStudentPicker\\Captures", CreationCollisionOption::OpenIfExists);
        ShellExecuteA(nullptr, "open", to_string(capturesFolder.Path()).c_str(), 0, nullptr, SW_SHOWNORMAL);
    }

    // main code ends here

    // To-do: swap two students' seats when dragged

    // To-do: Animate swapping procedure
    
    // To-do: Let user decide their own seat maps
}