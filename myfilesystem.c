#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include "myfilesystem.h"
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define BLOCK_SIZE 256
#define concat(a,b) a##b

pthread_mutex_t lock; 

/*  meta  
    A struct I use to simpify the process of accessing and modifying my directory_table.
    
    char name[64]: the name of the file at the current location.
    unsigned int offset: the start point of bytes belonging to this file inside file_data.
    unsigned int length: the number of bytes belonging to this file inside file_data.

*/
typedef struct metaData{
    char name[64];
    unsigned int offset;
    unsigned int length;
} meta;

/*  help
    A struct I use to help with various operations

    void* file_data:        a pointer to which I map to my file_data file.
    meta* files:            a pointer to which I map my directory_table file. Uses the meta struct shown above.
    uint8_t* hash_table:    a pointer to which I map my hash_data file. Uses uint8_t because that allows me to dereference, something which void * doesn't allow
    int n_processors:       the number of processors that the system has.
    int num_files:          the number of distinct files the system can hold.
    int fsize:              the size of file_data.
    int count_hash_blocks:  the number of hash blocks necessary to hash file_data.
    int hash_k:             the number of levels the merkle tree has. 
    size_t h_size           the size of hash_data.

*/
typedef struct Helper {
    void* file_data;
    meta* files;
    uint8_t* hash_table;
    int n_processors;
    int num_files;
    int fsize;
    int count_hash_blocks;
    int hash_k;
    size_t h_size;

} help;

/*  print_file 
    Prints important information about our file system.
    Used for debugging.

    ARGS(
       help * h:    a pointer to the helper, already cast to the help struct. 
    )

    RETURN: Nothing, is a void.
*/
void print_file(help * h){
    printf("Size: %d, Count: %d \n", h->fsize, h->num_files);
    for(int i = 0; i < h->num_files; i++) {
        meta * cur = (h->files) + i;
        printf("%s, %d, %d \n", cur->name, cur->offset, cur->length);
    }
}

/*  init_fs
    Initializes the entire file system, creates references to the 3 "real" files
    Also initializes various other useful things, mutexes and other helper variables
    All of this information is stored in a void *, can be cast by other functions to a help type, in order to read information

    ARGS(
        char * file_data: name of the file containing the data for our file system.
        char * directory_table: the name of the file containing the metadata for our file system.
        char * hash_data: the name of the file containing the hashes for the file data, in our file system.
        int n_processors: the number of processors our virtual system is running on. 
    )

    RETURN: (void*) A pointer to the helper variable is returned.
*/
void * init_fs(char * file_data, char * directory_table, char * hash_data, int n_processors) {
    help* h = (help*)malloc(sizeof(help));
    struct stat st;
    h->n_processors = n_processors;

    
    int dTable = open(directory_table, O_RDWR, S_IRWXG);
    stat(directory_table, &st);
    h->files = (meta*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, dTable, 0);
    
    h->num_files = st.st_size/sizeof(meta);

    
    int fileData = open(file_data, O_RDWR, S_IRWXG);
    stat(file_data, &st);
    h->file_data = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileData, 0);
    
    h->count_hash_blocks = st.st_size / 256;
    h->fsize = st.st_size;

    
    int hashTable = open(hash_data, O_RDWR, S_IRWXG);
    stat(hash_data, &st);
    h->hash_table = (uint8_t *)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED , hashTable, 0);
    
    h->h_size = st.st_size;
    int count = h->count_hash_blocks;
    h->hash_k = 0;
    while(count > 1){
        h->hash_k++;
        count = count / 2;
    }
    
    close(dTable);
    close(hashTable);
    close(fileData);

    pthread_mutex_init(&lock, NULL);
    if(0){              //Used for testing.
        print_file(h);
    }    
    return h;
}

/*  close_fs
    Closes our file system, frees all allocated memory, destorys all mutexes

    ARGS(
        void * helper: the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: No return, is a void.
*/
void close_fs(void * helper) {
    help* h = (help*)helper;
    munmap(h->files, h->num_files*sizeof(meta));
    munmap(h->hash_table, h->h_size);
    munmap(h->file_data, h->fsize);
    pthread_mutex_destroy(&lock);
    free(h);
}

