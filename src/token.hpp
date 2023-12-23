#pragma once

#include <string>
#include <optional>
#include <vector>
#include <cctype>
#include <iostream>

using namespace std;

typedef unsigned int uint;
template<typename T>
using Vec = std::vector<T>;
typedef string String;

enum class TokenType {
    _return,
    _int,
    semicolon
};

struct Token {
    TokenType type;
    optional<String> val;
};

class Tokenizer 
{
public:

    inline explicit Tokenizer(const String& content) : content(std::move(content))
    {
    }

    inline Vec<Token> tokenize() 
    {
        String buffer;
        Vec<Token> tokens;
        while (peek().has_value()) 
        {
            if (isalpha(peek().value())) 
            {
                buffer.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value())) 
                {
                    buffer.push_back(consume());
                }

                if (buffer == "return") {
                    tokens.push_back({TokenType::_return});
                    buffer.clear();
                    continue;
                }
            }

            else if (isdigit(peek().value())) 
            {
                buffer.push_back(consume());
                while (peek().has_value() && isdigit(peek().value())) 
                {
                    buffer.push_back(consume());
                }
                tokens.push_back({TokenType::_int, buffer});
                buffer.clear();
                continue;
            }

            else if (peek().value() == ';') 
            {
                tokens.push_back({TokenType::semicolon});
                consume();
                continue;
            }
                
            else if (isspace(peek().value())) 
            {
                consume();
                continue;
            }


            else 
            {
                cerr << "Unexpected character: " << peek().value() << endl;
                return {};
            }
        }
        index = 0;
        return tokens;
    }


private:
    String content;
    size_t index = 0;
    inline optional<char> peek(size_t ahead = 0) const
    {
        if (index + ahead >= content.length()) 
        {
            return nullopt;
        }
        return content[index + ahead];
    }

    inline char consume() 
    {
        return content[index++];
    }
};