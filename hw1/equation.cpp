using namespace std;
#include <string>
#include <regex>
#include <iostream>
#include <cmath>

extern "C" {
int toCoef(const std::string& coef) // implicitly add 1 for unspecified a or b
{
    return coef.empty() ? 1 : std::stoi(coef);
}

void parseEq(const char* equationIn, int* a, int* b, int* c)
{
    std::string equation = equationIn;
    // remove whitespace
    std::erase_if(equation,
                  [](unsigned char ch) { return std::isspace(ch) != 0; }
    );

    // normalize unicode minus sign to ascii since python passes it encoded differently
    const std::string minus = "\xE2\x88\x92";
    for (std::size_t pos = 0; (pos = equation.find(minus, pos)) != std::string::npos;)
    {
        equation.replace(pos, minus.size(), "-");
    }


    std::smatch m; // regex match results
    static const std::regex parenthesesRe(R"(([+-]?)(\d*)\(([^()]*)\)\*\*2)"); //(possibly minus)(digits)(parentheses)
    static const std::regex innerRe(R"(^([+-]?)(\d*)x(?:([+-])(\d+))?$)"); // expression inside
    static const std::regex aRe(R"(([+-]?)(\d*)x\*\*2)");
    static const std::regex bRe(R"(([+-]?)(\d*)x)");
    static const std::regex cRe(R"([+-]?\d+)");

    while (std::regex_search(equation, m, parenthesesRe))
    {
        const int outerK = (m[1].str() == "-" ? -1 : 1) * toCoef(m[2].str());
        // innerK/outerK are used to handle sings instead of trying to regex them together with the valua

        const std::string mInner = m[3].str();
        std::string innerExpr = mInner; // deepcopy
        for (std::size_t pos = 0; (pos = innerExpr.find(minus, pos)) != std::string::npos;)
        {
            innerExpr.replace(pos, minus.size(), "-");
        }

        if (std::smatch innerMatch; std::regex_match(innerExpr, innerMatch, innerRe))
        {
            int aMatch = toCoef(innerMatch[2].str());
            if (innerMatch[1].str() == "-") aMatch *= -1;

            int innerK = 0;
            if (innerMatch[3].matched)
            {
                innerK = std::stoi(innerMatch[4].str());
                if (innerMatch[3].str() == "-") innerK *= -1;
            }

            *a += outerK * aMatch * aMatch;
            *b += outerK * 2 * aMatch * innerK;
            *c += outerK * innerK * innerK;
        }

        // remove substring inside parentheses so c can be matched
        equation = m.prefix().str() + m.suffix().str();
    }

    // a
    if (std::regex_search(equation, m, aRe))
    {
        *a += (m[1].str() == "-" ? -1 : 1) * toCoef(m[2].str());
        equation = m.prefix().str() + m.suffix().str();
    }

    // b
    if (std::regex_search(equation, m, bRe))
    {
        *b += (m[1].str() == "-" ? -1 : 1) * toCoef(m[2].str());
        equation = m.prefix().str() + m.suffix().str();
    }

    // c/remaining digits
    for (std::sregex_iterator it(equation.begin(), equation.end(), cRe), end;
         it != end; ++it)
        *c += std::stoi(it->str());
}

void findMin(const char* equationIn,float* min, int* failed)
{
    int a = 0, b = 0, c = 0;
    parseEq(equationIn, &a, &b, &c);
    const float extremum = -1*b / (static_cast<float>(a)*2);

    *min = a*std::pow(extremum,2.0f)  + b*extremum +c;
    /* check if minimum or maximum was found;
     * create some dirrerent x than found result
     * obviously works only for quadratic
     */
    if (
        const float notExtremum = a*std::pow(extremum+1,2.0f)  + b*(extremum+1) +c;
        *min > notExtremum ) {
        *failed = 1;
    }
}
}
