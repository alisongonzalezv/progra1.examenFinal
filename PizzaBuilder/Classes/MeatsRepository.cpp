#include "MeatsRepository.h"

// TODO: carne.

MeatsRepository::MeatsRepository() = default;

const std::vector<std::string>& MeatsRepository::Names() const {
    return names_;
}

int MeatsRepository::GetAvailableQuantity(const std::string&) const {
    return 0;
}
