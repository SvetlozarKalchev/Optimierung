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

    cout << "Beispiel 1:" << '\n' << '\n' <<"Kostenmatrix: " << '\n' << '\t' << '\t';
    for(int i = 0; i < 3; i++) {
        cout << "Task " << i+1 << '\t';
    }
    for(int j = 0; j < 3; j++) {
        cout << '\n' << "Agent " << j << '\t' << '\t';
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

    cout << '\n' << '\n' << "Minimale Kosten: " << summe << '\n' << endl;

    //Beispiel 2


    matrix = new int*[4];
    for(int i = 0; i < 4; ++i)
        matrix[i] = new int[4];

    matrix[0][0]=10;
    matrix[0][1]=6;
    matrix[0][2]=5;
    matrix[0][3]=6;
    matrix[1][0]=5;
    matrix[1][1]=4;
    matrix[1][2]=6;
    matrix[1][3]=7;
    matrix[2][0]=4;
    matrix[2][1]=2;
    matrix[2][2]=4;
    matrix[2][3]=2;
    matrix[3][0]=3;
    matrix[3][1]=4;
    matrix[3][2]=3;
    matrix[3][3]=7;

    cout << "Kostenmatrix: " << '\n' << '\t' << '\t';
    for(int i = 0; i < 4; i++) {
        cout << "Task " << i+1 << '\t';
    }
    for(int j = 0; j < 4; j++) {
        cout << '\n' << "Agent " << j << '\t' << '\t';
        for(int k = 0; k < 4; k++) {
            cout << matrix[j][k] << '\t';
        }
    }
    bb = new BranchAndBound();
    assignment = bb->assignment(4, matrix);
    cout << '\n' << '\n' << "Assignment: " << endl;
    for(int i = 0; i < 4; i++) {
        cout << assignment[i] << '\t';
    }

    summe = 0;
    for(int i = 0; i < 4; i++) {
        summe += matrix[assignment[i]][i];
    }

    cout << '\n' << '\n' << "Minimale Kosten: " << summe << '\n' << endl;

    //Beispiel 3

    matrix = new int*[5];
    for(int i = 0; i < 5; ++i)
        matrix[i] = new int[5];

    matrix[0][0]=5;
    matrix[0][1]=7;
    matrix[0][2]=3;
    matrix[0][3]=6;
    matrix[0][4]=1;

    matrix[1][0]=3;
    matrix[1][1]=1;
    matrix[1][2]=6;
    matrix[1][3]=2;
    matrix[1][4]=9;

    matrix[2][0]=4;
    matrix[2][1]=5;
    matrix[2][2]=9;
    matrix[2][3]=1;
    matrix[2][4]=2;

    matrix[3][0]=5;
    matrix[3][1]=8;
    matrix[3][2]=3;
    matrix[3][3]=4;
    matrix[3][4]=6;

    matrix[4][0]=2;
    matrix[4][1]=8;
    matrix[4][2]=7;
    matrix[4][3]=9;
    matrix[4][4]=3;

    cout << "Kostenmatrix: " << '\n' << '\t' << '\t';
    for(int i = 0; i < 5; i++) {
        cout << "Task " << i+1 << '\t';
    }
    for(int j = 0; j < 5; j++) {
        cout << '\n' << "Agent " << j << '\t' << '\t';
        for(int k = 0; k < 5; k++) {
            cout << matrix[j][k] << '\t';
        }
    }
    bb = new BranchAndBound();
    assignment = bb->assignment(5, matrix);
    cout << '\n' << '\n' << "Assignment: " << endl;
    for(int i = 0; i < 5; i++) {
        cout << assignment[i] << '\t';
    }

    summe = 0;
    for(int i = 0; i < 5; i++) {
        summe += matrix[assignment[i]][i];
    }

    cout << '\n' << '\n' << "Minimale Kosten: " << summe << endl;
}
