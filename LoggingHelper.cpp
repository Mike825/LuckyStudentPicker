#include "pch.h"
#include "LoggingHelper.h"
#include <chrono>
#include <winrt/Microsoft.Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

LoggingHelper& LoggingHelper::Instance() {
    static LoggingHelper instance;  
    return instance; 
}

hstring logFolder, logFile;
std::ofstream logger;

std::string getTimeFormated() 
{
    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
    std::tm timeInfo;
    localtime_s(&timeInfo, &timeNow);
    std::stringstream rtime;
    rtime << "[" << std::put_time(&timeInfo, "%Y/%m/%d %H:%M:%S") << "]\t";
    return rtime.str();
}


void LoggingHelper::WriteLog(const std::stringstream & log) 
{
    if (logger) logger << getTimeFormated() << log.str();
}

void LoggingHelper::WriteLog(hstring log)
{
    if (logger) logger << getTimeFormated() << to_string(log);
}

fire_and_forget LoggingHelper::InitializeLogFile() 
{
    co_await resume_background();

    StorageFolder localTempFolder = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
    hstring folderName = L"LuckyStudentPicker";
    StorageFolder Folder = co_await localTempFolder.CreateFolderAsync(folderName, CreationCollisionOption::OpenIfExists);
    logFolder = Folder.Path();
    hstring fileName = L"UnluckyStudentsLog.log";
    StorageFile File = co_await Folder.CreateFileAsync(fileName, CreationCollisionOption::OpenIfExists);
    logFile = File.Path();

    logger.open(to_string(logFile), std::ios::out | std::ios::app);
}

void LoggingHelper::ClearLogFile()
{
    logger.close();
    logger.open(to_string(logFile));
}

void LoggingHelper::Flush()
{
    logger.close();
    logger.open(to_string(logFile), std::ios::out | std::ios::app);
}

hstring LoggingHelper::getLogFile()
{
    return logFile;
}