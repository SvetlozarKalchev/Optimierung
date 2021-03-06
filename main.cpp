#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <time.h>
#include <istream>
#include <stdexcept>

#include "BranchAndBound.h"

using namespace std;

/* Reads and returns only the first line of the input file, because it contains the number of jobs*/
int read_number_of_jobs(string file_name)
{
    int number_of_jobs;
    string line;
    ifstream input_file;

    input_file.open(file_name);

    if(!input_file)
    {
        cout << "Cannot open input file when reading # of jobs" << endl;
        return 1;
    }

    while(input_file.good())
    {
        getline(input_file, line);

        number_of_jobs = atoi(line.c_str());

        return number_of_jobs;
    }
}

/* Creates and returns an empty cost matrix with the length, specified in the input file*/
int** create_a_matrix(int length)
{
    int **matrix = new int*[length];

    /* Allocate each row of the matrix */
    for(int row = 0; row < length; row++)
    {
        matrix[row] = new int[length];
    }

    /* Fill the matrix with 0s */
    for(int row = 0; row < length; row++)
    {
        for(int col = 0; col < length; col++)
        {
            matrix[row][col] = 0;
        }
    }

    return matrix;
}

/* Reads the job costs from the input file and copies the values in a matrix */
void copy_input_values_to_matrix(string file_name, int **matrix)
{
    int line_number = 0;
    int number_of_jobs = 0;
    int matrix_row = 0;
    /* Split input on whitespace*/
    char delim = ' ';
    string temp;
    string line;

    ifstream file;

    /* Temporary array for the values in each row */
    vector<int> matrix_line;

    /* Put string here in order to filter it */
    stringstream line_as_stream;

    file.open(file_name);

    /* Throw error if file can't be found */
    if(!file)
    {
        throw invalid_argument("Input file not found");
    }

    /* Go over each line of the file */
    while(file.good())
    {
        getline(file, line);

        /* The first line is the number of rows and columns */
        if(line_number == 0)
        {
            number_of_jobs = atoi(line.c_str());
        }
            /* Other lines contain the matrix values we need to read */
        else if(line_number > 0 && line_number <= number_of_jobs)
        {
            /* Put the read line in a string stream for easy filtering */
            line_as_stream << line;

            /* Go over each value in the read line and save it to a temp array */
            while(getline(line_as_stream, temp, delim))
            {
                matrix_line.push_back(atoi(temp.c_str()));
            }

            /* Copy the separated values to the matrix */
            for(int col = 0; col < number_of_jobs; col++)
            {
                matrix[matrix_row][col] = matrix_line[col];
            }

            matrix_row++;

            /* Stream and temp array need to be flushed to accept new values */
            line_as_stream.clear();
            matrix_line.clear();
        }

        line_number++;
    }
}

/* Accepts the cost matrix and runs the branch & bound algorithm on it */
void run_branch_and_bound(BranchAndBound *bb, int number_of_jobs, int **matrix)
{
    int *optimal_assignment = bb->assignment(number_of_jobs, matrix);

    cout << '\n' << '\n' << "Optimal assignment: " << endl;

    /** Matrix form of optimal assignment:
     * x: task is not assigned to agent
     * |: task is assigned to agent
     * */
    cout << '\n' << '\t' << '\t';
    for(int i = 0; i < number_of_jobs; i++)
    {
        cout << "Task " << i+1 << '\t';
    }
    cout << '\n' << '\n' ;

    for(int row = 0; row < number_of_jobs; row++)
    {
        cout << "Agent " << row+1 << '\t' << '\t';
        for( int col = 0; col < number_of_jobs; col++)
        {
            if(optimal_assignment[col] == row)
            {
                cout << "|" << '\t';
            }
            else
            {
                cout << "x" << '\t';
            }
        }
        cout << '\n' << '\n';
    }

    int cost_sum = 0;

    for(int i = 0; i < number_of_jobs; i++)
    {
        cost_sum += matrix[optimal_assignment[i]][i];
    }

    cout << '\n' << '\n' << "Minimale Kosten: " << cost_sum << '\n' << endl;
}

bool match(const string& s, const char * c) {
    return c && s.length() > 0 && s.compare(0, s.length(), c, s.length()) == 0;
}

