#include <iostream>
#include <algorithm>
#include <stack>
#define NULL __null
// AVL
using namespace std;

// nodeAVL 구조체 정의
struct nodeAVL{
    int data;          // key 값
    int BF;            // 균형인수 (-1 <= BF <= 1)
    int height;        // 노드의 높이
    nodeAVL *left;     // 왼쪽 포인터 
    nodeAVL *right;    // 오른쪽 포인터
};

// 트리 구조체 정의
struct Tree{
    nodeAVL* root;
};

stack<nodeAVL> s;
string rotationType;

void initTree(Tree *T){
    T -> root = nullptr;
}

// getNode()
nodeAVL* getNode(){
    nodeAVL *newNode = (nodeAVL*)malloc(sizeof(nodeAVL));
}

// height()
int height(nodeAVL* T){
    if(T == NULL)
        return 0;
    else
        return max(height(T -> left) + 1, height(T -> right) + 1);
}


// insertBST(T, newKey)
void insertBST(Tree* T, int newKey){
    nodeAVL* parent = NULL;
    nodeAVL* curr = T->root; 

    // find position to insert newKey while storig parent node on stack
    while(curr != NULL){
        if(curr->data == newKey)
            return;
        
        parent = curr;
        s.push(*parent);

        if(newKey < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    // create newNode
    nodeAVL *newNode = getNode();
    newNode->data = newKey;
    newNode->left = NULL;
    newNode->right = NULL;

    // insert newNode for child of parentNode
    if(T->root == NULL)
        T->root = newNode;
    else if(newKey < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
}

// checkBalance(T,newKey, rotationType, p, q)
void checkBalance(Tree* T, string &rotationType, nodeAVL* firstInbalanced, nodeAVL* parentInbalanced){

    // update height and BF while popping parent from stack
    while(!s.empty()){
        nodeAVL *checkNode = &s.top();
        s.pop();

        nodeAVL *leftchild = checkNode->left;
        nodeAVL *rightchild = checkNode->right; 

        checkNode->height = 1 + max(leftchild->height, rightchild->height);
        checkNode->BF = leftchild->height - rightchild->height;

        // BF 불균형 발생하면
        if(1 < checkNode->BF or checkNode->BF < -1){
            if(firstInbalanced == NULL){        // 최초 노드일 경우에만  
                firstInbalanced = checkNode;       // 불균형 발생 노드 
                parentInbalanced = &s.top();    // 불균형 발생 노드의 부모노드
            }
        }
    }

    // rotationType 지정
    if(firstInbalanced == NULL){
        rotationType = "NO";
        return;
    }

    nodeAVL* leftnode = firstInbalanced->left;
    nodeAVL* rightnode = firstInbalanced->right;

    if(firstInbalanced->BF == 2){       // LL or LR
        switch (leftnode->BF)
        {
        case 1:
            rotationType = "LL";
            break;

        case -1:
            rotationType = "LR";
            break;

        default:
            break;
        }

    }else if(firstInbalanced->BF == -2){   // RR or RL
        switch (rightnode->BF)
        {
        case -1:
            rotationType = "RR";
            break;
        
        case 1:
            rotationType = "RL";
            break;
        
        default:
            break;
        }
    }
}

// rotateTree(T, rotationType, p, q)
void rotateTree(Tree* T, string rotationType, nodeAVL* firstInbalanced, nodeAVL* parentInbalanced){
    nodeAVL* newparent = NULL;
    string flag;
    if(parentInbalanced->left = firstInbalanced)
        flag = "left";
    else
        flag = "right";

    if(rotationType == "LL"){
        // 최초 불균형노드의 부모노드에 최초 불균형 노드의 삽입 자식트리의 루트노드 연결
        if(flag == "left"){
            parentInbalanced->left = firstInbalanced->left;
            newparent = parentInbalanced->left;
        }else{
            parentInbalanced->right = firstInbalanced->right;
            newparent = parentInbalanced->right;
        }

        // 불균형노드의 왼쪽에 연결한 새로운 부모 노드의 오른쪽 트리를연결
        if(newparent->right != NULL)
            firstInbalanced->left = newparent->right;

        // 새로운 부모 노드의 오른쪽에 불균형 노드트리 연결
        newparent->right = firstInbalanced;
        
    }else if(rotationType == "RR"){
        // 최초 불균형노드의 부모노드에 최초 불균형 노드의 삽입 자식트리의 루트노드 연결
        if(flag == "left"){
            parentInbalanced->left = firstInbalanced->right;
            newparent = parentInbalanced->left;
        }else{
            parentInbalanced->right = firstInbalanced->right;
            newparent = parentInbalanced->right;
        }

        // 불균형노드의 오른쪽에 연결한 새로운 부모 노드의 왼쪽 트리를연결
        if(newparent->left != NULL)
            firstInbalanced->right = newparent->left;

        // 새로운 부모 노드의 왼쪽에 불균형 노드트리 연결
        newparent->left = firstInbalanced;

    }else if(rotationType == "LR"){
        nodeAVL* A = firstInbalanced->left;     //L
        nodeAVL* B = A->right;                  //LR

        // 최초 불균형노드의 부모노드에 LR트리 연결
        if(flag == "left"){
            parentInbalanced->left = B;
            newparent = parentInbalanced->left;
        }else{
            parentInbalanced->right = B;
            newparent = parentInbalanced->right;
        }

        // L트리의 오른쪽에 새로운 부모노드의 왼쪽 트리 연결
        if(newparent->left != NULL)
            A->right = newparent->left;
        
        // 새로운 부모노드의 왼쪽에 기존L트리 연결
        newparent->left = A;

        // 최초 불균형트리의 왼쪽에 새로운 부모노드의 오른쪽 트리 연결
        if(newparent->right != NULL)
            firstInbalanced->left = newparent->right;
        
        // 새로운 부모노드의 오른쪽에 최초 불균형트리 루트노드 연결
        newparent->right = firstInbalanced;
        
    }else if(rotationType == "RL"){
        nodeAVL* A = firstInbalanced->right;        // R
        nodeAVL* B = A->left;                       // RL

        // 최초 불균형노드의 부모노드에 RL트리 연결
        if(flag == "left"){
            parentInbalanced->left = B;
            newparent = parentInbalanced->left;
        }else{
            parentInbalanced->right = B;
            newparent = parentInbalanced->right;
        }

        // R트리의 왼쪽에 새로운 부모노드의 오른쪽트리 연결
        if(newparent->right != NULL)
            A->left = newparent->right;
        
        // 새로운 부모노드의 오른쪽에 기존 R트리 연결
        newparent->right = A;

        // 최초 불균형노드의 오른쪽에 새로운 부모노드의 왼쪽트리 연결
        if(newparent->left != NULL)
            firstInbalanced->right = newparent->left;
        
        // 새로운 부모노드의 왼쪽에 최초 불균형노드 연결
        newparent->left = firstInbalanced;
        
    }
}

// insertAVL
void insertAVL(Tree* T, int newKey){
    nodeAVL* firstInbalanced = NULL;        // newInsert로 인하여 불균형으로 판명된 최초의 노드
    nodeAVL* parentInbalanced = NULL;       // firstInbalanced 부모노드

    insertBST(T, newKey);

    /* checkBalance(T, newKey, rotationType, curr, parent) */
    // update height and BF while popping parent from stack
    checkBalance(T, rotationType, firstInbalanced, parentInbalanced);
    
    /* rotataeTree(T, rotationType, p, q) */
    // if Tree is unbalanced, rebalance Tree
    if(rotationType == "NO")
        return;
    else
        rotateTree(T, rotationType, firstInbalanced, parentInbalanced);

}


// inorderBST(T)
void inorderBST(nodeAVL* T){
    if(T != NULL){
        inorderBST(T -> left);
        cout << T -> data << " ";
        inorderBST(T -> right);
    }
}

int main(void){
    int arr[] = {40, 11, 77, 33, 20, 90, 99, 70, 88, 80, 66, 10, 22, 30, 44, 55, 50, 60, 25, 49};
    int arrsize = sizeof arr / sizeof arr[0];

    Tree T;
    initTree(&T);

    for(int i = 0; i < arrsize; i++){
        insertAVL(&T, arr[i]);
        cout << rotationType << " ";
        inorderBST(T.root);
        cout << "\n";
    }

    return 0;
}