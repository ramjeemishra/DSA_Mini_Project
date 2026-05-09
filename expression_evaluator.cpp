#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
using namespace std;

const int MAX_STACK_SIZE = 100;   
const int MAX_EXPR_LEN   = 256;  
const int MAX_HISTORY    = 50;   

class CharStack {
private:
    char data[MAX_STACK_SIZE];
    int  top;                  

public:
    CharStack() {
        top = -1;
    }

    bool push(char ch) {
        if (isFull()) {
            cout << "  [Error] Stack Overflow — cannot push '" << ch << "'\n";
            return false;
        }
        data[++top] = ch;
        return true;
    }

    char pop() {
        if (isEmpty()) {
            cout << "  [Error] Stack Underflow — nothing to pop\n";
            return '\0';
        }
        return data[top--];
    }

    char peek() const {
        if (isEmpty()) return '\0';
        return data[top];
    }

    bool isEmpty() const {
        return (top == -1);
    }

    bool isFull() const {
        return (top == MAX_STACK_SIZE - 1);
    }

    void display() const {
        if (isEmpty()) {
            cout << "  Stack is empty.\n";
            return;
        }
        cout << "  Stack (bottom -> top): ";
        for (int i = 0; i <= top; i++) {
            cout << data[i] << " ";
        }
        cout << "\n";
    }
};

class DoubleStack {
private:
    double data[MAX_STACK_SIZE]; 
    int    top;                  

public:
    DoubleStack() {
        top = -1;
    }

    bool push(double val) {
        if (isFull()) {
            cout << "  [Error] Stack Overflow — cannot push value\n";
            return false;
        }
        data[++top] = val;
        return true;
    }

    double pop() {
        if (isEmpty()) {
            cout << "  [Error] Stack Underflow — nothing to pop\n";
            return 0;
        }
        return data[top--];
    }

    double peek() const {
        if (isEmpty()) return 0;
        return data[top];
    }

    bool isEmpty() const {
        return (top == -1);
    }

    bool isFull() const {
        return (top == MAX_STACK_SIZE - 1);
    }
    void display() const {
        if (isEmpty()) {
            cout << "  Stack is empty.\n";
            return;
        }
        cout << "  Stack (bottom -> top): ";
        for (int i = 0; i <= top; i++) {
            cout << data[i] << " ";
        }
        cout << "\n";
    }
};

struct HistoryEntry {
    char expression[MAX_EXPR_LEN];
    char result[MAX_EXPR_LEN];    
    char type[64];                
};

class HistoryManager {
private:
    HistoryEntry entries[MAX_HISTORY]; 
    int count;                         

public:
    HistoryManager() {
        count = 0;
    }

    void addEntry(const char* expr, const char* res, const char* opType) {
        if (count >= MAX_HISTORY) {
            for (int i = 1; i < MAX_HISTORY; i++) {
                entries[i - 1] = entries[i];
            }
            count = MAX_HISTORY - 1;
        }
        strncpy(entries[count].expression, expr, MAX_EXPR_LEN - 1);
        strncpy(entries[count].result,     res,  MAX_EXPR_LEN - 1);
        strncpy(entries[count].type,       opType, 63);
        count++;
    }

    void displayHistory() const {
        if (count == 0) {
            cout << "\n  No history yet.\n";
            return;
        }
        cout << "EXPRESSION HISTORY\n";
        cout << "# | Type | Expression | Result\n";
        for (int i = 0; i < count; i++) {
            cout << (i + 1) << " | ";
            int tLen = strlen(entries[i].type);
            cout << entries[i].type;
            for (int p = tLen; p < 20; p++) cout << " ";
            cout << " | ";
            int eLen = strlen(entries[i].expression);
            int ePrint = (eLen > 17) ? 17 : eLen;
            for (int p = 0; p < ePrint; p++) cout << entries[i].expression[p];
            for (int p = ePrint; p < 17; p++) cout << " ";
            cout << " | ";
            int rLen = strlen(entries[i].result);
            int rPrint = (rLen > 16) ? 16 : rLen;
            for (int p = 0; p < rPrint; p++) cout << entries[i].result[p];
            for (int p = rPrint; p < 16; p++) cout << " ";
            cout << "\n";
        }
    }

