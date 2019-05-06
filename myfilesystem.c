#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include "myfilesystem.h"


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

} help;

void * init_fs(char * file_data, char * directory_table, char * hash_data, int n_processors) {
    help* h = (help*)malloc(sizeof(help));

    h->file_ptrs[0] = fopen(file_data, "wb+");
    h->file_ptrs[1] = fopen(directory_table, "rb+");
    h->file_ptrs[2] = fopen(hash_data, "wb+");

    h->n_processors = n_processors;
    fseek(h->file_ptrs[1], 0, SEEK_END);
    h->count = ftell(h->file_ptrs[1])/sizeof(meta);
    fseek(h->file_ptrs[1], 0, 0);


    void * space = calloc(h->count, sizeof(meta));
    fread(space, sizeof(meta), h->count, h->file_ptrs[1]);

    printf("%s", ((meta *)space)->name);

    h->files = ((meta*)space);
    printf("%s", (h->files+1)->name);

    return h;
}

void close_fs(void * hv) {
    help* h = (help*)hv;
    fclose(h->file_ptrs[0]);
    fclose(h->file_ptrs[1]);
    fclose(h->file_ptrs[2]);
    //free(h->files);
    free(h);
    return;
}

bool check_gap_after(meta* item, meta* all, size_t length, int total){
    for(int f = 0; f < total; f++){
        meta* incur = all + f;
        if (incur == item|| item->offset < incur->offset)
            continue;
        if (incur->offset - (item->offset + item->length) < length){
            return false;
        }
    }
    return true;
}

meta* find_gap(size_t length, help * h, FILE* dtable){
    unsigned long m = sizeof(meta);
    const int count = h->count;
    meta* after = NULL;
    for(int i = 0; i < count; i++){
        meta* cur = (h->files + i);
        if (check_gap_after(cur, h->files, length, count)){
            after = cur;
            break;
        }
    }
    return after;
}

int create_file(char * filename, size_t length, void * helper) {
    help* h = (help*)helper;
    FILE* dtable = h->file_ptrs[1];

    meta* new = (meta*)malloc(sizeof(meta));

    unsigned int noffset = 0; 
    if(h->count != 0) {
        meta* after = find_gap(length, helper, dtable);
        noffset = after->offset + after->length;
        printf("yeet");
        if(after == NULL) {
        }
        else { 
        }
    }
    strncpy(new->name, filename,64);
    new->length = length;
    new->offset = noffset;
    fseek(dtable, 0, noffset);

    fwrite(new, sizeof(meta), 1, dtable);

    fclose(dtable);

    return 0;
}



int resize_file(char * filename, size_t length, void * helper) {
    if(1){

    }
}

void repack(void * helper) {
    return;
}

int delete_file(char * filename, void * helper) {
    return 0;
}

int rename_file(char * oldname, char * newname, void * helper) {
    return 0;
}

int read_file(char * filename, size_t offset, size_t count, void * buf, void * helper) {
    return 0;
}

int write_file(char * filename, size_t offset, size_t count, void * buf, void * helper) {
    return 0;
}

ssize_t file_size(char * filename, void * helper) {
    return 0;
}

void fletcher(uint8_t * buf, size_t length, uint8_t * output) {
    return;
}

void compute_hash_tree(void * helper) {
    return;
}

void compute_hash_block(size_t block_offset, void * helper) {
    return;
}
