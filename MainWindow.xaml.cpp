#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <random>
#include <ctime>
#include <winrt/Windows.Media.SpeechSynthesis.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Windows::Media::SpeechSynthesis;
using namespace Windows::Storage::Streams;
using namespace Windows::Media::Core;
using namespace Windows::Media::Playback;
using namespace Microsoft::UI::Dispatching;
using namespace Windows::UI::Xaml::Controls;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LuckyStudentPicker::implementation
{
    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
    
    bool rnumList[100] = { 0 };
    hstring nameList[22] = { L"����ة", L"�׼�", L"����ܰ", L"���", L"������", L"֣����", L"������", L"�����", L"�Գ���", L"��ۿ��", L"����", L"��ƿ�", L"������", L"�������", L"���건", L"�����", L"����׿", L"����", L"��ʫ�", L"�ζG", L"������", L"����̴" };
    int int_count = 0, int_rnum = 0;
    DispatcherQueue uiDispatcherQueue = DispatcherQueue::GetForCurrentThread();

    hstring randName() {
        std::default_random_engine e;
        std::uniform_int_distribution<int> u(1, 22);
        e.seed(static_cast<unsigned int>(time(0)) ^ static_cast<unsigned int>(GetCurrentThreadId()));
        
        int_rnum = u(e);
        while (rnumList[int_rnum]) {
            int_rnum = u(e);
        }
        rnumList[int_rnum] = 1;
        winrt::hstring hs = nameList[int_rnum - 1];
        int_count++;
        return hs;
    }
    
    void MainWindow::reset(IInspectable const&, RoutedEventArgs const&) {
        if (int_count > 21) {
            for (int i = 0; i < 100; i++) rnumList[i] = 0;
            int_count = 0;
            text().Text(L"");
            buttonText().Text(L"  ѡһ��");
            return;
        }
        return;
    }

    SpeechSynthesizer synthesizer{ nullptr };
    MediaPlayer mediaPlayer{ nullptr };

    winrt::Windows::Foundation::IAsyncAction MainWindow::SpeakTextAsync(hstring text) {
        synthesizer = SpeechSynthesizer();
        if (!synthesizer) {
            OutputDebugString(L"synthesizer is null!");
            co_return; 
        }
        try {
            SpeechSynthesisStream stream = co_await synthesizer.SynthesizeTextToStreamAsync(text);
            mediaPlayer = MediaPlayer();
            mediaPlayer.AutoPlay(true);
            mediaPlayer.Source(MediaSource::CreateFromStream(stream, stream.ContentType()));
            mediaPlayer.Play();
            
        }
        catch (const hresult_error& ex) {
            OutputDebugString(ex.message().c_str());
        }
    }

    void MainWindow::updRandName(IInspectable const& sender, RoutedEventArgs const& r)
    {
        
        if (int_count <= 21) text().Text(randName());
        else {
            if (buttonText().Text() == L"  ѡһ��") {
                text().Text(L"��������");
                buttonText().Text(L"  ����");
            }
            else reset(sender, r);
        }
        SpeakTextAsync(text().Text());
        
    }
    
}