int main() {
    cout << "Welcome!" << endl;
    cout << "create <n>........creates new testfile with n jobs / agents and runs branch and bound algorithm" << endl;
    cout << "read <path>.......reads file from path and runs branch and bound algorithm" << endl;
    cout << "exit..............exit program" << endl;

    while (true) {
        cout.setf(ios_base::boolalpha);
        cout << endl << "> ";

        /** The string cmdline receives the input from the commandline via istringstream*/
        string cmdline;

        if (!getline(cin, cmdline)) break;

        istringstream cmdstream(cmdline);
        /** The string command is used to discriminate between the different functions:
         * "exit".....quit programme
         * "create"...create a new test file
         * "read".....read a specified file*/
        string command;

        cmdstream >> command;
        try {
            if (command.length() == 0) {
            }
            else if (match(command, "exit")) {
                break;
            }
            else if (match(command, "create")) {
                /** n = number of tasks = number of agents; by using ostringstream, int-value of n can be retrieved*/
                int n = 0;
                cmdstream >> n;
                ostringstream ss;
                ss << n;

                /** contains the fileName of the new file: <n>jobs.txt*/
                string fileName;
                fileName.append(ss.str()).append("jobs.txt");

                /**
                 * 1. Open file with the specified path (new file is created if it does not exist yet)
                 * 2. Write n into file
                 * 3. Write n x n matrix with random values into file
                 * 4. Close file
                 */
                ofstream out;
                out.open(fileName);
                out << n << '\n';
                srand(time(0));
                for(int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        out << rand() % 90 + 1 << ' ';
                    }
                    out << '\n';
                }
                out.close();
                cout << "file " << fileName << " created" << endl;

                /** Branch and bound algorithm with the data of the created file*/
                BranchAndBound *bb = new BranchAndBound();
                int **matrix;

                int number_of_jobs = read_number_of_jobs(fileName);

                cout << "Jobs: " << number_of_jobs << endl;

                matrix = create_a_matrix(number_of_jobs);

                copy_input_values_to_matrix(fileName, matrix);

                /** Column titles */
                cout << '\t' << '\t';
                for(int column = 0; column < number_of_jobs; column++) {
                    cout << "Task " << column+1 << '\t';
                }
                cout << '\n' ;

                /** Fill matrix*/
                for (int row = 0; row < number_of_jobs; row++) {
                    cout << '\n' << "Agent " << row+1 << '\t' << '\t';
                    for (int col = 0; col < number_of_jobs; col++) {
                        cout << matrix[row][col] << '\t';
                    }

                    cout << endl;
                }

                run_branch_and_bound(bb, number_of_jobs, matrix);

            }
            else if (match(command, "read")) {
                string path;
                while (cmdstream >> path) {
                    BranchAndBound *bb = new BranchAndBound();
                    int **matrix;

                    int number_of_jobs = read_number_of_jobs(path);

                    cout << "Jobs: " << number_of_jobs << endl;

                    matrix = create_a_matrix(number_of_jobs);

                    copy_input_values_to_matrix(path, matrix);

                    /** Column titles */
                    cout << '\t' << '\t';
                    for(int column = 0; column < number_of_jobs; column++) {
                        cout << "Task " << column+1 << '\t';
                    }
                    cout << '\n' ;

                    /** Fill matrix*/
                    for (int row = 0; row < number_of_jobs; row++) {
                        cout << '\n' << "Agent " << row+1 << '\t' << '\t';
                        for (int col = 0; col < number_of_jobs; col++) {
                            cout << matrix[row][col] << '\t';
                        }

                        cout << endl;
                    }

                    run_branch_and_bound(bb, number_of_jobs, matrix);
                }
            }
            else
            {
                cout << "Input was not correct, please try again:" << '\n' << endl;
                cout << "Welcome!" << endl;
                cout << "create <n>........creates new testfile with n jobs / agents and runs branch and bound algorithm" << endl;
                cout << "read <path>.......reads file from path and runs branch and bound algorithm" << endl;
                cout << "exit..............exit program" << endl;
            }
        }
        catch (...) {
            cout << "Exception occurred!" << endl;
        }


    }
}
