#ifndef BRANCHANDBOUND_BRANCHANDBOUND_H
#define BRANCHANDBOUND_BRANCHANDBOUND_H

#include <iostream>
#include <list>

using namespace std;

class BranchAndBound {
private:
    struct Problem {
        int l; //lower bound
        int *assignment; //assigns tasks to agents
        int count; //counts how many tasks are already assigned

        /* Contstructor and destructor */
        Problem(int n) : l{0}, assignment{new int[n]{}}, count{0} {}
        ~Problem() {delete[] assignment;}
        Problem(const Problem& p) : l{p.l}, count{p.count}, assignment{new int[sizeof(p.assignment)]} {
            for(int i = 0; i < sizeof(p.assignment); i++) {
                assignment[i] = p.assignment[i];
            }; }

        /* = operator */
        Problem& operator=(const Problem& p) {
            int *newAssignment = new int[sizeof(p.assignment)];
            for(int i = 0; i < sizeof(p.assignment); i++) {
                newAssignment[i] = p.assignment[i];
            }
            delete[] assignment;
            assignment = newAssignment;
            l = p.l;
            count = p.count;
            return *this;
        }
    };

    typedef list<Problem> BBlist;
    BBlist liste; //list containing problems that need to be processed
    int n; //number of tasks, number of agents
    int** matrix; //cost matrix
    int u; // upper bound, max range of int instead of infinity
    int * minimalAssignment;

public:
    /* Constructor and destructor*/
    BranchAndBound() : u{2147483647}, liste{} {}
    ~BranchAndBound() {  delete[] matrix;}

    int* assignment(int n, int **kosten);
    BBlist branch(Problem problem);
    bool is_solution_valid(int *current_assignment);
    void bound(Problem teilproblem);
    Problem minProblem();
    BranchAndBound::Problem* setProblem(int n);

};

/*
    Array: [row][column]
    Method receives number of jobs/workers and cost matrix. Returns an int-array with the assignment so that the costs are minimal.
*/
int* BranchAndBound::assignment(int n, int **kosten) {
    /* Assign number of workers and cost matrix to class variables */
    this->n = n;
    this->matrix=kosten;
    liste.push_front(Problem(n));
    minimalAssignment = new int[n]{};

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
    BranchAndBound::BBlist problem_list;
    int number_of_assignments = problem.count;
    int number_of_workers = this->n;
    int **matrix = this->matrix;

    /* 1. Only proceed if there are unassigned jobs*/
    if(number_of_assignments != number_of_workers)
    {
        /* 2. Increment allocation counter in the problem, because we are allocating +1 job */
        problem.count++;

        /* 3. Start from the unallocated job */
        for(int job = number_of_assignments; job < number_of_assignments + 1; job++)
        {
            /* 4. Go through each row, e.g worker and assign a job */
            for(int agent = 0; agent < number_of_workers; agent++)
            {
                problem.assignment[job] = agent; //agent statt matrix[agent][job]

                /* 5. Push problem variation to BBlist */
                Problem neuesProblem = Problem(n);
                neuesProblem = problem;
                problem_list.push_front(neuesProblem);
            }
        }
    }

    /* 6. Return list with branches */
    return problem_list;
}


/*
    Method that accepts an assignment and checks wheter the assignemnt is valid or not.
 */
bool BranchAndBound::is_solution_valid(int *current_assignment)
{
    bool is_valid = true;
    int number_of_jobs = this->n;

    /* Compare each value with assignment array with all other values to find if there are any duplicates */
    for(int i = 0; i < number_of_jobs - 1; i++)
    {
        for(int j = i+1; j < number_of_jobs; j++)
        {
            if(current_assignment[i] == current_assignment[j])
            {
                is_valid = false;
                break;
            }
        }
    }

    return is_valid;
}
/*
    Method that accepts a problem and calculates its lower bound.
*/
void BranchAndBound::bound(Problem sub_problem)
{
    int lowerBound = 0;
    int *currentAssignment = new int[n] {};
    /*
        Go through the cost matrix and sum up the corresponding worker/job cost values
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
        2. Take cost value on position j from each row of the cost matrix. If agent is already assigned, set value to 2147483647 (maximal int value, this agent can only do one task)
    */
    for(int j = allocations; j < n; j++)
    {
        int *array = new int[n];
        for(int i = 0; i < n; i++) {
            array[i] = matrix[i][j];

            for(int x = 0; x < allocations; x++)
            {
                if(currentAssignment[x] == i)
                {
                    array[i]=2147483647;
                }
            }
        }

        /*
            Search array for minimal value and assign agent with minimal value to this task.
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
    bool valid_solution = is_solution_valid(currentAssignment);

    /*
        Lowerbound is smaller than the upper bound and solution is valid: Change upper bound to lower bound, change minimalAssignment to currentAssignment
     */
    if(lowerBound < u && valid_solution) {
        this->u = lowerBound;
        minimalAssignment = currentAssignment;
    }
        /*
            Solution is not valid: Add Problem to list.
        * */
    else if(!valid_solution) {
        liste.push_front(sub_problem);
    }
}

/*
    Method finds Problem with minimal lowerbound in the list of problems.
 * */
BranchAndBound::Problem BranchAndBound::minProblem() {
    //liste->sort();
    return liste.front();
}

BranchAndBound::Problem* BranchAndBound::setProblem(int n) {
    Problem* neuesProblem = new Problem(n);
    return neuesProblem;
}

#endif //BRANCHANDBOUND_BRANCHANDBOUND_H
