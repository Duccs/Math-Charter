#ifndef _SCANNER_H
#define _SCANNER_H

#include <Tokenizer.h>
#include <StateMachine.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

class ScannerClass {
    private:
        std::istream* input;
        bool ownsStream;
        int LineNumber;
    
    public:
        ScannerClass(const std::string& inputString, bool isFileName = false);
        ~ScannerClass();
        TokenClass GetNextToken();
        int GetLineNumber() const { return LineNumber; }
};

#endif /* _SCANNER_H */