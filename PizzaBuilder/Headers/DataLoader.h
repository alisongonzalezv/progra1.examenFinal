#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

struct InventoryFile {
    std::filesystem::path path;
    std::vector<std::string> order;
    std::unordered_map<std::string, std::vector<std::string>> rows;
};

class DataLoader {
public:
    static InventoryFile LoadFromFile(const std::string& filename);
    static void SaveToFile(const InventoryFile& file);
};
