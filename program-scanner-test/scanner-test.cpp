
#include <Scanner.h>
#include <config.h>
#include <iostream>

int main() {
    ScannerClass scanner("code.txt");
    MSG("Scanner initialized. Starting tokenization...");
    TokenType tt;
    do {
        TokenClass tc = scanner.GetNextToken();
        MSG("Line number" << scanner.GetLineNumber() << " " << tc);
        tt = tc.GetTokenType();
    } while (tt != EOF_TOKEN);

    return 0;
}