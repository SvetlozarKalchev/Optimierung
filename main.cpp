#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <time.h>


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
    int matrix_row = 0;
    int line_length = 0;
    int number_of_jobs = 0;
    char symbol;
    string line;
    ifstream input_file;

    input_file.open(file_name);

    if(!input_file)
    {
        cout << "Cannot open input file when copying cost values to matrix" << endl;
        return;
    }

    while(input_file.good())
    {
        getline(input_file, line);

        if(line_number == 0)
        {
            number_of_jobs = line[0] - '0';
        }

        /* Cache line.length() so it's not evaluated on every loop iteration */
        line_length = line.length();

        /*
            First line contains # of jobs, so skip it
            2nd condition is present, because for some reason the file always has an extra line. ???
        */
        if(line_number > 0 && line_number <= number_of_jobs)
        {
            /* Copy values to matrix. position+=2 to skip empty spaces between values */
            for(int column = 0, position = 0; column < number_of_jobs; column++, position+=2)
            {
                /* Substracting char '0' from a string turns it into an integer */
                matrix[matrix_row][column] = line[position] - '0';

            }

            matrix_row++;
        }

        line_number++;
    }
}

/* Accepts the cost matrix and runs the branch & bound algorithm on it */
void run_branch_and_bound(BranchAndBound *bb, int number_of_jobs, int **matrix)
{
    int *optimal_assignment = bb->assignment(number_of_jobs, matrix);

    cout << '\n' << '\n' << "Optimal assignment: " << endl;

    for(int i = 0; i < number_of_jobs; i++)
    {
        cout << optimal_assignment[i] << '\t';
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
    cout << "create <n>" << '\t' << "creates new testfile with n jobs / agents and runs branch and bound algorithm" << endl;
    cout << "read <path>" << '\t' << "reads file from path and runs branch and bound algorithm" << endl;
    cout << "exit" << '\t' << '\t' << "exit program" << endl;
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
                        out << rand() % 9 + 1 << '\t';
                    }
                    out << '\n';
                }
                out.close();
                cout << "file created" << endl;

                /** Branch and bound algorithm with the data of the created file*/
                BranchAndBound *bb = new BranchAndBound();
                int **matrix;

                int number_of_jobs = read_number_of_jobs(fileName);

                cout << "Jobs: " << number_of_jobs << endl;

                matrix = create_a_matrix(number_of_jobs);

                copy_input_values_to_matrix(fileName, matrix);

                for (int row = 0; row < number_of_jobs; row++) {
                    for (int col = 0; col < number_of_jobs; col++) {
                        cout << matrix[row][col] << ' ';
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

                    for (int row = 0; row < number_of_jobs; row++) {
                        for (int col = 0; col < number_of_jobs; col++) {
                            cout << matrix[row][col] << ' ';
                        }

                        cout << endl;
                    }

                    run_branch_and_bound(bb, number_of_jobs, matrix);
                }
            }
        }
        catch (...) {
            cout << "Exception occurred!" << endl;
        }


    }
}

/*
    vector<string> input_data = {"C:\\Users\\Sabrina1\\ClionProjects\\BranchAndBound\\5-jobs.txt"};//, "5-jobs.txt", "9-jobs.txt"};//, "10-jobs.txt"};

    int input_data_length = input_data.size(); */

/* Run branch&bound for each input file */
/*for(int job = 0; job < input_data_length; job++)
{
    BranchAndBound *bb = new BranchAndBound();
    int **matrix;

    int number_of_jobs = read_number_of_jobs(input_data[job]);

    cout << "Jobs: " << number_of_jobs << endl;

    matrix = create_a_matrix(number_of_jobs);

    copy_input_values_to_matrix(input_data[job], matrix);

    for(int row = 0; row < number_of_jobs; row++)
    {
        for(int col = 0; col < number_of_jobs; col++)
        {
            cout << matrix[row][col] << ' ';
        }

        cout << endl;
    }

    run_branch_and_bound(bb, number_of_jobs, matrix);
//        delete bb;
//        delete[] matrix;
}*/


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
*/
