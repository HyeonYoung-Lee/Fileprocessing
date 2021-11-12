#include <iostream>
#include <algorithm>
#include <stack>
#define NULL __null
// AVL
using namespace std;

// nodeAVL ����ü ����
struct nodeAVL{
    int data;          // key ��
    int BF;            // �����μ� (-1 <= BF <= 1)
    int height;        // ����� ����
    nodeAVL *left;     // ���� ������ 
    nodeAVL *right;    // ������ ������
};

// Ʈ�� ����ü ����
struct Tree{
    nodeAVL* root;
};

stack<nodeAVL*> s;
string rotationType;
string deleteFlag;
nodeAVL* firstInbalanced = NULL;        // newInsert�� ���Ͽ� �ұ������� �Ǹ�� ������ ���
nodeAVL* parentInbalanced = NULL;       // firstInbalanced �θ���

void initTree(Tree *T){
    T -> root = nullptr;
}

// getNode()
nodeAVL* getNode(){
    nodeAVL *newNode = (nodeAVL*)malloc(sizeof(nodeAVL));
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
        

        // ��尡 �� �� ���� ��
        if(leftchild != NULL && rightchild != NULL){
            checkNode->height = 1 + max(leftchild->height, rightchild->height);
            checkNode->BF = leftchild->height - rightchild->height;
        // ���� ��常 ���� ��    
        }else if(leftchild != NULL && rightchild == NULL){
            checkNode->height = 1 + leftchild->height;
            checkNode->BF = leftchild->height;
        // ������ ��常 ���� ��    
        }else if(leftchild == NULL && rightchild != NULL){
            checkNode->height = 1 + rightchild->height;
            checkNode->BF = 0 - rightchild->height;
        }
            

        // BF �ұ��� �߻��ϸ�
        if(1 < checkNode->BF or checkNode->BF < -1){
            if(firstInbalanced == NULL){            // ���� ����� ��쿡��  
                firstInbalanced = checkNode;       // �ұ��� �߻� ��� 
                if(!s.empty())                      // �ұ��� �߻� ����� �θ���
                    parentInbalanced = s.top();
                else
                    parentInbalanced = NULL;    
            }
        }
    }


    // rotationType ����
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
    if(parentInbalanced == NULL)            // ���� �ұ��� ��尡 ��Ʈ����϶�
        flag = "root";
    else if(parentInbalanced->left == firstInbalanced)
        flag = "left";
    else
        flag = "right";
    