/*  check_gap_after
    Checks the size of the gap in file_data immediately following a specific file. 

    ARGS(
        meta* item:     The item to check after.
        help * h:    a pointer to the helper, already cast to the help struct. 
    )
    
    RETURN: (size_t) the size of the gap. 
*/
size_t check_gap_after(meta* item, help * h){
    size_t s = h->fsize - (item->offset + item->length);
    for(int f = 0; f < h->num_files; f++){
        meta* incur = h->files + f;
        if (incur == item || item->offset > incur->offset)
            continue;
        if(incur->name[0] == '\0'){
            continue;
        }
        if (incur->offset - (item->offset + item->length) < s){
            s = incur->offset - (item->offset + item->length);
        }
    }
    return s;
}

/*  get_free_space
    Finds the total amount of free space avaliable in the system, excluding one item.
    Very useful for checking resizes.

    ARGS(
        help * h:       a pointer to the helper, already cast to the help struct. 
        meta* exclude:  the item to exclude
    )
    
    RETURN: (int) the amount of free space avaliable. 
*/
int get_free_space(help * h, meta * exclude){
    int total = 0;
    for(int i = 0; i < h->num_files; i++){
        meta* cur = (h->files + i);
        if(cur == exclude){
            if(exclude->offset == 0 || check_gap_after(exclude, h) + exclude->length + exclude->offset == h->fsize){
                total += check_gap_after(exclude, h) + exclude->length;
            }
            else {
                continue;
            }
        }
        else if((h->files + i)->name[0] == '\0') {
            continue;
        }
        else {
            total += check_gap_after(cur, h);
        }
    }
    return total;
}

/*  get_zero
    Checks if there is a file at offset 0. 

    ARGS(
        help * h:       a pointer to the helper, already cast to the help struct. 
    )
    
    RETURN: (int) 0 if there is none, 1 if there is. 
*/
int get_zero(help * h){
    for(int i = 0; i < h->num_files; i++){
        if((h->files + i)->offset == 0 && (h->files + i)->name[0] != '\0')
            return 1;
    }
    return 0;
}

/*  find_first_empty
    Finds the first empty slot in directory_table.

    ARGS(
        help * h:       a pointer to the helper, already cast to the help struct. 
    )
    
    RETURN: (int) the first empty slot in directory_table. 
*/
int find_first_empty(help * h) {
    for(int i = 0; i < h->num_files; i++) {
        meta * cur = (h->files + i);
        if(cur->name[0] == '\0' || (cur->length == 0 && cur->offset == 0)){
            return i;
        }   
    }
    return -1;
}

/*  find_gap
    Finds the first gap in file_data that is of sufficient size.

    ARGS(
        size_t length:  the minimum size of the gap
        help * h:       a pointer to the helper, already cast to the help struct. 
    )
    
    RETURN: (meta*) a file, after which the is enough room to place at least *length* bytes of data. Null if there is none. 
*/
meta* find_gap(size_t length, help * h) {
    meta* after = NULL;
    if(get_zero(h) == 0){
        int tmp = find_first_empty(h);
        (h->files + tmp)->name[0] = 't';
        int score = check_gap_after((h->files + tmp), h);
        (h->files + tmp)->name[0] = '\0';
        if(score >= length){
            return (h->files + tmp);
        }   
    }
    for(int i = 0; i < h->num_files; i++){
        meta* cur = (h->files + i);
        if(cur->name[0] == '\0') {
            continue;
        }
        if (check_gap_after(cur, h) >= length){
            return cur;
        }
    }
    return after;
}

/*  find_next
    Finds the file which is placed (in file_data) after a specified file. 

    ARGS(
        meta* cur:      the specified file.
        help * h:       a pointer to the helper, already cast to the help struct. 
    )
    
    RETURN: (meta*) the file which is following the specified file.
*/
meta * find_next(meta* cur, help * h){
    int o = -1;
    int l = INT32_MAX;
    meta * best = NULL;
    if(cur)
        o = cur->offset+cur->length;
    
    for(int i = 0; i < h->num_files; i++) {
        meta* incur = h->files + i;
        if(incur->name[0] == '\0' || incur == cur) {
            continue;
        }
        if(incur->offset < l && (int)incur->offset > (int)o) {
            l = incur->offset;
            best = incur;
        }
    }
    return best;
}

