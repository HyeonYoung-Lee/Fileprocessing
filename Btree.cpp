#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <memory.h>

using namespace std;

struct BTnode{
  int n, *K, *A;
  struct BTnode **P;   
};

struct BTree{
    BTnode* root;
};

BTnode* getBTnode(int m){
  BTnode *node = (BTnode *)malloc(sizeof(BTnode));
  node->n = 0;
  node->K = (int *)malloc(sizeof(int) * (m-1));
  for(int i = 1; i <= m-1; i++)
    (node->K)[i] = 0;

  node->A = NULL;
  node->P = (BTnode **)calloc(m, sizeof(BTnode *)); // sizeof(BTnode *)크기의 변수를 m개 저장할 수 있는 공간 할당
  for(int i = 0; i < m-1; i++)
    (node->P)[i] = NULL;

  return node;
}

stack<BTnode*> s;
stack<int> i_stack;



void initTree(BTree *T){
  T->root = nullptr;
}

/**
 * binarySearch returns index i where K[i-1] < key <= K[i].
 * @param K: an array
 * @param n: size of array
 * @param key: a key to search
 * @return index of key in K
 */

int binarySearch(int K[], int n, int key){
  // K[1] ~ K[end]
  int index = 1;
  int start = 0;
  int end = n;
  int median = end/2;
  

  while((end-start) > 1){
    if(K[median] == key){       // key값이 이미 존재할 때
      index == median;
      break;
    }        
    else if(K[median] > key)     // key값이 중간보다 작을때
      end = median;
    else                        // key값이 중간보다 클 때
      start = median;


    n = end - start;
    if(n%2 == 0)
      median = start + n/2;
    else
      median = start + (n/2 + 1);
  }

  if(K[end] > key)
    index = start;
  else if(K[end] < key)
    index = end;

  return index;
}


/**
 * insertBT inserts newKey into T.
 * @param T: a B-tree
 * @param m: fanout of B-tree
 * @param newKey: a key to insert
 */
void insertBT(BTree *T, int m, int newKey){
  if(T->root == NULL){      // 루트노드 없으면 생성
    T->root = getBTnode(m);
    T->root->K[1] = newKey;
    T->root->n = 1;
  }

  BTnode* curr = T->root;
  BTnode* second = NULL;
  BTnode* parent = NULL;
  

  // find position to insert newKey while storing parent node on stack
  while(curr != NULL){
    int index = binarySearch(curr->K, curr->n, newKey);

    if(curr->K[index] == newKey)  // newKey already exists in T
      return;
    
    s.push(curr);
    i_stack.push(index);

    curr = curr->P[index];
  }
  // now, curr is null

  BTnode *newRoot = NULL;

  // insert key and node while popping parent node from stack
  while(!s.empty()){
    curr = s.top();
    s.pop();

    int index = i_stack.top();
    i_stack.pop();

    if(curr->n < m-1){      // not overflow
      /* insert newKey into x.K */
      for(int i = curr->n; i > index; i--){
        int value = curr->K[i];
        curr->K[i+1] = value;
        curr->P[i+1] = curr->P[i];
      }
         
      curr->K[index+1] = newKey;
      curr->n += 1;

      if(newRoot != NULL){
        curr->P[index] = newRoot->P[0];
        //newRoot->P[0] = NULL;
        curr->P[index+1] = newRoot->P[1];
        //newRoot->P[1] = NULL;
        delete(newRoot);
      }

      while(!s.empty())
        s.pop();
      while(!i_stack.empty())
        i_stack.pop();
      
      return;

    }else{                  //case of overflow
      BTnode *tempNode = getBTnode(m+1);
      memcpy(&tempNode, &curr, sizeof(curr));        // 1. 새로운 값 삽입했다고 가정 -> tempnode

      /* insert newKey into tempNode.K */
      for(int i = curr->n; i > index; i--)
         tempNode ->K[i+1] = curr->K[i];
      tempNode ->K[index+1] = newKey;
      

      int centerIndex = m/2 + 1;
      newKey = tempNode->K[centerIndex];            // 2. 중간값 구하기

                                                    // 3. split
      for(int i = 1; i < centerIndex; i++){         // copy 1st half of tempnode to curr          
        int value = tempNode->K[i];
        curr->K[i] = value;
      }
      curr->n = m/2;

      second = getBTnode(m);                    
      int newI = 1;
      for(int i = centerIndex + 1; i <= m; i++){    // copy 2nd half of tempnode to second
        int value = tempNode->K[i];
        second->K[newI] = value;
        newI++;
      }
      second->n = newI-1;
      
    }

    if(newRoot == NULL){
      newRoot = getBTnode(m);
      newRoot->K[1] = newKey;
      newRoot->P[0] = curr;
      newRoot->P[1] = second;
      newRoot->n = 1;
      if(s.empty())
        T->root = newRoot;
    }else{
      BTnode *temp = getBTnode(m);
      memcpy(&temp, &newRoot, sizeof(newRoot));

      newRoot->K[1] = newKey;
      newRoot->P[0] = curr;
      newRoot->P[1] = second;

      newRoot->P[index-1]->P[0] = temp->P[0];
      newRoot->P[index-1]->P[1] = temp->P[1];

    }
  }
}


/**
 * deleteBT deletes oldKey from T.
 * @param T: a B-tree
 * @param m: fanout of B-tree
 * @param oldKey: a key to delete
 */
void deleteBT(BTree* T, int m, int oldKey){
  
}