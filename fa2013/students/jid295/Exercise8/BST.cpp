#include <stdint.h>
#include <stdio.h>

struct Node {
	int32_t value; // this is our payload, really should be something useful, like "Customer"
	Node* left; // left and right are pointers to our two children. 
	Node* right; // either or both of left/right can be null (i.e., equal to zero)

	Node* parent; // the parent pointer points to our parent. This pointer turns out
	              // to be useful when we want to start traversing a tree (without recursion)
	              // for now, I'll ignore this component
};

Node* insert(Node* root, int32_t v) {
	Node* child = new Node;
	child->value = v;
	child->left = 0;
	child->right = 0;
	child->parent = 0;

	if (root == 0) { 
		return child;
	}

	Node* parent = root;
	while (true) {
		if (v < parent->value) {
			if (parent->left == 0) { break; }
			parent = parent->left;
		} else {
			if (parent->right == 0) { break; }
			parent = parent->right;
		}
	}

	if (v < parent->value) { 
		parent->left = child;
	} else {
		parent->right = child;
	}
	child->parent = parent;

	return root;
}

Node* smallest(Node* root) {
	if (! root) { return 0; } // WTF? smallest in an empty tree????

	while (root->left != 0) {
		root = root->left;
	}

	return root;
}

Node* find(Node* root, int32_t v) {
	Node* p = root;

	while (p != 0) {
		if (p->value == v) { return p; }
		else if (v < p->value) {
			p = p->left;
		} else {
			p = p->right;
		}
	}
	return 0;
}

Node* successor(Node* p) {
	if (p == 0) { // WTF?
		return 0;
	}

	if (p->right != 0) {
		Node* s = p->right;
		while (s->left != 0) {
			s = s->left;
		}
		return s;
	} else {
		Node* s = p->parent;
		while (s != 0 && s->left != p) {
			p = s;
			s = p->parent;
		}
		return s;
	}
}


Node* remove(Node* root, int32_t v) {
	Node* p = find(root, v);
	if (p == 0) { // WTF???
		return root;
	}

	if (p->right == 0) { // case A, easy case, no right child
		Node* parent = p->parent;
		if (parent == 0) {
			root = p->left;
		} 	else {
			if (p == parent->right) {
				parent->right = p->left;
			} else {
				parent->left = p->left;
			}
		}
		if (p->left) {
			p->left->parent = parent;
		}
		delete p;

	} else { // case B, hard case, possibly two children 
		Node* s = successor(p);
		p->value = s->value;
		Node* parent = s->parent;
		if (s == parent->right) {
			parent->right = s->right;
		} else {
			parent->left = s->right;
		}
		if (s->right) {
			s->right->parent = parent;
		}
		delete s;
	}
	
	return root;
}

Node* nextNode(Node* root) {
    Node* next = NULL;
    /* If there is no tree, there is no next node. */
    if (!root) {
        return NULL;
    }
    if (root->right) {
        /* If node has right child, traverse that subtree. */
        next = root->right;
        while (next->left) {
            next = next->left;
        }
        return next;
    }
    next = root->parent;
    /* If node has parent on right, return that parent.
     * If node has parent on left, keep moving up until the parent is
     * greater. Otherwise, this is the last node.
     */
    while (next && next->parent->value < root->value) {
        next = next->parent;
    }
    return next;
}

Node* prevNode(Node* root) {
    Node* prev = NULL;
    /* If there is no tree, there is no next node. */
    if (!root) {
        return NULL;
    }
    if (root->left) {
        /* If node has left child, traverse that subtree. */
        prev = root->left;
        while (prev->right) {
            prev = prev->right;
        }
        return prev;
    }
    prev = root->parent;
    /* If node has parent on left, return that parent.
     * If node has parent on right, keep moving up until the parent is
     * less. Otherwise, this is the last node.
     */
    while (prev && root->value < prev->parent->value) {
        prev = prev->parent;
    }
    return prev;
}

