#pragma once

#include <nlohmann/json_fwd.hpp>

class JsonReader {
public:
    static nlohmann::json loadFromFile(const std::string &filename);
};
