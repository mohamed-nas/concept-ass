#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>

const int LETTER = 0;
const int DIGIT = 1;
const int UNKNOWN = 99;

const int INT_LIT = 10;
const int IDENT = 11;
const int ASSIGN_OP = 20;
const int ADD_OP = 21;
const int SUB_OP = 22;
const int MULT_OP = 23;
const int DIV_OP = 24;
const int LEFT_PAREN = 25;
const int RIGHT_PAREN = 26;
const int EOF_TOKEN = 98;

class LexicalAnalyzer {
private:
    int charClass;
    std::string lexeme;
    char nextChar;
    int lexLen;
    int token;
    int nextToken;
    std::istream* input;
    bool usingFile;

public:
    LexicalAnalyzer(const std::string& filename) {
        lexLen = 0;
        lexeme = "";
        usingFile = true;
        
        std::ifstream* inFile = new std::ifstream(filename);
        if (!inFile->is_open()) {
            std::cerr << "ERROR - cannot open " << filename << std::endl;
            exit(1);
        }
        
        input = inFile;
        getChar();
    }
    
    LexicalAnalyzer(const std::string& inputStr, bool isTest) {
        lexLen = 0;
        lexeme = "";
        usingFile = false;
        
        std::stringstream* ss = new std::stringstream(inputStr);
        input = ss;
        
        getChar();
    }
    
    ~LexicalAnalyzer() {
        if (usingFile) {
            static_cast<std::ifstream*>(input)->close();
        }
        delete input;
    }

    void addChar() {
        if (lexLen < 98) {
            lexeme += nextChar;
            lexLen++;
        } else {
            std::cerr << "Error - lexeme is too long" << std::endl;
        }
    }

    void getChar() {
        if (input->get(nextChar)) {
            if (std::isalpha(nextChar)) {
                charClass = LETTER;
            } else if (std::isdigit(nextChar)) {
                charClass = DIGIT;
            } else {
                charClass = UNKNOWN;
            }
        } else {
            charClass = EOF_TOKEN;
            nextChar = '\0';
        }
    }

    void getNonBlank() {
        while (std::isspace(nextChar)) {
            getChar();
        }
    }

    int lookup(char ch) {
        switch (ch) {
            case '(':
                addChar();
                nextToken = LEFT_PAREN;
                break;
            case ')':
                addChar();
                nextToken = RIGHT_PAREN;
                break;
            case '+':
                addChar();
                nextToken = ADD_OP;
                break;
            case '-':
                addChar();
                nextToken = SUB_OP;
                break;
            case '*':
                addChar();
                nextToken = MULT_OP;
                break;
            case '/':
                addChar();
                nextToken = DIV_OP;
                break;
            case '=':
                addChar();
                nextToken = ASSIGN_OP;
                break;
            default:
                addChar();
                nextToken = EOF_TOKEN;
                break;
        }
        return nextToken;
    }

    std::string getTokenName(int token) {
        switch (token) {
            case INT_LIT: return "INT_LIT";
            case IDENT: return "IDENT";
            case ASSIGN_OP: return "ASSIGN_OP";
            case ADD_OP: return "ADD_OP";
            case SUB_OP: return "SUB_OP";
            case MULT_OP: return "MULT_OP";
            case DIV_OP: return "DIV_OP";
            case LEFT_PAREN: return "LEFT_PAREN";
            case RIGHT_PAREN: return "RIGHT_PAREN";
            case EOF_TOKEN: return "EOF";
            default: return "UNKNOWN";
        }
    }

    int lex() {
        lexLen = 0;
        lexeme = "";
        getNonBlank();
        
        switch (charClass) {
            case LETTER:
                addChar();
                getChar();
                while (charClass == LETTER || charClass == DIGIT) {
                    addChar();
                    getChar();
                }
                nextToken = IDENT;
                break;
                
            case DIGIT:
                addChar();
                getChar();
                while (charClass == DIGIT) {
                    addChar();
                    getChar();
                }
                nextToken = INT_LIT;
                break;
                
            case UNKNOWN:
                lookup(nextChar);
                getChar();
                break;
                
            case EOF_TOKEN:
                nextToken = EOF_TOKEN;
                lexeme = "EOF";
                break;
        }
        
        std::cout << "Next token is: " << nextToken << " (" << getTokenName(nextToken) << ") Next lexeme is " << lexeme << std::endl;
        return nextToken;
    }

    void analyze() {
        do {
            lex();
        } while (nextToken != EOF_TOKEN);
    }
};

void testWithExpression(const std::string& expression) {
    std::cout << "\nTesting expression: \"" << expression << "\"\n";
    std::cout << "----------------------------------------\n";
    
    LexicalAnalyzer testAnalyzer(expression, true);
    testAnalyzer.analyze();
    
    std::cout << "----------------------------------------\n";
}

int main() {
    testWithExpression("(sum + 47) / total");
    testWithExpression("result = (a + b) * 2 / c");
    
    std::cout << "\nTesting complete. If you have a front.in file, uncomment the following code to process it.\n";
    
    return 0;
}
