#include "Plugin.h"
#include <windows.h>
#include <regex>
#include <iostream>
#include <filesystem>

void LookupFilesOnDirectory(std::string path, std::string regex, std::function<void(std::smatch)> callback) {

    std::regex regexPattern(regex);
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(path)) {
        auto str = dirEntry.path().filename().string();
        std::smatch match;
        if (std::regex_match(str, match, regexPattern)) {
            callback(match);
        }
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {

    SKSE::Init(skse);
    SetupLog();

    LookupFilesOnDirectory("Data\\SKSE\\Plugins\\", "^(.*)_load_dll\\.txt$", [](std::smatch match) {
        auto dllName = "Data\\SKSE\\Plugins\\" + match[1].str() + ".dll";
        std::wstring stemp = std::wstring(dllName.begin(), dllName.end());
        LPCWSTR path = stemp.c_str();
        LoadLibrary(path);
        logger::info("DLL Loaded: {}", dllName);
    });

    return true;
}


