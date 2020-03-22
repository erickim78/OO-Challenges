/******************************************************************************

1) Constructs a binary search tree, in order, from the integers given in the
   input argument vector
2) Returns the distance (number of edges) between nodes with two given values.
   If either node was not found in the binary tree,
   then the function returns -1

For example, given the following vector of input values:

   8 7 13 6 2 5 1 9 11 3 4 10

and the following two node arguments:

   4 2

the function returns 3.

Given the same input vector, but the following input arguments:

   4 0

the function returns -1

******************************************************************************/



#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//Node Struct Definition
struct node {
    node* left;
    node* right;
    int val;
    
    node( int x ) {
        left = nullptr;
        right = nullptr;
        val = x;
    }
};

//Helper Function, Inserts new Node into BST
void insert( node* root, int value ) {
    node* temp = root;
    while(temp) {
        if( value > temp->val ) {
            if( temp->right )
                temp = temp->right;
            else {
                temp->right = new node(value);
                return;
            }
        }
        else {
            if( temp->left )
                temp = temp->left;
            else {
                temp->left = new node(value);
                return;
            }
        }
    }
    
}

//Helper Function to Print BST
void printBST( node* root ) {
    if(!root)
        return;
    
    printBST( root->left );
    std::cout << root->val << " " << std::endl;
    printBST( root->right );
}

//Helper Function to find if a value x exists in a BST
bool find( node* root, int x ) {
    node* temp = root;
    
    while(temp) {
        if( x == temp->val )
            return true;
        else if( x < temp->val )
            temp = temp->left;
        else if( x > temp->val )
            temp = temp->right;
    }
    return false;
      
}

//Helper to find common root, assuming both exists
node* commonRoot( node* root, int a, int b ) {
    node* temp = root;
    while( temp ) {
        if( a > temp->val && b > temp->val )
            temp = temp->right;
        else if( a < temp->val && b < temp-> val )
            temp = temp->left;
        else
            return temp;
    }
    return nullptr;
}

//Helper function, get distance between root and a node, assuming x exists
int distanceFromRoot( node* root, int x ){
    int distance = 0;
    node* temp = root;
    while( temp ) {
      if( x == temp->val )
          return distance;
      else if( x > temp->val ) {
          temp = temp->right;
          distance++;
      }
      else {
          temp = temp->left;
          distance++;
      }
    }
    
    return -1;
}

int BSTdistance(const std::vector<int>& values, int nodeA, int nodeB)
{
    //Create the BST
    node* head = new node( values[0] );
    for( int i = 1; i < values.size(); i++ )
        insert( head, values[i] );
    
    //Return -1 if cant find either nodeA or nodeB
    if( !find(head, nodeA) || !find(head, nodeB) )
        return -1;
    
    //Common Root
    node* common = commonRoot( head, nodeA, nodeB );
    
    //Return sum of distances from common root
    return distanceFromRoot( common, nodeA) + distanceFromRoot( common, nodeB );
}

int main ()
{
    BSTdistance({ 8, 7, 13, 6, 2, 5, 1, 9, 11, 3, 4, 10}, 8, 10);
    //std::cout << BSTdistance({2,4}, 2, 4) << std::endl;
}
