#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include "myfilesystem.h"
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>



#define BLOCK_SIZE 256
#define concat(a,b) a##b

pthread_mutex_t lock; 


typedef struct metaData{
    char name[64];
    unsigned int offset;
    unsigned int length;
} meta;

typedef struct Helper {
    void* file_data;
    int n_processors;
    int count;
    meta* files;
    uint8_t* hash_table;
    int fsize;
    int count_hash_blocks;
    int hash_k;
    size_t h_size;

} help;

void print_file(help * h){
    printf("Size: %d, Count: %d \n", h->fsize, h->count);
    for(int i = 0; i < h->count; i++) {
        meta * cur = (h->files) + i;
        printf("%s, %d, %d \n", cur->name, cur->offset, cur->length);
    }
}

char* ls(void * helper){
    help* h = (help*)helper;
    for(int i = 0; i < h->count; i++)
        if((h->files + i)->name[0] != '\0')
            return ((h->files + i)->name);
    return "No Files";
}

/*  NAME_OF_FUNCTION
*   WHAT IT DOES
*   ARGS(TYPE, NAME: DESCRIPTION)
*   RETURN VALUE
*/

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

    A void * pointer to the helper variable is returned.
*/


void * init_fs(char * file_data, char * directory_table, char * hash_data, int n_processors) {
    help* h = (help*)malloc(sizeof(help));
    struct stat st;
    h->n_processors = n_processors;

    
    int dTable = open(directory_table, O_RDWR, S_IRWXG);
    stat(directory_table, &st);
    h->files = (meta*)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, dTable, 0);
    
    h->count = st.st_size/sizeof(meta);

    
    int fileData = open(file_data, O_RDWR, S_IRWXG);
    stat(file_data, &st);
    h->file_data = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileData, 0);
    
    h->count_hash_blocks = st.st_size / 256;
    h->fsize = st.st_size;

    
    int hashTable = open(hash_data, O_RDWR, S_IRWXG);
    stat(hash_data, &st);
    h->hash_table = (uint8_t *)mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, hashTable, 0);
    
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
    if(0){
        print_file(h);
    }    
    return h;
}

/*  close_fs
    Closes our fle system, frees all allocated memory, destorys all mutexes

    ARGS(
        void * helper: Our helper variable, contains information about our files, as well as pointers to 
    )
    RETURN VALUE
*/

void close_fs(void * helper) {
    help* h = (help*)helper;
    munmap(h->files, h->count*sizeof(meta));
    munmap(h->hash_table, h->h_size);
    munmap(h->file_data, h->fsize);
    pthread_mutex_destroy(&lock);
    free(h);
}


size_t check_gap_after(meta* item, help * h){
    size_t s = h->fsize - (item->offset + item->length);
    for(int f = 0; f < h->count; f++){
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

int get_free_space(help * h, meta * exclude){
    int total = 0;
    for(int i = 0; i < h->count; i++){
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

int get_zero(help * h){
    for(int i = 0; i < h->count; i++){
        if((h->files + i)->offset == 0 && (h->files + i)->name[0] != '\0')
            return 1;
    }
    return 0;
}



int find_first_empty(help * h) {
    for(int i = 0; i < h->count; i++) {
        meta * cur = (h->files + i);
        if(cur->name[0] == '\0' || (cur->length == 0 && cur->offset == 0)){
            return i;
        }   
    }
    return -1;
}

meta* find_gap(size_t length, help * h) {
    const int count = h->count;
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
    for(int i = 0; i < count; i++){
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

int find_file(char * name, help * h) {
    for(int i = 0; i < h->count; i++) {
        if(strcmp((h->files + i) ->name, name)==0) {
            return i;
        }
    }
    return -1;
}

void compute_hash_blocks(int s_offset, int end_offset, void * helper){
    int num_blocks = floor((float)s_offset / 256.0f) - floor((float)(end_offset) / 256.0f)+1;
    for(int i = 0; i < num_blocks; i++){
        compute_hash_block(floor((float)s_offset / 256.0f) + i, helper);
    }
}

void write_meta(meta * m, size_t place, help* h){
    memcpy(h->files + place, m, sizeof(meta));
}

int create_file(char * filename, size_t length, void * helper) {

    help* h = (help*)helper;

    if(find_file(filename, h) != -1){
        return 1;
    }

    unsigned int noffset = 0; 
    if(h->count != 0) {
        meta* after = find_gap(length, helper);
        if(after == NULL) {
            if(get_free_space(h, NULL) >= length){
                repack(helper);
                meta* after = find_gap(length, helper);
                noffset = after->offset + after->length;
            }
        }
        else {
            noffset = after->offset + after->length;
        }
    }

    int placement = find_first_empty(h);

    meta* new = (meta*)malloc(sizeof(meta));
        
    strncpy(new->name, filename, 64);
    new->length = length;
    new->offset = noffset;


    void * empty = calloc(1, new->length);

    write_meta(new, placement, h);
    
    write_file(new->name, 0, new->length, empty, helper);    

    compute_hash_tree(helper);
    //compute_hash_blocks(new->offset, new->offset + new->length, helper);
    free(empty);
    free(new);


    return 0;
}

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

int resize_file(char * filename, size_t length, void * helper) {

    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x == -1)
        return 0;

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

meta * find_next(meta* cur, help * h){
    int o = -1;
    int l = INT32_MAX;
    meta * best = NULL;
    if(cur)
        o = cur->offset+cur->length;
    
    for(int i = 0; i < h->count; i++) {
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

int delete_file(char * filename, void * helper) {
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x==-1){
        return 1;
    }
    (h->files + x)->name[0] = '\0';
    return 0;
}

int rename_file(char * oldname, char * newname, void * helper) {
    help * h = (help *)helper;
    int x = find_file(oldname, h);
    if(x==-1 || find_file(newname, h) != -1)
        return 1;
    meta * f = h->files + x;
    
    strncpy(f->name, newname, 64);
    return 0;
}

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

    /*int start_offset = floor((float)f->offset / 256.0f);
    int num_blocks = (floor((float)(f->offset + f->length) / 256.0f) - start_offset) + 1;
    uint8_t* b = (uint8_t*)malloc(16);
    printf("%d", num_blocks);
    for(int i = 0; i < num_blocks; i++){
        for(int f = 0; f )
        fletcher((uint8_t*)(h->file_data + (start_offset + i) * 256), 256, b);
        
        if(memcmp(b, h->hash_table + hash_block_placement, 16) != 0){
            free(b);
            return 3;
        }
    }*/
    free(b);

    memcpy(buf, h->file_data + f->offset + offset, count);
    return 0;
}

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

ssize_t file_size(char * filename, void * helper) {
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x==-1)
        return -1;
    meta * f = h->files + x;
    return f->length;
}

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


void compute_hash_tree(void * helper) {
    help* h = (help*)helper;
    for(int i = 0; i < h->count_hash_blocks; i++){
        compute_hash_block(i, helper);
    }
}

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
