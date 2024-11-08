#include <iostream>  // Include the iostream library for input/output operations
#include <vector>    // Include the vector library to use the vector container
#include <algorithm> // Include the algorithm library for functions like find

using namespace std; // Use the standard namespace

// Function to check if a number is prime
bool Is(long long num) {
    if (num <= 1) // Numbers less than or equal to 1 are not prime
        return false;
    for (long long i = 2; i * i <= num; i++) { // Loop from 2 to the square root of num
        if (num % i == 0) // If num is divisible by i, it's not prime
            return false;
    }
    return true; // If no divisors were found, the number is prime
}

// Function to compute the greatest common divisor (GCD) of two numbers
long long gcd(long long a, long long b) {
    while (b != 0) { // Repeat until b becomes 0
        long long temp = b; // Store b in a temporary variable
        b = a % b;          // Update b with the remainder of a divided by b
        a = temp;           // Update a with the value of b from the previous iteration
    }
    return a; // When b is 0, a is the GCD
}

// Function to compute the least common multiple (LCM) of two numbers
long long lcm(long long a, long long b) {
    return a * b / gcd(a, b); // LCM is calculated as (a * b) / GCD(a, b)
}

int main() {
    long long num = 0, n;                  // Declare variables for input number and a copy of the input
    bool isLattice = true;                 // Boolean flag to check if the number forms a lattice with complements
    vector<long long> arr, arrab, son, none; // Vectors to store prime factors, distinct prime factors, divisors, and non-complement elements

    cout << "Please enter the number: ";   // Prompt the user to enter a number
    cin >> num;                            // Read the input number
    n = num;                               // Store a copy of the input in n

    if (Is(num)) {                         // Check if the input number is prime
        cout << "<1," << num << ">\nIt is a lattice with complements\n";
        return 0;                          // If prime, output that it's a lattice with complements and exit
    }

    for (long long i = 1; i <= num; i++) { // Loop through all numbers from 1 to num
        if (num % i == 0)                  // If i is a divisor of num
            son.push_back(i);              // Add i to the list of divisors
    }

    // Factorize the number into its prime factors and store in arr and arrab
    while (!Is(num)) {                     // Repeat until num becomes a prime number
        for (long long i = 2;; i++) {      // Loop to find the smallest prime factor of num
            if (!Is(i)) continue;          // Skip if i is not prime
            if (num % i == 0) {            // If i is a prime factor of num
                num /= i;                  // Divide num by i
                arr.push_back(i);          // Add i to the list of prime factors
                if (arrab.empty() || arrab.back() != i) // If arrab is empty or the last element is not i
                    arrab.push_back(i);    // Add i to the list of distinct prime factors
                break;                     // Break to reset the loop with the new num
            }
        }
    }
    arr.push_back(num);                    // Add the final prime factor (num itself) to arr
    if (arrab.back() != num)               // If the last element in arrab is not num
        arrab.push_back(num);              // Add num to arrab as a distinct prime factor

    cout << "\nPartial order relations:\n"; // Output label for partial order relations
    for (auto p = son.begin(); p != son.end(); p++) { // Loop through each divisor in son
        bool hasComplement = false;        // Flag to check if a complement exists for the current divisor
        for (auto j = son.begin(); j != son.end(); j++) { // Loop through each divisor in son again
            if ((find(arrab.begin(), arrab.end(), *j / *p) != arrab.end()) && (*j % *p == 0) && (j != p)) {
                cout << "<" << *p << "," << *j << "> "; // Print partial order relation if the condition holds
            }
            else if (gcd(*p, *j) == 1 && lcm(*p, *j) == n) { // Check if p and j are complements
                hasComplement = true;      // Set the flag to true if a complement is found
            }
        }
        if (!hasComplement) {              // If no complement was found for p
            none.push_back(*p);            // Add p to the list of elements without complements
            isLattice = false;             // Set isLattice to false as not all elements have complements
            if (*p != n)                   // If p is not equal to the input number
                cout << "(" << *p << " has no complement)"; // Output that p has no complement
        }
        cout << endl;                      // Print a newline after each set of relations
    }

    if (!isLattice) {                      // If not all elements have complements
        cout << "It is not a lattice with complements."; // Output that it is not a lattice with complements
    }
    else {
        cout << "It is a lattice with complements.\n"; // Output that it is a lattice with complements
    }

    return 0;                              // End the program
}
