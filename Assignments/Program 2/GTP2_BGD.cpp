/*
    Name: 		Byron Dowling
	Course:		5323 Graph Theory
	Semester:	Summer 2, 2022	
	Assignment:	Program 2 - Minimum Spanning Tree - Prim's Algorithm

	Assignment Details:

        Write a program that will implement a Minimum Spanning Tree algorithm. You may
        write your own code, if you are industrious, or use a pre-built algorithm.
        Visually represent the MST using code from your previous project or another
        visualization tool. Show the initial graph and the MST in one picture. For example,
        draw the initial graph with the edges of the MST using dashed lines or a different
        color.

        My solution is a litte over-engineered but I was creating it with an eye to the future
        where if we implement Dijkstra's, BFS, DFS, or other functionality that I can reuse
        most of this code and not have to make too many changes.

        The overall strategy is utilizing object pointers to store all the information that we
        receive from the adjacency matrix in the event that it may be useful at a later time and
        since we're using object pointers, it is more memory efficient than creating copies of
        each object.

*/

#include<iostream>
#include<vector>
#include<fstream>
#include<deque>
#include<map>
#include<string>
#include<algorithm>

using namespace std;

/* 
    Declaring these without implementation so sections of code are aware of these structs
    before they are actually implemented.
*/
struct Edge;
struct Graph_Node;

typedef vector<vector<int>> AdjacencyMatrix;   // Typedef for a vector of vector ints for the Adjacency Matrix
typedef vector<Edge*> AdjacencyList;           // Typedef for a vecotr of Edge pointers for the Adjacency List
typedef pair<char, char> EdgePair;             // Typedef for a pair of chars for looking up edges


/*
 __    __                  __
|  \  |  \                |  \
| $$\ | $$  ______    ____| $$  ______
| $$$\| $$ /      \  /      $$ /      \
| $$$$\ $$|  $$$$$$\|  $$$$$$$|  $$$$$$\
| $$\$$ $$| $$  | $$| $$  | $$| $$    $$
| $$ \$$$$| $$__/ $$| $$__| $$| $$$$$$$$
| $$  \$$$ \$$    $$ \$$    $$ \$$     \
 \$$   \$$  \$$$$$$   \$$$$$$$  \$$$$$$$

    Description:

		This struct is used to represent a Graph Node or vertex of the graph that we
        are building. The object will have an ID, a visited status used in traversals
        and an Adjacency list that is a list of edge pairs that the node has.


	Public Methods:
		- Graph_Node() - Default Constructor
        - Graph_Node(int n) - Overloaded Constructor


	Private/Protected Methods:
		- None

	Usage:
		- This class creates a graph node object

*/

struct Graph_Node
{
    char ID;
    AdjacencyList AL;
    bool Visited;

    Graph_Node()
    {
        ID = 'A';
        Visited = false;
    }

    Graph_Node(int n)
    {
        ID = 'A' + n;
        Visited = false;
    }
};


/*
 ________       __
|        \     |  \
| $$$$$$$$ ____| $$  ______    ______
| $$__    /      $$ /      \  /      \
| $$  \  |  $$$$$$$|  $$$$$$\|  $$$$$$\
| $$$$$  | $$  | $$| $$  | $$| $$    $$
| $$_____| $$__| $$| $$__| $$| $$$$$$$$
| $$     \\$$    $$ \$$    $$ \$$     \
 \$$$$$$$$ \$$$$$$$ _\$$$$$$$  \$$$$$$$
                   |  \__| $$
                    \$$    $$
                     \$$$$$$

    Description:

		This struct is used to represent a Graph edge that represents the weighted
        path between two graph vertices. It has both the start and end Graph Node
        as well as the weight of the edge and a boolean to indicate if the edge is
        included in Kruskals MST.


	Public Methods:
        - Edge(Graph_Node* S, Graph_Node* E, int w) - Overloaded Constructor


	Private/Protected Methods:
		- None

	Usage:
		- This class creates a graph edge object
*/

struct Edge
{
    Graph_Node* Start;
    Graph_Node* End;
    int Weight;
    bool K;

    Edge(Graph_Node* S, Graph_Node* E, int w)
    {
        Start = S;
        End = E;
        Weight = w;
        K = false;
    }
};


