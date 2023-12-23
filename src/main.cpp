#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

#include "./token.hpp"
#include "./parser.hpp"
#include "./generation.hpp"

typedef unsigned int uint;
template <typename T>
using Vec = std::vector<T>;
typedef std::string String;

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

    Parser parser(move(tokens));
    optional<ret> tree = parser.parse();
    if (!tree.has_value())
    {
        std::cerr << "Not all paths return a value" << std::endl;
        return EXIT_FAILURE;
    }
    
    Generator generator(tree.value());

    {
        std::fstream output("out.asm", std::ios::out);
        output << generator.generate();
        output.close();
    }

    system("nasm -felf64 -o out.o out.asm"); // Compile to object file
    system("ld -o output out.o"); // Link object file to executable

    return EXIT_SUCCESS;
}