unsigned int treeNodes(Node* root) {
    if (!root) { return 0; }
    int nodes = 1;
    root = smallest(root);
    while ((root = successor(root))) {
        ++nodes;
    }
    return nodes;
    /* Marginally better performance in a balenced tree.
    if (root == nullptr) { return 0; }
    int nodes = 1;
    Node* curr = root;
    while ((curr = nextNode(curr)) != nullptr) {
        ++nodes;
    }
    curr = root;
    while ((curr = prevNode(curr)) != nullptr) {
        ++nodes;
    }
    return nodes;
    */
}

unsigned int recTreeNodes(Node* root) {
    if (!root) {
        return 0;
    }
    return 1 + recTreeNodes(root->left) + recTreeNodes(root->right);
    /* Fun version
    return root == nullptr ?
        0 : 1 + recTreeNodes(root->left) + recTreeNodes(root->right);
    */
}

unsigned int recTreeHeight(Node* root) {
    if (!root) {
        return 0;
    }
    unsigned int heightLeft = recTreeHeight(root->left);
    unsigned int heightRight = recTreeHeight(root->right);
    return 1 + ((heightLeft > heightRight) ? heightLeft : heightRight);
    /* Fun version
    return root == nullptr ?
        0 :
        1 + ((recTreeHeight(root->left) > recTreeHeight(root->right)) ?
            recTreeHeight(root->left) : recTreeHeight(root->right))
    */
}

unsigned int treeHeight(Node* root) {
    Node* nodeCurr = root;
    Node* nodeLast = root;
    unsigned int depthCurr = 1;
    unsigned int depthMax = 1;
    /* If there is no tree, there is no height. */
    if (!root) { return 0; }
    /* Move to the smallest element in the tree. Take note of the depth. */
    while (nodeCurr->left) {
       nodeCurr = nodeCurr->left;
       ++depthCurr;
    }
    printf("Starting at nodeCurr=%d\n", nodeCurr->value);
    /* Find the largest node in the tree. */
    while (nodeLast->right) {
        nodeLast = nodeLast->right;
    }
    printf("Ending at nodeLast=%d\n", nodeLast->value);
    root = nodeCurr;
    depthMax = depthCurr;
    /* Traverse to the greater end of the tree, moving left to right. Keep
     * track of the greatest depth we acheive.
     */
    while (root != nodeLast) {
        /* If node has right child, traverse that subtree for the smallest
         * node in the subtree.
         */
        if (root->right) {
            nodeCurr = nodeCurr->right;
            ++depthCurr;
            while (nodeCurr->left) {
                nodeCurr = nodeCurr->left;
                ++depthCurr;
            }
            /* If we achieved a greater depth (always on this branch), then
             * update the max depth.
             */
            if (depthMax < depthCurr) {
                depthMax = depthCurr;
            }
            root = nodeCurr;
            continue;
        }
        /* If node has parent on right, return that parent.
         * If node has parent on left, keep moving up until the parent is
         * greater. Otherwise, this is the last node.
         */
        nodeCurr = nodeCurr->parent;
        --depthCurr;
        while (nodeCurr && (nodeCurr->value < root->value)) {
            nodeCurr = nodeCurr->parent;
            --depthCurr;
        }
        root = nodeCurr;
    }
    return depthMax;
}

void printTree(Node* root) {
	char* prefix = "";

	Node* p = smallest(root);

	while (p) {
		printf("%s%d", prefix, p->value);
		prefix = ", ";
		p = successor(p);
	}
	printf("\n");
}

void printInfo(Node* root){
	printf("number of nodes in this tree (iterative):\t%d\n", treeNodes(root) );
	printf("number of nodes in this tree (recursive):\t%d\n", recTreeNodes(root) );
	printf("height of this tree (iterative)\t\t%d\n", treeHeight(root) );
	printf("height of this tree (recursive)\t\t%d\n\n", recTreeHeight(root) );
}
