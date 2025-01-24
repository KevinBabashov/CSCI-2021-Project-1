#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

#define MAX_CMD_LEN 128
int spell_check_file(const char *file_name, const dictionary_t *dict) {
    FILE *fp = fopen(file_name, "r");
    char word[MAX_CMD_LEN];
    if (fp == NULL) {
        return 0;
    }
    while(fscanf(fp, "%s", word) != EOF) { // Running until end of file is reached
         if (dict_find(dict, word) == 0) { // checks if word is present in dictionary 
            printf("%s[X] ", word); // If not we print the word with an X next to it
            if (fgetc(fp) == '\n') { // checks if the next character after this word is newline, if so we add one 
                printf("\n");
            }
         }
         else {
            printf("%s ", word); // else the word is present in dictionary then we print the word without X
            if (fgetc(fp) == '\n') {
            printf("\n");
         }
        }
    }
    fclose(fp);
    return 1;
}
int main(int argc, char **argv) {
    dictionary_t *dict = NULL;
    char cmd[MAX_CMD_LEN];

    if (argc == 1) { // case of 1 arguement so we make a dict for you.
        dict = create_dictionary();
        }
    else if (argc == 2) { // 2 arguements so we read the dictionary that was given and then store that into a dictionary AVL.
        dict = read_dict_from_text_file(argv[1]);
        if (dict == NULL) {
            printf("Failed to read dictionary from text file\n");
            return 0;
        }
        printf("Dictionary successfully read from text file\n");
    }
    else if (argc == 3) { // Provided txt file and dictionary to write into, in which case we write into the dictionary the given txt file.
        dict = read_dict_from_text_file(argv[1]);
        if (dict == NULL) {
            printf("Failed to read dictionary from text file\n");
            return 0;
        }
        printf("Dictionary successfully read from text file\n");
        int result = spell_check_file(argv[2], dict);
        if (result == 0) {
            printf("Spell check failed");
            dict_free(dict);
            return 0;
        }
        dict_free(dict);
        return 0;
    }
    else {
        return 0;
    }


    printf("CSCI 2021 Spell Check System\n");
    printf("Commands:\n");
    printf("  add <word>:              adds a new word to dictionary\n");
    printf("  lookup <word>:           searches for a word\n");
    printf("  print:                   shows all words currently in the dictionary\n");
    printf("  load <file_name>:        reads in dictionary from a file\n");
    printf("  save <file_name>:        writes dictionary to a file\n");
    printf("  check <file_name>: spell checks the specified file\n");
    printf("  exit:                    exits the program\n");

    while (1) {
        printf("spell_check> "); // Main functions and calls, self explanatory.
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        }
        else if (strcmp("print", cmd) == 0){
            dict_print(dict);
        }
        else if (strcmp("add", cmd) == 0) {
            scanf("%s", cmd);
            dict_insert(dict, cmd);
            }
        else if (strcmp("lookup", cmd) == 0) {
            scanf("%s", cmd);
            int x = dict_find(dict, cmd);
            if (x==1) {
                printf("'%s' present in dictionary\n", cmd);
            }
            else if (x==0) {
                printf("'%s' not found\n", cmd);
            }
        }
        else if (strcmp("load",cmd) == 0) {
            scanf("%s", cmd);
            dictionary_t *new_dict = read_dict_from_text_file(cmd);
            if (new_dict != NULL) {
                printf("Dictionary successfully read from text file\n");
                dict_free(dict);
                dict = new_dict;
            }
            else {
                printf("Failed to read dictionary from text file\n");
            }
            }
        else if (strcmp("save", cmd) == 0) {
            scanf("%s", cmd);
            int x = write_dict_to_text_file(dict, cmd);
            if (x == 0) {
                printf("Dictionary successfully written to text file\n");
            }
            }
        else if (strcmp("check", cmd) == 0) {
            scanf("%s", cmd);
            int x = spell_check_file(cmd, dict);
            if (x == 0) {
                printf("Spell check failed\n");
            }
            }
        else {
            printf("Unknown command %s\n", cmd);
        }
    }
dict_free(dict);
return 0;
}