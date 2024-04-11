#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->root = nil;
  p->nil = nil;
  p->root->parent = nil;

  return p;
}

void delete_rbtree_recur(rbtree *t, node_t *cur) {
  if(cur->left == t->nil && cur->right == t->nil) {
    free(cur);
    return;
  }

  if(cur->left != t->nil) {
    delete_rbtree_recur(t, cur->left);
  }
  if(cur->right != t->nil) {
    delete_rbtree_recur(t, cur->right);
  }
  free(cur);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if(t->root != t->nil) {
    delete_rbtree_recur(t, t->root);
  }
  free(t->nil);  
  free(t);
}

void left_rotation(rbtree *t, node_t *n) {
  node_t *right = n->right;
  n->right = right->left;

  if(right->left != t->nil) {
    right->left->parent = n;
  }
  right->parent = n->parent;
  if(n->parent == t->nil) {
    t->root = right;
  }else if(n == n->parent->left) {
    n->parent->left = right;
  }else{
    n->parent->right = right;
  }
  right->left = n;
  n->parent = right;
}

void right_rotation(rbtree *t, node_t *n) {

  node_t *left = n->left;
  n->left = left->right;

  if(left->right != t->nil) {
    left->right->parent = n;
  }
  left->parent = n->parent;
  if(n->parent == t->nil) {
    t->root = left;
  }else if(n == n->parent->right) {
    n->parent->right = left;
  }else{
    n->parent->left = left;
  }
  left->right = n;
  n->parent = left;

}

