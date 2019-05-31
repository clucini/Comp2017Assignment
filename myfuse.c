/* Do not change! */
#define FUSE_USE_VERSION 29
#define _FILE_OFFSET_BITS 64
/******************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fuse.h>
#include <errno.h>
#include "myfilesystem.c"

#define raw_helper fuse_get_context()->private_data

char * file_data_file_name = NULL;
char * directory_table_file_name = NULL;
char * hash_data_file_name = NULL;

int myfuse_getattr(const char * filename, struct stat * result) {
    memset(result, 0, sizeof(struct stat));
    help* h = ((help*)raw_helper);
    if (strcmp(filename, "/") == 0) {
        result->st_mode = S_IFDIR;
	result->st_nlink = 2; 
   } else {
        int x = find_file((char*)++filename, h);
	printf("%d\n", x);
   	if(x == -1)
		return -ENOENT;
	result->st_mode = S_IFREG;
	result->st_nlink = 1;
        result->st_size = (h->files + x)->length;
    }
    return 0;
}

int myfuse_readdir(const char * filename, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi) {
    printf("Reading Directory: %s\n", filename);
    help* h = ((help*)raw_helper);
    for(int i = 0; i < h->count; i++)
        if((h->files + i)->name[0] != '\0')
            filler(buf, (h->files + i)->name, NULL, 0);
    return 0;
}

int myfuse_unlink(const char * filename){
    printf("Deleting: %s\n", filename);
    delete_file(((char*)filename)+1, raw_helper);
    return 0;
}

int myfuse_rename(const char * filename, const char * new_name){
    printf("Renaming:%s, to: %s\n", filename, new_name);
    rename_file(((char*)filename)+1, (char*)new_name++, raw_helper);
    return 0;
}

int myfuse_truncate(const char * filename, off_t newsize){
    printf("Resizing :%s, to: %lu\n", filename, newsize);
    resize_file(((char*)filename)+1, newsize, raw_helper);
    return 0;
}

int myfuse_open(const char * filename, struct fuse_file_info * fi){
    return 0;
}
int myfuse_read(const char * filename, char * buf, size_t length, off_t offset, struct fuse_file_info * fi){
    printf("Reading :%s\n", filename);
    read_file(((char*)filename)+1, offset, length, buf, raw_helper);
    return 0;
}

int myfuse_write(const char * filename, const char * buf, size_t length, off_t offset, struct fuse_file_info * fi){
    printf("Reading :%s\n", filename);
    write_file(((char*)filename)+1, offset, length, (void*)buf, raw_helper);
    return 0;
}

int myfuse_release(const char * filename, struct fuse_file_info * fi){
    return 0;
}

void * myfuse_init(struct fuse_conn_info * info){ 
    printf("Initializing filesystem\n");
    void * helper = init_fs("files/file_data", "files/directory_table", "files_hash_data", 4); 
    return helper;
}

void myfuse_destroy(void * something){
    close_fs(raw_helper);
}

int myfuse_create(const char * filename, mode_t mode, struct fuse_file_info * fi){
    printf("Creating file: %s\n", filename);
    create_file(((char*)filename)+1, 10, 0);
    return 0;
}
struct fuse_operations operations = {
    .getattr = myfuse_getattr,
    .readdir = myfuse_readdir,
    .unlink = myfuse_unlink,
    .rename = myfuse_rename,
    .truncate = myfuse_truncate,
    .read = myfuse_read,
    .write = myfuse_write,
    .init = myfuse_init,
    .destroy = myfuse_destroy,
    .create = myfuse_create,
};

int main(int argc, char * argv[]) {
    if (argc >= 5) {
        if (strcmp(argv[argc-4], "--files") == 0) {
            file_data_file_name = argv[argc-3];
            directory_table_file_name = argv[argc-2];
            hash_data_file_name = argv[argc-1];
            argc -= 4;
        }
    }
    int ret = fuse_main(argc, argv, &operations, NULL);
    return ret;
}
