#include "PizzaBuilder.h"

#include "IngredientSelector.h"
#include "TomatoesRepository.h"

PizzaBuilder::PizzaBuilder() = default;

bool PizzaBuilder::AddTomato(const std::string& name, std::string& message) {
    if (finished_) {
        message = "La pizza ya está terminada.";
        return false;
    }
    IngredientSelector<TomatoesRepository> selector(tomatoRepo_, "tomate");
    const AddResult result = selector.AddIngredientToPizza(name, selectedTomatoes_);
    message = result.message;
    return result.ok;
}

bool PizzaBuilder::AddCheese(const std::string& name, std::string& message) {
#ifdef HABILITAR_QUESO
    if (finished_) { message = "La pizza ya está terminada."; return false; }
    IngredientSelector<CheeseRepository> selector(cheeseRepo_, "queso");
    const AddResult result = selector.AddIngredientToPizza(name, selectedCheeses_);
    message = result.message;
    return result.ok;
#else
    (void)name;
    message = "Queso pendiente.";
    return false;
#endif
}

bool PizzaBuilder::AddMeat(const std::string& name, std::string& message) {
#ifdef HABILITAR_CARNE
    if (finished_) { message = "La pizza ya está terminada."; return false; }
    IngredientSelector<MeatsRepository> selector(meatsRepo_, "carne");
    const AddResult result = selector.AddIngredientToPizza(name, selectedMeats_);
    message = result.message;
    return result.ok;
#else
    (void)name;
    message = "Carne pendiente.";
    return false;
#endif
}

bool PizzaBuilder::AddHerb(const std::string& name, std::string& message) {
#ifdef HABILITAR_HIERBA
    if (finished_) { message = "La pizza ya está terminada."; return false; }
    IngredientSelector<HerbsRepository> selector(herbsRepo_, "hierba");
    const AddResult result = selector.AddIngredientToPizza(name, selectedHerbs_);
    message = result.message;
    return result.ok;
#else
    (void)name;
    message = "Hierba pendiente.";
    return false;
#endif
}

void PizzaBuilder::Finish() {
    finished_ = true;
}

bool PizzaBuilder::IsFinished() const {
    return finished_;
}

void PizzaBuilder::AppendGroup(std::string& text, const std::string& title, const std::vector<std::string>& items) const {
    text += title + " (" + std::to_string(items.size()) + "):\n";
    if (items.empty()) {
        text += "- Ninguno\n";
        return;
    }
    for (const auto& item : items) {
        text += "- " + item + "\n";
    }
}

int PizzaBuilder::CountFilledGroups() const {
    int groups = 0;
    if (!selectedTomatoes_.empty()) {
        ++groups;
    }
    if (!selectedCheeses_.empty()) {
        ++groups;
    }
    if (!selectedMeats_.empty()) {
        ++groups;
    }
    if (!selectedHerbs_.empty()) {
        ++groups;
    }
    return groups;
}

std::string PizzaBuilder::Summary() const {
    std::string text = "Tu pizza tiene los siguientes ingredientes:\n\n";
    AppendGroup(text, "Tomates", selectedTomatoes_);
    AppendGroup(text, "Queso", selectedCheeses_);
    AppendGroup(text, "Carnes", selectedMeats_);
    AppendGroup(text, "Hierbas", selectedHerbs_);
    text += "\n";

    switch (CountFilledGroups()) {
    case 0:
        text += "No agregaste ingredientes a tu pizza.\n";
        break;
    case 1:
        if (!selectedTomatoes_.empty()) {
            text += "Tu pizza solo tiene tomate.\n";
        } else {
            text += "Tu pizza tiene un solo tipo de ingrediente.\n";
        }
        break;
    case 2:
        text += "Tu pizza ya tiene un segundo ingrediente.\n";
        break;
    case 3:
        text += "Tu pizza ya tiene un tercer ingrediente.\n";
        break;
    default:
        text += "Tu pizza ya tiene los cuatro ingredientes.\n";
        break;
    }

    text += "¡Buen provecho!";
    return text;
}

const TomatoesRepository& PizzaBuilder::Tomatoes() const { return tomatoRepo_; }
const CheeseRepository& PizzaBuilder::Cheeses() const { return cheeseRepo_; }
const MeatsRepository& PizzaBuilder::Meats() const { return meatsRepo_; }
const HerbsRepository& PizzaBuilder::Herbs() const { return herbsRepo_; }

const std::vector<std::string>& PizzaBuilder::SelectedTomatoes() const { return selectedTomatoes_; }
const std::vector<std::string>& PizzaBuilder::SelectedCheeses() const { return selectedCheeses_; }
const std::vector<std::string>& PizzaBuilder::SelectedMeats() const { return selectedMeats_; }
const std::vector<std::string>& PizzaBuilder::SelectedHerbs() const { return selectedHerbs_; }
