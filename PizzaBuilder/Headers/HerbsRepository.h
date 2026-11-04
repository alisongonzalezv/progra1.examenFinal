#pragma once

#include <string>
#include <vector>

class HerbsRepository {
public:
    HerbsRepository();

    const std::vector<std::string>& Names() const;
    int GetAvailableQuantity(const std::string& name) const;

private:
    std::vector<std::string> names_;
};