/*  find_file
    Finds the position of a file in directory name, given its name.

    ARGS(
        char* name:     the name of the file that it is looking for.
        help * h:       a pointer to the helper, already cast to the help struct. 
    )
    
    RETURN: (int) the offset from which the file can be access. E.G (h->files + find_file("test_file", h)). 
        -1 if it can't be found. 
*/
int find_file(char * name, help * h) {
    for(int i = 0; i < h->num_files; i++) {
        if(strcmp((h->files + i)->name, name)==0) {
            return i;
        }
    }
    return -1;
}

/*  compute_hash_blocks
    Computes all necesary hash blocks, given the start and end point of an edit.

    ARGS(
        int s_offset:   the offset, from the start of file_data, where the edit began.
        int end_offset: the offset, from the start of file_data, where the edit ended.
        void * helper:  the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: Nothing, is a void. 
*/
void compute_hash_blocks(int s_offset, int end_offset, void * helper){
    int num_blocks = floor((float)s_offset / 256.0f) - floor((float)(end_offset) / 256.0f)+1;   //Finds the number of blocks we need to recompute
    for(int i = 0; i < num_blocks; i++){
        compute_hash_block(floor((float)s_offset / 256.0f) + i, helper);                        //Recomputes the required blocks. 
    }
}

/*  write_meta
    Writes an entry into directory_table.
    Can overwrite existing placements.

    ARGS(
        meta* meta_to_write:    the entry to write into directory_table.
        int place:              the placement, in directory_table, for the entry. 
        help * h:               a pointer to the helper, already cast to the help struct. 
    )
    
    RETURN: Nothing, is a void.
*/
void write_meta(meta * meta_to_write, size_t place, help* h){
    memcpy(h->files + place, meta_to_write, sizeof(meta));
}

/*  remove_repack_replace
    Creates a file of a specified length, with a specified name.
    Stores the associated metadata in directory_table.
    Zeros out the space allocated to it in file_data.

    ARGS(
        char* filename:     the name of the file to create.
        size_t length:      the number of bytes the file needs allocated to it.   
        void * helper:  the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: (int), 
        0 if successful, 
        1 if a file with that name already exists, 
        2 if there is not enough room for this file.
*/
void remove_repack_replace(meta* f, size_t length, void * helper){
    help* h = (help*)helper;
    void * temp = calloc(length, 1);
    char t_letter = f->name[0];

    memcpy(temp, h->file_data + f->offset, f->length);

    memset(h->file_data + f->offset, 0, f->length);
    f->name[0] = '\0';

    repack(h);

    f->length = length;
    meta * after = find_gap(f->length, h);
    f->name[0] = t_letter;
    f->offset = after->offset + after->length;
    
    memcpy(h->file_data + f->offset, temp, f->length);
    free(temp);
}

/*  create_file
    Creates a file of a specified length, with a specified name.
    Stores the associated metadata in directory_table.
    Zeros out the space allocated to it in file_data.

    ARGS(
        char* filename:     the name of the file to create.
        size_t length:      the number of bytes the file needs allocated to it.   
        void * helper:  the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: (int), 
        0 if successful,
        1 if a file with that name already exists, 
        2 if there is not enough room for this file.
*/
int create_file(char * filename, size_t length, void * helper) {

    help* h = (help*)helper;

    if(find_file(filename, h) != -1) {
        return 1;       //If the filename is taken.
    }

    unsigned int new_offset = 0; 
    meta* after = find_gap(length, helper);
    if(after == NULL) {     //If there isn't a gap big enough for this file, attempt to repack and retry.
        if(get_free_space(h, NULL) >= length) {         //Checks if there is enough free space for this file.
            repack(helper);
            meta* after = find_gap(length, helper);
            new_offset = after->offset + after->length;
        } else {
            return 2;       //If there's not enough free space
        }
    }
    else {
        new_offset = after->offset + after->length;
    }

    int placement = find_first_empty(h);

    meta* new = (meta*)malloc(sizeof(meta));
    strncpy(new->name, filename, 64);
    new->length = length;
    new->offset = new_offset;

    write_meta(new, placement, h);    
    memset(h->file_data + new->offset, 0, new->length);

    compute_hash_tree(helper);
    free(new);

    return 0;
}