/*
 __     __                        __
|  \   |  \                      |  \
| $$   | $$  ______    _______  _| $$_     ______    ______
| $$   | $$ /      \  /       \|   $$ \   /      \  /      \
 \$$\ /  $$|  $$$$$$\|  $$$$$$$ \$$$$$$  |  $$$$$$\|  $$$$$$\
  \$$\  $$ | $$    $$| $$        | $$ __ | $$  | $$| $$   \$$
   \$$ $$  | $$$$$$$$| $$_____   | $$|  \| $$__/ $$| $$
    \$$$    \$$     \ \$$     \   \$$  $$ \$$    $$| $$
     \$      \$$$$$$$  \$$$$$$$    \$$$$   \$$$$$$  \$$

  ______                        __
 /      \                      |  \
|  $$$$$$\  ______    ______  _| $$_
| $$___\$$ /      \  /      \|   $$ \
 \$$    \ |  $$$$$$\|  $$$$$$\\$$$$$$
 _\$$$$$$\| $$  | $$| $$   \$$ | $$ __
|  \__| $$| $$__/ $$| $$       | $$|  \
 \$$    $$ \$$    $$| $$        \$$  $$
  \$$$$$$   \$$$$$$  \$$         \$$$$


    Public Function bool: Compare

    Description:

        The purpose of this function is essentially overloading the built-in sort
        method that is a part of #include<algorithm> header file.

        Essentially, we pass in the beginning and end indexes of the vector of
        objects that we want to sort and then we specify a function to call, in
        this case the one here. We take in two Edge Pointers since that's what the
        vector will hold and we are sorting by weight because we want to order the
        edges in our Edge list from smallest to largest weight. This makes it easier
        than pushing the edges into the priority queue and then traversing it or having
        to sort the priority queue.

    Params:
        - const Edge* LHS
        - const Edge* RHS

    Returns:
        - bool

*/

bool Compare(const Edge* LHS, const Edge* RHS)
{
  if (LHS->Weight != RHS->Weight)
  {
    return LHS->Weight < RHS->Weight;
  }

  else
    return LHS->End < RHS->End;
}



/*
  ______                                __
 /      \                              |  \
|  $$$$$$\  ______   ______    ______  | $$____
| $$ __\$$ /      \ |      \  /      \ | $$    \
| $$|    \|  $$$$$$\ \$$$$$$\|  $$$$$$\| $$$$$$$\
| $$ \$$$$| $$   \$$/      $$| $$  | $$| $$  | $$
| $$__| $$| $$     |  $$$$$$$| $$__/ $$| $$  | $$
 \$$    $$| $$      \$$    $$| $$    $$| $$  | $$
  \$$$$$$  \$$       \$$$$$$$| $$$$$$$  \$$   \$$
                             | $$
                             | $$
                              \$$

	Description:

		This class is the main driver for the program. It builds the graph by receiving an adjacency
        matrix and creating Maps of Vertices and Edges. The maps will offer faster lookup times than
        traversing the containers that they will be stored in such as a vector or deque.

        When implementing the minimum spanning tree algorithm, this class will actually store both
        the Prims and Kruskals variations by storing the vertices in order for the traversal as well
        as the edge paths. 


	Public Methods:
		- Graph() - Default Constructor
        - void LoadMatrix(ifstream& input)
        - void PrintMatrix()
        - void PrintMap()
        - void isSymmetric()
        - void MakeGraphVizFile()
        - void SortWeights()
        - void ResetVisitStatus()
        - void MinimumSpanningTree_Prims()


	Private/Protected Methods:
		- None

	Usage:
		- This class creates an object that determines the blob count for a 2D Matrix

*/

class Graph
{
    protected:

        AdjacencyMatrix AM;
        map<char, Graph_Node*> Vertices;
        map<EdgePair, Edge*> Edges;
        vector<Graph_Node*> Prims;
        vector<Edge*> Kruskals;
        deque<Edge*> PriorityQ;
        bool Symmetric;
        ofstream out;

    public:

        Graph()
        {
            Symmetric=false;
            out.open("GV_results.dot");
        }

