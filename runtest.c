#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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

int test_create_file(){
    void * helper = init_fs("file_data", "directory_table", "hash_data", 4);
    int ret = create_file("file_1.txt", 1, helper); 
    close_fs(helper);
    return ret;
}

int test_create_file_exists(){
    void * helper = init_fs("file_data", "directory_table", "hash_data", 4);
    int ret = create_file("file_1.txt", 1, helper); 
    ret = create_file("file_1.txt", 1, helper); 
    close_fs(helper);
    if(ret != 1)
        return 1;
    return 0;
}


int test_create_resize_delete(){
    void * helper = init_fs("file_data", "directory_table", "hash_data", 4);
    int ret = create_file("file_1.txt", 1, helper);  
    if(ret != 0)
        return 1;
    ret = resize_file("file_1.txt", 16, helper);
    if(ret != 0)
        return 1;
    ret = delete_file("file_1.txt", helper);
    if(ret != 0)
        return 1;

    close_fs(helper);
    return 0;
}

int test_read_write(){
    void * helper = init_fs("file_data", "directory_table", "hash_data", 4);
    int ret = create_file("file_1.txt", 1, helper); 
    if(ret != 0)
        return 1;  
    
    char * b = malloc(sizeof(char) * 10);
    strcpy(b, "thisisthin");

    ret = write_file("file_1.txt", 0 ,10, b, helper);
    if(ret != 0)
        return 1;  

    memset(b, 0, 10);

    ret = read_file("file_1.txt", 0 ,10, b, helper);
    if(ret != 0)
        return 1;

    close_fs(helper);
    return strcmp(b, "thisisthin");   //If this isn't 0, it'll fail the testcase
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


/***********************
IMPORTANT: if a difference ever appears while running the below test cases, the underlying data has failed.
***********************/


int main(int argc, char * argv[]) {
    
    chdir("test_files/");
    system("./reset.sh");
    TEST(test_create_file);
    system("./test_create_file.sh");
    system("./reset.sh");
    TEST(test_create_file_exists);
    system("./test_create_file_exists.sh");    
    system("./reset.sh");
    TEST(test_create_resize_delete);
    system("./test_create_resize_delete.sh");    
    system("./reset.sh");
    TEST(test_read_write);
    system("./test_read_write.sh");    
    system("./reset.sh");
    return 0;
}