#ifndef DICTIONARY_H
#define DICTIONARY_H

#define MAX_WORD_LEN 128

// Data type for nodes in binary search tree
typedef struct node {
    char word[MAX_WORD_LEN]; // Word, as a null-terminated string
    struct node *left;       // Left child in tree, NULL if no child
    struct node *right;      // Right child in tree, NULL if no child
    int height;
} node_t;

// Data type for a dictionary
typedef struct {
    node_t *root;   // Root of binary search tree storing words, NULL if empty
    unsigned size;  // Total number of nodes in tree, 0 if empty
} dictionary_t;

/*
 * Create a new empty dictionary
 * Returns: Pointer to a dictionary_t representing an empty dictionary
 *          or NULL if an error occurs
 */
dictionary_t *create_dictionary();

/*
 * Add a new word to the dictionary (insert into the binary search tree)
 * dict: A pointer to a dictionary to add the word to
 * word: The new word to add, as a null-terminated string
 * Returns: 0 if the word was successfully added
 *          or -1 if the word could not be added
 */
int dict_insert(dictionary_t *dict, const char *word);

/*
 * Search for a specific word in the dictionary
 * dict: A pointer to the dictionary to search
 * query: The word to search for, as a null-terminated string
 * Returns: 1 if the score was found in the dictionary, 0 otherwise
 */
int dict_find(const dictionary_t *dict, const char *query);

/*
 * Print out all words in a dictionary, sorted alphabetically
 * dict: A pointer to the dictionary containing all words to print
 */
void dict_print(const dictionary_t *dict);

/*
 * Frees all memory used to store the contents of a dictionary
 * dict: A pointer to the dictionary to free
 */
void dict_free(dictionary_t *dict);

/*
 * Create a new dictionary containing all words listed in a text file
 * file_name: The name of the file to read words from
 * Returns: A pointer to a new dictionary containing all of the file's words
 *          or NULL if the read operation fails
 */
dictionary_t *read_dict_from_text_file(const char *file_name);

/*
 * Writes the contents of a dictionary to a text file
 * dict: The dictionary to write
 * file_name: The name of the text file to write to
 * Returns: 0 on success or -1 on failure
 */
int write_dict_to_text_file(const dictionary_t *dict, const char *file_name);

#endif // DICTIONARY_H
