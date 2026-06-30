#pragma once

#include <string>
#include <vector>

namespace opcuaregler {

struct NodeDefinition {
    std::string path;
    std::string browseName;
    std::string dataType;
    std::string description;
    bool writable{false};
};

class Euromap83WorkingPointModel {
public:
    [[nodiscard]] std::string namespaceUri() const;
    [[nodiscard]] std::string euromapGeneralTypesUri() const;
    [[nodiscard]] std::vector<NodeDefinition> nodes() const;
};

} // namespace opcuaregler

