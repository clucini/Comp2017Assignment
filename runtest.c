#include <stdio.h>

#define TEST(x) test(x, #x)
#include "myfilesystem.c"

/* You are free to modify any part of this file. The only requirement is that when it is run, all your tests are automatically executed */

/* Some example unit test functions */
int success() {
    return 0;
}

int failure() {
    return 1;
}

int no_operation() {
    void * helper = init_fs("file1", "file2", "file3", 1); // Remember you need to provide your own test files and also check their contents as part of testing
    close_fs(helper);
    return 0;
}

int test_write_lots(){
    void * helper = init_fs("test_files/file_data", "test_files/file_data", "test_files/file_data", 4);
    char buf[] = "tests";
    create_file("file1.txt", 1, helper);
    create_file("file2.txt", 1, helper);
    for(int i = 0; i < 10000; i++){
        write_file("file1.txt", 0, i % 32, buf, helper);
        write_file("file2.txt", 0, i % 32, buf, helper);
    }
    close_fs(helper);
    
}


/****************************/

/* Helper function */
void test(int (*test_function) (), char * function_name) {
    int ret = test_function();
    if (ret == 0) {
        printf("Passed %s\n", function_name);
    } else {
        printf("Failed %s returned %d\n", function_name, ret);
    }
}
/************************/


int main(int argc, char * argv[]) {
    
    TEST(no_operation);
    TEST(test_write_lots);
    // Add more tests here

    return 0;
}
