#include "attribute.h"

template <typename T>
std::optional<T> onnxcc::get_attr(const Node& node, std::string_view name) {
    // Unordered_map .find wants std::string so we convert name into it.
    auto it = onnxcc::node.attributes.find(std::string(name));
    if (it == onnxcc::node.attributes.end()) {
        // Returns nullopt when attribute not found as per requirement
        return std::nullopt;
    }

    // Look inside the variant stored as it->second. If it currently contains a T, give me a pointer to that T.    
    if (const T* p = std::get_if<T>(&it->second)) {
        return *p;  
    }

    // Type mismatch runtime error: clearly reports node name, op_type, attribute name and both types.
    throw std::runtime_error(
    "Node '" + node.name + "' (op_type '" + node.op_type + "'): attribute '" +
    std::string(name) + "' has type " + held_name(it->second) +
    " but was requested as " + type_name<T>());
}


template <typename T>
T onnxcc::get_attr_or(const Node& node, std::string_view name, T fallback) {
    auto it = get_attr<T>(node, name);
    // Returns fallback if attribute is absent
    return it ? std::move(*it) : std::move(fallback);
}

template <typename T>
T onnxcc::get_attr_required(const Node& node, std::string_view name) {
    auto it = get_attr<T>(node, name);
    // Throws error if attribute is absent
    return it ? std::move(*it) :throw std::runtime_error("Attribute does not contain" + name); 

}


