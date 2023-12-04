#include <iostream>
#include <stack>
#include <vector>
#include <cctype>
#include <cmath>

using namespace std;
// Function to check if a character is an operator
class ExpressionEvaluator {
public://initialize public variables
    double evaluate(const string& expression);//evaluate function

private://initialize private variables
    vector<string> tokenize(const string& expression);//tokenize function
    bool isOperator(const string& token);//isOperator function
    int getPrecedence(const string& op);//getPrecedence function
    vector<string> handleParentheses(const vector<string>& tokens); //handleParentheses function
    double evaluateExpression(const vector<string>& tokens); //evaluateExpression function
};
//tokenize the equation and getting the values and the operators from user input
vector<string> ExpressionEvaluator::tokenize(const string& expression) {
    vector<string> tokens;//initialize vector
    string currentToken;//initialize currentToken
//checks how long the input is, check if i is < len(expression), increment i
  //iterates through everything in the expression
    for (size_t i = 0; i < expression.length(); ++i) {//iterates through expression
        char ch = expression[i];//initialize ch
//checks if the character is a digit, if it is, add it to the current token
        if (isspace(ch)) {
            continue; //skips whitespace
          //checks if the character is a digit or a decimal point, if it is, add it to the current token
        }
        //checks if the character is a digit or a decimal point, if it is, add it to the current token
        else if (isdigit(ch) || ch == '.') {//checks if the character is a digit or a decimal point
            currentToken += ch;//adds the character to the current token
        } else if (isOperator(string(1, ch))) {//checks if the character is an operator
            if (!currentToken.empty()) {//checks if the current token is not empty
                tokens.push_back(currentToken);//adds the current token to the tokens vector
                currentToken.clear();//clears the current token
            }

            // Check for unary operators
            if ((i == 0 || expression[i - 1] == '(') && (ch == '+' || ch == '-')) {
                currentToken += ch; // Treat as part of the number
            } else {
                tokens.push_back(string(1, ch));//adds the character to the tokens vector
            }
        } else if (ch == '(' || ch == ')') {//checks if the character is a parenthesis
            if (!currentToken.empty()) {//checks if the current token is not empty
                tokens.push_back(currentToken);//adds the current token to the tokens vector
                currentToken.clear();//clears the current token
            }
            tokens.push_back(string(1, ch));//adds the character to the tokens vector
        } else {
            cerr << "Error: Invalid character in expression\n";//error message
            exit(1);//exits the program
        }
    }
  //checks if the current token is not empty, if it is, add it to the vector
    if (!currentToken.empty()) {//checks if the current token is not empty
        tokens.push_back(currentToken);//adds the current token to the tokens vector
    }

    return tokens;//returns the tokens vector
}
//checks if the token is an operator
bool ExpressionEvaluator::isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";//if the toke is an operator, then it will return what it is
}
//checks the precedence of the operator
int ExpressionEvaluator::getPrecedence(const string& op) {//getPrecedence function
    if (op == "^") {//checks if the operator is ^
        return 3;//returns 3
    } else if (op == "*" || op == "/" || op == "%") {//checks if the operator is *, /, or %
        return 2;//returns 2
    } else if (op == "+" || op == "-") {//checks if the operator is + or -
        return 1;//returns 1
    } else {
        return 0;//returns 0
    }
}
//checks if the token is a parenthesis
//if it is a parenthesis, it will return the tokens vector
vector<string> ExpressionEvaluator::handleParentheses(const vector<string>& tokens) {
    stack<string> operatorStack;//initialize operatorStack
    vector<string> postfixExpression;//initialize postfixExpression
//iterates through the tokens
    for (size_t i = 0; i < tokens.size(); ++i) {
        const string& token = tokens[i];//initialize token

        // Check for unary operators
        if (isOperator(token) && (i == 0 || tokens[i - 1] == "(")) {//checks if the token is an operator and the previous token is a parenthesis
            postfixExpression.push_back("0"); // Add a placeholder for the unary operator's operand
        }
        if (isdigit(token[0]) || (token.length() > 1 && (token[0] == '-' || token[0] == '+') && isdigit(token[1]))) {//checks if the token is a digit or a decimal point
            postfixExpression.push_back(token);//adds the token to the postfixExpression
        } else if (token == "(") {//checks if the token is a left parenthesis
            operatorStack.push(token);//adds the token to the operatorStack
        } else if (token == ")") {//checks if the token is a right parenthesis
            while (!operatorStack.empty() && operatorStack.top() != "(") {//checks if the operatorStack is not empty and the top of the operatorStack is not a parenthesis
                postfixExpression.push_back(operatorStack.top());//adds the operator to the postfixExpression
                operatorStack.pop();//pops the operator from the operatorStack
            }
            if (!operatorStack.empty() && operatorStack.top() == "(") {//checks if the operatorStack is not empty and the top of the operatorStack is a parenthesis
                operatorStack.pop(); // Discard the open parenthesis
            } else {//if the stack is empty, throw an error
                cerr << "Error: Mismatched parentheses\n";//error message
                exit(1);//exits the program
            }
        } else if (isOperator(token)) {//checks if the token is an operator
            while (!operatorStack.empty() && getPrecedence(operatorStack.top()) >= getPrecedence(token)) {//checks if the operatorStack is not empty and the top of the operatorStack is greater than or equal to the
                postfixExpression.push_back(operatorStack.top());//adds the operator to the postfix expression
                operatorStack.pop(); //pops the operator from the operatorStack
            }
            operatorStack.push(token); //adds the token to the operator stack
        } else {//if the token is not an operator, it will return an error
            cerr << "Error: Invalid token in expression\n";//error message
            exit(1); //exits the program
        }
    }

    while (!operatorStack.empty()) {//checks if the operator stack is not empty
        if (operatorStack.top() == "(" || operatorStack.top() == ")") {//checks if the top of the stack is a parenthesis
            cerr << "Error: Mismatched parentheses\n";//error message
            exit(1); //checks if the parenthesis are matched
        }
        postfixExpression.push_back(operatorStack.top());//adds the remaining operators to the postfix expression
        operatorStack.pop(); // Discard the remaining operators
    }

    return postfixExpression; //returns the postfix expression
}

