#include <cstdio>
#include <cstring>
#include <windows.h>
#include <iostream>
#include <set>
#include <map>
using namespace std;

// Global variables for formula representation
string Original_Formula;  // Original formula entered by the user
string Simp_Formula;      // Simplified formula (after replacing symbols like -> with >)
string Suffix_Formula;    // Postfix (suffix) notation of the formula
string PCNF;              // The Principal Conjunctive Normal Form of the formula
string PDNF;              // The Principal Disjunctive Normal Form of the formula
char ch[15] = "()!&|-><"; // Allowed characters in the formula
map<char, int> variableMap; // Map of variables and their values (0 or 1)
set<char> variables;      // Set of distinct variables found in the formula
int res;                  // Result of the evaluation of the formula
int var_cnt;              // Count of distinct variables in the formula

// Stack class for expression handling
class Stack {
public:
    Stack(int mSize) {
        maxtop = mSize - 1;
        st = new char[mSize];  // Initialize the stack array
        top = -1;  // Start with an empty stack
    }
    ~Stack() {
        delete[] st;  // Destructor to free the allocated memory
    }
    bool Push(char x) {  // Push operation
        if (top == maxtop) return false;  // If stack is full, return false
        st[++top] = x;  // Push element onto the stack
        return true;
    }
    bool Pop() {  // Pop operation
        if (top == -1) return false;  // If stack is empty, return false
        top--;  // Remove the top element
        return true;
    }
    char Top() {  // Get the top element
        return st[top];
    }
private:
    int top;  // Index of the top element
    char* st; // Stack array
    int maxtop; // Maximum index for the stack
};

Stack Sim_stack(200); // Create a stack with a maximum size of 200
int a, b;  // Temporary variables used for calculations

// Logical NOT operation
void Not() {
    a = Sim_stack.Top();
    Sim_stack.Pop();
    res = (a == 1 ? 0 : 1);  // If a is 1, result is 0; otherwise, result is 1
    Sim_stack.Push(res);  // Push the result back onto the stack
}

// Logical AND operation
void And() {
    res = a * b;  // AND operation is equivalent to multiplication
    Sim_stack.Push(res);  // Push the result onto the stack
}

// Logical OR operation
void Or() {
    res = (a + b == 0) ? 0 : 1;  // OR operation: if both are 0, result is 0; otherwise, 1
    Sim_stack.Push(res);  // Push the result onto the stack
}

// Logical implication (IF) operation
void If() {
    res = (b == 1 && a == 0) ? 0 : 1;  // IF b is 1 and a is 0, result is 0; otherwise, 1
    Sim_stack.Push(res);  // Push the result onto the stack
}

// Logical biconditional (IFF) operation
void Iif() {
    res = (a == b ? 1 : 0);  // If a and b are equal, result is 1; otherwise, 0
    Sim_stack.Push(res);  // Push the result onto the stack
}

// Helper function to handle NOT operation separately
void Not_Not(char c) {
    if (c != '!') {  // If the character is not '!', pop two values from the stack
        a = Sim_stack.Top();
        Sim_stack.Pop();
        b = Sim_stack.Top();
        Sim_stack.Pop();
    }
}

// Switch to handle different logical operators
void Switch_Operator(char c) {
    switch (c) {
    case '~': Iif(); break;  // Handle biconditional
    case '>': If(); break;   // Handle implication
    case '|': Or(); break;   // Handle OR
    case '&': And(); break;  // Handle AND
    case '!': Not(); break;  // Handle NOT
    }
}

// Function to compare operator precedence
bool Jude_canin(char out) {
    char in = Sim_stack.Top();
    int i, o;
    switch (in) {  // Assign precedence values for operators on the stack
    case '#': i = 0; break;
    case '(': i = 1; break;
    case '~': i = 3; break;
    case '>': i = 5; break;
    case '|': i = 7; break;
    case '&': i = 9; break;
    case '!': i = 11; break;
    case ')': i = 12; break;
    }
    switch (out) {  // Assign precedence values for incoming operators
    case '(': o = 12; break;
    case '~': o = 2; break;
    case '>': o = 4; break;
    case '|': o = 6; break;
    case '&': o = 8; break;
    case '!': o = 10; break;
    case ')': o = 1; break;
    }
    return i < o;  // Return true if the operator on the stack has lower precedence
}

