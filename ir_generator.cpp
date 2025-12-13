#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Node structure for expression tree
struct Node {
    string value;
    Node* left;
    Node* right;

    Node(string val) {
        value = val;
        left = right = NULL;
    }
};

// Vector to store TAC instructions
vector<string> tac;
int tempCount = 1;

// Generate new temporary variable (t1, t2, t3…)
string newTemp() {
    return "t" + to_string(tempCount++);
}

// Generate TAC from expression tree
string generateTAC(Node* root) {
    if (!root) return "";

    // If leaf node → operand
    if (!root->left && !root->right)
        return root->value;

    // Recursively generate code for left and right subtree
    string leftVal = generateTAC(root->left);
    string rightVal = generateTAC(root->right);

    // Create temporary variable for this operation
    string temp = newTemp();

    // Create TAC instruction
    string code = temp + " = " + leftVal + " " + root->value + " " + rightVal;
    tac.push_back(code);

    return temp;
}

int main() {
    /*
        Example Expression:
            a + b * c

        Expression Tree:
               +
              / \
             a   *
                / \
               b   c
    */

    Node* root = new Node("+");
    root->left = new Node("a");

    root->right = new Node("*");
    root->right->left = new Node("b");
    root->right->right = new Node("c");

    // Generate TAC
    generateTAC(root);

    // Print TAC instructions
    cout << "Three Address Code (TAC):" << endl;
    for (string line : tac) {
        cout << line << endl;
    }

    return 0;
}
