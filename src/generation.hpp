#pragma once

using namespace std;

#include <string>
#include <optional>
#include <vector>
#include <cctype>
#include <iostream>
#include <sstream>
#include <./parser.hpp>

typedef unsigned int uint;
template<typename T>
using Vec = std::vector<T>;
typedef string String;

class Generator {
public:
    inline Generator(ret root) : root(std::move(root))
    {
    }

    String generate() const 
    {
        stringstream assembly;
        assembly << "global _start\n_start:\n";
        assembly << "move rax, 60\n";
        assembly << "mov rdi, " << root.expr.int_literal.val.value() << "\n";
        assembly << "syscall\n";

        return assembly.str();
    }

private:
    const ret root;
};