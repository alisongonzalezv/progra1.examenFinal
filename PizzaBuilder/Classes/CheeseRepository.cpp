#include "CheeseRepository.h"

// TODO: queso.

CheeseRepository::CheeseRepository() = default;

const std::vector<std::string>& CheeseRepository::Names() const {
    return names_;
}

int CheeseRepository::GetAvailableQuantity(const std::string&) const {
    return 0;
}
