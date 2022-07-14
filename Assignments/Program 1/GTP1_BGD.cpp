/*
    Name: 		Byron Dowling
	Course:		5323 Graph Theory
	Semester:	Summer 2, 2022	
	Assignment:	Program 1

	Assignment Details:

	Write a program that will read an adjacency matrix from a file. From that adjacency matrix,
	create a dot file that can be read by GraphViz to generate a visual representation of the
	graph:

		• The input file will contain an optional integer value that indicates the number of
		  vertices followed by the adjacency matrix. You can assume that the graph will have
		  10 or fewer vertices.

		• Check the adjacency matrix for symmetry before creating a graph or writing to
		  a file. In undirected graphs, the adjacency matrix will always be symmetric. If the
		  adjacency matrix is symmetric, create an undirected graph. If the adjacency matrix is
		  not symmetric, create a directed graph.

		• The adjacency matrix contains redundancy in a simple, undirected graph. Your
		  program should not write redundant edges to the file. Example: In the first sample
		  file shown, there is an undirected edge between nodes 0 and 1. In the dot file, only
		  0--1 is written, 1--0 is not.

		• This program does not require any graph library support1

	After reading the matrix into a 2D array and checking for symmetry, use output statements to create
	comments in the dot language, followed by the necessary graph heading (Ex. "graph g {"). Then, read 
	edges based on the adjacency matrix. For each edge in the adjacency matrix, write a corresponding
	output statement. For example, if the adjacency matrix has a 1 in position A[0, 3], print "0--3" to
	the dot file for an undirected graph or "0->3" for a directed graph. Finally, close the dot file by
	writing curly bracket "}" to the file.

		• The code in the file created by your program should generate a graph in GraphViz
		  without any changes made to the file. To check for accuracy, the file can be copied
		  and pasted at: https://dreampuf.github.io/GraphvizOnline/
*/

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;

// Typedef for a vecor of vector ints for easier syntax
typedef vector<vector<int>> AdjacenyMatrix;


/*
    Public Function void: setInput()

    Description:
        Function for allowing the user to set the input file to run
        the program.

    Params:
        - ifstream&

    Returns:
        - None
        - Parameter updated by reference
*/

void setInput(ifstream& input)
{
    char inFileName[40];

    cout << "Enter the input file name: \n";
    cin >> inFileName;
    cout << "\n";
    input.open(inFileName);
}


/*
    Public Function void: setOutput()

    Description:
        Function for allowing the user to set the output file destination to
        send the graphviz file results to.

    Params:
        - ofstream&

    Returns:
        - None
        - Parameter updated by reference
*/

void setOutput(ofstream& outfile)
{
    char outFileName[40];

    cout << "Enter the output file name: \n";
    cin >> outFileName;
    cout << "\n";
    outfile.open(outFileName);
}


/*
    Public Function void: isSymmetric()

    Description:
        Function that is used to determine if the adjacency matrix is
        symmetric or not. This is important because it tells us if we 
        have a directed or undirected graph. 

        This is checked by traversing the matrix and checking the symmetric
        pairs of each index, so Matrix[2][3] and Matrix[3][2] for example. If
        there is a mismatch in these pairs, we know the matrix is directed and 
        return false. If we traverse the entire matrix while matching symmetric
        pairs, we know that is is symmetric and return true.

    Params:
        - vector<vector<int>>

    Returns:
        - bool
*/

bool isSymmetric(AdjacenyMatrix& AdjMat)
{
    for (int i = 0; i < AdjMat.size(); i++)
    {
        for (int j = 0; j < AdjMat[i].size(); j++)
        {
            if (AdjMat[i][j] != 0)
            {
                if (AdjMat[i][j] != AdjMat[j][i])
                {
                    cout << "Matrix is not symmetric\n";
                    return false;
                }
            }
        }
    }

    cout << "Matrix is symmetric\n";
    return true;
}


int main()
{
    // Setting file destinations
    ifstream infile;
    ofstream outfile;
    setInput(infile);
    setOutput(outfile);

    // Reading in the size of the Matrix
    int size;
    infile >> size;

    /*
        Below, we are declaring a vector of vector ints, which has been typedef-ed
        as Adjacency Matrix. Next we are resizing the empty vector to the size of
        the matrix that we have read in. After that, since we know adjacency matrices
        are square, we will then resize the 2D portion of the matrix.

            Example on Size 5

            AdjacenyMatrix AM = {} *empty

            AM.resize(5) >> AM = [ ]->[ ]->[ ]->[ ]->[ ]

            AM[i].resize(5):

            AM = {
                 i = 0   [ ]->[ ]->[ ]->[ ]->[ ]
                          ^    ^    ^    ^    ^
                 i = 1   [ ]->[ ]->[ ]->[ ]->[ ]
                          ^    ^    ^    ^    ^
                 i = 2   [ ]->[ ]->[ ]->[ ]->[ ]
                          ^    ^    ^    ^    ^
                 i = 3   [ ]->[ ]->[ ]->[ ]->[ ]
                          ^    ^    ^    ^    ^
                 i = 4   [ ]->[ ]->[ ]->[ ]->[ ]
                }

    */
    AdjacenyMatrix AM;
    AM.resize(size);

    for (int i = 0; i < size; i++)
    {
        AM[i].resize(size);
    }

    int row = -1;


    /*
        Reading till the end of the file, at each iteration, we perform a
        getline and place the result in a temp string.

        Next we loop through that string while ignoring ASCII 32 (space) and 
        convert the character into an int by casting the character as an int
        and subtracting 48 to get the actual integer value.
    */

    while (!infile.eof())
    {
        string temp;
        int ct = 0;
        getline(infile, temp);

        for (int i = 0; i < temp.length(); i++)
        {
            if (temp[i] != 32)
            {
                int num = (int(temp[i]) - 48);
                AM[row][ct] = num;
                ct++;
            }
        }

        row++;
    }


    /*
        Below is setting the edge and graph type for the .dot file in graphviz.
        If the graph is symmetric, we will use a '--' for the connecting edge, 
        if the graph is not symmetric, we will use a '->' to connect the vertices.

        Additionally, graph or digraph is specified for proper format in graphviz.
    */
    string edge;
    string type;
    bool symmetric = isSymmetric(AM);

    if (!symmetric)
    {
        edge = "->";
        type = "digraph g";
    }

    else
    {
        edge = "--";
        type = "graph g";
    }

    outfile << type << " {\n";


    /*
        Here we are writing the results to the outfile. To do this, we
        loop through the adjaceny matrix and if the graph is symmetric, 
        to minimize redundant edges being added to the graph, we flip the 
        symmetric vertices to a -1 so that when we traverse that section of
        the graph, it will be ignored.

        Code is also set up to write the directed graph results to the file
        by utilizing the boolean operatator to adjudge whether the graph is
        symmetric or not.
    */

    for (int i = 0; i < AM.size(); i++)
    {
        for (int j = 0; j < AM[i].size(); j++)
        {
            if (AM[i][j] == 1 && symmetric)
            {
                outfile << j << edge << i << "\n";
                AM[j][i] = -1;
            }

            else if (AM[i][j] == 1 && !symmetric)
            {
                outfile << i << edge << j << "\n";
            }
        }
    }

    outfile << "}";
}
