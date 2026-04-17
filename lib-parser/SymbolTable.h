#ifndef _SYMBOL_Table_H_
#define _SYMBOL_Table_H_

#include <string>
#include <vector>
#include <set>
#include <stdexcept>

enum class VariableType {
    Coordinate,
    Constant  
};

class SymbolTable {
    private:
        struct Variable {
            std::string Label;
            float Value;
            VariableType Type;
        };

        std::vector<Variable> Variables;
    public:
        SymbolTable() = default;
        ~SymbolTable() = default;
        
        // Check if a variable name is a coordinate
        static bool IsReserved(const std::string& name);
        
        bool Exists(const std::string & s) const;

        void AddEntry(const std::string & s, VariableType type = VariableType::Constant);

        bool RemoveEntry(const std::string& s);
        
        // Add or update entry
        void SetOrAddEntry(const std::string& s, float value, VariableType type = VariableType::Constant);

        float GetValue(const std::string & s) const;
        void SetValue(const std::string & s, float v);
        
        VariableType GetType(const std::string& s) const;

        int GetIndex(const std::string & s) const;
        
        size_t GetCount() const;
        
        // Get all variable names
        std::set<std::string> GetAllVariables() const;
        
        // Merge constants from another symbol table
        void MergeConstants(const SymbolTable& source);
        
        // Clear all entries
        void Clear();
};

#endif /* _SYMBOL_Table_H_ */