    bool removeLast(char* outExpr, char* outRes, char* outType) {
        if (count == 0) return false;
        count--;
        strncpy(outExpr, entries[count].expression, MAX_EXPR_LEN - 1);
        strncpy(outRes,  entries[count].result,     MAX_EXPR_LEN - 1);
        strncpy(outType, entries[count].type,        63);
        return true;
    }

    int getCount() const { return count; }
};

class UndoManager {
private:
    HistoryEntry undoStack[MAX_HISTORY]; 
    int top;                             

public:
    UndoManager() {
        top = -1;
    }

    void pushUndo(const char* expr, const char* res, const char* opType) {
        if (top >= MAX_HISTORY - 1) {
            cout << "  [Warning] Undo stack is full.\n";
            return;
        }
        top++;
        strncpy(undoStack[top].expression, expr, MAX_EXPR_LEN - 1);
        strncpy(undoStack[top].result,     res,  MAX_EXPR_LEN - 1);
        strncpy(undoStack[top].type,       opType, 63);
    }

    bool isEmpty() const { return (top == -1); }

    void displayLastUndo() const {
        if (isEmpty()) {
            cout << "  Nothing to display.\n";
            return;
        }
        cout << "  Last undone: [" << undoStack[top].type << "] "
             << undoStack[top].expression << " -> "
             << undoStack[top].result << "\n";
    }
};

bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

bool isOperand(char ch) {
    return ((ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= '0' && ch <= '9'));
}

int precedence(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^':           return 3;
        default:            return 0;
    }
}

bool isLeftAssociative(char op) {
    return (op != '^');
}

