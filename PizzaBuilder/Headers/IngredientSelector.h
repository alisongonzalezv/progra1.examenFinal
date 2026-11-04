#pragma once

#include <stdexcept>
#include <string>
#include <vector>

struct AddResult {
    bool ok = false;
    std::string message;
};

template<typename TRepository>
class IngredientSelector {
public:
    IngredientSelector(TRepository& repository, const std::string& ingredientType);

    AddResult AddIngredientToPizza(const std::string& ingredientName, std::vector<std::string>& selectedIngredients);

private:
    TRepository& repository_;
    std::string ingredientType_;
};

template<typename TRepository>
IngredientSelector<TRepository>::IngredientSelector(TRepository& repository, const std::string& ingredientType)
    : repository_(repository), ingredientType_(ingredientType) {
}

template<typename TRepository>
AddResult IngredientSelector<TRepository>::AddIngredientToPizza(
    const std::string& ingredientName,
    std::vector<std::string>& selectedIngredients) {
    AddResult result;

    try {
        const int quantity = repository_.GetAvailableQuantity(ingredientName);
        if (quantity <= 0) {
            result.message = "Lo sentimos, no se puede agregar " + ingredientName + " (" + ingredientType_ + ") en este momento. Elige otro.";
            return result;
        }

        selectedIngredients.push_back(ingredientName);
        try {
            repository_.UpdateQuantity(ingredientName, quantity - 1);
        } catch (const std::exception& ex) {
            selectedIngredients.pop_back();
            result.message = ex.what();
            return result;
        }

        result.ok = true;
        result.message = "Se agregó " + ingredientName + " a tu pizza.";
        return result;
    } catch (const std::invalid_argument&) {
        result.message = "Lo sentimos, " + ingredientName + " no es un tipo de " + ingredientType_ + " válido. Elige una de las opciones disponibles.";
        return result;
    }
}
