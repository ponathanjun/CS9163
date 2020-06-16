//
//  main.c
//  CS9163-Assignment-1
//
//  Created by Jonathan Pun on 6/12/20.
//  Copyright © 2020 Pun. All rights reserved.
//

#include <stdio.h>
#include "dictionary.h"

int main(int argc, char* argv[])
{
    hashmap_t hashtable[HASH_SIZE];
    char* misspelled[MAX_MISSPELLED];
    char* file = argv[1];
    char* dict = argv[2];
    int num_misspelled = 0;
    
    // Open up return file
    FILE* fptr = fopen(file, "r");
    
    // Load dictionary
    bool success = load_dictionary(dict, hashtable);
    if (success)
    {
        num_misspelled = check_words(fptr, hashtable, misspelled);
    }
    
    printf("There are %d misspelled words", num_misspelled);
    
    return 0;
}
