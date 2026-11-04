#pragma once

#include <string>
#include <vector>

#include "CheeseRepository.h"
#include "HerbsRepository.h"
#include "MeatsRepository.h"
#include "TomatoesRepository.h"


#define HABILITAR_QUESO
// #define HABILITAR_CARNE
// #define HABILITAR_HIERBA

class PizzaBuilder {
public:
    PizzaBuilder();

    bool AddTomato(const std::string& name, std::string& message);
    bool AddCheese(const std::string& name, std::string& message);
    bool AddMeat(const std::string& name, std::string& message);
    bool AddHerb(const std::string& name, std::string& message);

    void Finish();
    bool IsFinished() const;
    std::string Summary() const;

    const TomatoesRepository& Tomatoes() const;
    const CheeseRepository& Cheeses() const;
    const MeatsRepository& Meats() const;
    const HerbsRepository& Herbs() const;

    const std::vector<std::string>& SelectedTomatoes() const;
    const std::vector<std::string>& SelectedCheeses() const;
    const std::vector<std::string>& SelectedMeats() const;
    const std::vector<std::string>& SelectedHerbs() const;

private:
    void AppendGroup(std::string& text, const std::string& title, const std::vector<std::string>& items) const;
    int CountFilledGroups() const;

    TomatoesRepository tomatoRepo_;
    CheeseRepository cheeseRepo_;
    MeatsRepository meatsRepo_;
    HerbsRepository herbsRepo_;

    std::vector<std::string> selectedTomatoes_;
    std::vector<std::string> selectedCheeses_;
    std::vector<std::string> selectedMeats_;
    std::vector<std::string> selectedHerbs_;
    bool finished_ = false;
};
