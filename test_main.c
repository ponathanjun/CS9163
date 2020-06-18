#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    // Here we can test if certain words ended up in certain buckets
    // to ensure that our load_dictionary works as intended. I leave
    // this as an exercise.
    char* strings[4];
    strings[0] = "first";
    strings[1] = "second";
    strings[2] = "third";
    strings[3] = "test";
    int expected[4];
    for (int i = 0; i < 4; i++) {
        expected[i] = hash_function(strings[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        int bucket = expected[i];
        ck_assert_msg(strcmp(strings[i], hashtable[bucket]->word) == 0);
    }
}
END_TEST

// Check if length maximum boundary works
START_TEST(test_dictionary_boundary)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    char* boundary_string = "pneumonoultramicroscopicsilicovolcanoconiosiss";
    char* max_string = "pneumonoultramicroscopicsilicovolcanoconiosis";
    int boundary_bucket = hash_function(boundary_string);
    int max_bucket = hash_function(max_string);
    ck_assert(hashtable[boundary_bucket] == NULL);
    ck_assert_msg(strcmp(max_string, hashtable[max_bucket]->word) == 0);
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
}
END_TEST

// Test here: What if a word begins and ends with "? (My program filters out punctuation in check_words, thus, any word with a punctuation here should return false)
START_TEST(test_check_word_punctuation)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* beg_word = "?dog";
    const char* end_word = "fish?";
    const char* both_word = "?apple?";
    const char* correct_word = "apple";
    ck_assert(!check_word(beg_word, hashtable));
    ck_assert(!check_word(end_word, hashtable));
    ck_assert(!check_word(both_word, hashtable));
    ck_assert(check_word(correct_word, hashtable));
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

// Check if length maximum boundary works
START_TEST(test_check_words_boundary)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test2.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 2);
    char* expected[2];
    expected[0] = "and";
    expected[1] = "thisisafakewordthatimusingtotestsomeofmycases";
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    bool test2 = strlen(misspelled[1]) == strlen(expected[1]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    int len3 = strlen(misspelled[0]);
    int len4 = strlen(expected[0]);
    ck_assert_msg(test2, "%d!=%d", len3, len4);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
}
END_TEST

// Check number and punctuation cases
START_TEST(test_check_words_weird)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test3.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 6);
    char* expected[6];
    expected[0] = "32dog";
    expected[1] = "fish32";
    expected[2] = "54cat54";
    expected[3] = "50baseball";
    expected[4] = "football50";
    expected[5] = "50basketball50";
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    bool test2 = strlen(misspelled[1]) == strlen(expected[1]);
    bool test3 = strlen(misspelled[2]) == strlen(expected[2]);
    bool test4 = strlen(misspelled[3]) == strlen(expected[3]);
    bool test5 = strlen(misspelled[4]) == strlen(expected[4]);
    bool test6 = strlen(misspelled[5]) == strlen(expected[5]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    int len3 = strlen(misspelled[0]);
    int len4 = strlen(expected[0]);
    ck_assert_msg(test2, "%d!=%d", len3, len4);
    int len5 = strlen(misspelled[1]);
    int len6 = strlen(expected[1]);
    ck_assert_msg(test3, "%d!=%d", len5, len6);
    int len7 = strlen(misspelled[2]);
    int len8 = strlen(expected[2]);
    ck_assert_msg(test4, "%d!=%d", len7, len8);
    int len9 = strlen(misspelled[3]);
    int len10 = strlen(expected[4]);
    ck_assert_msg(test5, "%d!=%d", len9, len10);
    int len11 = strlen(misspelled[5]);
    int len12 = strlen(expected[5]);
    ck_assert_msg(test6, "%d!=%d", len11, len12);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
    ck_assert_msg(strcmp(misspelled[3], expected[3]) == 0);
    ck_assert_msg(strcmp(misspelled[4], expected[4]) == 0);
    ck_assert_msg(strcmp(misspelled[5], expected[5]) == 0);
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_dictionary_boundary);
    tcase_add_test(check_word_case, test_check_word_punctuation);
    tcase_add_test(check_word_case, test_check_words_boundary);
    tcase_add_test(check_word_case, test_check_words_weird);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

