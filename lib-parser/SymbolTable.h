#ifndef _SYMBOL_Table_H_
#define _SYMBOL_Table_H_

#include <string>
#include <vector>
#include <iostream>

class SymbolTable {
    private:
        struct Variable {
            std::string Label;
            float Value;
        };

        std::vector<Variable> Variables;
    public:
        SymbolTable() = default;
        ~SymbolTable() = default;
        
        bool Exists(const std::string & s);

        void AddEntry(const std::string & s);

        float GetValue(const std::string & s);
        void SetValue(const std::string & s, float v);

        int GetIndex(const std::string & s);
        
        size_t GetCount();
};

#endif /* _SYMBOL_Table_H_ */