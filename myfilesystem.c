#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include "myfilesystem.h"
#include <math.h>
#define BLOCK_SIZE 256
#define concat(a,b) a##b


typedef struct metaData{
    char name[64];
    unsigned int offset;
    unsigned int length;
} meta;

typedef struct Helper {
    FILE* file_ptrs[3];
    int n_processors;
    int count;
    meta* files;
    int fsize;

} help;

void print_file(help * h){
    printf("Size: %d, Count: %d \n", h->fsize, h->count);
    for(int i = 0; i < h->count; i++) {
        meta * cur = (h->files) + i;
        printf("%s, %d, %d \n", cur->name, cur->offset, cur->length);
    }
}

void * init_fs(char * file_data, char * directory_table, char * hash_data, int n_processors) {
    help* h = (help*)malloc(sizeof(help));

    h->file_ptrs[0] = fopen(file_data, "rb+");
    h->file_ptrs[1] = fopen(directory_table, "rb+");
    h->file_ptrs[2] = fopen(hash_data, "rb+");
 
    h->n_processors = n_processors;
    fseek(h->file_ptrs[1], 0, SEEK_END);
    h->count = ftell(h->file_ptrs[1])/sizeof(meta);

    unsigned int v = ftell(h->file_ptrs[1]);

    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    h->fsize = v;

    fseek(h->file_ptrs[1], 0, SEEK_SET);


    void * space = calloc(h->count, sizeof(meta));
    fread(space, sizeof(meta), h->count, h->file_ptrs[1]);

    h->files = ((meta*)space);

    if(0){
        print_file(h);
    }    

    return h;
}


void close_fs(void * hv) {
    help* h = (help*)hv;
    fclose(h->file_ptrs[0]);
    fclose(h->file_ptrs[1]);
    fclose(h->file_ptrs[2]);
    free(h->files);
    free(h);
    return;
}


size_t check_gap_after(meta* item, help * h){
    size_t s = h->fsize - item->offset;
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
            continue;
        }
        if((h->files + i)->name[0] == '\0') {
            continue;
        }
        else{
            total += check_gap_after(cur, h);
        }
    }
    return total;
}

meta* find_gap(size_t length, help * h, FILE* dtable) {
    const int count = h->count;
    meta* after = NULL;
    for(int i = 0; i < count; i++){
        meta* cur = (h->files + i);
        if (check_gap_after(cur, h) >= length){
            return cur;
        }
    }
    return after;
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

int find_file(char * name, help * h) {
    for(int i = 0; i < h->count; i++) {
        if(strcmp((h->files + i) ->name, name)==0) {
            return i;
        }
    }
    return -1;
}

int create_file(char * filename, size_t length, void * helper) {
    help* h = (help*)helper;
    FILE* dtable = h->file_ptrs[1];

    if(find_file(filename, h) != -1){
        return 1;
    }

    meta* new = (meta*)malloc(sizeof(meta));

    printf("%s, %ld asasdasd\n", filename, length);

    unsigned int noffset = 0; 
    if(h->count != 0) {
        meta* after = find_gap(length, helper, dtable);
        print_file(h);
        if(after == NULL) {
            if(get_free_space(h, NULL) >= length){
                repack(helper);
                meta* after = find_gap(length, helper, dtable);
                noffset = after->offset + after->length;
            }
        }
        else {
            noffset = after->offset + after->length;
        }
    }

    int placement = find_first_empty(h);


    strncpy(new->name, filename,64);
    new->length = length;
    new->offset = noffset;
    fseek(dtable, 0, placement * sizeof(meta));

    fwrite(new, sizeof(meta), 1, dtable);

    memcpy(h->files+placement, new, sizeof(meta));

    free(new);
    print_file(h);

    return 0;
}

int resize_file(char * filename, size_t length, void * helper) {
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x == -1)
        return 0;
    meta * f = (h->files + x);
    if(length < f->length){    
        f->length = length;
        fseek((h->file_ptrs[1]), 0, x * sizeof(meta));
        fwrite(f, sizeof(meta), 1, h->file_ptrs[1]);
    }
    else if(check_gap_after(f, h) >= length){
        f->length = length;
        fseek((h->file_ptrs[1]), 0, x * sizeof(meta));
        fwrite(f, sizeof(meta), 1, h->file_ptrs[1]);
    }
    else if(get_free_space(h, f) >= length) {
        f->length = length;

        repack(h);
    }
    else {
        printf("%d", get_free_space(h, f));
        return 2;
    }
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

void repack(void * helper, meta * end) {
    help * h = (help *)helper;
    meta * curn = find_next(NULL, h);
    void * s = malloc(end->length);
    fseek(h->file_ptrs[0], end->offset, SEEK_SET);
    fread(s, curn->length, 1, h->file_ptrs[0]);

    int cur = 0;
    while(curn) {
        void * storage = malloc(curn->length);
        fseek(h->file_ptrs[0], curn->offset, SEEK_SET);
        fread(storage, curn->length, 1, h->file_ptrs[0]);
        fseek(h->file_ptrs[0], cur, SEEK_SET);
        fwrite(storage, curn->length, 1, h->file_ptrs[0]);
        free(storage);
        curn->offset = cur;
        fseek((h->file_ptrs[1]), find_file(curn->name, h) * sizeof(meta), SEEK_SET);
        fwrite(curn, sizeof(meta), 1, h->file_ptrs[1]);
        cur = curn->offset + curn->length;
        curn = find_next(curn, h);
        if(curn = end){
            curn = find_next(curn, h);
        }
    }
    fseek(h->file_ptrs[0], cur, SEEK_SET);
    fwrite(s, curn->length, 1, h->file_ptrs[0]);
        
}

int delete_file(char * filename, void * helper) {
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x==-1){
        return 1;
    }
    (h->files + x)->name[0] = '\0';
    fseek((h->file_ptrs[1]), x * sizeof(meta), SEEK_SET);
    fputc('\0', h->file_ptrs[1]);
    return 0;
}