        void LoadMatrix(ifstream& input);
        void PrintMatrix();
        void PrintMap();
        void isSymmetric();
        void MakeGraphVizFile();
        void SortWeights();
        void ResetVisitStatus();
        int DijkstrasShortestPath(Graph_Node* Start, Graph_Node* End);      // Not implemented yet
        void MinimumSpanningTree_Prims();
        vector<Graph_Node*> DepthFirstSearch();                             // Not implemented yet
        vector<Graph_Node*> BreadthFirstSearch();                           // Not implemented yet
};


/*
    Utility function to reset the visited status of our vertices before we test
    the data set again in the same run. 
*/
void Graph::ResetVisitStatus()
{
    for (auto it = Vertices.cbegin(); it != Vertices.cend(); it++)
    {
        it->second->Visited = false;
    }
}


/*
 __       __  __            __
|  \     /  \|  \          |  \
| $$\   /  $$ \$$ _______   \$$ ______ ____   __    __  ______ ____
| $$$\ /  $$$|  \|       \ |  \|      \    \ |  \  |  \|      \    \
| $$$$\  $$$$| $$| $$$$$$$\| $$| $$$$$$\$$$$\| $$  | $$| $$$$$$\$$$$\
| $$\$$ $$ $$| $$| $$  | $$| $$| $$ | $$ | $$| $$  | $$| $$ | $$ | $$
| $$ \$$$| $$| $$| $$  | $$| $$| $$ | $$ | $$| $$__/ $$| $$ | $$ | $$
| $$  \$ | $$| $$| $$  | $$| $$| $$ | $$ | $$ \$$    $$| $$ | $$ | $$
 \$$      \$$ \$$ \$$   \$$ \$$ \$$  \$$  \$$  \$$$$$$  \$$  \$$  \$$
  ______                                           __
 /      \                                         |  \
|  $$$$$$\  ______    ______   _______   _______   \$$ _______    ______
| $$___\$$ /      \  |      \ |       \ |       \ |  \|       \  /      \
 \$$    \ |  $$$$$$\  \$$$$$$\| $$$$$$$\| $$$$$$$\| $$| $$$$$$$\|  $$$$$$\
 _\$$$$$$\| $$  | $$ /      $$| $$  | $$| $$  | $$| $$| $$  | $$| $$  | $$
|  \__| $$| $$__/ $$|  $$$$$$$| $$  | $$| $$  | $$| $$| $$  | $$| $$__| $$
 \$$    $$| $$    $$ \$$    $$| $$  | $$| $$  | $$| $$| $$  | $$ \$$    $$
  \$$$$$$ | $$$$$$$   \$$$$$$$ \$$   \$$ \$$   \$$ \$$ \$$   \$$ _\$$$$$$$
          | $$                                                  |  \__| $$
          | $$                                                   \$$    $$
           \$$                                                    \$$$$$$
 ________
|        \
 \$$$$$$$$______    ______    ______
   | $$  /      \  /      \  /      \
   | $$ |  $$$$$$\|  $$$$$$\|  $$$$$$\
   | $$ | $$   \$$| $$    $$| $$    $$
   | $$ | $$      | $$$$$$$$| $$$$$$$$
   | $$ | $$       \$$     \ \$$     \
    \$$  \$$        \$$$$$$$  \$$$$$$$


    Public Method void: MinimumSpanningTree_Prims

    Description:

        The purpose of this function is to traverse our Graph and determine the
        Minimum Spanning Tree. The algorithm of choice here is Prims algorithm
        where by default the graph starts at Node A and determines the minimum
        spanning tree from this node.

        Essentially, starting at Node A, we have at this point already sorted the
        Graph Node neighbors by weight so that when we push A's neighbors into the
        Priority Queue, they are already sorted and we don't have to traverse or sort
        the priority queue to find the edge with the lowest weight.

        Once we visit a node and push its neighbors into the queue, we mark it as visited
        and push it to the Prims vector. Then while our Priority Queue is not empty, we 
        continue this trend of popping from the front of the Priority Queue, this is why
        we are using a deque which is a vector with pop_front and push_front methods.



    Params:
        - Utilizes class member variables

    Returns:
        - None
        - Updates class member variables


*/

