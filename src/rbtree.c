#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  struct node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  nil_node->color = RBTREE_BLACK ;

  p->nil = nil_node;
  p->root = nil_node;
  return p;
}

void postorder(struct node_t* node, rbtree* t){
  if (node->left != t->nil){
    postorder(node->left, t);
  }
  else if(node->right != t->nil){
    postorder(node->right, t);
  }
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil){
    postorder(t->root, t);
  }
  free(t->nil);
  free(t);
}

void left_rot(rbtree *t, node_t* node_x){
  node_t *node_y = node_x->right;
  node_x->right = node_y->left;
  if (node_y->left != t->nil){
    node_y->left->parent = node_x;
  }
  node_y->parent = node_x->parent;
  if (node_x->parent == t->nil){
    t->root = node_y;
  } 
  else if(node_x == node_x->parent->left){
    node_x->parent->left = node_y;
  }
  else{
    node_x->parent->right = node_y;
  }
  node_y->left = node_x;
  node_x->parent = node_y;
  // return node_x;
}

void right_rot(rbtree *t, node_t* node_y){
  node_t *node_x = node_y->left;
  node_y->left = node_x->right;
  if (node_x->right != t->nil){
    node_x->right->parent = node_y;
  }
  node_x->parent = node_y->parent;
  if (node_y->parent == t->nil){
    t->root = node_x;
  } 
  else if(node_y == node_y->parent->left){
    node_y->parent->left = node_x;
  }
  else{
    node_y->parent->right = node_x;
  }
  node_x->right = node_y;
  node_y->parent = node_x;
  // return node_y;
}

void insert_fixup(rbtree *t, node_t* node_z){
  node_t *node_y;
  while (node_z->parent->color == RBTREE_RED){
    if (node_z->parent == node_z->parent->parent->left){
      node_y = node_z->parent->parent->right;
      if(node_y->color == RBTREE_RED){
        node_z->parent->color = RBTREE_BLACK;
        node_y->color = RBTREE_BLACK;
        node_z->parent->parent->color = RBTREE_RED;
        node_z = node_z->parent->parent;
      }
      else{
        if(node_z == node_z->parent->right){
          node_z = node_z->parent;
          left_rot(t, node_z);
        }
        node_z->parent->color = RBTREE_BLACK;
        node_z->parent->parent->color = RBTREE_RED;
        right_rot(t, node_z->parent->parent);
      }
      
    }
    else{
      node_y = node_z->parent->parent->left;
      if (node_y->color == RBTREE_RED){
        node_z->parent->color = RBTREE_BLACK;
        node_y->color = RBTREE_BLACK;
        node_z->parent->parent->color = RBTREE_RED;
        node_z = node_z->parent->parent;
      }
      else{
        if(node_z == node_z->parent->left){
          node_z = node_z->parent;
          right_rot(t, node_z);
        }
        node_z->parent->color = RBTREE_BLACK;
        node_z->parent->parent->color = RBTREE_RED;
        left_rot(t, node_z->parent->parent);
      }
    }
  }
  
  t->root->color = RBTREE_BLACK;
  // return node_z;
}

node_t *rbtree_insert(rbtree *t, const key_t key) { 
  // struct node_t *node = (node_t *)calloc(1, sizeof(node_t));
  // node->key = key;
  // node->left = node->right = node ->parent = t->nil;
  // t->root = node;
  // ↑ insert single
  node_t *node_z = (node_t *)calloc(1, sizeof(node_t));
  node_t *node_x = t->root;
  node_t *node_y = t->nil;
  node_z->key = key;
  // node_z->left = node_z->right = node_z ->parent = t->nil;
  while (node_x != t->nil){
    node_y = node_x;
    if (node_z->key < node_x->key){
      node_x = node_x->left;
    }
    else{
      node_x = node_x->right;
      }
  }
  node_z->parent = node_y;
  if (node_y == t->nil){
    t->root = node_z;
  }
  else if (node_z->key < node_y->key){
    node_y->left = node_z;
  }
  else{
    node_y->right = node_z;
  }
  node_z->left = t->nil;
  node_z->right = t->nil;
  node_z->color = RBTREE_RED;
  
  insert_fixup(t, node_z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  struct node_t* node = t->root;
  while(node != t->nil){
      if(node->key == key){
        return node;
      }
      else if (node->key < key){
        node = node->right; 
      } 
      else{
        node = node->left;
      }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  struct node_t *current = t->root;

  while (current != t->nil && current->left != t->nil)
    current = current->left;z
  return current;
  // return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  struct node_t *current = t->root;

  while (current != t->nil && current->right != t->nil)
    current = current->right;
  return current;
  // return t->root;
}


void transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil){
    t->root = v;
  }
  else{
    if(u == u->parent->left){
      u->parent->left = v;
    }
    else{
      u->parent->right = v;
      }
    
  }
  v->parent = u->parent;
}

node_t *tree_min(rbtree*t, node_t *z){
  node_t * current = z;
  while (current != t->nil && current->left != t->nil){
    current = current->left;
  }
  return current;
}

void del_fixup(rbtree *t, node_t *x){
  node_t * w;
  while (x != t->root && x->color == RBTREE_BLACK){
    if (x == x->parent->left){
      w = x->parent->right;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rot(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rot(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rot(t,x->parent);
        x = t->root;
      }
    }
    else{
      w = x->parent->left;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rot(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else{
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rot(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rot(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t * y;
  y = z;
  node_t * x;
  color_t y_origin = y->color;
 
  if (z->left == t->nil){
    x = z->right;
    transplant(t, z, z->right);
  }
  else if (z->right == t->nil){
    x = z->left;
    transplant(t, z, z->left);
  }
  else{
    y = tree_min(t, z->right);
    y_origin = y->color;
    x = y -> right;
    if (y != z->right){
      transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
      
    }
    else{
      x->parent = y;
  }
    transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  
  if (y_origin == RBTREE_BLACK){
    
    del_fixup(t, x);
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
