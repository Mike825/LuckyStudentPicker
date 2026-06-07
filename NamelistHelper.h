#pragma once

using namespace winrt;

class NamelistHelper
{
public:
	static NamelistHelper& Instance();
	fire_and_forget InitializeNamelistFile();
	std::vector<hstring> GetNamelist();
	int GetNamelistLength();

private:
	NamelistHelper() = default;
};