void Graph::MinimumSpanningTree_Prims()
{
    Graph_Node* Current = Vertices['A'];

    Prims.push_back(Current);
    Current->Visited = true;


    // Adding A's neighbors to the queue
    for (int i = 0; i < Current->AL.size(); i++)
    {
        PriorityQ.push_back(Current->AL[i]);
        cout << "\nPushing " << Current->AL[i]->End->ID << " with weight " << Current->AL[i]->Weight << " to the Queue\n";
    }


    /*
        We will loop until the Priority Queue is empty at which point we will have traversed the
        entire graph and should have the result of the Minimum Spanning Tree.
    */

    while (!PriorityQ.empty())
    {
        cout << "Queue Size: " << PriorityQ.size() << "\n";

        Graph_Node* Current = Vertices[PriorityQ.front()->End->ID];

        bool cycle = false;


        // Check that a node in the queue isn't already visited indicating that a cycle would occur
        if (Current->Visited)
        {
            cout << "Node " << Current->ID << " already visited, popping from queue.\n";
            cycle = true;
            PriorityQ.pop_front();
        }
        

        /*
            If no cycle is detected, we continue the process of pushing the current node's neighbors
            onto the Queue and continue with the process.

            Since almost everything I'm using here is object pointers, the syntax can get a little 
            ugly, for example:
                    
                    !Current->AL[i]->End->Visited 

                    Indicates we are checking if the edge pair has been visited or not by taking the
                    current node, accessing the current node of its adjacency matrix, and then accessing
                    the edge pair held at that position and seeing if it has been visited or not.
        */
        if (!cycle)
        {   
            cout << "No cycle detected.\n";
            Prims.push_back(Current);                       // Push to Prims Vector

            Current->Visited = true;                        // Mark Current as Visited

            cout << Current->ID << "'s neigbors are: ";

            for (int i = 0; i < Current->AL.size(); i++)
            {
                cout << Current->AL[i]->End->ID << " ";

                if (!Current->AL[i]->End->Visited)
                {
                    PriorityQ.push_back(Current->AL[i]);
                    cout << "Pushing " << Current->AL[i]->End->ID << " with weight " << Current->AL[i]->Weight << " to the Queue\n";
                }
            }

            cout << "\n";

            // Pushing to the Kruskals vector and marking the edge as a kruskals edge for printing purposes
            Kruskals.push_back(PriorityQ.front());
            PriorityQ.front()->K = true;
            PriorityQ.pop_front();

            // Having to implicitly declare std sort to get rid of ambiguity error
            std::sort(PriorityQ.begin(), PriorityQ.end(), Compare);
        }
    }

    // Sanity check that we grabbed the correct vertices.
    for (int i = 0; i < Prims.size(); i++)
    {
        cout << Prims[i]->ID << " ";
    }

    cout << "\n";

    // Sanity check that we grabbed the correct edges.
    for (int z = 0; z < Kruskals.size(); z++)
    {
        cout << Kruskals[z]->Start->ID << Kruskals[z]->End->ID << " ";
    }

    cout << "\n";
}


/*
  ______                                __       __     __  __
 /      \                              |  \     |  \   |  \|  \
|  $$$$$$\  ______   ______    ______  | $$____ | $$   | $$ \$$ ________
| $$ __\$$ /      \ |      \  /      \ | $$    \| $$   | $$|  \|        \
| $$|    \|  $$$$$$\ \$$$$$$\|  $$$$$$\| $$$$$$$\\$$\ /  $$| $$ \$$$$$$$$
| $$ \$$$$| $$   \$$/      $$| $$  | $$| $$  | $$ \$$\  $$ | $$  /    $$
| $$__| $$| $$     |  $$$$$$$| $$__/ $$| $$  | $$  \$$ $$  | $$ /  $$$$_
 \$$    $$| $$      \$$    $$| $$    $$| $$  | $$   \$$$   | $$|  $$    \
  \$$$$$$  \$$       \$$$$$$$| $$$$$$$  \$$   \$$    \$     \$$ \$$$$$$$$
                             | $$
                             | $$
                              \$$
*/

