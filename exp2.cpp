#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#define MAX 100
bool flag_ref, flag_irr, flag_sym, flag_dis, flag_tra;
int arr[MAX][MAX];
int closure[MAX][MAX];
int n;

void initial() {
    cout << "Enter the number of elements (enter 0 to exit):" << endl;
    cin >> n;
    if (n == 0) exit(0);
    srand(time(0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            arr[i][j] = rand() % 2;

    cout << endl << "Generated relation matrix:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << arr[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

void Reflexive() {
    for (int i = 0; i < n; i++)
        if (arr[i][i] == 0) flag_ref = 0;
}

void Irreflexive() {
    for (int i = 0; i < n; i++)
        if (arr[i][i] == 1) flag_irr = 0;
}

void Symmetrical() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (arr[i][j] != arr[j][i]) flag_sym = 0;
}

void Dissymmetrical() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (arr[i][j] == arr[j][i]) flag_dis = 0;
}

void Transitive() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                if (arr[i][j] && arr[j][k] && !arr[i][k]) flag_tra = 0;
}

void Warshall() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            closure[i][j] = arr[i][j];

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);
}

void printClosure() {
    cout << "Transitive closure:" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            cout << closure[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

void print() {
    cout << "Reflexivity: " << (flag_ref ? "Satisfied" : "Not satisfied") << endl;
    cout << "Irreflexivity: " << (flag_irr ? "Satisfied" : "Not satisfied") << endl;
    cout << "Symmetry: " << (flag_sym ? "Satisfied" : "Not satisfied") << endl;
    cout << "Antisymmetry: " << (flag_dis ? "Satisfied" : "Not satisfied") << endl;
    cout << "Transitivity: " << (flag_tra ? "Satisfied" : "Not satisfied") << endl;
    cout << endl;
}

int main() {
    while (1) {
        initial();
        flag_ref = flag_irr = flag_sym = flag_dis = flag_tra = 1;
        Reflexive();
        Irreflexive();
        Symmetrical();
        Dissymmetrical();
        Transitive();
        print();
        Warshall();
        printClosure();
    }
    return 0;
}
