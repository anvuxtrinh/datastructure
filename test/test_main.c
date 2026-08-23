#include <stdio.h>
#include "framework/framework.h"

extern void run_vec_init_tests(void);
extern void run_vec_free_tests(void);
extern void run_vec_push_tests(void);
extern void run_vec_clear_tests(void);
extern void run_vec_at_tests(void);
extern void run_vec_remove_tests(void);

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

    printf("\n" COLOR_CYAN "All test suites executed successfully." COLOR_RESET "\n");
    
    TEST_END();
    return 0;
}