void Graph::MakeGraphVizFile()
{
    string edge;
    string type;

    if (!Symmetric)
    {
        edge = "->";
        type = "digraph g";
    }

    else
    {
        edge = "--";
        type = "graph g";
    }

    out << type << " {\n";


    /*
        Here we are writing the results to the outfile. To do this, we
        loop through the adjaceny matrix and if the graph is symmetric, 
        to minimize redundant edges being added to the graph, we flip the 
        symmetric vertices to a -1 so that when we traverse that section of
        the graph, it will be ignored.

        Code is also set up to write the directed graph results to the file
        by utilizing the boolean operatator to adjudge whether the graph is
        symmetric or not.

        To format the minimum spanning tree, we utilize our edge map to look up
        if the edge pair is marked K = true which would indicate the edge is part
        of the minimum spanning tree. As a result, the Graphviz file will show the
        graph in its entirety with the edges colored red that are part of the Minimum
        spanning Tree.
    */

    for (int i = 0; i < AM.size(); i++)
    {
        for (int j = 0; j < AM[i].size(); j++)
        {
            if (AM[i][j] != 0 && Symmetric)
            {
                char U = 'A' + j;
                char V = 'A' + i;

                /*
                    Checking both variation of the edge pair to see if the current edge
                    is part of the spanning tree or not.

                    Auto is used for easier and cleaner syntax.
                */
                auto P1 = make_pair(U,V);
                auto P2 = make_pair(V,U);
            
                out << V << edge << U << "[label= " << AM[i][j] << ", weight=" << AM[i][j];

                // Indicates edge is part of the MST
                if (Edges[P1]->K)
                {
                    out << ", color = red";
                }

                // Indicates edge is part of the MST
                else if (Edges[P2]->K)
                {
                    out << ", color = red";
                }

                out << "]\n";
                AM[j][i] = 0;
            }

            else if (AM[i][j] != 0 && !Symmetric)
            {
                char U = 'A' + j;
                char V = 'A' + i;

                auto P = make_pair(U, V);

                out << V << edge << U << "[label= " << AM[i][j] << ", weight=" << AM[i][j] << "]\n";

                if (Edges[P]->K)
                {
                    out << ", color = red";
                }
            }
        }
    }

    out << "}";

    out.close();
}


/*
 __       __             __                __
|  \     /  \           |  \              |  \
| $$\   /  $$  ______  _| $$_     ______   \$$ __    __
| $$$\ /  $$$ |      \|   $$ \   /      \ |  \|  \  /  \
| $$$$\  $$$$  \$$$$$$\\$$$$$$  |  $$$$$$\| $$ \$$\/  $$
| $$\$$ $$ $$ /      $$ | $$ __ | $$   \$$| $$  >$$  $$
| $$ \$$$| $$|  $$$$$$$ | $$|  \| $$      | $$ /  $$$$\
| $$  \$ | $$ \$$    $$  \$$  $$| $$      | $$|  $$ \$$\
 \$$      \$$  \$$$$$$$   \$$$$  \$$       \$$ \$$   \$$

  ______                                                    __
 /      \                                                  |  \
|  $$$$$$\ __    __  ______ ____   ______ ____    ______  _| $$_     ______   __    __
| $$___\$$|  \  |  \|      \    \ |      \    \  /      \|   $$ \   /      \ |  \  |  \
 \$$    \ | $$  | $$| $$$$$$\$$$$\| $$$$$$\$$$$\|  $$$$$$\\$$$$$$  |  $$$$$$\| $$  | $$
 _\$$$$$$\| $$  | $$| $$ | $$ | $$| $$ | $$ | $$| $$    $$ | $$ __ | $$   \$$| $$  | $$
|  \__| $$| $$__/ $$| $$ | $$ | $$| $$ | $$ | $$| $$$$$$$$ | $$|  \| $$      | $$__/ $$
 \$$    $$ \$$    $$| $$ | $$ | $$| $$ | $$ | $$ \$$     \  \$$  $$| $$       \$$    $$
  \$$$$$$  _\$$$$$$$ \$$  \$$  \$$ \$$  \$$  \$$  \$$$$$$$   \$$$$  \$$       _\$$$$$$$
          |  \__| $$                                                         |  \__| $$
           \$$    $$                                                          \$$    $$
            \$$$$$$                                                            \$$$$$$

    Public Method void: isSymmetric()

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
        - Utilizes member class variable AdjacencyMatrix

    Returns:
        - None
        - Updates member class variable
*/