void reverseString(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void removeSpaces(char* str) {
    int j = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

bool validateExpression(const char* expr) {
    int len = strlen(expr);
    if (len == 0) {
        cout << "  [Error] Expression is empty.\n";
        return false;
    }

    for (int i = 0; i < len; i++) {
        char ch = expr[i];
        if (!isOperand(ch) && !isOperator(ch) &&
            ch != '(' && ch != ')' &&
            ch != '{' && ch != '}' &&
            ch != '[' && ch != ']' &&
            ch != '.' && ch != ' ') {
            cout << "  [Error] Invalid character '" << ch
                 << "' found in expression.\n";
            return false;
        }
    }
    return true;
}

bool checkBalancedParentheses(const char* expr) {
    CharStack stk;
    int len = strlen(expr);

    for (int i = 0; i < len; i++) {
        char ch = expr[i];

        if (ch == '(' || ch == '{' || ch == '[') {
            stk.push(ch);
        }
        else if (ch == ')' || ch == '}' || ch == ']') {
            if (stk.isEmpty()) return false;

            char top = stk.pop();
            if ((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '[')) {
                return false;
            }
        }
    }

    return stk.isEmpty();
}

bool infixToPostfix(const char* infix, char* postfix) {
    CharStack stk;
    int j = 0;
    int len = strlen(infix);

    for (int i = 0; i < len; i++) {
        char ch = infix[i];

        if (isOperand(ch)) {
            postfix[j++] = ch;
        }
        else if (ch == '(' || ch == '[' || ch == '{') {
            stk.push('('); 
        }
        else if (ch == ')' || ch == ']' || ch == '}') {
            while (!stk.isEmpty() && stk.peek() != '(') {
                postfix[j++] = stk.pop();
            }
            if (stk.isEmpty()) {
                cout << "  [Error] Mismatched parentheses.\n";
                return false;
            }
            stk.pop(); 
        }
        else if (isOperator(ch)) {
            while (!stk.isEmpty() && stk.peek() != '(' &&
                   (precedence(stk.peek()) > precedence(ch) ||
                    (precedence(stk.peek()) == precedence(ch) &&
                     isLeftAssociative(ch)))) {
                postfix[j++] = stk.pop();
            }
            stk.push(ch);
        }
    }

    while (!stk.isEmpty()) {
        if (stk.peek() == '(') {
            cout << "  [Error] Mismatched parentheses.\n";
            return false;
        }
        postfix[j++] = stk.pop();
    }

    postfix[j] = '\0';
    return true;
}

bool evaluatePostfix(const char* postfix, double &result) {
    DoubleStack stk;
    int len = strlen(postfix);

    for (int i = 0; i < len; i++) {
        char ch = postfix[i];

        if (ch >= '0' && ch <= '9') {
            stk.push((double)(ch - '0'));
        }
        else if (isOperator(ch)) {
            if (stk.isEmpty()) {
                cout << "  [Error] Invalid postfix expression.\n";
                return false;
            }
            double b = stk.pop(); 

            if (stk.isEmpty()) {
                cout << "  [Error] Invalid postfix expression.\n";
                return false;
            }
            double a = stk.pop(); 

            double res = 0;
            switch (ch) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/':
                    if (b == 0) {
                        cout << "  [Error] Division by zero!\n";
                        return false;
                    }
                    res = a / b;
                    break;
                case '^':
                    res = pow(a, b);
                    break;
            }
            stk.push(res);
        }
        else {
            cout << "  [Error] Invalid character '" << ch
                 << "' in postfix expression.\n";
            return false;
        }
    }

    result = stk.pop();
    if (!stk.isEmpty()) {
        cout << "  [Error] Invalid postfix expression (extra operands).\n";
        return false;
    }
    return true;
}

bool infixToPrefix(const char* infix, char* prefix) {
    char reversed[MAX_EXPR_LEN];
    strncpy(reversed, infix, MAX_EXPR_LEN - 1);
    reversed[MAX_EXPR_LEN - 1] = '\0';
    int len = strlen(reversed);
    reverseString(reversed);

    for (int i = 0; i < len; i++) {
        if (reversed[i] == '(')      reversed[i] = ')';
        else if (reversed[i] == ')') reversed[i] = '(';
        else if (reversed[i] == '[') reversed[i] = ']';
        else if (reversed[i] == ']') reversed[i] = '[';
        else if (reversed[i] == '{') reversed[i] = '}';
        else if (reversed[i] == '}') reversed[i] = '{';
    }

    CharStack stk;
    int j = 0;

    for (int i = 0; i < len; i++) {
        char ch = reversed[i];

        if (isOperand(ch)) {
            prefix[j++] = ch;
        }
        else if (ch == '(' || ch == '[' || ch == '{') {
            stk.push('(');
        }
        else if (ch == ')' || ch == ']' || ch == '}') {
            while (!stk.isEmpty() && stk.peek() != '(') {
                prefix[j++] = stk.pop();
            }
            if (stk.isEmpty()) {
                cout << "  [Error] Mismatched parentheses.\n";
                return false;
            }
            stk.pop();
        }
        else if (isOperator(ch)) {
            while (!stk.isEmpty() && stk.peek() != '(' &&
                   precedence(stk.peek()) > precedence(ch)) {
                prefix[j++] = stk.pop();
            }
            stk.push(ch);
        }
    }

    while (!stk.isEmpty()) {
        if (stk.peek() == '(') {
            cout << "  [Error] Mismatched parentheses.\n";
            return false;
        }
        prefix[j++] = stk.pop();
    }
    prefix[j] = '\0';

    reverseString(prefix);
    return true;
}

void printBanner() {
    cout << "\n";
    cout << "EXPRESSION EVALUATOR USING STACK\n";
    cout << "DSA Mini Project\n";
    cout << "\n";
}

void printMenu() {
    cout << "MAIN MENU\n";
    cout << "1. Check Balanced Parentheses\n";
    cout << "2. Convert Infix to Postfix\n";
    cout << "3. Evaluate Postfix Expression\n";
    cout << "4. Convert Infix to Prefix\n";
    cout << "5. View Expression History\n";
    cout << "6. Undo Last Operation\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}

void printSeparator() {
    cout << "---------------------------------------------------\n";
}

int main() {
    HistoryManager history;
    UndoManager    undoMgr;

    char input[MAX_EXPR_LEN];
    int  choice;

    printBanner();

    while (true) {
        printMenu();
        cin >> choice;
        cin.ignore(256, '\n');

        switch (choice) {

        case 1: {
            printSeparator();
            cout << "  [Balanced Parentheses Checker]\n";
            cout << "  Enter expression: ";
            cin.getline(input, MAX_EXPR_LEN);

            if (!validateExpression(input)) break;

            bool balanced = checkBalancedParentheses(input);
            if (balanced) {
                cout << "\n  Result: BALANCED ✓\n";
                history.addEntry(input, "Balanced", "Paren Check");
            } else {
                cout << "\n  Result: NOT BALANCED ✗\n";
                history.addEntry(input, "Not Balanced", "Paren Check");
            }
            printSeparator();
            break;
        }

        case 2: {
            printSeparator();
            cout << "  [Infix to Postfix Converter]\n";
            cout << "  Enter infix expression: ";
            cin.getline(input, MAX_EXPR_LEN);
            removeSpaces(input);

            if (!validateExpression(input)) break;
            if (!checkBalancedParentheses(input)) {
                cout << "  [Error] Unbalanced parentheses.\n";
                break;
            }

            char postfix[MAX_EXPR_LEN];
            if (infixToPostfix(input, postfix)) {
                cout << "\n  Infix   : " << input   << "\n";
                cout << "  Postfix : " << postfix << "\n";
                history.addEntry(input, postfix, "Infix->Postfix");
            }
            printSeparator();
            break;
        }

        case 3: {
            printSeparator();
            cout << "  [Postfix Expression Evaluator]\n";
            cout << "  Enter postfix expression: ";
            cin.getline(input, MAX_EXPR_LEN);
            removeSpaces(input);

            double result;
            if (evaluatePostfix(input, result)) {
                char resStr[64];
                snprintf(resStr, 64, "%.4f", result);

                cout << "\n  Postfix : " << input  << "\n";
                cout << "  Result  : " << result << "\n";
                history.addEntry(input, resStr, "Postfix Eval");
            }
            printSeparator();
            break;
        }

        case 4: {
            printSeparator();
            cout << "  [Infix to Prefix Converter]\n";
            cout << "  Enter infix expression: ";
            cin.getline(input, MAX_EXPR_LEN);
            removeSpaces(input);

            if (!validateExpression(input)) break;
            if (!checkBalancedParentheses(input)) {
                cout << "  [Error] Unbalanced parentheses.\n";
                break;
            }

            char prefix[MAX_EXPR_LEN];
            if (infixToPrefix(input, prefix)) {
                cout << "\n  Infix  : " << input  << "\n";
                cout << "  Prefix : " << prefix << "\n";
                history.addEntry(input, prefix, "Infix->Prefix");
            }
            printSeparator();
            break;
        }

        case 5: {
            printSeparator();
            history.displayHistory();
            printSeparator();
            break;
        }

        case 6: {
            printSeparator();
            char uExpr[MAX_EXPR_LEN], uRes[MAX_EXPR_LEN], uType[64];
            if (history.removeLast(uExpr, uRes, uType)) {
                undoMgr.pushUndo(uExpr, uRes, uType);
                cout << "  Undo successful!\n";
                cout << "  Removed: [" << uType << "] "
                     << uExpr << " -> " << uRes << "\n";
            } else {
                cout << "  Nothing to undo.\n";
            }
            printSeparator();
            break;
        }

        case 7: {
            cout << "\n  Thank you for using Expression Evaluator!\n";
            cout << "  Goodbye.\n\n";
            return 0;
        }

        default:
            cout << "  [Error] Invalid choice. Please enter 1-7.\n";
            break;
        }

        cout << "\n";
    }

    return 0;
}