/*  resize_file
    Resizes a specified file, to a new specified length, if possible. 

    ARGS(
        char* filename:     the name of the file to resize.
        size_t length:      the number of bytes the file would like allocated to it. 
        void * helper:      the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: (int), 
        0 if successful,
        1 if a file with that name doesn't exist, 
        2 if there is not enough room for this operation.
*/
int resize_file(char * filename, size_t length, void * helper) {

    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x == -1)
        return 1;

    meta * f = (h->files + x);
    if(length < f->length) {
        f->length = length;
        write_meta(f, x, h);
    }
    else if(check_gap_after(f, h) + f->length >= length){
        f->length = length;
        write_meta(f, x, h);
    }
    else if(get_free_space(h, f) >= length) {
        remove_repack_replace(f, length, helper);
    }
    else {
        return 2;
    }
    compute_hash_tree(helper);

    return 0;
}

/*  repack
    Resizes a specified file, to a new specified length, if possible. 

    ARGS(
        char* filename:     the name of the file to resize.
        size_t length:      the number of bytes the file would like allocated to it. 
        void * helper:      the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: (int), 
        0 if successful, 
        1 if a file with that name doesn't exist, 
        2 if there is not enough room for this operation.
*/
void repack(void * helper) {
    help * h = (help *)helper;
    meta * curn = find_next(NULL, h);
    int cur = 0;
    while(curn) {
        void * storage = malloc(curn->length);
        memcpy(storage, h->file_data + curn->offset, curn->length);

        memset(h->file_data + curn->offset, 0, curn->length);

        curn->offset = cur;

        memcpy(h->file_data + curn->offset, storage, curn->length);
        free(storage);
        
        write_meta(curn, find_file(curn->name, h), h);
        
        cur = curn->offset + curn->length;
        curn = find_next(curn, h);

    }        
    compute_hash_tree(helper);
}

/*  delete_file
    Deletes a specified file. 

    ARGS(
        char* filename:     the name of the file to delete.
        void * helper:      the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: (int), 
        0 if successful, 
        1 if a file with that name doesn't exist.
*/
int delete_file(char * filename, void * helper) {
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x==-1){
        return 1;
    }
    (h->files + x)->name[0] = '\0';
    return 0;
}

/*  rename_file
    Renames a specified file, if possible.

    ARGS(
        char* filename:     the name of the file to rename.
        char* filename:     the new name of the file.
        void * helper:      the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: (int), 
        0 if successful, 
        1 if a file with that name doesn't exist, or if the new name is already taken.
*/
int rename_file(char * oldname, char * newname, void * helper) {
    help * h = (help *)helper;
    int x = find_file(oldname, h);
    if(x==-1 || find_file(newname, h) != -1)
        return 1;
    meta * f = h->files + x;
    
    strncpy(f->name, newname, 64);
    return 0;
}

/*  read_file
    Reads specified bytes from a file, into a buffer provided.

    ARGS(
        char* filename:     the name of the file to read from.
        size_t offset:      the offset from the start of the file, from which we would like to begin reading.
        size_t count:       the number of bytes we would like to read.
        void* buf:          the buffer we copy the read data into.
        void * helper:      the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: (int),
        0 if successful, 
        1 if a file with that name doesn't exist, 
        2 if you're trying to read outside of the file.
*/
int read_file(char * filename, size_t offset, size_t count, void * buf, void * helper) {
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x==-1)
        return 1;
    
    meta * f = h->files + x;
    if(offset+count > f->length)
        return 2;

    void * b = malloc(h->h_size);
    memcpy(b, h->hash_table, h->h_size);

    compute_hash_tree(helper);
    if(memcmp(b, h->hash_table, h->h_size) != 0){
        free(b);
        return 3;
    }
    
    free(b);

    memcpy(buf, h->file_data + f->offset + offset, count);
    return 0;
}