void Graph::isSymmetric()
{
    for (int i = 0; i < AM.size(); i++)
    {
        for (int j = 0; j < AM[i].size(); j++)
        {
            if (AM[i][j] != 0)
            {
                if (AM[i][j] != AM[j][i])
                {
                    Symmetric = false;
                }
            }
        }
    }

    Symmetric = true;
}


/*
  ______                        __
 /      \                      |  \
|  $$$$$$\  ______    ______  _| $$_
| $$___\$$ /      \  /      \|   $$ \
 \$$    \ |  $$$$$$\|  $$$$$$\\$$$$$$
 _\$$$$$$\| $$  | $$| $$   \$$ | $$ __
|  \__| $$| $$__/ $$| $$       | $$|  \
 \$$    $$ \$$    $$| $$        \$$  $$
  \$$$$$$   \$$$$$$  \$$         \$$$$
 __       __            __            __         __
|  \  _  |  \          |  \          |  \       |  \
| $$ / \ | $$  ______   \$$  ______  | $$____  _| $$_     _______
| $$/  $\| $$ /      \ |  \ /      \ | $$    \|   $$ \   /       \
| $$  $$$\ $$|  $$$$$$\| $$|  $$$$$$\| $$$$$$$\\$$$$$$  |  $$$$$$$
| $$ $$\$$\$$| $$    $$| $$| $$  | $$| $$  | $$ | $$ __  \$$    \
| $$$$  \$$$$| $$$$$$$$| $$| $$__| $$| $$  | $$ | $$|  \ _\$$$$$$\
| $$$    \$$$ \$$     \| $$ \$$    $$| $$  | $$  \$$  $$|       $$
 \$$      \$$  \$$$$$$$ \$$ _\$$$$$$$ \$$   \$$   \$$$$  \$$$$$$$
                           |  \__| $$
                            \$$    $$
                             \$$$$$$


    Utility Function that loops through our Map of vertices and sorts
    the edges in each Vertex's Adjacency matrix by its weight. This helps
    our priority queue by guaranteeing that the edges that are pushed to
    the queue are in order of smallest weight to largest weight.
*/
void Graph::SortWeights()
{
    for (auto it = Vertices.cbegin(); it != Vertices.cend(); it++)
    {
        auto Current = it->second;
        std::sort(Current->AL.begin(), Current->AL.end(), Compare);
    }
}


/*
    Utility Function that is used to make sure the Map looks correct.
    Used in debugging and will have use in future programs.
*/
void Graph::PrintMap()
{
    for (auto it = Vertices.cbegin(); it != Vertices.cend(); it++)
    {
        cout << it->first << " ";
    }

    cout << "\n\n";

    for (auto it = Edges.cbegin(); it != Edges.cend(); it++)
    {
        cout << it->second->Start->ID << it->second->End->ID << " ";
    }

    cout << "\n\n";
}


/*
 __                                 __
|  \                               |  \
| $$       ______    ______    ____| $$
| $$      /      \  |      \  /      $$
| $$     |  $$$$$$\  \$$$$$$\|  $$$$$$$
| $$     | $$  | $$ /      $$| $$  | $$
| $$_____| $$__/ $$|  $$$$$$$| $$__| $$
| $$     \\$$    $$ \$$    $$ \$$    $$
 \$$$$$$$$ \$$$$$$   \$$$$$$$  \$$$$$$$

 __       __             __                __
|  \     /  \           |  \              |  \
| $$\   /  $$  ______  _| $$_     ______   \$$ __    __
| $$$\ /  $$$ |      \|   $$ \   /      \ |  \|  \  /  \
| $$$$\  $$$$  \$$$$$$\\$$$$$$  |  $$$$$$\| $$ \$$\/  $$
| $$\$$ $$ $$ /      $$ | $$ __ | $$   \$$| $$  >$$  $$
| $$ \$$$| $$|  $$$$$$$ | $$|  \| $$      | $$ /  $$$$\
| $$  \$ | $$ \$$    $$  \$$  $$| $$      | $$|  $$ \$$\
 \$$      \$$  \$$$$$$$   \$$$$  \$$       \$$ \$$   \$$


    Public Method void: LoadMatrix()

    Description:

        Function will create a graph by reading from an adjacenecy matrix file
        and creating the graph by allocating new Graph_Node object pointers. These
        vertices are pushed to our vertex map for faster lookups and then edge pair
        object pointers are also created and added to each graph_node's adjacency list
        and these edge pairs are also pushed to our edge map which will again allow for
        faster lookup speeds later on in the program.

        So in summary, this method is creating all of graph objects from the original
        adjacenecy matrix that is read in from the file that is passed in.

    Params:
        - ifstream& input

    Returns:
        - None
        - Updates member class variable
*/

