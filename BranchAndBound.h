//
// Created by Sabrina on 10.11.2015.
//

#ifndef BRANCHANDBOUND_BRANCHANDBOUND_H
#define BRANCHANDBOUND_BRANCHANDBOUND_H
using namespace std;
#include <iostream>
#include <list>



class BranchAndBound {
public:
    struct Problem{
        int l; //lower bound
        int * assignment; //Assignment, dh welche Person welchen Job macht, wird hier reingeschrieben
        int count; //zaehlt wie viele Aufgaben schon vergeben wurden
        Problem(int n) : l{0}, assignment{new int[n]{}}, count{0} {}
        ~Problem() {delete[] assignment;}
    };

private:
    typedef std::list<Problem> BBlist;
    BBlist liste; //Liste, die die Probleme enthaelt
    int n; //Anzahl der Jobs bzw. Personen
    int** matrix; //Kostenmatrix
    int u; // Max range of int instead of infinity
    //Problem * min; //zeigt auf Problem mit kleinster Lower bound
    int * minimalAssignment;
    Problem *s;

public:
    BranchAndBound() : u{2147483647}, liste{}, s {new Problem(n)}, minimalAssignment{new int[n]{}} {liste.push_front(
                (const Problem &) s);}
    ~BranchAndBound() {  delete[] matrix;}

    int* assignment(int n, int **kosten);
    BBlist branch(Problem problem);
    void bound(Problem teilproblem);
    Problem minProblem();
    BranchAndBound::Problem* setProblem(int n);

};

//Array: [Zeile][Spalte]
int* BranchAndBound::assignment(int n, int **kosten) {
    this->n=n;
    this->matrix=kosten;
    while(!liste.empty()){
        Problem problem = minProblem(); //Problem mit minimaler Lower Bound
        liste.pop_back(); //Problem mit minimaler Lower Bound wird aus Liste entfernt
        BBlist teilProblemVector = branch(problem); //Teilt Problem in Teilprobleme
        for(int i = 0; i < teilProblemVector.size(); i++) { //Fur jedes Teilproblem wird bound-Funktion aufgerufen um L zu berechnen
            Problem teilproblem = teilProblemVector.front();
            bound(teilproblem);
            teilProblemVector.pop_back();
        }
    }
    return minimalAssignment;
}

BranchAndBound::BBlist BranchAndBound::branch(Problem problem) {

}

void BranchAndBound::bound(Problem teilproblem) {
    //lower bound berechnen
    int lowerBound = 0;
    int * currentAssignment = new int[n] {};
    for(int i = 0; i < teilproblem.count; i++) { //bereits festgelegte Agenten
        lowerBound += matrix[teilproblem.assignment[i]][i];   //teilproblem.assignment[i]*(i+1)]; //teilproblem.assignment[i-1]
        currentAssignment[i] = teilproblem.assignment[i];
    }
    for(int j = teilproblem.count; j < n; j++) { //noch nicht festgelegt --> minimum
        // Spalte j durchlaufen und Minimum suchen
        int agentMin = 0;
        for(int k = 0; k<n; k++) {
            if(matrix[k][j]<matrix[agentMin][j]) agentMin=k;
        }
        lowerBound +=matrix[agentMin][j];
        currentAssignment[j]=agentMin;
    }

    //pruefen ob currentAssignment gueltige Loesung ist
    bool gueltigeLoesung = true;
    for(int l=0; l<n; l++) {
        for(int m=l+1; m<n; m++) {
            if(currentAssignment[l] == currentAssignment[m]) gueltigeLoesung == false;
        }
    }

    //gueltige Loesung
    if(lowerBound < u && gueltigeLoesung) {
        this->u = lowerBound;
        minimalAssignment = currentAssignment;
    }

        //keine gueltige Loesung
    else {
        liste.push_front(teilproblem);
    }
}


BranchAndBound::Problem BranchAndBound::minProblem() {
    //liste->sort();
    return liste.front();
}

BranchAndBound::Problem* BranchAndBound::setProblem(int n) {
    Problem* neuesProblem = new Problem(n);
    return neuesProblem;
}

#endif //BRANCHANDBOUND_BRANCHANDBOUND_H