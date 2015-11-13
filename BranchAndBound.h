//
// Created by Sabrina on 10.11.2015.
//

#ifndef BRANCHANDBOUND_BRANCHANDBOUND_H
#define BRANCHANDBOUND_BRANCHANDBOUND_H

#include <iostream>
#include <bits/stl_list.h>
using namespace std;

class BranchAndBound {
public:
    list * liste; //Liste, die die Probleme enthält
    int n; //Anzahl der Jobs bzw. Personen
    static int** matrix; //Kostenmatrix
    int u; // Max range of int instead of infinity
    Problem * min; //zeigt auf Problem mit kleinster Lower bound

    struct Problem{
        int l; //lower bound
        int * assignment; //Assignment, dh welche Person welchen Job macht, wird hier reingeschrieben
        //int count; //zählt wie viele Aufgaben schon vergeben wurden
        Problem(int n) : l{0}, assignment{new int[n]{}}, count{0} {}
        ~Problem() {}
    };

    Problem *s;
    BranchAndBound() : u{2147483647}, liste{new list()}, s {new Problem(n)} {liste->push_front(s);}
    ~BranchAndBound() { delete[] liste; delete[] matrix;}

    int* assignment(int n, int **kosten).
    list branch(Problem problem);
    void bound(Problem teilproblem);
    Problem minProblem();

};




#endif //BRANCHANDBOUND_BRANCHANDBOUND_H

//Array: [Zeile][Spalte]
int* BranchAndBound::assignment(int n, int **kosten) {
    this.n=n;
    this.matrix=kosten;
    while(!liste->empty()){
        Problem problem = minProblem(); //Problem mit minimaler Lower Bound
        liste->pop_front(); //Problem mit minimaler Lower Bound wird aus Liste entfernt
        list teilProblemListe = branch(problem); //Teilt Problem in Teilprobleme
        for(int i = 0; i < teilProblemListe.size(); i++) { //Für jedes Teilproblem wird bound-Funktion aufgerufen um L zu berechnen
            Problem teilproblem = teilProblemListe.front();
            bound(teilproblem);
            teilProblemListe.pop_front();
        }
    }
    return min->assignment;
}

list BranchAndBound::branch(Problem problem) {

}

void BranchAndBound::bound(Problem teilproblem) {

}


Problem BranchAndBound::minProblem() {
    //liste->sort();
    //return liste->front();
}
