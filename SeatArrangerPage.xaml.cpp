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
#include <winrt/Windows.Globalization.NumberFormatting.h>

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
using namespace Windows::Globalization::NumberFormatting;


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
    std::vector<hstring> studentsSeatsCurrent = { };
    void SeatArrangerPage::InitializeSeatGrid(bool isForced)
    {            
        int seatIndex = 0;
        seatCount = 0;

        if (studentsSeatsCurrent.size() == 0 || isForced)
        {
            // Clear the SeatGrid()
            SeatGrid().Children().Clear();
            m_seats.clear();
            
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

                    seatIndex++; // this "seatIndex" is for all seats
                }
            }
        }
        else
        {
            // Lay SeatGrid() out in the order of studentsSeatsCurrent
            for (int row = 0; row < m_seatLayout.size(); row++) {
                for (int col = 0; col < m_seatLayout[row].size(); col++) {
                    bool isEmpty = !m_seatLayout[row][col];

                    winrt::Microsoft::UI::Xaml::Controls::Grid seatControl = nullptr;

                    if (!isEmpty) {                    
                        seatControl = CreateSeatControl(row, col, isEmpty, studentsSeatsCurrent[seatIndex]);
                        m_seats.push_back({ seatControl, row, col, studentsSeatsCurrent[seatIndex], false });
                        seatCount++, seatIndex++; // this "seatIndex" is for "non-empty seats"
                    }
                    else {
                        seatControl = CreateSeatControl(row, col, isEmpty, L"");
                        m_seats.push_back({ seatControl, row, col, L"", true});
                    }

                    // Grid placement
                    Grid::SetRow(seatControl, row);
                    Grid::SetColumn(seatControl, col);
                    SeatGrid().Children().Append(seatControl);

                }
            }
        }
    }

    // Grid_Loaded: Execute InitializeSeatGrid() when SeatGrid() is ready to avoid crash
    void winrt::LuckyStudentPicker::implementation::SeatArrangerPage::Grid_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (SeatGrid())
        {
            InitializeSeatGrid(false);
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

    // CreateSeatControl: Create a single seat
    winrt::Microsoft::UI::Xaml::Controls::Grid SeatArrangerPage::CreateSeatControl(int row, int col, bool isEmpty, hstring sname)
    {
        // Basic settings
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

            // Settings for normal seats

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

        // Constraints about randomize times
        auto value = RandomizingTimes().Value();
        if (value > 50 || value <= 0)
        {
            RandomizingTimesFlyout().ShowAt(RandomizeButton());
            co_return;
        }
        
        // Randomize
        auto shuffledStudents = NamelistHelper::Instance().GetNamelist();
        int currentTimes = 0, waitMilliseconds = 50;

        winrt::Microsoft::UI::Xaml::DispatcherTimer DT = winrt::Microsoft::UI::Xaml::DispatcherTimer();
        DT.Interval(std::chrono::milliseconds(150));

        DT.Tick
        ([this, currentTimes, value, shuffledStudents, DT, waitMilliseconds](auto&, auto&) mutable
            {
                if (currentTimes < value) 
                {
                    // Prevent user from hitting the button for too many times 
                    RandomizeButton().IsEnabled(false);

                    // Update randomize times indicator
                    RandomizeTimesIndicator().Text(to_hstring(currentTimes + 1));

                    // Determine how long should each tick wait
                    if (currentTimes <= value * 0.3) waitMilliseconds = 50;
                    else if (currentTimes <= value * 0.6) waitMilliseconds = 100;
                    else if (currentTimes <= value * 0.8) waitMilliseconds = 150;
                    else waitMilliseconds = 200;

                    DT.Interval(std::chrono::milliseconds(waitMilliseconds));

                    // Sort students randomly
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
                    currentTimes++;
                }
                else
                {
                    DT.Stop();
                    RandomizeButton().IsEnabled(true);
                }
            }
        );

        DT.Start();
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
    
    winrt::Windows::Foundation::IAsyncAction winrt::LuckyStudentPicker::implementation::SeatArrangerPage::RandomizingTimes_ValueChanged(winrt::Microsoft::UI::Xaml::Controls::NumberBox const&, winrt::Microsoft::UI::Xaml::Controls::NumberBoxValueChangedEventArgs const&)
    {
        auto value = RandomizingTimes().Value();
        if (value > 50 || value <= 0) RandomizeTimesErrorPrompt().IsOpen(true);
        else RandomizeTimesErrorPrompt().IsOpen(false);
        co_return;
    }
    
    winrt::Windows::Foundation::IAsyncAction winrt::LuckyStudentPicker::implementation::SeatArrangerPage::RandomizingTimes_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        Windows::Globalization::NumberFormatting::IncrementNumberRounder INR = Windows::Globalization::NumberFormatting::IncrementNumberRounder::IncrementNumberRounder();
        Windows::Globalization::NumberFormatting::DecimalFormatter DF = Windows::Globalization::NumberFormatting::DecimalFormatter::DecimalFormatter();
        DF.FractionDigits(0);
        DF.NumberRounder(INR);
        RandomizingTimes().NumberFormatter(DF);
        co_return;
    }
    
    void winrt::LuckyStudentPicker::implementation::SeatArrangerPage::RandomizeTimesErrorPrompt_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        RandomizingTimes().ValueChanged({ this, &winrt::LuckyStudentPicker::implementation::SeatArrangerPage::RandomizingTimes_ValueChanged });
    }

    void winrt::LuckyStudentPicker::implementation::SeatArrangerPage::ResetButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        InitializeSeatGrid(true);
        RandomizeTimesIndicator().Text(L"0");
        studentsSeatsCurrent.clear();
    }

    // main code ends here

    // To-do: swap two students' seats when dragged

    // To-do: Animate swapping procedure
    
    // To-do: Let user decide their own seat maps
}