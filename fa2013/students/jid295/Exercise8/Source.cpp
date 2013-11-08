#include "BST.h"
#include <assert.h>

int main(void){
    Node* root = 0;

    root = insert(root, 8);
    root = insert(root, 3);
    root = insert(root, 10);
    root = insert(root, 1);
    root = insert(root, 6);
    root = insert(root, 4);
    root = insert(root, 7);
    printTree(root);
    printInfo(root);
    
    return 0;
}
