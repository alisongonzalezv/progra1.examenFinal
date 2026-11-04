#pragma once

#include <string>
#include <vector>

#include "DataLoader.h"

class IngredientRepository {
public:
    explicit IngredientRepository(const std::string& filename);
    IngredientRepository(const IngredientRepository&) = delete;
    IngredientRepository& operator=(const IngredientRepository&) = delete;

    const std::vector<std::string>& Names() const;
    int GetAvailableQuantity(const std::string& name) const;
    void UpdateQuantity(const std::string& name, int newQuantity);

private:
    InventoryFile data_;
};
