#include <stdint.h>

struct Node {
	int32_t value; // this is our payload, really should be something useful, like "Customer"
	Node* left; // left and right are pointers to our two children. 
	Node* right; // either or both of left/right can be null (i.e., equal to zero)

	Node* parent; // the parent pointer points to our parent. This pointer turns out
	              // to be useful when we want to start traversing a tree (without recursion)
	              // for now, I'll ignore this component
};

Node* insert(Node* root, int32_t v);

Node* smallest(Node* root);

Node* find(Node* root, int32_t v);

Node* successor(Node* p);

Node* remove(Node* root, int32_t v);

int treeNodes(Node* root);	//EDIT THIS FUNCTION!!!

int recTreeNodes(Node* root); //EDIT THIS FUNCTION!!!

int treeHeight(Node* root);  //EDIT THIS FUNCTION!!!

int recTreeHeight(Node* root); //EDIT THIS FUNCTION!!!

void printTree(Node* root);

void printInfo(Node* root);