#pragma once

using namespace winrt;

class LoggingHelper
{
public:
	static LoggingHelper& Instance();
	void WriteLog(const std::stringstream & log);
	void WriteLog(hstring log);
	fire_and_forget InitializeLogFile();
	void ClearLogFile();
	void Flush();
	hstring getLogFile();

private:
	LoggingHelper() = default;
};

