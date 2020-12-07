#ifndef PROJECT3B_AVLTREE_H
#define PROJECT3B_AVLTREE_H
#include "Headline.h"
#include <iostream>
#include <vector>
#include <queue>
using std::string;
using std::cout;
using std::endl;
using std::vector;

struct Node {
    Node* left;                     //Node pointing to older day
    Node* right;                    //Node pointing to more recent day
    vector<Headline> vals;          //Vector of headlines in a particular day
    int height;                     //Height used to efficiently balance tree
    int total;                      //Total score of a given day
    string date;                    //Date held by node
    Node(Headline* val){
        vals.push_back(*val);
        date = val->getDate();
        total = val->getScore();
        height = 1;
        right = nullptr;
        left = nullptr;
    };
};


struct AVLTree {
    Node* root;                    //Root of tree

    AVLTree(){root = nullptr;}                  //Default constructor

    Node* rotateLeft(Node* node);               //Performs left rotation on a given node

    Node* rotateRight(Node* node);              //Performs right rotation on a given node

    Node* rotateLeftRight(Node* node);          //Performs left right rotation on a given node

    Node* rotateRightLeft(Node* node);          //Performs right left rotation on a given node

    Node* balanceTree(Node* node);

    Node* insert(Node* node, Headline* headline);      //Inserts new node, or adds to existing node, then balances tree

    int getHeight(Node* node);                 //Returns height of a node

    Node* getDay(string date);                 //Returns a node pointer for a given date
};


int AVLTree::getHeight(Node* node){ //Returns height of a node, or 0 if node is a nullptr
    if(node == nullptr)
        return 0;
    return node->height;
}

Node* AVLTree::rotateLeft(Node* node){ /*Reuses some code my (Ethan Graff's) Stepik 5.1.1
 *                                       https://stepik.org/submissions/1480367?unit=379728
                                         but slightly updated to keep track of new height of nodes after rotation*/
    Node* grandchild = node->right->left;
    Node* newParent = node->right;
    newParent->left = node;
    node->right = grandchild;
    if(getHeight(node->right) < getHeight(node->left)){
        node->height = getHeight(node->left) + 1;
    } else {
        node->height = getHeight(node->right) + 1;
    }

    if(getHeight(newParent->right) < getHeight(newParent->left)){
        newParent->height = getHeight(newParent->left) + 1;
    } else {
        newParent->height = getHeight(newParent->right) + 1;
    }

    return newParent;
}


Node* AVLTree::rotateRight(Node* node){ /*Reuses some code from my (Ethan Graff's) Stepik 5.1.1
 *                                       https://stepik.org/submissions/1480367?unit=379728
                                         but slightly updated to keep track of new height of nodes after rotation*/
    Node* grandchild = node->left->right;
    Node* newParent = node->left;
    newParent->right = node;
    node->left = grandchild;
    if(getHeight(node->right) < getHeight(node->left)){
        node->height = getHeight(node->left) + 1;
    } else {
        node->height = getHeight(node->right) + 1;
    }

    if(getHeight(newParent->right) < getHeight(newParent->left)){
        newParent->height = getHeight(newParent->left) + 1;
    } else {
        newParent->height = getHeight(newParent->right) + 1;
    }

    return newParent;
}


Node* AVLTree::rotateRightLeft(Node* node){ /*Reused my (Ethan Graff's) code from Stepik 5.1.3
                                            https://stepik.org/submissions/1480369?unit=379728*/
    Node* newNode = rotateRight(node->right);
    node->right = newNode;
    return rotateLeft(node);
}


Node* AVLTree::rotateLeftRight(Node* node){ /*Reused my (Ethan Graff's) code from Stepik 5.1.1
                                             https://stepik.org/submissions/1480367?unit=379728*/
    Node* newNode = rotateLeft(node->left);
    node->left = newNode;
    Node* newParent = rotateRight(node);
    return newParent;
}



Node* AVLTree::balanceTree(Node* node){ //Determines which rotation to perform on a given node
    if((getHeight(node->left) - getHeight(node->right) > 1)&&(getHeight(node->left->left) - getHeight(node->left->right) >= 0)){
        return rotateRight(node);
    }

    if((getHeight(node->left) - getHeight(node->right) < -1)&&(getHeight(node->right->left) - getHeight(node->right->right) <= 0)){
        return rotateLeft(node);
    }

    if((getHeight(node->left) - getHeight(node->right) > 1)&&(getHeight(node->left->left) - getHeight(node->left->right) < 0)){
        return rotateLeftRight(node);
    }

    if((getHeight(node->left) - getHeight(node->right) < -1)&&(getHeight(node->right->left) - getHeight(node->right->right) > 0)){
        return rotateRightLeft(node);
    }

    return node;
}


Node* AVLTree::insert(Node* node, Headline* headline){ //Recursively inserts Node, then calls balanceTree()
    if(node == nullptr){
        Node* newNode = new Node(headline);
        return newNode;
    }

    if(headline->getDate() < node->date){
        node->left = insert(node->left, headline);
    } else if(headline->getDate() > node->date){
        node->right = insert(node->right, headline);
    } else {  //If node already exists, then headline is added to existing node
        node->vals.push_back(*headline);
        node->total = node->total + headline->getScore();
        return node;
    }

    if(getHeight(node->right) < getHeight(node->left)){  //Updates height of each node
        node->height = getHeight(node->left) + 1;
    } else {
        node->height = getHeight(node->right) + 1;
    }

    return balanceTree(node);//Balances tree
}


Node* AVLTree::getDay(string date){//Searches through AVL Tree until it reaches given date, then returns a pointer to that Node
    Node* node = root;
    while(node != nullptr) {
        if (date < node->date) {
            node = node->left;
        } else if (date > node->date) {
            node = node->right;
        } else {
            return node;
        }
    }
    cout << "Error: Date not in data set" << endl;
    return nullptr;
}

#endif //PROJECT3B_AVLTREE_H
