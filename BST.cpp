#include <iostream>
#include <algorithm>
#include <stack>
#include <cstddef>
#define NULL __null
// BST
using namespace std;


struct node{
    int data;
    node* left;
    node* right;
};

struct Tree{
    node* root;
};

void initTree(Tree *T){
    T -> root = nullptr;
}

node* getNode(){
    node *newNode = (node*)malloc(sizeof(node));
}

// deleteNode
// height() ����Ʈ���� ���� ��ȯ
int height(node *T){
    if(T == NULL)
        return 0;
    else
        return max(height(T -> left) + 1, height(T -> right) + 1);
    }

// noNodes() ����Ʈ���� �� ��� ���� ��ȯ : ����Ʈ���� ���̰� ���� �� ��
int noNodes(node *T){
    if(T == NULL)
        return 0;
    else
        return noNodes(T -> left) + noNodes(T -> right) + 1;
}

// maxNode() �ִ��� ���ϱ� - left
node *maxNode(node *T){
    node *maxnode = T;
    if(T==NULL)
        return NULL;

    while(maxnode->right != NULL){
        maxnode = maxnode->right;
    }    
    return maxnode;
}

// minNode() �ּҳ�� ���ϱ�- right
node *minNode(node *T){
    node *minnode = T;
    if(T==NULL)
        return NULL;

    while(minnode->left != NULL){
        minnode = minnode->left;
    }

    return minnode;
}

node *searchParentBST(Tree *T, int searchKey, node* p, node* c){ // Ʈ��, ã����, �θ���, ã�� ���
    node *temproot = T->root;
    while(temproot->data != searchKey){
        if(searchKey == temproot->data)
            return NULL;
        
        p = temproot;
        
        if(temproot->data < searchKey)
            temproot = temproot->right;
        else
            temproot = temproot->left;
    }
    return p;
}

void insertBST(Tree *T, int newKey){
    node *parent = NULL;
    node *curr = T->root; 
    //int heightParent = 0;

    while(curr != NULL){
        if(curr -> data == newKey)
            return;
        
        parent = curr;
        //s.push(parent);

        if(newKey < curr->data)       
            curr = curr -> left;        // K < K_i --> go to left
        else                            
            curr = curr -> right;       // K > K_i --> go to right
        
    }

    // create newNode
    node *newNode = getNode();
    newNode->data = newKey;
    newNode->left = NULL;
    newNode->right = NULL;


    // insert newKey for child of parentNode (Link parent and newNode)
    if(T->root == NULL)        // ��Ʈ��尡 ������
        T->root = newNode;
    else if(newKey < parent -> data)
        parent -> left = newNode;
    else
        parent -> right = newNode;

    /*
    while(!s.empty()){
        s.pop();                // pop from stack parent
        heightParent = height(parent);
        
    }*/
}

void deleteBST(Tree *T, int delKey){                
    node *curr = T->root;   // ������ ���  
    node *parent = nullptr; // ������ ����� �θ���
    //parent = searchParentBST(T, delKey, parent, curr);

    node *r = nullptr;                            // ���Ҷ� ������ ��� ����
    string flag;                        // ������ ����Ʈ�� ����

    while((curr!= NULL) && (delKey != curr->data)){         // �θ���, Ž����� ����
        parent = curr;

        if(delKey < curr->data) 
            curr = curr -> left;
        else
            curr = curr  -> right;
    }
    
    // ������ ���Ұ� ������
    if(curr == NULL)
        return;
    
    if((curr->left == NULL) && (curr->right == NULL)){          // ������ ����� ���� == 0
        if(parent == NULL)                                          // ��Ʈ������ �� -> ��Ʈ����
            return;
        if(parent->left == curr)
            parent->left = nullptr;
        else
            parent->right = nullptr;

    }else if((curr->left == nullptr)||(curr->right == nullptr)){  // ������ ����� ���� == 1
        if(parent->left != nullptr){                                           //if-1    /parent             //if-1    /parent             
            if(parent->left == curr)                                              //curr                        //curr  
                parent->left = curr->left;                                    ///node                                 ///node
            else   
                parent->right = curr->left;
        }else{                                                              //if-1      /parent                     /parent
            if(parent->left == curr)                                                         //curr                     //curr
                parent->left = curr->right;                                              ///node                             ////node
            else   
                parent->right = curr->right;
        }
    }else{                                                  // ������ ����� ���� ==2 -> ���� ��

        if(height(curr->left) > height(curr->right)){            // ������ �� ���ſ�� ����Ʈ������ ���� ū ��� ��������
            r = maxNode(curr->left);    
            flag = "LEFT";
        }else if(height(curr->left) < height(curr->right)){       // �������� �� ���ſ�� ������ Ʈ������ ���� ���� ��� ��������
            r = minNode(curr->right);
            flag = "RIGHT";                       
        }else{                                                   // ���̰� ������ ����Ʈ���� ��ü �������� ��� ���� ���ϱ� -> �� ���ſ� �� ��
            if(noNodes(curr->left) >= noNodes(curr->right)){
                r = maxNode(curr->left);
                flag = "LEFT";
            }else{
                r = minNode(curr->right);
                flag = "RIGHT";
            }
        }    
        curr->data = r->data;           // ������ ��� �ڸ��� ��ü�� ��� ����
        free(r);

        Tree *t = (Tree*)malloc(sizeof(struct Tree));

        if(flag == "LEFT"){
            t->root = curr->left;
        }else{
            t->root = curr->right;
        }   
        
        deleteBST(t,r->data);
        free(t);
    }       
    
    
}

void inorderBST(node* T){
    if(T != NULL){
        inorderBST(T -> left);
        cout << T -> data << " ";
        inorderBST(T -> right);

    }
}


int main(void){

    Tree T;
    initTree(&T);

    int arr[] = {0, 25, 500, 33, 49, 17, 403, 29, 105, 39, 66, 305, 44, 19, 441, 390, 12, 81, 50, 100, 999};
    
    int arrSize = sizeof arr / sizeof arr[0];
    for(int i = 1; i <= arrSize-1; i++){
        insertBST(&T, arr[i]);
        inorderBST(T.root);
        cout << endl;

    }


    for(int i = 1; i < arrSize-1; i++){
        deleteBST(&T, arr[i]);
        inorderBST(T.root);
        cout << endl;
    }
    

    return 0;
}