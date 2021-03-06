/*
*   Operating System
*       Synchronization
*       Student name : Kwon boyoon, Cho minseo
*
*   bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>


#include "lab2_sync_types.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree *tree) {
    // You need to implement lab2_node_print_inorder function.
   if(tree->root ==NULL){
      printf(" Tree is EMPTY!!\n");
      return 0;
   }
   return inorder(tree->root);
}
int inorder(lab2_node *node){
   if(node){
      inorder(node->left);
      printf("%d\t",node->key);
      inorder(node->right);
      free(node);
      node=NULL;
   }
   return 1;
}
/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    // You need to implement lab2_tree_create function.
   lab2_tree *tree =(lab2_tree*)malloc(sizeof(lab2_tree));
   tree->root =NULL;
   return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    // You need to implement lab2_node_create function.
   lab2_node * node =(lab2_node *)malloc(sizeof(lab2_node));
   pthread_mutex_init(&node ->mutex,NULL);
   node->left = NULL;
   node->right =NULL;
   node->key =key;
   return node;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert function.
   if(!tree->root){
      tree->root =new_node;
      return 0;
   }
   lab2_node *child =tree->root;
   lab2_node *parent = NULL;

   while(child){
      if(new_node->key==child->key)
         return -1;
      else if(new_node->key < child->key){
         parent =child;
         child= child->left;
      }
      else{
         parent=child;
         child= child->right;
      }
   }

   if(new_node->key < parent->key)
      parent->left = new_node;
   else
      parent->right=new_node;
   
   return 0;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
      // You need to implement lab2_node_insert_fg function.
	  pthread_mutex_lock(&lock);
   if(!tree->root){
      tree->root =new_node;
		pthread_mutex_unlock(&lock);
      return 0;

   }
   pthread_mutex_unlock(&lock);
   
   
   lab2_node *child =tree->root;
   lab2_node *parent = NULL;

   while(child){
	   pthread_mutex_lock(&lock);
      if(new_node->key==child->key){
		  pthread_mutex_unlock(&lock);
	     return -1;
	  }	 
      else if(new_node->key < child->key){
         parent =child;
         child= child->left;
      }
      else{
         parent=child;
         child= child->right;
      }
       
   }


   if(new_node->key < parent->key)
      parent->left = new_node;
   else
      parent->right=new_node;
   pthread_mutex_unlock(&lock);
   return 0;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){
    // You need to implement lab2_node_insert_cg function.
	pthread_mutex_lock(&lock);
   if(!tree->root){
      tree->root =new_node;
      return 0;
   }
   lab2_node *child =tree->root;
   lab2_node *parent = NULL;

   while(child){
      if(new_node->key==child->key){
	pthread_mutex_unlock(&lock);
         return -1;
   }
      else if(new_node->key < child->key){
         parent =child;
         child= child->left;
      }
      else{
         parent=child;
         child= child->right;
      }
   }

   if(new_node->key < parent->key)
      parent->left = new_node;
   else
      parent->right=new_node;
   
   pthread_mutex_unlock(&lock);
   return 0;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove function.
   lab2_node *parent= NULL;
   lab2_node *child = tree->root;

   while(child){
      if(key == child->key)
         break;
      else if(key < child->key){
         parent = child;
         child = child->left;
      }
      else{
         parent = child;
         child = child->right;
      }
   }
   // If tree doesn't have a key
   if(!child)
      return -1;

   if(child->left && child->right){
      lab2_node *parent_r = child, *child_r = child->right;
      
      while(child_r->left){
         parent_r = child_r;
         child_r = child_r->left;
      }
      
      child->key = child_r->key;
      
      if(parent_r->right==child_r)
         parent_r->right=child_r->right;
      
      else
         parent_r->left=child_r->right;
      free(child_r);
   }
   else if(child -> left || child->right){
      
      if(child->left){
         if(child==tree->root)
            tree->root=child->left;
         else if(parent->left==child)
            parent->left=child->left;
         else
            parent->right=child->left;
      }
      
      else{
         if(child==tree->root)
            tree->root=child->right;
         else if(parent->left==child)
            parent->left=child->right;
         else
            parent->right=child->right;
      }
      free(child);
   }
   
   else{
      
      if(child==tree->root)
         tree->root=NULL;
      
      else if(parent->left==child)
         parent->left=NULL;
      
      else
         parent->right=NULL;
      free(child);
   }
   
   return 0;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_fg function.
   lab2_node *parent= NULL;
   lab2_node *child = tree->root;

   while(child){
      if(key == child->key){
         break;
      }else if(key < child->key){
         parent = child;
         child = child->left;
      }
      else{
         parent = child;
         child = child->right;
      }
   }
   // If tree doesn't have a key
   if(!child)
      return -1;

   if(child->left && child->right){
      lab2_node *parent_r = child, *child_r = child->right;
      
      while(child_r->left){
         parent_r = child_r;
         child_r = child_r->left;
      }
      
      child->key = child_r->key;
      
      if(parent_r->right==child_r)
         parent_r->right=child_r->right;
      
      else
         parent_r->left=child_r->right;
      free(child_r);
   }
   else if(child -> left || child->right){
      
      if(child->left){
         if(child==tree->root)
            tree->root=child->left;
         else if(parent->left==child)
            parent->left=child->left;
         else
            parent->right=child->left;
      }
      
      else{
         if(child==tree->root)
            tree->root=child->right;
         else if(parent->left==child)
            parent->left=child->right;
         else
            parent->right=child->right;
      }
      free(child);
   }
   
   else{
      
      if(child==tree->root)
         tree->root=NULL;
      
      else if(parent->left==child)
         parent->left=NULL;
      
      else
         parent->right=NULL;
      free(child);
   }
   
   return 0;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    // You need to implement lab2_node_remove_cg function.
   lab2_node *parent= NULL;
   lab2_node *child = tree->root;

   while(child){
      if(key == child->key)
         break;
      else if(key < child->key){
         parent = child;
         child = child->left;
      }
      else{
         parent = child;
         child = child->right;
      }
   }
   // If tree doesn't have a key
   if(!child)
      return -1;

   if(child->left && child->right){
      lab2_node *parent_r = child, *child_r = child->right;
      
      while(child_r->left){
         parent_r = child_r;
         child_r = child_r->left;
      }
      
      child->key = child_r->key;
      
      if(parent_r->right==child_r)
         parent_r->right=child_r->right;
      
      else
         parent_r->left=child_r->right;
      free(child_r);
   }
   else if(child -> left || child->right){
      
      if(child->left){
         if(child==tree->root)
            tree->root=child->left;
         else if(parent->left==child)
            parent->left=child->left;
         else
            parent->right=child->left;
      }
      
      else{
         if(child==tree->root)
            tree->root=child->right;
         else if(parent->left==child)
            parent->left=child->right;
         else
            parent->right=child->right;
      }
      free(child);
   }
   
   else{
      
      if(child==tree->root)
         tree->root=NULL;
      
      else if(parent->left==child)
         parent->left=NULL;
      
      else
         parent->right=NULL;
      free(child);
   }
   
   return 0;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
    // You need to implement lab2_tree_delete function.
   free(tree);
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
    // You need to implement lab2_node_delete function.
   node = NULL;
   free(node);
}
