#include <iostream>
#include "BranchAndBound.h"

using namespace std;

int main() {
    int** matrix = new int*[3];
    for(int i = 0; i < 3; ++i)
        matrix[i] = new int[3];

    matrix[0][0]=5;
    matrix[0][1]=7;
    matrix[0][2]=3;
    matrix[1][0]=3;
    matrix[1][1]=1;
    matrix[1][2]=6;
    matrix[2][0]=4;
    matrix[2][1]=5;
    matrix[2][2]=9;

    cout << "Kostenmatrix: " << '\n' << '\t' << '\t';
    for(int i = 0; i < 3; i++) {
        cout << "Task " << i+1 << '\t';
    }
    for(int j = 0; j < 3; j++) {
        cout << '\n' << "Agent " << j+1 << '\t' << '\t';
        for(int k = 0; k < 3; k++) {
            cout << matrix[j][k] << '\t';
        }
    }
    BranchAndBound* bb = new BranchAndBound();
    int* assignment = bb->assignment(3, matrix);
    cout << '\n' << '\n' << "Assignment: " << endl;
    for(int i = 0; i < 3; i++) {
        cout << assignment[i] << '\t';
    }

    int summe = 0;
    for(int i = 0; i < 3; i++) {
        summe += matrix[assignment[i]][i];
    }

    cout << '\n' << '\n' << "Minimale Kosten: " << summe << endl;

    return 0;
}