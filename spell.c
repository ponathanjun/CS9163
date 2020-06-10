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
#include <stdio.h>
#include <stdlib.h>

/* Returns the lower case version of the word
 */
void lower_case(char* word)
{
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
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
    
    // Determine output of hash_function of the word
    bucket = hash_function(word);
    // Set cursor equal to the respected bucket
    cursor = hashtable[bucket];
    // Check if lower case word is already in hash table
    lower_case(word);
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
    
    return false;
}


/* Loads dictionary into memory and returns true if successful else false
 */
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
    FILE* fptr;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
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
    while ((read = getline(&line, &len, fptr)) != -1)
    {
        // Set a new node
        node* new_node = malloc(sizeof(struct node));
        // Set node->next to NULL
        new_node->next = NULL;
        // Set node->word to word
        strncpy(new_node->word, read, strlen(read));
        // Determine hashtable bucket of specific word
        int bucket = hash_function(read);
        // Check if hashtable's bucket has been created
        if (hashtable[bucket] == NULL)
        {
            // If not created, add node to the linked list
            hashtable[bucket] = new_node;
        }
        else {
            // If hash table already has bucket, set node's next to the linked list
            new_node->next = hashtable[bucket];
            // Add new node to the front of the linked list
            hashtable[bucket] = new_node;
        }
    }
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
    ssize_t read;
    char* word;
    
    // While line in the file is not EOF
    while ((read = getline(&line, &len, fp)) != -1)
    {
        // Split line on spaces
        word = strtok(read, " ");
        // For each word, remove punctuation from beg/end and run check_word
        while (word != NULL)
        {
            // Remove punctuation from the beginning
            while (isalpha(word[0]) == 0)
            {
                word++;
            }
            // Remove punctuation from the end
            while (isalpha(word[strlen(word)-1]) == 0)
            {
                word[strlen(word)-1] = '\0';
            }
            // Run check_word on each word
            if (check_word(word, hashtable) == false)
            {
                // If misspelled, add word to the misspelled hashtable
                misspelled[num_misspelled] = word;
                num_misspelled++;
            }
            // Continue onto next word
            word = strtok(NULL, " ");
        }
        
    }
    
    return num_misspelled;
}
