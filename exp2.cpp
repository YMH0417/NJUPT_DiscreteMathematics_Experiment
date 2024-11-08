#include <iostream>   // Include the input-output stream library
#include <cstdlib>    // Include the standard library for random functions
#include <ctime>      // Include the library for time functions
using namespace std;  // Use the standard namespace

#define MAX 100      // Define a constant for maximum size of arrays
bool flag_ref, flag_irr, flag_sym, flag_dis, flag_tra; // Flags for properties
int arr[MAX][MAX];   // Matrix to store the relation
int closure[MAX][MAX]; // Matrix to store transitive closure
int n;               // Number of elements in the relation

// Function to initialize the relation matrix
void initial() {
    cout << "Enter the number of elements (enter 0 to exit):" << endl; // Prompt user for input
    cin >> n; // Read the number of elements
    if (n == 0) exit(0); // Exit if input is 0
    srand(time(0)); // Seed the random number generator with current time
    // Populate the relation matrix with random 0s and 1s
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            arr[i][j] = rand() % 2; // Randomly assign 0 or 1

    cout << endl << "Generated relation matrix:" << endl; // Output the generated matrix
    // Print the relation matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << arr[i][j] << " "; // Print each element
        cout << endl; // New line after each row
    }
    cout << endl; // Additional new line for formatting
}

// Function to check if the relation is reflexive
void Reflexive() {
    for (int i = 0; i < n; i++)
        if (arr[i][i] == 0) flag_ref = 0; // Check diagonal elements
}

// Function to check if the relation is irreflexive
void Irreflexive() {
    for (int i = 0; i < n; i++)
        if (arr[i][i] == 1) flag_irr = 0; // Check diagonal elements
}

// Function to check if the relation is symmetrical
void Symmetrical() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (arr[i][j] != arr[j][i]) flag_sym = 0; // Check symmetry condition
}

// Function to check if the relation is dissymmetrical
void Dissymmetrical() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (arr[i][j] == arr[j][i]) flag_dis = 0; // Check dissymmetry condition
}

// Function to check if the relation is transitive
void Transitive() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                if (arr[i][j] && arr[j][k] && !arr[i][k]) flag_tra = 0; // Check transitivity
}

// Function to compute the transitive closure using Warshall's algorithm
void Warshall() {
    // Initialize closure matrix with the relation matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            closure[i][j] = arr[i][j];

    // Update the closure matrix based on transitive relations
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]); // Transitive step
}

// Function to print the transitive closure matrix
void printClosure() {
    cout << "Transitive closure:" << endl; // Output header for closure
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << closure[i][j] << " "; // Print each element of closure
        cout << endl; // New line after each row
    }
    cout << endl; // Additional new line for formatting
}

// Function to print the properties of the relation
void print() {
    // Print results of reflexivity, irreflexivity, symmetry, dissymmetry, and transitivity
    cout << "Reflexivity: " << (flag_ref ? "Satisfied" : "Not satisfied") << endl;
    cout << "Irreflexivity: " << (flag_irr ? "Satisfied" : "Not satisfied") << endl;
    cout << "Symmetry: " << (flag_sym ? "Satisfied" : "Not satisfied") << endl;
    cout << "Antisymmetry: " << (flag_dis ? "Satisfied" : "Not satisfied") << endl;
    cout << "Transitivity: " << (flag_tra ? "Satisfied" : "Not satisfied") << endl;
    cout << endl; // Additional new line for formatting
}

// Main function to run the program
int main() {
    while (1) { // Infinite loop to keep the program running
        initial(); // Call the initialization function
        flag_ref = flag_irr = flag_sym = flag_dis = flag_tra = 1; // Reset flags
        Reflexive(); // Check for reflexivity
        Irreflexive(); // Check for irreflexivity
        Symmetrical(); // Check for symmetry
        Dissymmetrical(); // Check for dissymmetry
        Transitive(); // Check for transitivity
        print(); // Print the properties
        Warshall(); // Compute the transitive closure
        printClosure(); // Print the transitive closure
    }
    return 0; // Return 0 to indicate successful completion
}
