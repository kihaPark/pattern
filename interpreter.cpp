#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <map>


// tested x64, c++17

using namespace std;

struct Token
{
    enum class Type { str, num, plus, minus, lparen, rparen } type;
    string text;

    explicit Token(Token::Type type, const string& text)
        : type{ type }, text{ text } {}

    friend ostream& operator<<(ostream& os, const Token& obj) {
        return os << "\"" << obj.text << "\"";
    }
};

bool isNumber(const string& str)
{
    return count_if(str.begin(), str.end(),
        [](unsigned char c) { return isdigit(c); });
}

vector<Token> lex(const string& str)
{
    vector<Token> result;
    regex reg("\\w+|\\+|\\-|\\(|\\)");

    const sregex_iterator end;
    for (sregex_iterator it(cbegin(str), cend(str), reg); it != end; ++it) {
        if ((*it)[0] == "+") {
            result.push_back(Token{ Token::Type::plus, "+" });
        }
        else if ((*it)[0] == "-") {
            result.push_back(Token{ Token::Type::minus, "-" });
        }
        else if ((*it)[0] == "(") {
            result.push_back(Token{ Token::Type::lparen, "(" });
        }
        else if ((*it)[0] == ")") {
            result.push_back(Token{ Token::Type::rparen, ")" });
        }
        else {
            bool isNum = isNumber((*it)[0]);
            result.push_back(Token{ (isNum ? Token::Type::num : Token::Type::str), (*it)[0] });
        }
    }

    return result;
}


struct Element
{
    virtual ~Element() = default;
    virtual int eval() const = 0;
};

struct Integer : Element
{
    int value;
    explicit Integer(const int value) : value(value) {}
    int eval() const override { return value; }
};

struct Operation : Element
{
    enum class Type { add, sub } type;
    shared_ptr<Element> lhs, rhs;

    int eval() const override {
        if (type == Operation::Type::add)
            return lhs->eval() + rhs->eval();

        return lhs->eval() - rhs->eval();
    }
};


int formulaValue(const string& str)
{
    if (str == "my_wis") {
        // find my unit wisdom stat
        return 15;
    }
    else if (str == "my_str") {
        // find my unit strength stat
        return 19;
    }
    else if (str == "basic_dmg") {
        // find content data damage info
        return 100;
    }
    else if (str == "basic_heal") {
        // find content hea damage info
        return 100;
    }

    return 0;
}

shared_ptr<Element> parse(const vector<Token>& tokens)
{
    auto result = make_shared<Operation>();

    for (int i = 0; i < tokens.size(); ++i) {
        auto& token = tokens[i];

        switch (token.type) {
        case Token::Type::str:
        case Token::Type::num:
        {
            int value = (token.type == Token::Type::num)
                ? stoi(token.text) : formulaValue(token.text);

            auto integer = make_shared<Integer>(value);

            if (result->lhs == nullptr)
                result->lhs = integer;
            else
                result->rhs = integer;
        }
        break;
        case Token::Type::plus:
        case Token::Type::minus:
        {
            // complete lhs, rhs
            if (result->rhs != nullptr) {
                auto newNode = make_shared<Operation>();
                swap(newNode, result);
                result->lhs = newNode;
            }

            result->type = (token.type == Token::Type::plus)
                ? Operation::Type::add : Operation::Type::sub;
        }
        break;
        case Token::Type::lparen:
        {
            int j = i;
            for (; j < tokens.size(); ++j) {
                if (tokens[j].type == Token::Type::rparen)
                    break;
            }

            vector<Token> subexpression(&tokens[i + 1], &tokens[j]);
            auto element = parse(subexpression);

            if (result->lhs == nullptr)
                result->lhs = element;
            else
                result->rhs = element;

            i = j;
        }
        break;
        }
    }
    return result;
}


int main()
{
    map<string, string> formulas = {
        { "item_cure", "basic_heal + 50" },
        { "magic_cure", "basic_heal + my_wis + 20" },
        { "throw_attack", "basic_dmg - 10" },
        { "melee_attack", "basic_dmg + my_str" }
    };

    for (const auto& formula : formulas) {
        auto tokens = lex(formula.second);
        auto parsed = parse(tokens);

        for (auto& t : tokens)
            cout << t << " ";

        cout << endl << formula.first << ": " << formula.second << endl;

        cout << formula.first << ": " << parsed->eval() << endl << endl;
    }

    return 0;
}

/* formula data, json format
{
    "my_wis": "15",
    "my_str": "19",
    "basic_dmg": "100",
    "basic_heal": "50",
    "item_cure": "basic_heal + 50",
    "magic_cure": "basic_heal + my_wis + 20",
    "throw_attack": "basic_dmg - 10",
    "melee_attack": "basic_dmg + my_str"
}
*/