int rename_file(char * oldname, char * newname, void * helper) {
    help * h = (help *)helper;
    int x = find_file(oldname, h);
    if(x==-1 || find_file(newname, h) != -1)
        return 1;
    meta * f = h->files + x;
    
    strncpy(f->name, newname, 64);
    fseek((h->file_ptrs[1]), x * sizeof(meta), SEEK_SET);
    fwrite(f, sizeof(meta), 1, h->file_ptrs[1]);
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

    fseek((h->file_ptrs[0]), f->offset + offset, SEEK_SET);
    fread(buf, 1, count, (h->file_ptrs[0]));
    return 0;
}

int write_file(char * filename, size_t offset, size_t count, void * buf, void * helper) {
    help * h = (help *)helper;
    int x = find_file(filename, h);
    if(x==-1)
        return 1;
        
    meta * f = h->files + x;
    
    if(offset > f->length)
        return 2;

    if(offset+count > f->length)
        if(resize_file(filename, offset + count, helper) == 2)
            return 3;
    
    fseek((h->file_ptrs[0]), f->offset + offset, SEEK_SET);
    fwrite(buf, 1, count, h->file_ptrs[0]);
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

    for(int i = 0; i < 1; ++i) {
        printf("%n", b + i);
        ints[0] = (ints[0] + (*b + i)) % ((2^32)-1);
        ints[1] = (ints[1] + ints[0]) % ((2^32)-1);
        ints[2] = (ints[2] + ints[1]) % ((2^32)-1);
        ints[3] = (ints[3] + ints[2]) % ((2^32)-1);
    }
    
    uint8_t hash_value[16];

    for(int i = 0; i < 4; i++) {
        for(int f = 0; f < 4; f++) {
            hash_value[i * 4 + f] = ((uint8_t*)ints)[i * 4 + f + 3];
        }
    }
    printf("%d", hash_value[0]);

}

/*
function fletcher_hash(uint32_t * data, length): //Consider data 4 bytes at a time
    uint32_t a = 0; //Treat 4 byte blocks as little-endian integers
    uint32_t b = 0;
    uint32_t c = 0;
    uint32_t d = 0;
    for i = 0 ... length-1:
        a = (a + data[i]) mod 2^32-1;
        b = (b + a) mod 2^32-1;
        c = (c + b) mod 2^32-1;
        d = (d + c) mod 2^32-1;
    uint8_t hash_value[16] = concatenate a, b, c, d //4x4 = 16 bytes in total
    return hash_value
*/

void compute_hash_tree(void * helper) {
    return;
}

void compute_hash_block(size_t block_offset, void * helper) {
    return;
}
