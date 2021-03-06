#ifndef __TEST_TOKENIZERTESTER_HPP__
#define __TEST_TOKENIZERTESTER_HPP__

#include "../anole/compiler/compiler.hpp"

#include <gtest/gtest.h>

using namespace anole;

struct TokenizerTester : testing::Test
{
    std::stringstream ss;
    Tokenizer tokenizer{ss, "<test>"};

    virtual void SetUp()
    {
        tokenizer.reset();
        ss.clear();
    }
};

TEST_F(TokenizerTester, Origin)
{
    ss.str(R"(@
use from prefixop infixop if elif else
while do foreach as break continue return
match delay enum dict class none true false
identifier 0123456789 0123456789.0123456789 "String"
, . ... () [] {} : ; + - * / % & | ^ ~ << >>
and or not ! is = != < <= > >= => ?)");
    for (Size type = 0; type < Size(TokenType::End); ++type)
    {
        ASSERT_EQ(tokenizer.next_token().type, TokenType(type));
        if (type == Size(TokenType::Not))
        {
            ASSERT_EQ(tokenizer.next_token().type, TokenType::Not);
        }
    }
}

#endif
