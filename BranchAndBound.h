#ifndef BRANCHANDBOUND_BRANCHANDBOUND_H
#define BRANCHANDBOUND_BRANCHANDBOUND_H

#include <iostream>
#include <list>

using namespace std;

class BranchAndBound {
public:
    struct Problem {
        int l; //lower bound
        int *assignment; //Assignment, dh welche Person welchen Job macht, wird hier reingeschrieben
        int count; //zaehlt wie viele Aufgaben schon vergeben wurden
        Problem(int n) : l{0}, assignment{new int[n]{}}, count{0} {}
        ~Problem() {delete[] assignment;}
    };

private:
    typedef list<Problem> BBlist;
    BBlist liste; //Liste, die die Probleme enthaelt
    int n; //Anzahl der Jobs bzw. Personen
    int** matrix; //Kostenmatrix
    int u; // Max range of int instead of infinity
    //Problem * min; //zeigt auf Problem mit kleinster Lower bound
    int * minimalAssignment;
    Problem *s;

public:
    /* Constructor and destructor*/
    BranchAndBound() : u{2147483647}, liste{}, s {new Problem(n)}, minimalAssignment{new int[n]{}} {liste.push_front(
                (const Problem &) s);}
    ~BranchAndBound() {  delete[] matrix;}


    int* assignment(int n, int **kosten);
    BBlist branch(Problem problem);
    void bound(Problem teilproblem);
    Problem minProblem();
    BranchAndBound::Problem* setProblem(int n);

};

/*
    Array: [Zeile][Spalte]
    Method receives number of jobs/workers and cost matrix.
*/
int* BranchAndBound::assignment(int n, int **kosten) {
    /* Assign number of workers and cost matrix to class variables */
    this->n = n;
    this->matrix=kosten;

    /*
        While the list contains problems:
        1. Go through each problem
        2. Create branches, e.g subproblems
        3. Go through each subproblem to calculate lower bound and determine validity
        4. Return worker/job combination with lowest cost
    */
    while(!liste.empty())
    {
        /* Get the problem with the minimal lower bound. Then remove it from the global problem list. */
        Problem problem = minProblem();
        liste.pop_back();

        /* Get a list containing all permutations of the problem. */
        BBlist sub_problem_list = branch(problem);

        /* Calculate lower bound for each subproblem */
        for(int i = 0; i < sub_problem_list.size(); i++)
        {
            Problem sub_problem = sub_problem_list.front();
            bound(sub_problem);
            sub_problem_list.pop_back();
        }
    }

    return minimalAssignment;
}
/*
    Method that accepts a problem and creates a list with all possible variations.
*/
BranchAndBound::BBlist BranchAndBound::branch(Problem problem)
{
    /*
        1. Find out how many jobs need to be allocated
        2. Start from the first empty worker/job position
        3. Find lowest value for each job
        4. Put the problem variation in the list
        5. At the end, return the branch list.
    */
    BranchAndBound::BBlist problem_list;
    int number_of_assignments = problem.count;
    int workers = this->n;
    int **matrix = this->matrix;
    int job;

    /* i = collumn */
    for(int i = number_of_assignments; i < workers; i++)
    {
        /* Create a new problem object. Its assignment field is updated and the object is later put in the branch list. */
        BranchAndBound::Problem sub_problem(this->n);

        /* Go over each row and get the lowest unassigned value. */
        job = i;

        for(int row = 0; row < workers; row++)
        {
            /* TODO:
                1. Add method to check if the current worker is already given a job.
                2. Build a new branch from the possible values
            */
        }

        problem_list.push_front((const Problem &)sub_problem);
    }

    return problem_list;
}

/*
    Method that accepts a problem and calculates the sum of all costs.
*/
void BranchAndBound::bound(Problem sub_problem)
{
    int lowerBound = 0;
    int *currentAssignment = new int[n] {};
    /*
        Go through the cost matrix and sum the corresponding worker/job cost values
        The problem struct contains an array, denoting which job is given to which worker
        Example: [1][0][2] means J0 is given to W1, J1 to W0, and J2 to W2

        The count variable shows how many workers are given jobs. For those problems where
        not all workers are given a job, the minimum job cost value is added to the LB for each
        unassigned job.
    */
    int worker;
    int allocations = sub_problem.count;
    /*
        For each assigned job, add the cost to the LB.
    */
    for(int i = 0; i < allocations; i++) {
        worker = sub_problem.assignment[i];

        lowerBound += matrix[worker][i];

        currentAssignment[i] = sub_problem.assignment[i];
    }

    /*
        For each unassigned job, find the minumum job cost and add it to the LB.
        1. Allocate new array with as many slots as workers/jobs.
        2. Take cost value on position j from each row of the cost matrix.
    */
    for(int j = allocations; j < n; j++)
    {
        int *array = new int[n];
        /*
            Starting from the first unassigned job, store all its cost value for each worker in an array.
            Then compare ???
        */
        for(int i = 0; i < n; i++) { //i = Spalte
            array[i] = matrix[i][j];

            for(int x = 0; x < allocations; x++)
            {
                if(currentAssignment[x] == i)
                {
                    array[i]=2147483647;
                }
            }
        }

        // Spalte j durchlaufen und Minimum suchen
        /*

        */
        int agentMin = 0;
        for(int k = 1; k<n; k++)
        {
            if(array[k]<array[agentMin]) agentMin=k;
        }
        lowerBound +=array[agentMin];
        currentAssignment[j]=agentMin;
    }

    /*
        Check if the current job assignment is a valid solution. Condition for that is each
        job must be given to a different worker.
    */
    bool is_valid_solution = true;
    for(int l = 0; l < n-1; l++)
    {
        for(int m = l+1; m < n; m++)
        {
            if(currentAssignment[l] == currentAssignment[m])
            {
                is_valid_solution = false;
                break;
            }
        }
    }

    //gueltige Loesung
    if(lowerBound < u && is_valid_solution) {
        this->u = lowerBound;
        minimalAssignment = currentAssignment;
    }

    //keine gueltige Loesung
    else {
        liste.push_front(sub_problem);
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
