/*
	Name: 		Byron Dowling
	Course:		5323 Graph Theory
	Semester:	Summer 2, 2022	
	Assignment:	Program 1

	***Program can be run on Replit from this Link***
		- https://replit.com/@ByronDowling/Graph-Theory-P1#main.go 


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

package main

import(
	"fmt"
	"bufio"
	"os"
	"log"
	"strconv"
	"strings"
)


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

func isSymmetric(AM [][]int, Size int) bool {

	for i := 0; i < Size; i++ {
		for j:= 0; j < Size; j++ {
			if AM[i][j] != AM[j][i] {
				fmt.Println("Matrix is not symmetric")
				return false
			}
		}
	}

	fmt.Println("Matrix is symmetric")
	return true
}


// Main Function
func main() {
	file, errorMessage := os.Open("input2.txt");	// Opening input file location

	// Error handling if file isn't opened successfully
	if errorMessage != nil {
		log.Fatalf("failed creating file: %s", errorMessage)
	}

	defer file.Close()								// Make sure to close our file

	scanner := bufio.NewScanner(file)				// Declaring new scanner object to read file

	AdjMat := [][]int{}								// Declaring blank 2D integer array
	count := 0
	var matrixSize int = 0

	/*
		Similar to the python syntax for line in line, we're traversing our
		file line by line by capturing the entire line as text. We will then
		parse the string and convert the significant values to integers to
		be pushed on to the array. Before this can happen however, we have to
		strip the file length size and then size the 2D array.
	*/
	for scanner.Scan() {
		str := scanner.Text()

		if count < 1 {
			fmt.Println(str)
			size, err := strconv.Atoi(str)
			if err != nil {
				panic(err)
			}

			matrixSize = size

			for i := 0; i < size; i++ {
				m := []int{}
				for j := 0; j < size; j++ {
					m = append(m, 1)
				}
				AdjMat = append(AdjMat, m)
			}
		} else {
			str := scanner.Text()
			newstr := strings.Replace(str, " ", "", -1)
			nums := strings.Split(newstr, "")

			MatLength := len([]rune(newstr))

			for i := 0; i < MatLength; i++ {
				num, err := strconv.Atoi(nums[i])
				if err != nil {
					panic(err)
				}

				AdjMat[count-1][i] = num
			}
		}
		

		count += 1
	}

	// Sanity check that our matrix matches the input file
	for i := 0; i < matrixSize; i++ {
		for j:= 0; j < matrixSize; j++ {
			fmt.Print(AdjMat[i][j])
		}
		fmt.Println()
	}

	// Calling the function to see if our matrix is symmetric or not and storing the result
	symmetric := isSymmetric(AdjMat, matrixSize)


	/*
        Below is setting the edge and graph type for the .dot file in graphviz.
        If the graph is symmetric, we will use a '--' for the connecting edge, 
        if the graph is not symmetric, we will use a '->' to connect the vertices.

        Additionally, graph or digraph is specified for proper format in graphviz.
    */

	var edge string = ""
	var graphType string = ""

	if symmetric != true {
		edge = "->"
		graphType = "digraph g"
	} else {
		edge = "--" 
		graphType = "graph g"
	}

	// open output file
    outfile, err := os.Create("output.dot")
    if err != nil {
        panic(err)
    }

	/*
        Here we are writing the results to the outfile. To do this, we
        loop through the adjaceny matrix and if the graph is symmetric, 
        to minimize redundant edges being added to the graph, we flip the 
        symmetric vertices to a -1 so that when we traverse that section of
        the graph, it will be ignored.

        Code is also set up to write the directed graph results to the file
        by utilizing the boolean operatator to adjudge whether the graph is
        symmetric or not.

		Additionally, it is easier to write results in Golang as a string so
		the results were converted back into strings and concatonated to a single
		string to write to the file.
    */

	outfile.WriteString(graphType + " {\n")

	for i := 0; i < matrixSize; i++ {
		for j:= 0; j < matrixSize; j++ {
			
			if AdjMat[i][j] == 1 && symmetric == true {
				V1 := strconv.Itoa(i)
				V2 := strconv.Itoa(j)
				AdjMat[j][i] = 0
				outfile.WriteString(V1 + edge + V2 +"\n")
			} else if AdjMat[i][j] == 1 && symmetric == false {
				V1 := strconv.Itoa(i)
				V2 := strconv.Itoa(j)
				outfile.WriteString(V1 + edge + V2 +"\n")
			}
		}
	}

	outfile.WriteString("}")


	defer outfile.Close()		// Closing the file
}
