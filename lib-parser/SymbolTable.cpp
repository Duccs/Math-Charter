#include "SymbolTable.h"


bool SymbolTable::IsReserved(const std::string& name) {
    // Currently: x, y, z are reserved
    // TODO: add t when parametric equations are supported
    return (name == "x" || name == "y" || name == "z");
}

bool SymbolTable::Exists(const std::string & s) const {
    // Check if <s> exists in the symbol table
    for (const auto& var : Variables) {
        if (var.Label == s) {
            return true;
        }
    }
    return false;
}

void SymbolTable::AddEntry(const std::string & s, VariableType type) {
    // Add <s> to the symbol table, or throw if it was already there
    if (Exists(s)) {
        throw std::runtime_error("Variable '" + s + "' already exists in the symbol table.");
    }
    Variables.push_back({s, 0.0f, type});
}

bool SymbolTable::RemoveEntry(const std::string& s) {
    // Remove <s> from the symbol table, return true if removed, false if not found
    for (auto it = Variables.begin(); it != Variables.end(); ++it) {
        if (it->Label == s) {
            Variables.erase(it);
            return true;
        }
    }
    return false;
}

void SymbolTable::SetOrAddEntry(const std::string& s, float value, VariableType type) {
    // For reassigning constants
    for (auto& var : Variables) {
        if (var.Label == s) {
            var.Value = value;
            var.Type = type;
            return;
        }
    }
    Variables.push_back({s, value, type});
}

float SymbolTable::GetValue(const std::string & s) const {
    // Get the current value of variable <s>
    for (const auto& var : Variables) {
        if (var.Label == s) {
            return var.Value;
        }
    }
    throw std::runtime_error("Undefined variable '" + s + "'");
}

void SymbolTable::SetValue(const std::string & s, float v) {
    // Set variable <s> to the given value
    for (auto& var : Variables) {
        if (var.Label == s) {
            var.Value = v;
            return;
        }
    }
    throw std::runtime_error("Variable '" + s + "' does not exist in the symbol table.");
}

VariableType SymbolTable::GetType(const std::string& s) const {
    for (const auto& var : Variables) {
        if (var.Label == s) {
            return var.Type;
        }
    }
    throw std::runtime_error("Variable '" + s + "' does not exist in the symbol table.");
}

int SymbolTable::GetIndex(const std::string & s) const {
    // Get the unique index of where variable <s> is
    for (size_t i = 0; i < Variables.size(); ++i) {
        if (Variables[i].Label == s) {
            return static_cast<int>(i);
        }
    }
    return -1; // Variable <s> is not there
}

size_t SymbolTable::GetCount() const {
    // Get the current number of variables in the symbol table
    return Variables.size();
}

std::set<std::string> SymbolTable::GetAllVariables() const {
    std::set<std::string> result;
    for (const auto& var : Variables) {
        result.insert(var.Label);
    }
    return result;
}

void SymbolTable::MergeConstants(const SymbolTable& source) {
    // Copy all Constant type entries from source
    for (const auto& var : source.Variables) {
        if (var.Type == VariableType::Constant) {
            SetOrAddEntry(var.Label, var.Value, VariableType::Constant);
        }
    }
}

void SymbolTable::Clear() {
    Variables.clear();
}