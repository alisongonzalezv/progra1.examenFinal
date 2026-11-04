#include "HerbsRepository.h"

// TODO: hierba.

HerbsRepository::HerbsRepository() = default;

const std::vector<std::string>& HerbsRepository::Names() const {
    return names_;
}

int HerbsRepository::GetAvailableQuantity(const std::string&) const {
    return 0;
}
