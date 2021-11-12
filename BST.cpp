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
// height() 서브트리의 높이 반환
int height(node *T){
    if(T == NULL)
        return 0;
    else
        return max(height(T -> left) + 1, height(T -> right) + 1);
    }

// noNodes() 서브트리의 총 노드 개수 반환 : 서브트리의 높이가 같을 때 비교
int noNodes(node *T){
    if(T == NULL)
        return 0;
    else
        return noNodes(T -> left) + noNodes(T -> right) + 1;
}

// maxNode() 최대노드 구하기 - left
node *maxNode(node *T){
    node *maxnode = T;
    if(T==NULL)
        return NULL;

    while(maxnode->right != NULL){
        maxnode = maxnode->right;
    }    
    return maxnode;
}

// minNode() 최소노드 구하기- right
node *minNode(node *T){
    node *minnode = T;
    if(T==NULL)
        return NULL;

    while(minnode->left != NULL){
        minnode = minnode->left;
    }

    return minnode;
}

node *searchParentBST(Tree *T, int searchKey, node* p, node* c){ // 트리, 찾을값, 부모노드, 찾을 노드
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
    if(T->root == NULL)        // 루트노드가 없을때
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
    node *curr = T->root;   // 삭제할 노드  
    node *parent = nullptr; // 삭제할 노드의 부모노드
    //parent = searchParentBST(T, delKey, parent, curr);

    node *r = nullptr;                            // 비교할때 저장할 노드 변수
    string flag;                        // 선택한 서브트리 방향

    while((curr!= NULL) && (delKey != curr->data)){         // 부모노드, 탐색노드 세팅
        parent = curr;

        if(delKey < curr->data) 
            curr = curr -> left;
        else
            curr = curr  -> right;
    }
    
    // 삭제할 원소가 없을때
    if(curr == NULL)
        return;
    
    if((curr->left == NULL) && (curr->right == NULL)){          // 삭제할 노드의 차수 == 0
        if(parent == NULL)                                          // 루트만있을 때 -> 루트삭제
            return;
        if(parent->left == curr)
            parent->left = nullptr;
        else
            parent->right = nullptr;

    }else if((curr->left == nullptr)||(curr->right == nullptr)){  // 삭제할 노드의 차수 == 1
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
    }else{                                                  // 삭제할 노드의 차수 ==2 -> 높이 비교

        if(height(curr->left) > height(curr->right)){            // 왼쪽이 더 무거우면 왼쪽트리에서 제일 큰 노드 가져오기
            r = maxNode(curr->left);    
            flag = "LEFT";
        }else if(height(curr->left) < height(curr->right)){       // 오른쪽이 더 무거우면 오른쪽 트리에서 제일 작은 노드 가져오기
            r = minNode(curr->right);
            flag = "RIGHT";                       
        }else{                                                   // 높이가 같으면 서브트리의 전체 레벨에서 노드 개수 구하기 -> 더 무거운 쪽 비교
            if(noNodes(curr->left) >= noNodes(curr->right)){
                r = maxNode(curr->left);
                flag = "LEFT";
            }else{
                r = minNode(curr->right);
                flag = "RIGHT";
            }
        }    
        curr->data = r->data;           // 삭제할 노드 자리에 대체할 노드 삽입
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