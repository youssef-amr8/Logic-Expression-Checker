    #include <iostream>
    #include <string>
    #include <cmath>
    #include <cctype>
    
    using namespace std;
    
    const int rows = 8; // Number of rows for 3 inputs is 8
    
    // Function to convert a string to uppercase
    void toUpperCase(char expr[]) {
        for (int i = 0; expr[i] != '\0'; ++i) {
            expr[i] = toupper(expr[i]);
        }
    }
    
    // Function to validate the input expression
    void validateExpression(const char expr[]) {
        for (int i = 0; expr[i] != '\0'; ++i) {
            char c = expr[i];
            if (!(c == 'A' || c == 'B' || c == 'C' || c == '&' || c == '|' || 
                  c == '!' || c == '(' || c == ')' || isspace(c))) {
                cout << "Error: Invalid character '" << c << "' in the expression." << endl;
                exit(1); // Terminate the program
            }
        }
    }
    // Helper function to find the matching closing parenthesis
    int findClosingParenthesis(const char expr[], int start) {
        int count = 0;
        for (int i = start; expr[i] != '\0'; i++) {
            if (expr[i] == '(') count++;
            else if (expr[i] == ')') count--;
    
            if (count == 0) return i;
        }
        return -1; // No matching closing parenthesis found
    }
    
    // Function to evaluate the logical expression considering parentheses
    bool Evaluate(const char expr[], bool A, bool B, bool C) {
        char subExpr[50];
        int subIndex = 0;
        bool currentValue = false;
        bool result = false;
        bool tempResult = false;
        bool notFlag = false;  // Flag for NOT operator
        char lastOperator = 0; // Checks if there is a last operator
    
        for (int i = 0; expr[i] != '\0'; i++) {
            if (expr[i] == '(') {
                int closingIndex = findClosingParenthesis(expr, i);
                if (closingIndex == -1) {
                    cout << "Error: Mismatched parentheses." << endl;
                    exit(1);
                }
    
                // Extract the subexpression within parentheses
                subIndex = 0;
                for (int j = i + 1; j < closingIndex; j++) {
                    subExpr[subIndex++] = expr[j];
                }
                subExpr[subIndex] = '\0';
    
                // Recursively evaluate the subexpression
                currentValue = Evaluate(subExpr, A, B, C);
    
                // Apply NOT operation if the flag is set
                if (notFlag) {
                    currentValue = !currentValue;
                    notFlag = false;
                }
    
                // Apply the last operator with proper precedence
                if (lastOperator == '&') {
                    tempResult = tempResult && currentValue;
                } else if (lastOperator == '|') {
                    result = result || tempResult;
                    tempResult = currentValue;
                } else {
                    tempResult = currentValue;
                }
    
                // Move index to closing parenthesis
                i = closingIndex;
            } else if (expr[i] == '!') {
                notFlag = true;
            } else if (expr[i] == '&') {
                lastOperator = '&';
            } else if (expr[i] == '|') {
                result = result || tempResult;
                tempResult = false;
                lastOperator = '|';
            } else if (expr[i] == 'A' || expr[i] == 'B' || expr[i] == 'C') {
                // Determine the value of the current variable
                if (expr[i] == 'A') currentValue = A;
                if (expr[i] == 'B') currentValue = B;
                if (expr[i] == 'C') currentValue = C;
    
                // Apply NOT operation if the flag is set
                if (notFlag) {
                    currentValue = !currentValue;
                    notFlag = false;
                }
    
                // Apply the last operator with proper precedence
                if (lastOperator == '&') {
                    tempResult = tempResult && currentValue;
                } else if (lastOperator == '|') {
                    result = result || tempResult;
                    tempResult = currentValue;
                } else {
                    tempResult = currentValue;
                }
            }
        }
    
        result = result || tempResult; // Apply the last term
        return result;
    }
    
    // Function to generate and print the truth table
    void generateTruthTable(const char expr[], bool truthTable[][2]) {
        cout << "A B C | Result\n";
        cout << "----------------"<<endl;
    
        for (int i = 0; i < 8; ++i) {
            bool A = (i & 4) ? true : false;
            bool B = (i & 2) ? true : false;
            bool C = (i & 1) ? true : false;
    
            bool result = Evaluate(expr, A, B, C);
    
            cout << A << " " << B << " " << C << " | " << result << "\n";
    
            truthTable[i][0] = result;
    
        }
    }
    
    // Function to compare two truth tables
    void compareTruthTables(bool table1[][2], bool table2[][2]) {
        for (int i = 0; i < rows; i++) {
            if (table1[i][0] != table2[i][0]) {
                cout << "The two expressions are not equivalent." <<endl;
                return; //exit the function if not equivalent
            }
        }
        cout << "The two expressions are equivalent." << endl;
    }
    
    //Function to check tautology
    bool isTautology(bool truthTable[][2]) {
        bool isTautologyResult = true; // Assume it is a tautology initially
        int i = 0;
        while (i < rows) {
            if (truthTable[i][0]==0) { // If any output is false in the truth table
                isTautologyResult = false;
                break; // No need to check further, we found that it's not a tautology
            }
            i++; // Move to the next row
        }
            if (isTautologyResult)
            {
                        cout << "The original expression is Tautology." << endl;
            }
        return isTautologyResult; // Return true if it was a tautology for all combinations
    }
    
    // Function to modify the original expression by removing NOT gates
    void removeNotGates(char expr[]) {
        for (int i = 0; expr[i] != '\0'; ++i) {
            if (expr[i] == '!') {
                // Shift all characters to the left to remove '!'
                int j = i;
                while (expr[j] != '\0') {
                    expr[j] = expr[j + 1];
                    ++j;
                }
                --i; // Adjust index to check the next character
            }
        }
    }
    
    // Function to check satisfiability and tautology
    void checkSatisfiability(char expr[],bool truthTable[][2]) {
        bool isSatisfiable = false;
        bool isTautologyResult = isTautology(truthTable);
        int i = 0;
        // Check if the expression is a tautology
        if (isTautologyResult) {
            cout << "The expression "<<expr<<" is a tautology." << endl;
            return; // Exit the function if it's a tautology
        }
    
        // If not a tautology, check for satisfiability
        while (i < rows) 
        {
            if (truthTable[i][0] == 1) { // If any output is true, it's satisfiable
                isSatisfiable = true;
                break;
            }
            i++; // Move to the next row
        }
    
        // Output based on satisfiability
        if (isSatisfiable) {
            cout << "The expression "<<expr<<" is satisfiable." << endl;
        } else {
            cout << "The expression "<<expr<<" is unsatisfiable. Modify some gates to make it satisfiable." << endl;
    
            // Remove NOT gates and retry
            cout << "Removing NOT gates from the expression..." << endl;
            removeNotGates(expr);
    
            cout << "Modified Expression: " << expr << endl;
    
            // Regenerate truth table for modified expression
            bool modifiedTruthTable[rows][2] = {0};
            generateTruthTable(expr, modifiedTruthTable);
    
            // Recheck satisfiability
            bool modifiedSatisfiable = false;
            for (int i = 0; i < rows; i++) {
                if (modifiedTruthTable[i][0] == 1) {
                    modifiedSatisfiable = true;
                    break;
                }
            }
    
            if (modifiedSatisfiable) {
                cout << "The modified expression is satisfiable." << endl;
            } else {
                cout << "The modified expression is still unsatisfiable." << endl;
            }
        }
    }
    
    
    int main() {
            char originalExpr[50];
            char simplifiedExpr[50];
    
        cout << "Enter the Original expression (use A, B, C, &, |, !):"<<endl;
        cin.getline(originalExpr, sizeof(originalExpr));
        cout << "Enter the Simplified expression (use A, B, C, &, |, !):"<<endl;
        cin.getline(simplifiedExpr, sizeof(simplifiedExpr));
        
        // Convert expressions to uppercase
        toUpperCase(originalExpr);
        toUpperCase(simplifiedExpr);
        
        // Validate expressions
        validateExpression(originalExpr);
        validateExpression(simplifiedExpr);
    
        
        bool truthTable1[rows][2] = {0};
        bool truthTable2[rows][2] = {0};
    
        // Generate truth tables
        cout<<"The Original expression is: "<<originalExpr<<endl;
        cout << "The Original expression's TruthTable:" << endl;
        generateTruthTable(originalExpr, truthTable1);
        cout<<"The Simplified expression is: "<<simplifiedExpr<<endl;
        cout << "The Simplified expression's TruthTable:" << endl;
        generateTruthTable(simplifiedExpr, truthTable2);
    
        // Compare truth tables
    
        compareTruthTables(truthTable1, truthTable2);
    
        // Check satisfiability for both expressions
        cout <<endl<<"Checking satisfiability for Original Expression:" << endl;
        checkSatisfiability(originalExpr, truthTable1);

        cout <<endl<<"Checking satisfiability for Simplified Expression:" << endl;
        checkSatisfiability(simplifiedExpr, truthTable2);
        return 0;
    }
