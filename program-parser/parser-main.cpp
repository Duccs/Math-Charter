
#include <Expression.h>
#include <config.h>
#include <iostream>

int main() {

    while (true)
    {
        // Read expression from user input
        std::cout << "Enter a mathematical expression in terms of x, or 'exit' to quit:\n";
        std::string equation;
        std::getline(std::cin, equation);

        if (equation == "exit") {
            std::cout << "Exiting." << std::endl;
            break;
        }

        Expression expr = Expression::parse(equation);
        MSG("Expression parsed.");

        if (!expr.isValid()) {
            MSG("Error parsing expression: " << expr.getError());
            return 1;
        }

        MSG("Postfix representation: " << expr.toPostfixString());

        while (true)
        {
            std::cout << "Enter a value for x, or 'quit' to return to expression input:\n";
            std::string input;
            std::getline(std::cin, input);

            if (input == "quit") {
                break;
            }

            try {
                float x = std::stof(input);
                float result = expr.evaluate(x);
                MSG("x = " << x << ", f(x) = " << result);
            } catch (const std::invalid_argument& e) {
                MSG("Invalid input. Please enter a number or 'quit'.");
            }
        }
        


    }
    return 0;
}