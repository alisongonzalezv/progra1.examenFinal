#pragma once

#include <string>
#include <vector>

class MeatsRepository {
public:
    MeatsRepository();

    const std::vector<std::string>& Names() const;
    int GetAvailableQuantity(const std::string& name) const;

private:
    std::vector<std::string> names_;
};
