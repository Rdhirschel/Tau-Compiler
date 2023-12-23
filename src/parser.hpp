#pragma once
#include <string>
#include <optional>
#include <vector>
#include <cctype>
#include <iostream>

#include "./token.hpp"

using namespace std;

typedef unsigned int uint;
template<typename T>
using Vec = std::vector<T>;
typedef string String;


struct _expression {
    Token int_literal;

};

struct ret {
    _expression expr;
};

class Parser {

public:
    inline explicit Parser(const Vec<Token> tokens) : tokens(move(tokens))
    {
    }
    optional<_expression> parse_expression() 
    {
        if (peek().has_value() && peek().value().type == TokenType::_int) 
        {
            _expression ret = { consume() };
            return ret;
        }

        return nullopt;
    }
    optional<ret> parse() 
    {
        optional<ret> retu;
        while (peek().has_value()) 
        {
            if (peek().value().type == TokenType::_return) 
            {
                consume();
                if (auto expr = parse_expression()) 
                {
                    retu = ret{ expr.value() };
                }
                else 
                {
                    cerr << "Invalid expression" << std::endl;
                    exit(EXIT_FAILURE);
                }

                if (peek().has_value() && peek().value().type == TokenType::semicolon) 
                {
                    consume();
                }
                else 
                {
                    cerr << "Expected semicolon" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        index = 0;
        return retu;
    }

private:
    const Vec<Token> tokens;
    size_t index = 0;
    inline optional<Token> peek(size_t ahead = 0) const
    {
        if (index + ahead >= tokens.size()) 
        {
            return nullopt;
        }
        return tokens[index + ahead];
    }

    inline Token consume() 
    {
        return tokens[index++];
    }
};