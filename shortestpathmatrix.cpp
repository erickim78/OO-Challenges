/******************************************************************************
This function takes a matrix object (defined below), and a start and an end cells within that matrix.
The matrix is filled with values of either 0 or 1.  The function returns
the shortest path between the start and the end cells given, such that no
matrix cells with value of 1 are part of the path.  The function returns
this path in the form of sequence of Matrix cells, including start and end.
A valid path consists of sequence of matrix cells adjacent to each other.
Cell A is adjacent to cell B if it is immediately to the right, left,
above, or below.  For example, in the following matrix:

 0  1  2
 3  4  5
 6  7  8

 Cell "4" is adjacent to cells 3, 1, 5, and 7.

There are no wraparounds, e.g. cells 7 and 1 are not adjacent.

Cells are identified via (row, col), starting from 0, i.e. in the following matrix
0 0 0 0 0
0 0 0 0 1
0 0 0 0 0

Cell with value 1 is at (1, 4)

Example 1:

Matrix:
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0
Start:(0,0) End:(2, 3)

Possible solution (one of many):  (0,0),(0,1),(0,2),(0,3),(1,3),(2,3)

Example 2:

Matrix:
0 1 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0 0
0 1 0 0 0 0 0 0 0
Start:(0,0) End:(2, 3)

Solution:  (0,0),(1,0),(1,1),(1,2),(2,2),(2,3)

If no path could be found, the function returns an empty list.
******************************************************************************/

#include <vector>
#include <memory>
#include <queue>

#include <iostream>
struct Location
{
    size_t row_;
    size_t col_;
};

using Locations = std::vector<Location>;

struct Matrix
{
public:
    explicit Matrix(size_t rows, size_t cols, const Locations& walls)
        : rows_{ rows }, cols_{ cols }, data_(new int[rows * cols])
    { InitializeMatrix(walls); }

    void InitializeMatrix(const Locations& walls)
    {
        // Zero data
        for (size_t row = 0; row < rows_; row++)
        {
            for (size_t col = 0; col < cols_; col++)
            {
                data_[cols_ * row + col] = 0;
            }
        }

        // Add walls
        for (const auto& wall : walls)
        {
            data_[cols_ * wall.row_ + wall.col_] = 1;
        }
    }

    const int& operator()(const Location& location) const
    {
        if (location.row_ >= rows_ || location.col_ >= cols_)
            throw std::out_of_range("OOB");

        return data_[cols_ * location.row_ + location.col_];
    }

    size_t NumRows() const { return rows_; }
    size_t NumCols() const { return cols_; }

private:
    size_t rows_;
    size_t cols_;
    std::unique_ptr<int[]> data_;
};

//HELPER FUNCTIONS

//prints a single location
void printLocation( const Location& l )
{
    std::cout << "(" << l.row_ << ", " << l.col_ << ")" << std::endl;
}

//prints a vector of locations
void printLocations( const Locations& l ) 
{
    for(int i = 0; i < l.size(); i++)
        std::cout << "(" << l[i].row_ << ", " << l[i].col_ << ")" << std::endl;
}

//Checks if a location is a valid in a matrix m
bool validLoc( const Matrix& m, const Location& l) 
{
    if( l.row_ >= 0 && l.row_ < m.NumRows() ) 
    {
        if( l.col_ >= 0 && l.col_ < m.NumCols() ) 
        {
            return true;
            std::cout << "Valid" << std::endl;
        }
    }
    return false;
}

//Node for Q, stores its own location 'q_' as well as the path taken to get there 'path'
struct Node
{
    Location q_;
    Locations path;
};

//Prints a Queue
void printQ( const std::queue<Node> q)
{
    std::cout << "Printing QUEUE: " << std::endl;
    std::queue<Node> temp = q;
    for( int i = 0; i <  q.size(); i++) 
    {
        Node n = temp.front();
        temp.pop();
        temp.push( n );
        printLocation(n.q_);
    }
    std::cout << "QUEUE END" << std::endl;
}

Locations FindPath(const Matrix& m, const Location& start, const Location& end)
{
    // TODO

    //If start and end are the same
    if( start.row_ == end.row_ && start.col_ == end.col_ )
        return { start,end };

    //Create a Queue for BFS and push the starting location
    Node source = {start, {start}};
    std::queue<Node> BFSQueue;
    BFSQueue.push(source);

    //Create an array to keep track of visited cells, 1 = visited or wall
    int visited[m.NumRows()][m.NumCols()];
    for(int i = 0; i < m.NumRows(); i++) 
    {
        for(int j = 0; j < m.NumCols(); j++)
        {
            if( m({i,j}) == 1 )
                visited[i][j] = 1;
            else
                visited[i][j] = 0;            
        }
    }

    //Set start location to visited
    visited[ start.row_][ start.col_] = 1;

    //BFS Algorithm
    while( !BFSQueue.empty() )
    {
        //Get a valid node from front of Q
        Node temp = BFSQueue.front();
        Location current = temp.q_;

        //If that node is the destination, return the path taken to get there
        if( current.row_ == end.row_ && current.col_ == end.col_)
            return temp.path;

        //Else pop that valid cell and continue
        BFSQueue.pop();
        

        //Now check which neighbors are valid, and add them to the queue if they are; **REPEAT 4 TIMES, ONCE FOR EACH NEIGHBOR CELL**
        //1st Neighbor
        int neighborR = current.row_ - 1;
        int neighborC = current.col_;
        Location neighbor1{ neighborR, neighborC };

        //If neighbor is valid, hasnt been visited, and is not a wall, push to queue
        if( validLoc(m,neighbor1) && (m(neighbor1) == 0) && (visited[neighborR][neighborC] == 0) ) 
        {
            Locations newpath = temp.path;  
            newpath.push_back(neighbor1);   //Add the neighbor to the path

            Node validcell = { neighbor1, newpath };
            BFSQueue.push( validcell );     //Push to Queue
            visited[neighborR][neighborC] = 1;
        }

        //2nd Neighbor
        neighborR = current.row_;
        neighborC = current.col_ - 1;
        Location neighbor2{ neighborR, neighborC };

        if( validLoc(m,neighbor2) && (m(neighbor2) == 0) && (visited[neighborR][neighborC] == 0) ) 
        {
            Locations newpath = temp.path;
            newpath.push_back(neighbor2);
            Node validcell = { neighbor2, newpath };
            BFSQueue.push( validcell );
            visited[neighborR][neighborC] = 1;
        }

        //3rd Neighbor
        neighborC = current.col_ + 1;
        Location neighbor3{ neighborR, neighborC };

        if( validLoc(m,neighbor3) && (m(neighbor3) == 0) && (visited[neighborR][neighborC] == 0) ) 
        {
            Locations newpath = temp.path;
            newpath.push_back(neighbor3);
            Node validcell = { neighbor3, newpath };
            BFSQueue.push( validcell );
            visited[neighborR][neighborC] = 1;
        }

        //4th Neighbor
        neighborR = current.row_ + 1;
        neighborC = current.col_;
        Location neighbor4{ neighborR, neighborC };

        if( validLoc(m,neighbor4) && (m(neighbor4) == 0) && (visited[neighborR][neighborC] == 0) ) 
        {
            Locations newpath = temp.path;
            newpath.push_back(neighbor4);
            Node validcell = { neighbor4, newpath };
            BFSQueue.push( validcell );
            visited[neighborR][neighborC] = 1;
        }
    }
    return { };
}


int main ()
{
    Locations walls;
    walls.push_back({1, 1});

    Matrix matrix(3,3, walls);
    FindPath(matrix, {0,0}, {2,2});
}