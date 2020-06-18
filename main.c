//
//  main.c
//  CS9163-Assignment-1
//
//  Created by Jonathan Pun on 6/12/20.
//  Copyright © 2020 Pun. All rights reserved.
//

#include <stdio.h>
#include "dictionary.h"
#include <stdlib.h>

void clean(hashmap_t hashtable[], char* misspelled[], int num_misspelled)
{
    // Clean hashtable
    for (int i = 0; i < HASH_SIZE; i++)
    {
        node* current = hashtable[i];
        while (current != NULL)
        {
            node* delete_node = current;
            current = current->next;
            free(delete_node);
        }
    }
    
    // Clean misspelled array
    for (int i = 0; i < num_misspelled; i++)
    {
        free(misspelled[i]);
    }
}

int main(int argc, char* argv[])
{
    hashmap_t hashtable[HASH_SIZE];
    char* misspelled[MAX_MISSPELLED];
    char* file = argv[1];
    char* dict = argv[2];
    int num_misspelled = 0;
    
    // Open up return file
    FILE* fptr = fopen(file, "r");
    
    // Check to see if file is valid
    if (fptr != NULL)
    {
        // Load dictionary
        bool success = load_dictionary(dict, hashtable);
        if (success)
        {
            num_misspelled = check_words(fptr, hashtable, misspelled);
        }
    
        printf("Misspelled words: %d\n", num_misspelled);
        
        // Close file
        fclose(fptr);
    
        // Clean
        clean(hashtable, misspelled, num_misspelled);
    }
    
    return 0;
}
