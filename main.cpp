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
        cout << '\n' << "Agent " << j+1 << '\t' << '\t';
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

    cout << '\n' << '\n' << "Minimale Kosten: " << summe << endl;

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
        cout << '\n' << "Agent " << j+1 << '\t' << '\t';
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

    //Test: Bound-Methode
    /*
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

    int n = 3;
    int u = 2147483647;

    struct Problem{
        int l; //lower bound
        int * assignment; //Assignment, dh welche Person welchen Job macht, wird hier reingeschrieben
        int count; //zaehlt wie viele Aufgaben schon vergeben wurden
        Problem(int n) : l{0}, assignment{new int[n]{}}, count{0} {}
        ~Problem() {delete[] assignment;}
    };

    typedef std::list<Problem> BBlist;
    BBlist liste; //Liste, die die Probleme enthaelt
    Problem teilproblem = Problem(3);
    teilproblem.assignment[0]=1;
    teilproblem.assignment[1]=2;
    teilproblem.count=2;
    int *minimalAssignment = new int[3]{};

        cout << "@bound" << endl;
        //lower bound berechnen
        int lowerBound = 0;
        int * currentAssignment = new int[n] {};
        for(int i = 0; i < teilproblem.count; i++) { //bereits festgelegte Agenten
            lowerBound += matrix[teilproblem.assignment[i]][i];   //teilproblem.assignment[i]*(i+1)]; //teilproblem.assignment[i-1]
            currentAssignment[i] = teilproblem.assignment[i];
        }
        for(int j = teilproblem.count; j < n; j++) { //noch nicht festgelegt --> minimum
            int *array = new int[n];
            for(int i = 0; i < n; i++) { //i = Spalte
                array[i] = matrix[i][j];
                for(int x = 0; x < teilproblem.count; x++) if(currentAssignment[x]==i) array[i]=2147483647;
            }

            // Spalte j durchlaufen und Minimum suchen
            int agentMin = 0;
            for(int k = 1; k<n; k++) {
                if(array[k]<array[agentMin]) agentMin=k;
            }
            lowerBound +=array[agentMin];
            currentAssignment[j]=agentMin;
        }

        //pruefen ob currentAssignment gueltige Loesung ist
        bool gueltigeLoesung = true;
        for(int l=0; l<n-1; l++) {
            for(int m=l+1; m<n; m++) {
                cout << "bla" << endl;
                if(currentAssignment[l] == currentAssignment[m]) {
                    gueltigeLoesung = false;
                    break;
                }
            }
        }

        //gueltige Loesung
        if(lowerBound < u && gueltigeLoesung) {
            u = lowerBound;   //edit: this->
            minimalAssignment = currentAssignment;
        }

            //keine gueltige Loesung
        else {
            liste.push_front(teilproblem);
        }
        cout << "Lower Bound: " << lowerBound << endl;
        cout << "Upper Bound: " << u << endl;
        cout << "Minimal Assignment: " << minimalAssignment[0] << '\t' << minimalAssignment[1] << '\t' << minimalAssignment[2];

        delete[]currentAssignment;
        */


    return 0;
}