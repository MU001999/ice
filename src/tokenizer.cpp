#include <cctype>
#include <memory>
#include "ast.hpp"
#include "helper.hpp"
#include "tokenizer.hpp"

using namespace std;

namespace ice_language
{
Tokenizer::Tokenizer(istream &in, string name_of_in)
  : cur_line_num_(1), last_line_num_(1)
  , cur_char_at_line_(0), last_char_at_line_(0)
  , input_stream_(in), name_of_in_(move(name_of_in))
  , last_input_(' ')
{
    // ...
}

void Tokenizer::cont()
{
    last_input_ = ' ';
}

void Tokenizer::reset()
{
    cur_line_num_ = last_line_num_ = 1;
    cur_char_at_line_ = last_char_at_line_ = 0;
    cur_line_.clear(), pre_line_.clear();
    last_input_ = ' ';
}

void Tokenizer::get_next_input()
{
    last_input_ = input_stream_.get();
    if (last_input_ == EOF)
    {
        return;
    }

    if (last_input_ == '\n')
    {
        ++cur_line_num_;
        cur_char_at_line_ = 0;
        pre_line_ = cur_line_;
        cur_line_.clear();
    }
    else
    {
        ++cur_char_at_line_;
        cur_line_ += last_input_;
    }
}

Token Tokenizer::next()
{
    enum class State
    {
        Begin,

        InAT,
        InNot,
        InCLT,
        InCGT,
        InRET,

        InComment,
        InInteger,
        InDouble,
        InIdentifier,
        InString,
        InStringEscaping
    };
    auto state = State::Begin;
    while (isspace(last_input_))
    {
        get_next_input();
    }

    last_char_at_line_ = cur_char_at_line_;
    last_line_num_ = cur_line_num_;

    shared_ptr<Token> token = nullptr;
    string value;
    while (!token)
    {
        switch (state)
        {
        case State::Begin:
            switch (last_input_)
            {
            case '$':
                token = make_shared<Token>(TokenType::End);
                break;

            case '#':
                state = State::InComment;
                break;

            case '@':
                state = State::InAT;
                break;

            case '"':
                state = State::InString;
                break;

            case ':':
                token = make_shared<Token>(TokenType::Colon);
                break;

            case ';':
                token = make_shared<Token>(TokenType::Semicolon);
                break;

            case ',':
                token = make_shared<Token>(TokenType::Comma);
                break;

            case '.':
                token = make_shared<Token>(TokenType::Dot);
                break;

            case '(':
                token = make_shared<Token>(TokenType::LParen);
                break;

            case ')':
                token = make_shared<Token>(TokenType::RParen);
                break;

            case '[':
                token = make_shared<Token>(TokenType::LBracket);
                break;

            case ']':
                token = make_shared<Token>(TokenType::RBracket);
                break;

            case '{':
                token = make_shared<Token>(TokenType::LBrace);
                break;

            case '}':
                token = make_shared<Token>(TokenType::RBrace);
                break;

            case '+':
                token = make_shared<Token>(TokenType::Add);
                break;

            case '-':
                token = make_shared<Token>(TokenType::Sub);
                break;

            case '*':
                token = make_shared<Token>(TokenType::Mul);
                break;

            case '/':
                token = make_shared<Token>(TokenType::Div);
                break;

            case '%':
                token = make_shared<Token>(TokenType::Mod);
                break;

            case '&':
                token = make_shared<Token>(TokenType::BAnd);
                break;

            case '|':
                token = make_shared<Token>(TokenType::BOr);
                break;

            case '^':
                token = make_shared<Token>(TokenType::BXor);
                break;

            case '~':
                token = make_shared<Token>(TokenType::BNeg);
                break;

            case '?':
                token = make_shared<Token>(TokenType::Ques);
                break;

            case '=':
                state = State::InRET;
                break;

            case '!':
                state = State::InNot;
                break;

            case '<':
                state = State::InCLT;
                break;

            case '>':
                state = State::InCGT;
                break;

            default:
                if (isdigit(last_input_))
                {
                    state = State::InInteger;
                    value += last_input_;
                }
                else if (last_input_ == '_' || isalpha(last_input_))
                {
                    state = State::InIdentifier;
                    value += last_input_;
                }
                break;
            }
            break;

        case State::InAT:
            switch (last_input_)
            {
            case '@':
                token = make_shared<Token>(TokenType::AtAt);
                break;

            default:
                return Token(TokenType::At);
            }
            break;

        case State::InRET:
            switch (last_input_)
            {
            case '>':
                token = make_shared<Token>(TokenType::Ret);
                break;

            default:
                return Token(TokenType::CEQ);
            }
            break;

        case State::InNot:
            switch (last_input_)
            {
            case '=':
                token = make_shared<Token>(TokenType::CNE);
                break;

            default:
                return Token(TokenType::Not);
            }
            break;

        case State::InCLT:
            switch (last_input_)
            {
            case '=':
                token = make_shared<Token>(TokenType::CLE);
                break;

            case '<':
                token = make_shared<Token>(TokenType::BLS);
                break;

            default:
                return Token(TokenType::CLT);
            }
            break;

        case State::InCGT:
            switch (last_input_)
            {
            case '=':
                token = make_shared<Token>(TokenType::CGE);
                break;

            case '>':
                token = make_shared<Token>(TokenType::BRS);
                break;

            default:
                return Token(TokenType::CGT);
            }
            break;

        case State::InInteger:
            switch (last_input_)
            {
            case '.':
                value += '.';
                state = State::InDouble;
                break;

            default:
                if (isdigit(last_input_))
                {
                    value += last_input_;
                }
                else
                {
                    return Token(TokenType::Integer, value);
                }
                break;
            }
            break;

        case State::InDouble:
            if (isdigit(last_input_))
            {
                value += last_input_;
            }
            else
            {
                return Token(TokenType::Double, value);
            }
            break;

        case State::InIdentifier:
            if (last_input_ == '_' || isalnum(last_input_))
            {
                value += last_input_;
            }
            else
            {
                return Token(value);
            }
            break;

        case State::InString:
            switch (last_input_)
            {
            case '\n':
                cout << "syntax error!" << endl;
                exit(0);

            case '\\':
                state = State::InStringEscaping;
                break;

            case '"':
                token = make_shared<Token>(TokenType::String, value);
                break;

            default:
                value += last_input_;
                break;
            }
            break;

        case State::InStringEscaping:
            switch (last_input_)
            {
            case '\n':
                cout << "syntax error!" << endl;
                exit(0);

            case 'n':
                value += '\n';
                break;

            case '\\':
                value += '\\';
                break;

            case '\"':
                value += '\"';
                break;

            case 'a':
                value += '\a';
                break;

            case 'b':
                value += '\b';
                break;

            case '0':
                value += '\0';
                break;

            case 't':
                value += '\t';
                break;

            case 'r':
                value += '\r';
                break;

            case 'f':
                value += '\f';
                break;

            default:
                value += '\\';
                value += last_input_;
                break;
            }
            state = State::InString;
            break;

        case State::InComment:
            while (last_input_ != '\n')
            {
                get_next_input();
            }
            return next();

        default:
            break;
        }
        if (last_input_ == EOF)
        {
            break;
        }
        get_next_input();
    }
    if (!token)
    {
        token = make_shared<Token>(TokenType::End);
    }
    return *token;
}

std::string Tokenizer::get_err_info(const string &message)
{
    auto line = (cur_line_num_ != last_line_num_)
        ? pre_line_ : cur_line_;

    return
        info::strong(name_of_in_ + ":"
            + to_string(last_line_num_) + ":"
            + to_string(last_char_at_line_) + ": ") +
        info::warning("error: ") + message + "\n" +
        line + "\n" +
        string(last_char_at_line_ == 0 ? 0 : last_char_at_line_ - 1, ' ')
            + info::warning("^");
}
}