/* LL */
    if(rotationType == "LL"){
        // ���� �ұ�������� �θ��忡 ���� �ұ��� ����� ���� �ڽ�Ʈ���� ��Ʈ��� ����
        if(flag == "left"){
            parentInbalanced->left = firstInbalanced->left;
            newparent = parentInbalanced->left;
        }else if(flag == "right"){
            parentInbalanced->right = firstInbalanced->left;
            newparent = parentInbalanced->right;
        }else{                  // ���� �ұ�����尡 ��Ʈ����϶�
            T->root = firstInbalanced->left;
            newparent = T->root;
        }
         
        firstInbalanced->left = NULL;                   // ���� �ұ�����忡�� ���� Ʈ�� ����       
        if(newparent->right != NULL)                    // �ұ�������� ���ʿ� ������ ���ο� �θ� ����� ������ Ʈ��������
            firstInbalanced->left = newparent->right;        
        newparent->right = firstInbalanced;             // ���ο� �θ� ����� �����ʿ� �ұ��� ���Ʈ�� ����

/* RR */        
    }else if(rotationType == "RR"){
        // ���� �ұ�������� �θ��忡 ���� �ұ��� ����� ���� �ڽ�Ʈ���� ��Ʈ��� ����
        if(flag == "left"){
            parentInbalanced->left = firstInbalanced->right;
            newparent = parentInbalanced->left;
        }else if(flag == "right"){
            parentInbalanced->right = firstInbalanced->right;
            newparent = parentInbalanced->right;
        }else{                  // ���� �ұ�����尡 ��Ʈ����϶�
            T->root = firstInbalanced->right;
            newparent = T->root;
        }
        
        firstInbalanced->right = NULL;                  // ���� �ұ�������� ������ Ʈ�� ����        
        if(newparent->left != NULL)                     // �ұ�������� �����ʿ� ������ ���ο� �θ� ����� ���� Ʈ��������
            firstInbalanced->right = newparent->left;
        newparent->left = firstInbalanced;              // ���ο� �θ� ����� ���ʿ� �ұ��� ���Ʈ�� ����

/* LR */
    }else if(rotationType == "LR"){
        nodeAVL* A = firstInbalanced->left;     //L
        nodeAVL* B = A->right;                  //LR

        // ���� �ұ�������� �θ��忡 LRƮ�� ����
        if(flag == "left"){
            parentInbalanced->left = B;
            newparent = parentInbalanced->left;
        }else if(flag == "right"){
            parentInbalanced->right = B;
            newparent = parentInbalanced->right;
        }else{                      // ���� �ұ�����尡 ��Ʈ����϶�
            T->root = B;
            newparent = T->root;
        }
        
        A->right = NULL;                                // L���� ������ ��� ����  
        if(newparent->left != NULL)                     // LƮ���� �����ʿ� ���ο� �θ����� ���� Ʈ�� ����
            A->right = newparent->left;        
        newparent->left = A;                            // ���ο� �θ����� ���ʿ� ����LƮ�� ����        
        firstInbalanced->left = NULL;                   // ���� �ұ���Ʈ���� ���� ����        
        if(newparent->right != NULL)                    // ���� �ұ���Ʈ���� ���ʿ� ���ο� �θ����� ������ Ʈ�� ����
            firstInbalanced->left = newparent->right;
        newparent->right = firstInbalanced;             // ���ο� �θ����� �����ʿ� ���� �ұ���Ʈ�� ��Ʈ��� ����

/* RL */        
    }else if(rotationType == "RL"){
        nodeAVL* A = firstInbalanced->right;        // R
        nodeAVL* B = A->left;                       // RL

        // ���� �ұ�������� �θ��忡 RLƮ�� ����
        if(flag == "left"){
            parentInbalanced->left = B;
            newparent = parentInbalanced->left;
        }else if(flag == "right"){
            parentInbalanced->right = B;
            newparent = parentInbalanced->right;
        }else{                      // ���� �ұ�����尡 ��Ʈ����϶�
            T->root = B;
            newparent = T->root;
        }

        A->left = NULL;                                 // RƮ������ ���� ��� ����
        if(newparent->right != NULL)                    // RƮ���� ���ʿ� ���ο� �θ����� ������Ʈ�� ����
            A->left = newparent->right; 
        newparent->right = A;                           // ���ο� �θ����� �����ʿ� ���� RƮ�� ����
        firstInbalanced->right = NULL;                  // ���� �ұ��� ��忡�� ���� RƮ�� ����
        if(newparent->left != NULL)                     // ���� �ұ�������� �����ʿ� ���ο� �θ����� ����Ʈ�� ����
            firstInbalanced->right = newparent->left;
        newparent->left = firstInbalanced;              // ���ο� �θ����� ���ʿ� ���� �ұ������ ����

    }
    
    /* update height and BF */
    
    if(rotationType == "LR" or rotationType == "RL"){
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

    }else{
        nodeAVL* newchild = NULL;
        if(rotationType == "LL")                // LL
            newchild = newparent->right;
        else                                    // RR
            newchild = newparent->left;

        if(newchild->left != NULL && newchild->right != NULL){
            newchild->height = 1 + max(newchild->left->height, newchild->right->height);
            newchild->BF = newchild->left->height - newchild->left->height;
        }else if(newchild->left != NULL && newchild->right == NULL){
            newchild->height = 1 + newchild->left->height;
            newchild->BF = newchild->left->height;
        }else if(newchild->left == NULL  && newchild->right != NULL){
            newchild->height = 1 + newchild->right->height;
            newchild->BF = 0 - newchild->right->height;
        }else{
            newchild->height = 1;
            newchild->BF = 0;
        }
    }
    if(parentInbalanced != NULL){
        parentInbalanced->height = 1 + max(parentInbalanced->left->height, parentInbalanced->right->height);
        parentInbalanced->BF = parentInbalanced->left->height - parentInbalanced->right->height;
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

// noNodes() ����Ʈ���� �� ��� ���� ��ȯ : ����Ʈ���� ���̰� ���� �� ��
int noNodes(nodeAVL *T){
    if(T == NULL)
        return 0;
    else
        return noNodes(T -> left) + noNodes(T -> right) + 1;
}

// maxNode() �ִ��� ���ϱ� - left
nodeAVL *maxNode(nodeAVL *T){
    nodeAVL *maxnode = T;
    if(T==NULL)
        return NULL;

    while(maxnode->right != NULL){
        maxnode = maxnode->right;
    }    
    return maxnode;
}

// minNode() �ּҳ�� ���ϱ�- right
nodeAVL *minNode(nodeAVL *T){
    nodeAVL *minnode = T;
    if(T==NULL)
        return NULL;

    while(minnode->left != NULL){
        minnode = minnode->left;
    }

    return minnode;
}

// deleteBST(T, deleteKey)
void deleteBST(Tree* T, int deleteKey){
    nodeAVL* parent = NULL;     // ������ ����� �θ���
    nodeAVL* curr = T->root;    // ������ ���
    nodeAVL* repalceNode = NULL;    // ������ �ڸ��� ��� ���� ���

    // find posiotion of deleteKey while storing parent on stack
    while(curr != NULL && deleteKey != curr->data){       // ���� ��尡 null�� �ƴϰ�, ���� ����� �����Ϳ� ����Ű�� ��ġ���� ������
        parent = curr;
        s.push(parent);

        if(deleteKey < curr->data)
            curr = curr->left;
        else
            curr = curr->right;
    }

    // ������ ���Ұ� ������
    if(curr == NULL)
        return;

    // ������ ����� ������ 2 �϶�      --> ���̺�   
    if(curr->left != NULL && curr->right != NULL){
        if(curr->left->height > curr->right->height){           // ������ �� ���� : ������ �ִ���
            repalceNode = maxNode(curr->left);
            deleteFlag = "LEFT";
        }else if(curr->left->height < curr->right->height){     // �������� �� ���� : �������� �ּҳ��
            repalceNode = minNode(curr->right);
            deleteFlag = "RIGHT";
        }else{                                                  // ���� ���� : ���Ժ�
            if(noNodes(curr->left) >= noNodes(curr->right)){         // ������ �� ���ſ�
                repalceNode = maxNode(curr->left);
                deleteFlag = "LEFT";
            }else{                                                   // �������� �� ���ſ�
                repalceNode = minNode(curr->right);
                deleteFlag = "RIGHT";
            }
        }
        
        // ������� �ڸ��� ��ü��� ����
        curr = repalceNode;

        Tree* subtree;
        subtree->root = NULL;
        
        if(deleteFlag == "LEFT")
            subtree->root = curr->left;
        else
            subtree->root = curr->right;

        // ��ü ����� ���� ��ͼ���
        deleteBST(subtree, repalceNode->data);


    // ������ ����� ������ 0�϶�       --> �׳� ����
    }else if(curr->left == NULL && curr->right == NULL){
        if(parent->left == curr)
            parent->left = NULL;
        else
            parent->right = NULL;
    
    // ������ ����� ������ 1�϶�       -- �θ���� �̾��ְ� ����
    }else{
        // ���� ��尡 �θ����� ��� ��ġ���ִ��� Ȯ��
        if(parent->left == curr)
            deleteFlag = "LEFT";
        else
            deleteFlag = "RIGHT";

        if(curr->left != NULL){
            if(deleteFlag == "LEFT")    // ��������� ���� ��尡 �ְ�, ������尡 �θ��� ���ʿ� ��ġ
                parent->left = curr->left;
            else                        // ��������� ���� ��尡 �ְ�, ������尡 �θ��� �����ʿ� ��ġ
                parent->right = curr->left;
        }else{                          
            if(deleteFlag == "LEFT")     // ��������� ������ ��尡 �ְ�, ������尡 �θ��� ���ʿ� ��ġ
                parent->left = curr->right;
            else                         // ��������� ������ ��尡 �ְ�, ������尡 �θ��� �����ʿ� ��ġ
                parent->right = curr->right;
        }
    }

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

    // ù��° ����
    for(int i = 0; i < arrsize; i++){
        insertAVL(&T, arr[i]);
        cout << rotationType << " ";
        inorderBST(T.root);
        cout << "\n";
    }

    // ������ bf�� height ����
    nodeAVL* parent = NULL;
    nodeAVL* rechecknode = T.root;

    int lastkey = arr[arrsize-1];
    while(rechecknode->data != lastkey){
        parent = rechecknode;
        s.push(parent);
        
        if(rechecknode->data < lastkey)
            rechecknode = rechecknode->right;
        else
            rechecknode = rechecknode->left;
    }
    checkBalance(&T);

    // ù��° ���� (���ϼ���)
    for(int i = 0; i < arrsize; i++){
        deleteAVL(&T, arr[i]);
        cout << rotationType << " ";
        inorderBST(T.root);
        cout << "\n";
    }

/*
    // �ι�° ����
    for(int i = 0; i < arrsize; i++){
        insertAVL(&T, arr[i]);
        cout << rotationType << " ";
        inorderBST(T.root);
        cout << "\n";
    }

    // ������ bf�� height ����
    nodeAVL* parent = NULL;
    nodeAVL* rechecknode = T.root;

    int lastkey = arr[arrsize-1];
    while(rechecknode->data != lastkey){
        parent = rechecknode;
        s.push(parent);
        
        if(rechecknode->data < lastkey)
            rechecknode = rechecknode->right;
        else
            rechecknode = rechecknode->left;
    }
    checkBalance(&T);

    // �ι�° ���� (����)
    */

    return 0;
}