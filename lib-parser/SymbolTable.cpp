#include "SymbolTable.h"

bool SymbolTable::Exists(const std::string & s) {
    // Check if <s> exists in the symbol table
    for (const auto& var : Variables) {
        if (var.Label == s) {
            return true;
        }
    }
    return false;
}

void SymbolTable::AddEntry(const std::string & s) {
    // Add <s> to the symbol table, or quit if it was already there
    if (Exists(s)) {
        std::cerr << "Error: Variable " << s << " already exists in the symbol table.";
        std::exit(1);
    }
    Variables.push_back({s, 0}); // Initialize with value 0
}

float SymbolTable::GetValue(const std::string & s) {
    // Get the current value of variable <s>
    for (const auto& var : Variables) {
        if (var.Label == s) {
            return var.Value;
        }
    }
    std::cerr << "Error: Variable " << s << " does not exist in the symbol table.";
    std::exit(1);
}

void SymbolTable::SetValue(const std::string & s, float v) {
    // Set variable <s> to the given value
    for (auto& var : Variables) {
        if (var.Label == s) {
            var.Value = v;
            return;
        }
    }
    std::cerr << "Error: Variable " << s << " does not exist in the symbol table.";
    std::exit(1);
}

int SymbolTable::GetIndex(const std::string & s) {
    // Get the unique index of where variable <s> is
    for (size_t i = 0; i < Variables.size(); ++i) {
        if (Variables[i].Label == s) {
            return static_cast<int>(i);
        }
    }
    return -1; // Variable <s> is not there
}

size_t SymbolTable::GetCount() {
    // Get the current number of variables in the symbol table
    return Variables.size();
}