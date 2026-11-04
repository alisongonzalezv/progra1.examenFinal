#include "DataLoader.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace {

std::string Trim(const std::string& text) {
    const auto start = text.find_first_not_of(" \t\r\n");
    const auto end = text.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    return text.substr(start, end - start + 1);
}

bool IsSafeFilename(const std::string& filename) {
    return !filename.empty()
        && filename.find('/') == std::string::npos
        && filename.find('\\') == std::string::npos
        && filename.find("..") == std::string::npos;
}

bool ParseQuantity(const std::string& text, int& value) {
    if (text.empty() || text.size() > 9) {
        return false;
    }
    for (unsigned char character : text) {
        if (!std::isdigit(character)) {
            return false;
        }
    }
    try {
        value = std::stoi(text);
    } catch (const std::exception&) {
        return false;
    }
    return value >= 0;
}

std::filesystem::path ExecutableDirectory() {
    std::wstring buffer(MAX_PATH, L'\0');
    for (;;) {
        const DWORD length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        if (length == 0) {
            throw std::runtime_error("No se pudo ubicar el ejecutable.");
        }
        if (length < buffer.size() - 1) {
            buffer.resize(length);
            break;
        }
        buffer.resize(buffer.size() * 2);
    }
    return std::filesystem::path(buffer).parent_path();
}

std::filesystem::path FindDataFile(const std::string& filename) {
    std::vector<std::filesystem::path> roots;
    roots.push_back(std::filesystem::current_path());

    std::filesystem::path directory = ExecutableDirectory();
    for (int level = 0; level < 5 && !directory.empty(); ++level) {
        roots.push_back(directory);
        const auto parent = directory.parent_path();
        if (parent == directory) {
            break;
        }
        directory = parent;
    }

    for (const auto& root : roots) {
        const auto candidate = root / "Data" / filename;
        std::error_code error;
        if (!std::filesystem::is_regular_file(candidate, error) || error) {
            continue;
        }
        const auto canonical = std::filesystem::weakly_canonical(candidate, error);
        if (error || canonical.filename() != filename || canonical.parent_path().filename() != "Data") {
            continue;
        }
        return canonical;
    }

    throw std::runtime_error("No se puede abrir el archivo: " + filename);
}

std::string JoinValues(const std::vector<std::string>& values) {
    std::string joined;
    for (size_t index = 0; index < values.size(); ++index) {
        if (index > 0) {
            joined += ", ";
        }
        joined += values[index];
    }
    return joined;
}

}  // namespace

InventoryFile DataLoader::LoadFromFile(const std::string& filename) {
    if (!IsSafeFilename(filename)) {
        throw std::runtime_error("Nombre de inventario no válido.");
    }

    const auto path = FindDataFile(filename);
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("No se puede abrir el archivo: " + filename);
    }

    InventoryFile file;
    file.path = path;

    std::string line;
    int lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        line = Trim(line);
        if (lineNumber == 1 && line.size() >= 3
            && static_cast<unsigned char>(line[0]) == 0xEF
            && static_cast<unsigned char>(line[1]) == 0xBB
            && static_cast<unsigned char>(line[2]) == 0xBF) {
            line = Trim(line.substr(3));
        }
        if (line.empty()) {
            continue;
        }

        const auto separator = line.find(':');
        if (separator == std::string::npos) {
            throw std::runtime_error("Formato inválido en la línea " + std::to_string(lineNumber) + ": falta ':'");
        }

        const std::string name = Trim(line.substr(0, separator));
        const std::string rest = Trim(line.substr(separator + 1));
        if (name.empty() || rest.empty()) {
            throw std::runtime_error("Entrada inválida en la línea " + std::to_string(lineNumber));
        }

        std::vector<std::string> values;
        std::stringstream stream(rest);
        std::string item;
        while (std::getline(stream, item, ',')) {
            item = Trim(item);
            if (!item.empty()) {
                values.push_back(item);
            }
        }
        if (values.empty()) {
            throw std::runtime_error("Entrada inválida en la línea " + std::to_string(lineNumber));
        }

        int quantity = 0;
        if (!ParseQuantity(values[0], quantity)) {
            throw std::runtime_error("Cantidad inválida en la línea " + std::to_string(lineNumber));
        }
        values[0] = std::to_string(quantity);

        if (file.rows.find(name) == file.rows.end()) {
            file.order.push_back(name);
        }
        file.rows[name] = std::move(values);
    }

    return file;
}

void DataLoader::SaveToFile(const InventoryFile& file) {
    if (file.path.empty() || file.path.filename().empty() || file.path.parent_path().filename() != "Data") {
        throw std::runtime_error("No hay ruta de inventario para guardar.");
    }

    const auto temporary = std::filesystem::path(file.path.wstring() + L".tmp");
    {
        std::ofstream output(temporary, std::ios::trunc);
        if (!output) {
            throw std::runtime_error("No se pudo guardar el inventario.");
        }
        for (const auto& name : file.order) {
            const auto row = file.rows.find(name);
            if (row == file.rows.end() || row->second.empty()) {
                throw std::runtime_error("El inventario quedó incompleto y no se guardó.");
            }
            output << name << ": " << JoinValues(row->second) << '\n';
            if (!output) {
                throw std::runtime_error("No se pudo guardar el inventario.");
            }
        }
    }

    std::error_code error;
    std::filesystem::copy_file(
        temporary,
        file.path,
        std::filesystem::copy_options::overwrite_existing,
        error);
    std::filesystem::remove(temporary);
    if (error) {
        throw std::runtime_error("No se pudo reemplazar el archivo de inventario.");
    }
}
