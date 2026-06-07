#include "pch.h"
#include "NamelistHelper.h"
#include <winrt/Microsoft.Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"

using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace std;
using json = nlohmann::json;

namespace nlohmann {
    template <>
    struct adl_serializer<winrt::hstring> {
        static winrt::hstring from_json(const json& j) {
            return winrt::to_hstring(j.get<std::string>());
        }
    };
}

NamelistHelper& NamelistHelper::Instance() {
    static NamelistHelper instance;
    return instance;
}

hstring namelistFolder, namelistFile;

fire_and_forget NamelistHelper::InitializeNamelistFile()
{
    co_await resume_background();

    StorageFolder localTempFolder = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
    hstring folderName = L"LuckyStudentPicker";
    StorageFolder Folder = co_await localTempFolder.CreateFolderAsync(folderName, CreationCollisionOption::OpenIfExists);
    namelistFolder = Folder.Path();
    hstring fileName = L"UnluckyStudentsNamelist.json";
    StorageFile File = co_await Folder.CreateFileAsync(fileName, CreationCollisionOption::OpenIfExists);
    namelistFile = File.Path();

}

vector<hstring> NamelistHelper::GetNamelist() {
    try {
        std::ifstream f(to_string(namelistFile));
        json j;
        f >> j;
        return j.get<std::vector<winrt::hstring>>();
    }
    catch (...) {
        std::vector<hstring> r = { };
        return r;
    }
}

int NamelistHelper::GetNamelistLength() {
    return GetNamelist().size();
}