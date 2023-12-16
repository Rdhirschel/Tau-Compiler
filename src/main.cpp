#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

#include "./token.hpp"

typedef unsigned int uint;
template <typename T>
using Vec = std::vector<T>;
typedef std::string String;

String tokens_to_assembly(const Vec<Token>& tokens)
{
    std::stringstream assembly;
    assembly << "global _start\nstart:\n";

    for (int i = 0; i < tokens.size(); i++)
    {
        const Token& token = tokens[i];
        if (token.type == TokenType::_return) 
        {
            if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::_int) 
            {
                if (i + 2 < tokens.size() && tokens[i + 2].type == TokenType::semicolon)
                {
                    assembly << "    mov rax, 60\n";
                    assembly << "    mov rdi, " << tokens[i + 1].val.value() << "\n";
                    assembly << "    syscall\n";
                    i += 2;
                }

                else
                {
                    std::cerr << "Expected semicolon after return statement" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

        }
    }
    return assembly.str();
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Incorrect usage. Correct usage: .\\tau <input.tau>" << std::endl;
        return EXIT_FAILURE;
    }
    String content;
    {
        std::stringstream buffer;
        std::fstream input(argv[1], std::ios::in);
        buffer << input.rdbuf();
        content = buffer.str();
        input.close();
    }

    Tokenizer tokenizer(std::move(content));
    std::vector<Token> tokens = tokenizer.tokenize();

    {
        std::fstream output("out.asm", std::ios::out);
        output << tokens_to_assembly(tokens);
        output.close();
    }

    system("nasm -f elf64 -o D:\\VisualStudioCodeProjects\\Tau-Compiler\\output\\out.o out.asm"); // Compile to object file
    system("ld -o D:\\VisualStudioCodeProjects\\Tau-Compiler\\output\\out D:\\VisualStudioCodeProjects\\Tau-Compiler\\output\\out.o"); // Link object file to executable

    return EXIT_SUCCESS;
}
