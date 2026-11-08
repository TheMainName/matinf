#include <cmath>
#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

enum TokenType {
    Number,
    X,
    Plus,
    Minus,
    Multiply,
    Power,
    Right,
    Left,
    End
};

struct Token {
    TokenType type;
    long double value{};
};

class Tokenizer {
public:
    // Construct the tokenizer and eagerly tokenize the provided expression string.
    explicit Tokenizer(const std::string &expression)
        : expr(expression), length(expr.size()) {
        tokenize();
    }

    // Return the generated sequence of tokens so the parser can consume them.
    const std::vector<Token> &tokensList() const { return tokens; }

private:
    // Scan the input string and emit tokens, inserting implicit multiplication where needed.
    void tokenize() {
        Token previous{TokenType::End, 0.0L};

        while (position < length) {
            skipWhitespace();
            if (position >= length) {
                break;
            }

            char ch = expr[position];

            Token current;

            if (std::isdigit(static_cast<unsigned char>(ch))) {
                current = parseNumber();
            } else if (ch == 'x' || ch == 'X') {
                ++position;
                current = {TokenType::X, 0.0L};
            } else if (ch == '+') {
                ++position;
                current = {TokenType::Plus, 0.0L};
            } else if (ch == '-') {
                ++position;
                current = {TokenType::Minus, 0.0L};
            } else if (ch == '*') {
                if (position + 1 < length && expr[position + 1] == '*') {
                    position += 2;
                    current = {TokenType::Power, 0.0L};
                } else {
                    ++position;
                    current = {TokenType::Multiply, 0.0L};
                }
            } else if (ch == '(') {
                ++position;
                current = {TokenType::Right, 0.0L};
            } else if (ch == ')') {
                ++position;
                current = {TokenType::Left, 0.0L};
            } else {
                throw std::runtime_error("Unsupported character in expression");
            }

            if (!tokens.empty() && needsImplicitMultiply(tokens.back().type, current.type)) {
                tokens.push_back({TokenType::Multiply, 0.0L});
            } else if (tokens.empty() && needsImplicitMultiply(previous.type, current.type)) {
                tokens.push_back({TokenType::Multiply, 0.0L});
            }

            tokens.push_back(current);
            previous = current;
        }

        tokens.push_back({TokenType::End, 0.0L});
    }

    // Parse a sequence of digits into an integer-valued numeric token.
    Token parseNumber() {
        size_t start = position;
        while (position < length && std::isdigit(static_cast<unsigned char>(expr[position]))) {
            ++position;
        }
        long long value = std::stoll(expr.substr(start, position - start));
        return {TokenType::Number, static_cast<long double>(value)};
    }

    // Skip any whitespace characters between lexical tokens.
    void skipWhitespace() {
        while (position < length && std::isspace(static_cast<unsigned char>(expr[position]))) {
            ++position;
        }
    }

    // Determine whether to insert an implicit multiplication token between two adjacent tokens.
    static bool needsImplicitMultiply(TokenType prev, TokenType current) {
        const bool prevAllows = prev == TokenType::Number || prev == TokenType::X || prev == TokenType::Left;
        const bool currentAllows = current == TokenType::Number || current == TokenType::X || current == TokenType::Right;
        return prevAllows && currentAllows;
    }

    const std::string &expr;
    size_t position = 0;
    size_t length;
    std::vector<Token> tokens;
};

class Parser {
public:
    // Construct the parser with a token stream and a numeric value to substitute for x.
    Parser(const std::vector<Token> &tokens, long double xValue)
        : tokens(tokens), x(xValue) {}

    // Entry point for parsing a full expression respecting operator precedence.
    long double parseExpression() {
        return parseAddSubtract();
    }

private:
    // Handle addition and subtraction operators with left-to-right evaluation.
    long double parseAddSubtract() {
        long double value = parseMultiply();
        while (true) {
            if (match(TokenType::Plus)) {
                value += parseMultiply();
            } else if (match(TokenType::Minus)) {
                value -= parseMultiply();
            } else {
                break;
            }
        }
        return value;
    }

    // Handle multiplication operators.
    long double parseMultiply() {
        long double value = parsePower();
        while (true) {
            if (match(TokenType::Multiply)) {
                value *= parsePower();
            } else {
                break;
            }
        }
        return value;
    }

    // Handle the right-associative power operator (**).
    long double parsePower() {
        long double base = parseUnary();
        if (match(TokenType::Power)) {
            long double exponent = parsePower();
            return std::pow(base, exponent);
        }
        return base;
    }

    // Handle unary prefix plus and minus operators.
    long double parseUnary() {
        if (match(TokenType::Plus)) {
            return parseUnary();
        }
        if (match(TokenType::Minus)) {
            return -parseUnary();
        }
        return parsePrimary();
    }

    // Parse literals, variables, and parenthesized subexpressions.
    long double parsePrimary() {
        if (match(TokenType::Number)) {
            return previous().value;
        }
        if (match(TokenType::X)) {
            return x;
        }
        if (match(TokenType::Right)) {
            long double value = parseAddSubtract();
            expect(TokenType::Left);
            return value;
        }
        throw std::runtime_error("Unexpected token while parsing expression");
    }

    // Consume a token if it matches the expected type and advance the cursor.
    bool match(TokenType type) {
        if (tokens[index].type == type) {
            previousToken = tokens[index];
            ++index;
            return true;
        }
        return false;
    }

    // Require that the next token matches the given type, throwing if it does not.
    void expect(TokenType type) {
        if (!match(type)) {
            throw std::runtime_error("Mismatched parentheses or incomplete expression");
        }
    }

    // Return the most recently consumed token.
    const Token &previous() const { return previousToken; }

    const std::vector<Token> &tokens;
    size_t index = 0;
    Token previousToken{TokenType::End, 0.0L};
    long double x;
};

// Evaluate the expression by tokenizing and parsing after substituting a numeric value for x.
long double evaluateExpression(const std::string &expression, long double xValue) {
    Tokenizer tokenizer(expression);
    Parser parser(tokenizer.tokensList(), xValue);
    return parser.parseExpression();
}

// Round a long double result to the nearest integer.
inline int roundToInt(long double value) {
    return static_cast<int>(std::llround(value));
}

} // namespace

// Parse the quadratic expression and extract integer coefficients a, b, and c.
// Numbers in the input are assumed to be integers, so evaluating at x = 0, 1, 2
// is sufficient to recover the coefficients exactly.
extern "C" void parse_eq(const char *expression, int *a, int *b, int *c) {
    if (a == nullptr || b == nullptr || c == nullptr) {
        return;
    }

    try {
        if (expression == nullptr) {
            *a = 0;
            *b = 0;
            *c = 0;
            return;
        }

        const std::string expr(expression);

        const long double valueAt0 = evaluateExpression(expr, 0.0L);
        const long double valueAt1 = evaluateExpression(expr, 1.0L);
        const long double valueAt2 = evaluateExpression(expr, 2.0L);

        const long double cValue = valueAt0;
        const long double v1 = valueAt1 - cValue;
        const long double v2 = valueAt2 - cValue;

        const long double aValue = (v2 - 2.0L * v1) / 2.0L;
        const long double bValue = v1 - aValue;

        *a = roundToInt(aValue);
        *b = roundToInt(bValue);
        *c = roundToInt(cValue);
    } catch (...) {
        *a = 0;
        *b = 0;
        *c = 0;
    }
}