//this function evaluates the expression
double ExpressionEvaluator::evaluateExpression(const vector<string>& tokens) {
    stack<double> operandStack; //Stack to hold operands

    // Iterate through the tokens
    for (const string& token : tokens) {
        // Check if the first part of the token is a number or a number with + or - in front
        if (isdigit(token[0]) || (token.length() > 1 && (token[0] == '-' || token[0] == '+') && isdigit(token[1]))) {
            operandStack.push(stod(token)); // Push the number onto the stack
        } else if (isOperator(token)) { // Check if the token is an operator
            if (operandStack.size() < 2) { // Check if there are enough operands on the stack
                cerr << "Error: Insufficient operands for operator\n";
                exit(1);
            }
            double operand2 = operandStack.top(); //Get the second operand and remove it from the stack
            operandStack.pop();
            double operand1 = operandStack.top(); //Get the first operand and remove it from the stack
            operandStack.pop();

            double result; // Variable to store the result of the operation

            // Check for invalid operators without operands on the left
          if ((token == "*" || token == "^" || token == "%" || token == "^") && operand1 == 0) {
              cerr << "Explanation: The " << token << " operator lacks operands on the left, making the expression invalid.\n";
              exit(1);
          }

            if (token == "+") { // Addition
                result = operand1 + operand2;
            } else if (token == "-") { // Subtraction
                result = operand1 - operand2;
            } else if (token == "*") { // Multiplication
                result = operand1 * operand2;
            } else if (token == "/") { // Division
                if (operand2 == 0) {
                    cerr << "Error: Division by zero\n"; // Cannot divide by zero
                    exit(1);
                }
                result = operand1 / operand2;
            } else if (token == "%") { // Modulus
                if (operand2 == 0) {
                    cerr << "Error: Modulo by zero\n"; // Cannot divide by zero
                    exit(1);
                }
                result = fmod(operand1, operand2);
            } else if (token == "^") { // Power
                result = pow(operand1, operand2);
            }

            operandStack.push(result); // Push the result back onto the stack
        } else {
          // Invalid token
            cerr << "Error: Invalid token in postfix expression\n"; 
            exit(1);
        }
    }

    if (operandStack.size() != 1) {// Check if there is only one operand left on the stack
        cerr << "Error: Invalid expression\n"; // Invalid expression
        exit(1);
    }

    return operandStack.top(); // Return the result
}

double ExpressionEvaluator::evaluate(const string& expression) {
    vector<string> tokens = tokenize(expression); // Tokenize the expression
    vector<string> postfixExpression = handleParentheses(tokens); // Handle parentheses
    return evaluateExpression(postfixExpression); // Evaluate the expression
}


int main() {
    // Test the ExpressionEvaluator class
    ExpressionEvaluator evaluator;
    string expression; // Expression to evaluate
    cout << "Enter arithmetic expression or type q to quit: "; //prints out the user input prompt
    getline(cin, expression); // Read the user input
    while (expression != "q") { // Loop until the user enters 'q' to quit')
      // Test the evaluator with a simple expression
      try {
          double result = evaluator.evaluate(expression);//evaluates the expression
          cout << "Result: " << result << endl;//prints out the result
      } catch (const exception& e) {//catches any exceptions
          cerr << "Error: " << e.what() << endl;//prints out the error message
      }
      cout << "Enter arithmetic expression or type q to quit: ";
      getline(cin, expression); // Read the user input
    } // Keep looping until the user enters 'q'
    return 0;
}