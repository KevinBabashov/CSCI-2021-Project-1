#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
dictionary_t *create_dictionary() {
    dictionary_t *dict = malloc(sizeof(dictionary_t));
    if (dict == NULL) {
        return NULL;
    }
    dict->root = NULL;
    dict->size = 0;
    return dict;
}
void new_node(node_t* node, const char *key) {
    strcpy(node->word, key);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
}
int max(int a, int b) {
    return (a > b) ? a : b;
}
node_t *rotate_right(node_t *y)
{
    node_t *x = y->left;
    node_t *t = x->right;
    x->right = y;
    y->left = t;
    y->height = max((y->left != NULL) ? y->left->height : 0, 
                    (y->right != NULL) ? y->right->height : 0) + 1;
    x->height = max((x->left != NULL) ? x->left->height : 0, 
                    (x->right != NULL) ? x->right->height : 0) + 1;
    return x;
}

// Helper to perform the "left" rotation.
// This code is correct and you should not modify it.
node_t *rotate_left(node_t *x)
{
    node_t *y = x->right;
    node_t *t = y->left;
    y->left = x;
    x->right = t;
    x->height = max((x->left != NULL) ? x->left->height : 0, 
                    (x->right != NULL) ? x->right->height : 0) + 1;
    y->height = max((y->left != NULL) ? y->left->height : 0, 
                    (y->right != NULL) ? y->right->height : 0) + 1;
    return y;
}
node_t* ins_node(node_t *node, const char *add) { 
    if (node == NULL) { // base case of when you reach leaf of bst and add that node to the tree 
        node = malloc(sizeof(node_t));
        if (node == NULL) {
            return NULL;
        }
        new_node(node,add);
        return node;
    }
    int a = strcmp(add, node->word);
    if (a < 0) { // root is greater than add so go left
       node->left = ins_node(node->left, add);
    }
    else if (a > 0) { // adding word is greater than root word.
        node->right = ins_node(node->right, add);
    }
    int leftH; // Lines 66-79 initialize the height of your left and right subtree respectively.
    int rightH;
    if (node->left != NULL) {
        leftH = node->left->height;
    }
    else {
        leftH = 0;
        }
    if (node->right != NULL) {
        rightH = node->right->height;
    }
    else {
        rightH = 0;
    }
    node->height = 1 + max(leftH, rightH); // calculates height as defined in project write up 
    int difference = leftH - rightH; // balance (left height - right height)
    if (difference > 1 && strcmp(add,node->left->word) < 0) { // left-left rotation
        return rotate_right(node);
    }
    else if (difference < -1 && strcmp(add,node->right->word) > 0) { // right-right rotation
        return rotate_left(node);
    }
    else if (difference > 1 && strcmp(add,node->left->word) > 0) { // left right rotation 
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    else if (difference < -1 && strcmp(add,node->right->word) < 0) { // right left rotation 
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    return node;
}

int dict_insert(dictionary_t *dict, const char *word) {
    dict->root = ins_node(dict->root, word);
    if (dict->root == NULL) { // If ins_node returned null, then return -1 since insert failed 
        return -1;
    }
    dict->size++; // increments size otherwise as size was increase with node.
    return 0;
}
int dict_finder(node_t *newNode, const char *query) {
    if (newNode == NULL) { // If node that is passed is null 
        return 0;
    }
    if (strcmp(newNode->word, query) == 0) { // checks if current node has the word we are querying for.
        return 1;
    }
    else if (strcmp(query, newNode->word) < 0) { // checks if its in the left subtree
        return dict_finder(newNode->left , query);
    }
    else  if (strcmp(query, newNode->word) > 0) { // checks if its in the right subtree.
        return dict_finder(newNode->right, query);
    }
    return 0;
}
int dict_find(const dictionary_t *dict, const char *query) {
    if (dict == NULL) { // Null catcher 
        return 0; 
    }
    int x = dict_finder(dict->root, query); // call to recursive function that does the work.
    if (x == 0) {
        return 0;
    }
    return 1;
}

void dict_helper(node_t* node) {
    if (node == NULL) {
        return;
    }
    dict_helper(node->left); // prints words in an in order fashion so that words are printed in the correct alphabetical order
    printf("%s\n", node->word);
    dict_helper(node->right);
}
void dict_print(const dictionary_t *dict) {
    if (dict == NULL) {
        return;
    }
    dict_helper(dict->root);
    }
void dict_free_helper(node_t *node) {
    if (node == NULL) {
        return;
    }
    dict_free_helper(node->left); // Free's in a post order fashion so that root is not freed before the roots right node is.
    dict_free_helper(node->right);
    free(node);
}
void dict_free(dictionary_t *dict) { // function call to free helper
    if (dict == NULL) {
        return;
    }
    dict_free_helper(dict->root);
    free(dict);
}


dictionary_t *read_dict_from_text_file(const char *file_name) {
    FILE *fp =fopen(file_name, "r");
    char word[MAX_WORD_LEN];
    if (fp == NULL) {
        return NULL;
    }
    dictionary_t *dict = create_dictionary();
    while (fscanf(fp, "%s\n", word) != EOF) { // Reads in each line of the txt file into the dict and inserts that word into bst of dict.
        if (dict == NULL) {
            return NULL;
        }
    dict_insert(dict, word);
}
fclose(fp);
return dict;
}
int write_dict_helper(node_t *node,FILE *fp) {
    if (node == NULL) {
        return -1;
    }
    write_dict_helper(node->left, fp); // prints in an in order fashion so that write word does so in an alphabetical fashion.
    fprintf(fp, "%s\n", node->word);
    write_dict_helper(node->right, fp);
    return 0;
}
int write_dict_to_text_file(const dictionary_t *dict, const char *file_name) {
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL) {
        return -1;
    }
    int a = write_dict_helper(dict->root, fp);
    fclose(fp);
    return a;
}
