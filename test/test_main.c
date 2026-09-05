#include <stdio.h>
#include "framework/framework.h"

extern void run_vec_init_tests(void);
extern void run_vec_free_tests(void);
extern void run_vec_push_tests(void);
extern void run_vec_clear_tests(void);
extern void run_vec_at_tests(void);
extern void run_vec_remove_tests(void);
extern void run_vec_shrink_to_fit_tests(void);
extern void run_hashmap_init_tests(void);
extern void run_hashmap_put_tests(void);
extern void run_hashmap_get_tests(void);
extern void run_hashmap_rehash_tests(void);
extern void run_hashmap_remove_tests(void);
extern void run_hashmap_contains_tests(void);
extern void run_hashmap_free_tests(void);
extern void run_hashmap_hash_tests(void);
extern void run_cstr_free_tests(void);
extern void run_cstr_appendn_tests(void);
extern void run_cstr_at_tests(void);
extern void run_cstr_clear_tests(void);
extern void run_cstr_copy_tests(void);
extern void run_cstr_pop_tests(void);
extern void run_cstr_remove_tests(void);
extern void run_cstr_reverse_tests(void);
extern void run_cstr_substr_tests(void);
extern void run_cstr_shrink_to_fit_tests(void);
extern void run_trie_init_tests(void);
extern void run_trie_insert_tests(void);
extern void run_trie_search_tests(void);
extern void run_trie_match_prefix_tests(void);
extern void run_trie_free_child_tests(void);
extern void run_trie_free_tests(void);

int main(void)
{
    TEST_BEGIN();

    printf("\n" COLOR_YELLOW "---> Running Vec Initialization Tests..." COLOR_RESET "\n");
    run_vec_init_tests();

    printf("\n" COLOR_YELLOW "---> Running Vec Free Tests..." COLOR_RESET "\n");
    run_vec_free_tests();

    printf("\n" COLOR_YELLOW "---> Running Vec Push Tests..." COLOR_RESET "\n");
    run_vec_push_tests();

    printf("\n" COLOR_YELLOW "---> Running Vec Clear Tests..." COLOR_RESET "\n");
    run_vec_clear_tests();

    printf("\n" COLOR_YELLOW "---> Running Vec At Tests..." COLOR_RESET "\n");
    run_vec_at_tests();

    printf("\n" COLOR_YELLOW "---> Running Vec Remove Tests..." COLOR_RESET "\n");
    run_vec_remove_tests();

    printf("\n" COLOR_YELLOW "---> Running Vec Shrink To Fit Tests..." COLOR_RESET "\n");
    run_vec_shrink_to_fit_tests();

    printf("\n" COLOR_YELLOW "---> Running Hashmap Init Tests..." COLOR_RESET "\n");
    run_hashmap_init_tests();

    printf("\n" COLOR_YELLOW "---> Running Hashmap Put Tests..." COLOR_RESET "\n");
    run_hashmap_put_tests();

    printf("\n" COLOR_YELLOW "---> Running Hashmap Get Tests..." COLOR_RESET "\n");
    run_hashmap_get_tests();

    printf("\n" COLOR_YELLOW "---> Running Hashmap Rehash Tests..." COLOR_RESET "\n");
    run_hashmap_rehash_tests();

    printf("\n" COLOR_YELLOW "---> Running Hashmap Remove Tests..." COLOR_RESET "\n");
    run_hashmap_remove_tests();

    printf("\n" COLOR_YELLOW "---> Running Hashmap Contains Tests..." COLOR_RESET "\n");
    run_hashmap_contains_tests();

    printf("\n" COLOR_YELLOW "---> Running Hashmap Free Tests..." COLOR_RESET "\n");
    run_hashmap_free_tests();

    printf("\n" COLOR_YELLOW "---> Running Hashmap Hash Tests..." COLOR_RESET "\n");
    run_hashmap_hash_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr Free Tests..." COLOR_RESET "\n");
    run_cstr_free_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr AppendN Tests..." COLOR_RESET "\n");
    run_cstr_appendn_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr At Tests..." COLOR_RESET "\n");
    run_cstr_at_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr Clear Tests..." COLOR_RESET "\n");
    run_cstr_clear_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr Copy Tests..." COLOR_RESET "\n");
    run_cstr_copy_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr Pop Tests..." COLOR_RESET "\n");
    run_cstr_pop_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr Remove Tests..." COLOR_RESET "\n");
    run_cstr_remove_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr Reverse Tests..." COLOR_RESET "\n");
    run_cstr_reverse_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr Substr Tests..." COLOR_RESET "\n");
    run_cstr_substr_tests();

    printf("\n" COLOR_YELLOW "---> Running CStr Shrink To Fit Tests..." COLOR_RESET "\n");
    run_cstr_shrink_to_fit_tests();

    printf("\n" COLOR_YELLOW "---> Running Trie Init Tests..." COLOR_RESET "\n");
    run_trie_init_tests();

    printf("\n" COLOR_YELLOW "---> Running Trie Insert Tests..." COLOR_RESET "\n");
    run_trie_insert_tests();

    printf("\n" COLOR_YELLOW "---> Running Trie Search Tests..." COLOR_RESET "\n");
    run_trie_search_tests();

    printf("\n" COLOR_YELLOW "---> Running Trie Match Prefix Tests..." COLOR_RESET "\n");
    run_trie_match_prefix_tests();

    printf("\n" COLOR_YELLOW "---> Running Trie Free Child Tests..." COLOR_RESET "\n");
    run_trie_free_child_tests();

    printf("\n" COLOR_YELLOW "---> Running Trie Free Tests..." COLOR_RESET "\n");
    run_trie_free_tests();

    printf("\n" COLOR_CYAN "All test suites executed successfully." COLOR_RESET "\n");
    
    TEST_END();
    return 0;
}