#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "./token.hpp"

typedef unsigned int uint;
template<typename T>
using Vec = std::vector<T>;
typedef std::string String;

String tokens_to_assembly(const Vec<Token>& tokens)
{
    String assembly;
    for (const Token& token : tokens) {
        switch (token.type) {
            case TokenType::_return:
                assembly += "mov rax, 0\n";
                assembly += "ret\n";
                break;
            case TokenType::_int:
                assembly += "mov rax, ";
                assembly += token.val.value();
                assembly += "\n";
                break;
            case TokenType::semicolon:
                assembly += "ret\n";
                break;
        }
    }
    return assembly;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Incorrect usuage. Correct usage: .\\tau <input.tau>" << std::endl;
        return EXIT_FAILURE;
    }
    String content; {
        std::stringstream buffer;
        std::fstream input(argv[1], std::ios::in);
        buffer << input.rdbuf();
        content = buffer.str();
        input.close();
    }

    Tokenizer tokenizer(std::move(content));
    std::vector<Token> tokens = tokenizer.tokenize();
    {
        std::fstream file("out.asm", std::ios::out);
        file << tokens_to_assembly(tokens);
    }

    system("nasm -f elf64 out.asm -o out.o"); // Compile to object file
    system("ld out.o -o out"); // Link object file to executable
    return EXIT_SUCCESS;
}
