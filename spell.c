//
//  spell.c
//  
//
//  Created by Jonathan Pun on 6/2/20.
//

#include <stdio.h>
#include "dictionary.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Returns the lower case version of the word
 */
char* lower_case(const char* word)
{
    char* lower = strdup(word);
    char* ptr = lower;
    
    if (ptr != NULL)
    {
        while (*ptr)
        {
            *ptr = tolower(*ptr);
            ptr++;
        }
    }
    return lower;
}


/* Returns a boolean indicating if the word was correctly spelled
 */
bool check_word(const char* word, hashmap_t hashtable[])
{
    int bucket;
    hashmap_t cursor = NULL;
    
    // Determine output of hash_function of the word
    bucket = hash_function(word);
    // Set cursor equal to the respected bucket
    cursor = hashtable[bucket];
   // Check if word is already in the hash table
    while (cursor != NULL)
    {
        if (strcmp(word,cursor->word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    
    // Check if lower case word is already in hash table
    char* lower = lower_case(word);
    // Determine output of hash_function of the word
    bucket = hash_function(lower);
    // Set cursor equal to the respected bucket
    cursor = hashtable[bucket];
    while (cursor != NULL)
    {
        if (strcmp(lower,cursor->word) == 0)
        {
            free(lower);
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    free(lower);
    return false;
}


/* Loads dictionary into memory and returns true if successful else false
 */
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    FILE* fptr;
    char* line = NULL;
    size_t len = 0;
    
    // Initialize all values in the hashtable to NULL
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    // Open the dictionary file
    fptr = fopen(dictionary_file, "r");
    // Check if file exists
    if (fptr == NULL)
    {
        return false;
    }
    // While word in the dictionary file is not at the end of file (last word)
    while (getline(&line, &len, fptr) != -1)
    {
        int string_length = strlen(line);
        // Get rid of hidden new line characters
        if (line[string_length-1] == '\n')
        {
            line[string_length-1] = 0;
        }
        string_length = strlen(line);
        if (string_length <= LENGTH)
        {
            // Set a new node
            node* new_node = malloc(sizeof(struct node));
            // Set node->next to NULL
            new_node->next = NULL;
            // Set node->word to word
            strcpy(new_node->word, line);
            // Determine hashtable bucket of specific word
            int bucket = hash_function(line);
            // Check if hashtable's bucket has been created
            if (hashtable[bucket] == NULL)
            {
                // If not created, add node to the linked list
                hashtable[bucket] = new_node;
            }
            else
            {
                // If hash table already has bucket, set node's next to the linked list
                new_node->next = hashtable[bucket];
                // Add new node to the front of the linked list
                hashtable[bucket] = new_node;
            }
        }
    }
    free(line);
    // Close file
    fclose(fptr);

    return true;
}


/* Returns the number of mispelled words
 */
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
    int num_misspelled = 0;
    char* line = NULL;
    size_t len = 0;
    char* word;
    
    // While line in the file is not EOF
    while (getline(&line, &len, fp) != -1)
    {
        // Split line on spaces
        word = strtok(line, " ");
        // For each word, remove punctuation from beg/end and run check_word
        while (word != NULL)
        {
            // Check boundaries
            if (strlen(word) <= LENGTH)
            {
                // Account for new line character
                if (word[strlen(word)-1] == '\n')
                {
                    word[strlen(word)-1] = '\0';
                }
                // Remove punctuation from the beginning
                while (ispunct(word[0]) != 0)
                {
                    word++;
                }
                // Remove punctuation from the end
                while (ispunct(word[strlen(word)-1]) != 0)
                {
                    word[strlen(word)-1] = '\0';
                }
                // Check if the word is all numbers
                bool all_numbers = true;
                for (int i = 0; i < strlen(word); i++)
                {
                    // If character is not a digit, change flag and break
                    if (isdigit(word[i]) == 0)
                    {
                        all_numbers = false;
                        break;
                    }
                }
                // If word is all numbers, skip check_word
                bool check;
                if (all_numbers == true)
                {
                    check = true;
                }
                else
                {
                    check = check_word(word, hashtable);
                }
                // If word is not in the hashtable
                if (check == false)
                {
                    // If misspelled, add word to the misspelled hashtable(account for boundary)
                    if (num_misspelled + 1 <= MAX_MISSPELLED)
                    {
                        misspelled[num_misspelled] = word;
                        num_misspelled++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            // Continue onto next word
            word = strtok(NULL, " ");
        }
    }
    free(line);
    return num_misspelled;
}
