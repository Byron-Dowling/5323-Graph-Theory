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