void Graph::LoadMatrix(ifstream& input)
{
    int row = -1;
    int position = 0;
    int size;

    char inFileName[40];
    char outFileName[40];

    cout << "Enter the input file name: \n";
    cin >> inFileName;
    cout << "\n";

    input.open(inFileName);

    input >> size;
    AM.resize(size);


    /*
        Resizing the 2D matrix and creating the graph nodes. The graph
        node id is created by passing in the i index and then the overloaded
        constructor for the graph node sets the id as char 'A' + i, so graph
        node id's are created as A, B, C, D, etc.

        The vertices are also added to the vertices map with the ID as the key.
    */
    for (int i = 0; i < AM.size(); i++)
    {
        Graph_Node* Temp = new Graph_Node(i);
        Vertices[Temp->ID] = Temp;
        AM[i].resize(size);
    }


    while (!input.eof())
    {
        string temp;
        int ct = 0;
        getline(input, temp);

        for (int i = 0; i < temp.length(); i++)
        {
            // Skipping white space
            if (temp[i] != 32)
            {
                // Deriving the actual numerical value by subtracting 48 from the ASCII value
                int num = (int(temp[i]) - 48);
                AM[row][ct] = num;
                ct++;
            }
        }

        row++;
    }


    for (int i = 0; i < AM.size(); i++)
    {
        char U = ('A' + i);

        for (int j = 0; j < AM[i].size(); j++)
        {
            if (AM[j][i] != 0)
            {
                // Using a Map for faster lookup in creating the edge pairs
                char V = ('A' + j);
                int Weight = AM[j][i];

                /* 
                    Creating the edge pairs by indicating the starting and ending graph nodes
                    Since they are pointers and not actual objects, this is more memory efficient
                    that creating copies of each object.
                */
                Graph_Node* Start = Vertices[U];
                Graph_Node* End = Vertices[V];

                Edge* E = new Edge(Start, End, Weight);

                // Making a char pair typedeffed as an edge pair and using that as the map key
                auto P1 = make_pair(Start->ID, End->ID);
                Edges[P1] = E;

                Start->AL.push_back(E);
            }
        }
    }
}


/*
    Utility function to make sure we're reading in the adjacency
    matrix correctly. Used in debugging and potential future use.
*/

void Graph::PrintMatrix()
{
    for (int i = 0; i < AM.size(); i++)
    {
        for (int j = 0; j < AM[i].size(); j++)
        {
            cout << AM[i][j] << " ";
        }

        cout << "\n";
    }
}


/*
 __       __            __
|  \     /  \          |  \
| $$\   /  $$  ______   \$$ _______
| $$$\ /  $$$ |      \ |  \|       \
| $$$$\  $$$$  \$$$$$$\| $$| $$$$$$$\
| $$\$$ $$ $$ /      $$| $$| $$  | $$
| $$ \$$$| $$|  $$$$$$$| $$| $$  | $$
| $$  \$ | $$ \$$    $$| $$| $$  | $$
 \$$      \$$  \$$$$$$$ \$$ \$$   \$$
*/

int main()
{
    Graph G;
    ifstream infile;

    G.LoadMatrix(infile);
    G.PrintMatrix();
    G.PrintMap();
    G.isSymmetric();
    G.SortWeights();
    G.ResetVisitStatus();
    G.MinimumSpanningTree_Prims();
    G.MakeGraphVizFile();
}
