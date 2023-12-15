#pragma once

#include <string>
#include <optional>
#include <vector>
#include <cctype>
#include <iostream>

typedef unsigned int uint;
template<typename T>
using Vec = std::vector<T>;
typedef std::string String;

enum class TokenType {
    _return,
    _int,
    semicolon
};

struct Token {
    TokenType type;
    std::optional<std::string> val;
};

class Tokenizer 
{
public:

    inline explicit Tokenizer(const std::string& content) : content(std::move(content))
    {
    }

    inline Vec<Token> tokenize() 
    {
        String buffer;
        Vec<Token> tokens;
        while (peak().has_value()) 
        {
            if (std::isalpha(peak().value())) 
            {
                buffer.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value())) 
                {
                    buffer.push_back(consume());
                }

                if (buffer == "return") {
                    tokens.push_back({TokenType::_return});
                    buffer.clear();
                    continue;
                }
            }

            else if (std::isdigit(peak().value())) 
            {
                buffer.push_back(consume());
                while (peak().has_value() && std::isdigit(peak().value())) 
                {
                    buffer.push_back(consume());
                }
                tokens.push_back({TokenType::_int, buffer});
                buffer.clear();
                continue;
            }

            else if (peak().value() == ';') 
            {
                tokens.push_back({TokenType::semicolon});
                consume();
                continue;
            }
                
            else if (std::isspace(peak().value())) 
            {
                consume();
                continue;
            }

            else 
            {
                std::cerr << "Unexpected character: " << peak().value() << std::endl;
                return {};
            }
        }

        return tokens;
    }


private:
    String content;
    uint index = 0;
    std::optional<char> peak(uint ahead = 1) const
    {
        if (index + ahead >= content.length()) 
        {
            return std::nullopt;
        }
        return content[index];
    }

    char consume() 
    {
        return content[index++];
    }
};