/*  write_file
    writes specified bytes into a file, from a buffer provided.

    ARGS(
        char* filename:     the name of the file to write into.
        size_t offset:      the offset from the start of the file, from which we would like to begin writing.
        size_t count:       the number of bytes we would like to write.
        void* buf:          the buffer we read bytes from, to write into our file.
        void * helper:      the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: (int), 
        0 if successful, 
        1 if a file with that name doesn't exist, 
        2 if you're trying to write outside of the file, 
        3 if you're trying to write more data than there is room left avaliable on disk.
*/
int write_file(char * filename, size_t offset, size_t count, void * buf, void * helper) {

    pthread_mutex_lock(&lock);
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x==-1){
        pthread_mutex_unlock(&lock);
        return 1;
    }

    meta * f = h->files + x;
    

    if(offset > f->length){
        pthread_mutex_unlock(&lock);
        return 2;
    }

    if(offset+count > f->length){
        if(get_free_space(h, NULL) < count){
            pthread_mutex_unlock(&lock);
            return 3;
        }
        else{
            remove_repack_replace(f, offset + count, helper);
        }
    }
    
    memcpy(h->file_data + f->offset + offset, buf, count);
    
    //compute_hash_blocks(f->offset, f->offset + f->length, helper);
    compute_hash_tree(helper);      //Only because of remove repack replace, should probs fix later
    pthread_mutex_unlock(&lock);

    return 0;
}

/*  file_size
    returns the size of a file, given the file's name

    ARGS(
        char* filename:     the name of the file to check the size of.
    )
    
    RETURN: (ssize_t), the size of the file, or -1 if the file does not exist
*/
ssize_t file_size(char * filename, void * helper) {
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x==-1)
        return -1;
    meta * f = h->files + x;
    return f->length;
}

/*  fletcher
    runs the fletcher hashing algorithm, and outputs the result into output.

    ARGS(
        uint8_t * buf:      the input, all of the data which needs to be hashed.
        size_t length:      the amount of data in bytes, that needs to be processed, from buf
        uint8_t * output:   the buffer to copy the result of the hash into.
    )
    
    RETURN: Nothing, is a void.
*/
void fletcher(uint8_t * buf, size_t length, uint8_t * output) {
    uint64_t ints[4] = {0};
    
    uint32_t* b = (uint32_t*)buf;

    for(int i = 0; i < length/4; ++i) {
        ints[0] = ((ints[0] + *(b + i)) % UINT32_MAX);
        ints[1] = ((ints[1] + ints[0])  % UINT32_MAX);
        ints[2] = ((ints[2] + ints[1]) % UINT32_MAX);
        ints[3] = ((ints[3] + ints[2]) % UINT32_MAX);
    }
    
    uint32_t* hash_value = (uint32_t*)malloc(sizeof(uint32_t) * 4);
    
    for(int i = 0; i < 4; i++) {
        memcpy(hash_value + i, (uint32_t*)(ints + i), sizeof(uint32_t));
    }

    memcpy(output, hash_value, sizeof(uint32_t) * 4);
    free(hash_value);
}

/*  compute_hash_tree
    Computes the entire merkle hash tree for our file_data.
    Essentially just calls compute_hash_block on every block in our file.

    ARGS(
        void * helper:      the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: Nothing, is a void.
*/
void compute_hash_tree(void * helper) {
    help* h = (help*)helper;
    for(int i = 0; i < h->count_hash_blocks; i++){
        compute_hash_block(i, helper);
    }
}

/*  compute_hash_tree
    Computes the hash for the block given, and recursively hashes all affected blocks up the merkle hash tree, to the root.

    ARGS(
        size_t block_offset:    the block in file_data to hash.
        void * helper:          the helper variable, contains information about our files, as well as pointers to the actual data 
    )
    
    RETURN: Nothing, is a void.
*/
void compute_hash_block(size_t block_offset, void * helper) {
    
    help * h = (help*)helper;
    
    uint8_t * space = malloc(sizeof(uint8_t) * 16);

    fletcher((uint8_t*)(h->file_data + 256 * block_offset), 256, space);

    int placement = ((pow(2, h->hash_k)-1) + block_offset) * 16;

    memcpy((h->hash_table + placement), space, sizeof(uint8_t) * 16);

    if(block_offset % 2 == 1){
        block_offset--;
    }

    int blocksize = sizeof(uint8_t) * 16;

    int copyfrom = (pow(2, h->hash_k)-1) + block_offset;
    int copyto = (copyfrom -1) / 2;
    while(copyto >= 0) {
        fletcher((uint8_t*)(h->hash_table + copyfrom*blocksize), blocksize * 2, space);
        memcpy((h->hash_table + copyto * blocksize), space, blocksize);
        if(copyto == 0)
            break;
        if(copyto % 2 == 0)
            copyto -= 1;
        copyfrom = copyto;
        copyto = (copyto-1)/2;
    }

    free(space);
}