// Convert infix formula to postfix (suffix) notation
void Change_to_sufexp() {
    string Tmp = "";
    Sim_stack.Push('#');  // Push a special character to mark the bottom of the stack
    for (int i = 0; i < Simp_Formula.length(); i++) {
        if (isalpha(Simp_Formula[i])) {  // If the character is a variable, add it to the result
            Tmp += Simp_Formula[i];
            continue;
        }
        if (Jude_canin(Simp_Formula[i]))  // If precedence is lower, push the operator onto the stack
            Sim_stack.Push(Simp_Formula[i]);
        else if (Simp_Formula[i] == ')') {  // If it's a closing parenthesis, pop until '(' is found
            while (Sim_stack.Top() != '(') {
                Tmp += Sim_stack.Top();
                Sim_stack.Pop();
            }
            Sim_stack.Pop();  // Remove '(' from the stack
        }
        else {  // Pop operators with higher precedence from the stack
            do {
                Tmp += Sim_stack.Top();
                Sim_stack.Pop();
            } while (!Jude_canin(Simp_Formula[i]));
            Sim_stack.Push(Simp_Formula[i]);  // Push the current operator onto the stack
        }
    }
    while (Sim_stack.Top() != '#') {  // Pop all remaining operators
        Tmp += Sim_stack.Top();
        Sim_stack.Pop();
    }
    Sim_stack.Pop();  // Clear the stack
    Suffix_Formula = Tmp;  // Store the resulting postfix formula
}

// Evaluate the postfix formula
void Calculate() {
    for (int i = 0; i < Suffix_Formula.length(); i++) {
        if (isalpha(Suffix_Formula[i])) {  // If it's a variable, push its value onto the stack
            Sim_stack.Push(variableMap[Suffix_Formula[i]]);
        }
        else {  // Otherwise, perform the corresponding logical operation
            Not_Not(Suffix_Formula[i]);
            Switch_Operator(Suffix_Formula[i]);
        }
    }
}

// Replace symbols with their corresponding logical symbols for display
string ReplaceSymbols(const string& formula) {
    string result;
    for (char ch : formula) {
        if (ch == '>') result += "→";
        else if (ch == '~') result += "←→";
        else if (ch == '&') result += "∧";
        else if (ch == '|') result += "∨";
        else if (ch == '!') result += "┐";
        else result += ch;
    }
    return result;
}

// Function to print the truth table and calculate PDNF and PCNF
void Print() {
    cout << "Truth Table:" << endl;
    for (char var : variables) {
        cout << var << "\t";
    }

    string formattedFormula = ReplaceSymbols(Simp_Formula);
    cout << formattedFormula << endl;

    int combinations = 1 << var_cnt;  // Number of possible truth combinations
    for (int i = 0; i < combinations; i++) {
        int index = 0;
        for (char var : variables) {
            variableMap[var] = (i >> index) & 1;  // Assign truth values to variables
            cout << variableMap[var] << "\t";
            index++;
        }

        Calculate();  // Evaluate the formula for the current truth assignment
        if (res == 1) {  // If the result is true, add to PDNF
            if (!PDNF.empty()) PDNF += " ∨ ";
            PDNF += "(";
            for (char var : variables) {
                if (variableMap[var] == 1) {
                    PDNF += var;
                }
                else {
                    PDNF += "┐" + string(1, var);
                }
                PDNF += " ∧ ";
            }
            PDNF.erase(PDNF.size() - 3);  // Remove the trailing " ∧ "
            PDNF += ")";
        }
        else {  // If the result is false, add to PCNF
            if (!PCNF.empty()) PCNF += " ∧ ";
            PCNF += "(";
            for (char var : variables) {
                if (variableMap[var] == 1) {
                    PCNF += "┐" + string(1, var);
                }
                else {
                    PCNF += var;
                }
                PCNF += " ∨ ";
            }
            PCNF.erase(PCNF.size() - 3);  // Remove the trailing " ∨ "
            PCNF += ")";
        }
        cout << res << endl;  // Print the result of the formula for the current truth assignment
    }

    if (!PDNF.empty() && PDNF.back() == '∨') PDNF.erase(PDNF.size() - 3);  // Clean up PDNF
    if (!PCNF.empty() && PCNF.back() == '∧') PCNF.erase(PCNF.size() - 3);  // Clean up PCNF
}

