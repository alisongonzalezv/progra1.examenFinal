#include "IngredientRepository.h"

#include <stdexcept>

IngredientRepository::IngredientRepository(const std::string& filename)
    : data_(DataLoader::LoadFromFile(filename)) {
}

const std::vector<std::string>& IngredientRepository::Names() const {
    return data_.order;
}

int IngredientRepository::GetAvailableQuantity(const std::string& name) const {
    const auto row = data_.rows.find(name);
    if (row == data_.rows.end() || row->second.empty()) {
        throw std::invalid_argument("Tipo desconocido: " + name);
    }

    try {
        return std::stoi(row->second[0]);
    } catch (const std::exception&) {
        throw std::invalid_argument("Cantidad inválida para: " + name);
    }
}

void IngredientRepository::UpdateQuantity(const std::string& name, int newQuantity) {
    const auto row = data_.rows.find(name);
    if (row == data_.rows.end() || row->second.empty()) {
        throw std::invalid_argument("Tipo desconocido: " + name);
    }
    if (newQuantity < 0) {
        throw std::invalid_argument("La cantidad no puede ser negativa.");
    }

    const auto previous = row->second;
    row->second[0] = std::to_string(newQuantity);
    try {
        DataLoader::SaveToFile(data_);
    } catch (...) {
        row->second = previous;
        throw;
    }
}