void insert_fixup(rbtree *t, node_t *n) {
  // 루트 노드 인 경우: color를 black으로 변경 하고 종료
  if(t->root == n) {
    t->root->color = RBTREE_BLACK;
    return;
  }

  node_t *grand; // 부모의 부모 노드
  node_t *parent; // 부모 노드
  node_t *uncle; // 부모의 형제 노드

  while(n->parent->color == RBTREE_RED) {
    grand = n->parent->parent;
    parent = n->parent;
    if(n->parent == grand->left){ // n의 부모가 왼쪽 자식인 경우
      uncle = grand->right;
      // CASE 1 - 부모의 형제가 Red 인 경우 : 부모와 부모의 형제의 color를 black, 부모의 부모 color를 red로 변경하고
      // 부모의 부모를 기준으로 다시 체크한다.
      if(uncle->color == RBTREE_RED) { 
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grand->color = RBTREE_RED;
        n = grand;
        if(n == t->root) {
          n->color = RBTREE_BLACK;
          break;
        }
      }else{
        // CASE 2 - n이 부모의 오른쪽 자식인 경우(꺽인 모양인 경우): left rotation을 통해 CASE 3로 만들어 주고, CASE 3 수행.
        if(n == parent->right){ // CASE 2
          n = n->parent;
          left_rotation(t, n);
        }
        // CASE 3 - n이 부모의 왼쪽 자식인 경우: 부모의 color을 black, 부모의 부모의 color를 red로 변경 후 right rotation.
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        right_rotation(t, n->parent->parent);
      }
    }else{ // n의 부모가 오른쪽 자식인 경우(n의 부모가 왼쪽 자식인 경우에서 right, left를 반대로 해주면 된다.)
      uncle = grand->left;
      if(uncle->color == RBTREE_RED) { // CASE 1
        parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grand->color = RBTREE_RED;
        n = grand;
        if(n == t->root) {
          n->color = RBTREE_BLACK;
          break;
        }
      }else{
        if(n == parent->left){ // CASE 2
          n = parent;
          right_rotation(t, n);
        }

        // CASE 3
        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        left_rotation(t, n->parent->parent);
      }
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *current = t->root;
  node_t *parent = t->root->parent;

  // 이진 탐색을 통해 새로운 노드가 들어갈 자리 찾기
  while(current != t->nil) {
    if(key < current->key){
      parent = current;
      current = current->left;
    } 
    else{
      parent = current;      
      current = current->right;
    }
  }

  // 키를 담을 새로운 노드 생성하기.
  node_t *new_node = (node_t*)malloc(sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->parent = parent;
  new_node->left = new_node->right = t->nil;

  // parent에 맞게 새로운 노드를 붙여 준다.
  if(parent == t->nil) { // root노드
    t->root = new_node;
  }
  else if(key < parent->key) { 
    parent->left = new_node;
  }else{
    parent->right = new_node;
  }

  // Red Black의 규칙을 위반하는 경우, Insert_fixup을 통해 고쳐준다.
  // 새로 들어오는 노드는 항상 Red이다. 따라서 부모 노드가 Red이거나, 새로운 노드가 루트 노드이면 규칙 위반이다.
  if(parent->color == RBTREE_RED || new_node == t->root) {
    insert_fixup(t, new_node);
  }

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *current = t->root;
  while(current != t->nil){
    if(current->key == key)
      return current;    
    if(key < current->key) 
      current = current->left;
    else
      current = current->right;    
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->left != t->nil) {
    current = current->left;
  }
  return current;
  // return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->right != t->nil) {
    current = current->right;
  }
  return current;
  // return t->root;
}

void delete_fixup(rbtree *t, node_t *n) {

  while(n->parent != t->nil && n->color == RBTREE_BLACK) {
    if(n == n->parent->left) { // n이 왼쪽 자식인 경우
      node_t *sibling = n->parent->right; // 형제 노드
      // CASE 1 - 형제 노드가 red인 경우
      // 형제의 color를 black, 부모의 color를 red로 만든뒤 부모를 기준으로 left lotation한 후 CASE 2, 3, 4 중 하나로 해결
      if(sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        n->parent->color = RBTREE_RED;
        left_rotation(t, n->parent);
        sibling = n->parent->right;
      }

      // CASE 2 - 형제 노드가 black이고, 형제의 두 자녀가 black인 경우
      // n과 형제의 black을 모아서 부모에게 전달(형제의 color를 red로 변경)
      if(sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) { // CASE 2
        sibling->color = RBTREE_RED;
        n = n->parent;
      }else{
        // CASE 3 - 형제 노드가 black 형제의 왼쪽 자녀가 red, 형제의 오른쪽 자녀가 black 인 경우
        // 형제와 오른쪽 형제의 왼쪽 자식 color change, 형제를 기준으로 right lotation 후 CASE 4를 통해 해결
        if(sibling->right->color == RBTREE_BLACK) { // CASE 3
          sibling->color = RBTREE_RED;
          sibling->left->color = RBTREE_BLACK;
          right_rotation(t, sibling);
          sibling = n->parent->right;
        }
        // CASE 4 - 형제 노드가 black이고, 형제의 오른쪽 자녀가 red인 경우
        // 형제는 부모의 색으로, 형제의 오른쪽 자녀와 부모의 색을 black으로 변경 후 부모를 기준으로 right lotation
        sibling->color = n->parent->color;
        sibling->right->color = RBTREE_BLACK;
        n->parent->color = RBTREE_BLACK;
        left_rotation(t, n->parent) ;
        n = t->root;
      }
    }else{ // n이 오른쪽 자식인 경우 (왼쪽 자식인 경우에서 right, left 바꾸기)
      node_t *sibling = n->parent->left;
      if(sibling->color == RBTREE_RED) { // CASE 1
        sibling->color = RBTREE_BLACK;
        n->parent->color = RBTREE_RED;
        right_rotation(t, n->parent);
        sibling = n->parent->left;
      }

      if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) { // CASE 2
        sibling->color = RBTREE_RED;
        n = n->parent;
      }else{
        if(sibling->left->color == RBTREE_BLACK) { // CASE 3
          sibling->color = RBTREE_RED;
          sibling->right->color = RBTREE_BLACK;
          left_rotation(t, sibling);
          sibling = n->parent->left;
        }
        // CASE 4
        sibling->color = n->parent->color;
        sibling->left->color = RBTREE_BLACK;
        n->parent->color = RBTREE_BLACK;
        right_rotation(t, n->parent);
        n = t->root;
     }
    }
  }

  n->color = RBTREE_BLACK;
}

// 노드를 대체하는 함수. (a노드 자리를 b로 대체한다.)
void change_node(rbtree *t, node_t *a, node_t *b) {
  if(a->parent == t->nil) {
    t->root = b;
  }else if(a == a->parent->left) {
    a->parent->left = b;
  }else{
    a->parent->right = b;
  }
  b->parent = a->parent;
}

node_t *find_successor(rbtree *t, node_t *n) {
  while(n->left != t->nil) {
    n = n->left;
  }

  return n;
}

int rbtree_erase(rbtree *t, node_t *p) {

  // TODO: implement erase
  color_t color = p->color; // 삭제되는 노드의 color
  node_t *delete_node = p; // 삭제되는 노드
  node_t *replace_node; // 삭제되는 노드를 대체하는 노드

  if(p->left == t->nil) { // 왼쪽 자식이 없거나, 양쪽 자식이 모두 없는 경우
    replace_node = p->right;
    change_node(t, p, p->right);
  }else if(p->right == t->nil) { // 오른쪽 자식이 없는 경우
    replace_node = p->left;
    change_node(t, p, p->left);
  }else{ // 양쪽 자식이 있는 경우 - 오른쪽 서브트리의 최소값(successor)을 찾아서 그 값으로 바꾸고, successor을 삭제한다.
    delete_node = find_successor(t, p->right);
    color = delete_node->color;
    replace_node = delete_node->right; // successor은 왼쪽 노드가 없으므로 대체 노드는 오른쪽 노드가 된다.

    change_node(t, delete_node, replace_node);
    p->key = delete_node->key; // p와 delete_node의 Key값만 바꿔주고, delete_node를 삭제한다.
    p = delete_node; // delete_node를 free하기 위해서 p에 delete_node 넣기

    // clsr책의 pseudo code 방식(p와 delete_node를 아예 바꾸는 방식)
    // if(delete_node != p->right){
    //   change_node(t, delete_node, delete_node->right);
    //   delete_node->right = p->right;
    //   delete_node->right->parent = delete_node;
    // }else{ // replace_node가 nil노드이기 때문에 parent연결해 주어야 함
    //   replace_node->parent = delete_node;
    // }
    // change_node(t, p, delete_node);
    // delete_node->left = p->left;
    // delete_node->left->parent = delete_node;
    // delete_node->color = p->color;
  }

  if(color == RBTREE_BLACK) {
    delete_fixup(t, replace_node);
  }
  free(p);

  return 0;
}

int traverse(const rbtree *t, node_t *cur, key_t *arr, const size_t n, int i) {
  if(i >= n) 
    return 0;
  if(cur->left != t->nil) {
    i = traverse(t, cur->left, arr, n, i);
  }
  *(arr+i) = cur->key;
  if(cur->right != t->nil) {
    i = traverse(t, cur->right, arr, n, i+1);
    return i;
  }
  return i+1;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  traverse(t, t->root, arr, n, 0);
  return 0;
}