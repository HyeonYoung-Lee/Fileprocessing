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

stack<nodeAVL*> s;
string rotationType;
nodeAVL* firstInbalanced = NULL;        // newInsert로 인하여 불균형으로 판명된 최초의 노드
nodeAVL* parentInbalanced = NULL;       // firstInbalanced 부모노드

void initTree(Tree *T){
    T -> root = nullptr;
}

// getNode()
nodeAVL* getNode(){
    nodeAVL *newNode = (nodeAVL*)malloc(sizeof(nodeAVL));
}

// height()
int height(nodeAVL* T){
    if(T != NULL)
        T->height = 1 + max(height(T -> left), height(T -> right));
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
        s.push(parent);

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
    newNode->height = 1;
    newNode->BF = 0;

    // insert newNode for child of parentNode
    if(T->root == NULL)
        T->root = newNode;
    else if(newKey < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
}

// checkBalance(T,newKey, rotationType, p, q)
void checkBalance(Tree* T){
    firstInbalanced = NULL;
    // update height and BF while popping parent from stack
    while(!s.empty()){
        nodeAVL *checkNode = s.top();
        s.pop();

        nodeAVL *leftchild = checkNode->left;
        nodeAVL *rightchild = checkNode->right; 
        

        // 노드가 둘 다 있을 때
        if(leftchild != NULL && rightchild != NULL){
            checkNode->height = 1 + max(leftchild->height, rightchild->height);
            checkNode->BF = leftchild->height - rightchild->height;
        // 왼쪽 노드만 있을 때    
        }else if(leftchild != NULL && rightchild == NULL){
            checkNode->height = 1 + leftchild->height;
            checkNode->BF = leftchild->height;
        // 오른쪽 노드만 있을 때    
        }else if(leftchild == NULL && rightchild != NULL){
            checkNode->height = 1 + rightchild->height;
            checkNode->BF = 0 - rightchild->height;
        }
            

        // BF 불균형 발생하면
        if(1 < checkNode->BF or checkNode->BF < -1){
            if(firstInbalanced == NULL){            // 최초 노드일 경우에만  
                firstInbalanced = checkNode;       // 불균형 발생 노드 
                if(!s.empty())                      // 불균형 발생 노드의 부모노드
                    parentInbalanced = s.top();
                else
                    parentInbalanced = NULL;    
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
void rotateTree(Tree* T, string rotationType){
    nodeAVL* newparent = NULL;
    string flag;
    if(parentInbalanced == NULL)            // 최초 불균형 노드가 루트노드일때
        flag = "root";
    else if(parentInbalanced->left == firstInbalanced)
        flag = "left";
    else
        flag = "right";
    
/* LL */
    if(rotationType == "LL"){
        // 최초 불균형노드의 부모노드에 최초 불균형 노드의 삽입 자식트리의 루트노드 연결
        if(flag == "left"){
            parentInbalanced->left = firstInbalanced->left;
            newparent = parentInbalanced->left;
        }else if(flag == "right"){
            parentInbalanced->right = firstInbalanced->left;
            newparent = parentInbalanced->right;
        }else{                  // 최초 불균형노드가 루트노드일때
            T->root = firstInbalanced->left;
            newparent = T->root;
        }
         
        firstInbalanced->left = NULL;                   // 최초 불균형노드에서 왼쪽 트리 삭제       
        if(newparent->right != NULL)                    // 불균형노드의 왼쪽에 연결한 새로운 부모 노드의 오른쪽 트리를연결
            firstInbalanced->left = newparent->right;        
        newparent->right = firstInbalanced;             // 새로운 부모 노드의 오른쪽에 불균형 노드트리 연결

/* RR */        
    }else if(rotationType == "RR"){
        // 최초 불균형노드의 부모노드에 최초 불균형 노드의 삽입 자식트리의 루트노드 연결
        if(flag == "left"){
            parentInbalanced->left = firstInbalanced->right;
            newparent = parentInbalanced->left;
        }else if(flag == "right"){
            parentInbalanced->right = firstInbalanced->right;
            newparent = parentInbalanced->right;
        }else{                  // 최초 불균형노드가 루트노드일때
            T->root = firstInbalanced->right;
            newparent = T->root;
        }
        
        firstInbalanced->right = NULL;                  // 최초 불균형노드의 오른쪽 트리 삭제        
        if(newparent->left != NULL)                     // 불균형노드의 오른쪽에 연결한 새로운 부모 노드의 왼쪽 트리를연결
            firstInbalanced->right = newparent->left;
        newparent->left = firstInbalanced;              // 새로운 부모 노드의 왼쪽에 불균형 노드트리 연결

/* LR */
    }else if(rotationType == "LR"){
        nodeAVL* A = firstInbalanced->left;     //L
        nodeAVL* B = A->right;                  //LR

        // 최초 불균형노드의 부모노드에 LR트리 연결
        if(flag == "left"){
            parentInbalanced->left = B;
            newparent = parentInbalanced->left;
        }else if(flag == "right"){
            parentInbalanced->right = B;
            newparent = parentInbalanced->right;
        }else{                      // 최초 불균형노드가 루트노드일때
            T->root = B;
            newparent = T->root;
        }
        
        A->right = NULL;                                // L에서 오른쪽 노드 제거  
        if(newparent->left != NULL)                     // L트리의 오른쪽에 새로운 부모노드의 왼쪽 트리 연결
            A->right = newparent->left;        
        newparent->left = A;                            // 새로운 부모노드의 왼쪽에 기존L트리 연결        
        firstInbalanced->left = NULL;                   // 최초 불균형트리의 왼쪽 제거        
        if(newparent->right != NULL)                    // 최초 불균형트리의 왼쪽에 새로운 부모노드의 오른쪽 트리 연결
            firstInbalanced->left = newparent->right;
        newparent->right = firstInbalanced;             // 새로운 부모노드의 오른쪽에 최초 불균형트리 루트노드 연결

/* RL */        
    }else if(rotationType == "RL"){
        nodeAVL* A = firstInbalanced->right;        // R
        nodeAVL* B = A->left;                       // RL

        // 최초 불균형노드의 부모노드에 RL트리 연결
        if(flag == "left"){
            parentInbalanced->left = B;
            newparent = parentInbalanced->left;
        }else if(flag == "right"){
            parentInbalanced->right = B;
            newparent = parentInbalanced->right;
        }else{                      // 최초 불균형노드가 루트노드일때
            T->root = B;
            newparent = T->root;
        }

        
        A->left = NULL;                                 // R트리에서 왼쪽 노드 제거
        if(newparent->right != NULL)                    // R트리의 왼쪽에 새로운 부모노드의 오른쪽트리 연결
            A->left = newparent->right; 
        newparent->right = A;                           // 새로운 부모노드의 오른쪽에 기존 R트리 연결
        firstInbalanced->right = NULL;                  // 최초 불균형 노드에서 기존 R트리 제거
        if(newparent->left != NULL)                     // 최초 불균형노드의 오른쪽에 새로운 부모노드의 왼쪽트리 연결
            firstInbalanced->right = newparent->left;
        newparent->left = firstInbalanced;              // 새로운 부모노드의 왼쪽에 최초 불균형노드 연결

    }
    
    /* update height and BF */
    if(rotationType == "LL" or rotationType == "RR"){
        newparent->left->height = 1;
        newparent->left->BF = 0;
        newparent->right->height = 1;
        newparent->right->BF = 0;

        if(parentInbalanced != NULL){
            parentInbalanced->height = 1+ max(parentInbalanced->left->height, parentInbalanced->right->height);
            parentInbalanced->BF = parentInbalanced->left->height - parentInbalanced->right->BF;
        }

    }else{
        nodeAVL* newleft = newparent->left;
        nodeAVL* newright = newparent->right;

        if(newleft->left != NULL && newleft->right != NULL){
            newleft->height = 1 + max(newleft->left->height, newleft->right->height);
            newleft->BF = newleft->left->height - newleft->right->height;
        
        }else if(newleft->left != NULL && newleft->right == NULL){
            newleft->height = 1 + newleft->left->height;
            newleft->BF = newleft->left->height;    
        
        }else if(newleft->left == NULL && newleft->right != NULL){
            newleft->height = 1 + newleft->right->height;
            newleft->BF = 0 - newleft->right->height;
        }else{
            newleft->height = 1;
            newleft->BF = 0;
        }

        if(newright->left != NULL && newright->right != NULL){
            newright->height = 1 + max(newright->left->height, newright->right->height);
            newright->BF = newright->left->height - newright->right->height;
        }else if(newright->left != NULL && newright->right == NULL){
            newright->height = 1 + newright->left->height;
            newright->BF = newright->left->height;

        }else if(newright->left == NULL && newright->right != NULL){
            newright->height = 1 + newright->right->height;
            newright->BF = 0 - newright->right->height;

        }else{
            newright->height = 1;
            newright->BF = 0;
        }

        newparent->height = 1 + max(newleft->height, newright->height);
        newparent->BF = newleft->height - newright->height;

        if(parentInbalanced != NULL){
            parentInbalanced->height = 1 + max(parentInbalanced->left->height, parentInbalanced->right->height);
            parentInbalanced->BF = parentInbalanced->left->height - parentInbalanced->right->height;
        }

        
   
    } 
}

// insertAVL
void insertAVL(Tree* T, int newKey){

    insertBST(T, newKey);

    /* checkBalance(T, newKey, rotationType, curr, parent) */
    // update height and BF while popping parent from stack
    checkBalance(T);
    
    /* rotataeTree(T, rotationType, p, q) */
    // if Tree is unbalanced, rebalance Tree
    if(rotationType == "NO")
        return;
    else{
        rotateTree(T, rotationType);
    }
}

// deleteBST(T, deleteKey)
void deleteBST(Tree* T, int deleteKey){
    nodeAVL* parent = NULL;
}

// deleteAVL
void deleteAVL(Tree* T, int deleteKey){
    
    /* deleteBST(T, deleteKey) */
    deleteBST(T, deleteKey);

    /* checkBalance(T) */
    // update height and BF while popping parent from stack
    checkBalance(T);
    
    /* rotataeTree(T, rotationType, p, q) */
    // if Tree is unbalanced, rebalance Tree
    if(rotationType == "NO")
        return;
    else{
        rotateTree(T, rotationType);
    }
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
/*
    for(int i = 0; i < arrsize; i++){
        insertAVL(&T, arr[i]);
        cout << rotationType << " ";
        inorderBST(T.root);
        cout << "\n";
    }
*/
   
    insertAVL(&T, 40);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 11);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 77);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 33);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 20);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 90);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 99);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 70);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 88);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";

    insertAVL(&T, 80);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 66);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 10);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 22);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 30);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 44);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 55);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 50);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 60);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 25);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    
    insertAVL(&T, 49);
    cout << rotationType << " ";
    inorderBST(T.root);
    cout << "\n";
    


    return 0;
}