// Generate PDNF and remove extra symbols if necessary
void generatePDNF() {
    if (!PDNF.empty() && PDNF.back() == '∨') PDNF.erase(PDNF.size() - 3);
}

// Generate PCNF and remove extra symbols if necessary
void generatePCNF() {
    if (!PCNF.empty() && PCNF.back() == '∧') PCNF.erase(PCNF.size() - 3);
}

// Function to display menu instructions
void Menu() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get console handle
    cout << "Symbol Input Rules:" << endl;
    cout << " ┐: !       " << endl;
    cout << " ∧: &        " << endl;
    cout << " ∨: |        " << endl;
    cout << " →: ->       " << endl;
    cout << "←→: <->    " << endl;
    cout << "Note: Parentheses and variables A-Z, a-z are all supported." << endl;
}

// Function to input and validate the formula
void InputANDjude_formula() {
    char str1[100] = { 0 }, str2[100];  // Buffer to hold user input and processed formula
    int cnt = 0;
    cout << "------------------------------------------------------" << endl;
    cout << "Please input the propositional formula (type 'exit' to exit):" << endl;
    gets_s(str1);  // Get input from the user
    for (int i = 0; i < strlen(str1);) {  // Replace symbols like -> with internal representations
        if (str1[i] == '-') { str2[cnt++] = '>'; i += 2; }
        else if (str1[i] == '<') { str2[cnt++] = '~'; i += 3; }
        else { str2[cnt++] = str1[i]; i += 1; }
    }
    str2[cnt++] = '\0';
    Original_Formula = str1;  // Store the original formula
    Simp_Formula = str2;      // Store the simplified formula
}

// Function to enhance robustness by checking for invalid characters
bool Enh_Robustness() {
    bool Rob = true;
    if (Original_Formula == "exit") return true;  // Exit condition
    for (int i = 0; i < Original_Formula.length(); i++) {  // Check each character in the formula
        if (strchr(ch, Original_Formula[i]) == nullptr && !isalpha(Original_Formula[i])) {
            cout << "Input error, please check illegal characters." << endl;
            Original_Formula = "";  // Clear the formula on error
            return false;
        }
    }
    return Rob;
}

// Function to count the number of distinct variables in the formula
void count_varcnt() {
    variables.clear();  // Clear any previously counted variables
    for (int i = 0; i < Simp_Formula.length(); i++) {
        if (isalpha(Simp_Formula[i])) {  // Add variables (alphabet characters) to the set
            variables.insert(Simp_Formula[i]);
        }
    }
    var_cnt = static_cast<int>(variables.size());  // Count the number of distinct variables
}

int main() {
    Menu();  // Display menu instructions
    while (1) {
        do {
            InputANDjude_formula();  // Get user input
        } while (!Enh_Robustness());  // Validate input
        if (Original_Formula == "exit") {  // If user types 'exit', end the program
            cout << "------------------------------------------------------" << endl;
            cout << "Thank you for using!" << endl;
            system("pause");
            return 0;
        }

        count_varcnt();  // Count the number of variables in the formula
        Change_to_sufexp();  // Convert the formula to postfix notation
        Print();  // Print the truth table and calculate PDNF and PCNF
        generatePDNF();  // Generate PDNF
        generatePCNF();  // Generate PCNF

        cout << "PDNF: " << endl << PDNF << endl;  // Output PDNF
        cout << "PCNF: " << endl << PCNF << endl;  // Output PCNF

        PDNF.clear();  // Clear PDNF for the next iteration
        PCNF.clear();  // Clear PCNF for the next iteration
    }
    return 0;
}
