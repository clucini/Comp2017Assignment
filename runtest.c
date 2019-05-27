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

int create_fil(){
    void * helper = init_fs("before/12_file_data", "before/12_directory_table", "before/12_hash_data", 4);
    char buf[] = "tests";
    int ret = write_file("file1.txt", 160, 50, buf, helper);
    ret = write_file("file1.txt", 16, 5, buf, helper);
    close_fs(helper);

}

int test_hash(){
    void * helper = init_fs("before/16_file_data", "before/16_directory_table", "before/16_hash_data", 4);
    uint8_t * data = (uint8_t*)malloc(sizeof(uint8_t) * 8);
    for(int i = 0; i < 8; i++){
        *(data + i) = i * 10;
    }
    fletcher(data, 20, NULL);

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
    
    // You can use the TEST macro as TEST(x) to run a test function named "x"
    //TEST(success);
    //TEST(failure);
    //TEST(no_operation);
    TEST(test_hash);
    // Add more tests here

    return 0;
}
