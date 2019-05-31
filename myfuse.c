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

/*  myfuse_getattr
    
    Returns important information about a file passed in.

    ARGS(
        const char * filename: the name of the file you are checking.
        struct stat * result: a struct stat in which the result is stored.
    )

    RETURN: 0 indicating sucess, or a negative int describing the fail condition.
*/

int myfuse_getattr(const char* filename, struct stat* result) {
<<<<<<< HEAD
	printf("Geting attributes: %s\n", filename);    
memset(result, 0, sizeof(struct stat));
    help* h = ((help*)raw_helper);
    char* fname = (char*)filename+1;
    if (strcmp(filename, "/") == 0) {
	printf("reeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n");
      
	result->st_mode = S_IFDIR;
	result->st_nlink = 2;
=======
    printf("Getting attributes: %s\n", filename);
    memset(result, 0, sizeof(struct stat));
    help* h = ((help*)raw_helper);
    char* fname = (char*)filename;
    if (strcmp(filename, "/") == 0) {
        result->st_mode = S_IFDIR;
>>>>>>> dc7c21dfa7409b42f2ab54d463c11162efbea045
    } else {
        int x = find_file(fname, h);
	printf("%d asdasdasdasda\n", (h->files+x)->length);
//        if(x == -1)
//            return -ENOENT;     //Invalid File
        result->st_mode = S_IFREG;
        result->st_nlink = 1;
        result->st_size = (h->files + x)->length;
  }
    return 0;   //Success
}

/*  myfuse_readdir
    
    Returns all items starting with the passed filename.
    In the case of filename being blank, i.e when using ls, returns the whole directory.

    ARGS(
        const char * filename: the name of the file you are checking.
        void * buf: a buffer we are told to ignore, presumably for filler to store stuff in
        fuse_fill_dir_t filler: a function which we pass necessary filenames to
        off_t offset: a variable we are told to ignore
        struct fuse_file_info: a struct containing data relating to our data structure, also ignored
    )

    RETURN: 1 indicating sucess, or a negative int describing the fail condition.
*/
int myfuse_readdir(const char * filename, void * buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info * fi) {
    printf("Reading Directory: %s\n", filename)
    help* h = ((help*)raw_helper);
    for(int i = 0; i < h->num_files; i++)
        if((h->files + i)->name[0] != '\0')
            if(filler(buf, (h->files + i)->name, NULL, 0) != 0){
                return -EINVAL;      //Result buffer is too small.
            }
    if(i == 0){
        return -ENOENT;   //No such files/directory.
    }
    return 0;   //Success | man 2 readdir says to return 1, however, fuse always errors when I do that.
>>>>>>> dc7c21dfa7409b42f2ab54d463c11162efbea045
}

/*  myfuse_unlink
   
    Deletes a file

    ARGS(
        const char * filename: the name of the file you are deleting.
    )

    RETURN: 0 indicating sucess, or a negative int describing the fail condition.
*/
int myfuse_unlink(const char * filename){
    printf("Deleting: %s\n", filename);
    
    if(delete_file(((char*)filename)+1, raw_helper) == 1){
        return -ENONET;     //File does not exist.
    }
    return 0;       //Success.
}

/*  myfuse_rename
   
    Renames a file

    ARGS(
        const char * filename: the name of the file you are renaming.
        const char * new_name: the name that you are changing the file to.
    )

    RETURN: 0 indicating sucess, or a negative int describing the fail condition.
*/
int myfuse_rename(const char * filename, const char * new_name){
    printf("Renaming:%s, to: %s\n", filename, new_name);
    
    if(rename_file(((char*)filename)+1, (char*)new_name++, raw_helper) == 1){
        return -ENOENT;     //File does not exist, or new_name is taken.
    }

    return 0;       //Success
}

/*  myfuse_truncate
   
    Resizes a file

    ARGS(
        const char * filename: the name of the file you are deleting.
        off_t newsize: the size that the file should be resized to.
    )

    RETURN: 0 indicating sucess, or a negative int describing the fail condition.
*/
int myfuse_truncate(const char * filename, off_t newsize){
    printf("Resizing :%s, to: %lu\n", filename, newsize);
    int ret = resize_file(((char*)filename)+1, newsize, raw_helper);
    if(ret == 1){
        return -ENOENT;     //File does not exist.
    } 
    else if (ret == 2) {
        return -EINVAL;      //Length is too long.
    }
    return 0;       //Success.
}

/*  myfuse_open
   
    Unused; does nothing.

    ARGS(
        const char * filename: the name of the file you are doing nothing to.
        struct fuse_file_info * fi: meta data about the file you are doing nothing to
    )

    RETURN: 0 indicating nothing done.
*/
int myfuse_open(const char * filename, struct fuse_file_info * fi){
    return 0; //Not used
}

/*  myfuse_read
   
    Reads the contents of a file and places them in buf.

    ARGS(
        const char * filename: the name of the file you are reading from.
        char * buf: the buffer we copy the read data into.
        size_t count: the number of bytes we would like to read.
        off_t offset: the offset from the start of the file, from which we would like to begin reading.
        struct fuse_file_info: a struct containing data relating to our data structure, ignored.
    )

    RETURN: a positive integer indicating the number of bytes read; sucess, or a negative int describing the fail condition.
*/
int myfuse_read(const char * filename, char * buf, size_t count, off_t offset, struct fuse_file_info * fi){
    printf("Reading: %s\n", filename);
    help* h = (help*)raw_helper;
    int x = find_file((char*)filename+1, h);
    
	printf("1\n");
   // if(x == -1)
    //    return -EBADF;      //Closest approximation I could find to file does not exist.
	printf("2\n");
    meta * f = h->files + x;
   // if(offset > f->length){
	printf("3\n");        
//return 0;           //Attempting to read outside of the file, return 0, the number of bytes read
  //  }
    if (offset + count > f->length){
	printf("%d\n",count);        
	count = f->length - offset;
	printf("%d\n",count);
    }
    read_file(((char*)filename)+1, offset, count, buf, raw_helper);
//if == 3){
  //      return -EINVAL; //Closest I could find to "Corrupted file".
   // }
    printf("%d\n", count);
    return count;      //Success, return number of bytes read.
}

/*  myfuse_write
   
    Writes the contents of a buffer to a file.

    ARGS(
        const char * filename: the name of the file you are doing nothing to.
        const char * buf: the buffer we copy data from.
        size_t count: the number of bytes we would like to write.
        off_t offset: the offset from the start of the file, from which we would like to begin writing.
        struct fuse_file_info: a struct containing data relating to our data structure, ignored.
    )

    RETURN: a positive integer indicating the number of bytes written; sucess, or a negative int describing the fail condition.
*/
int myfuse_write(const char * filename, const char * buf, size_t count, off_t offset, struct fuse_file_info * fi){
    printf("Writing: %s, bytes: %ld, offset:, %ld\n", filename, count, offset);
    
    help * h = (help*)raw_helper;
    int x = find_file(((char*)filename)+1, h);    
    if(x == -1)
        return -EBADF;       //Closest to invalid file
    meta * f = h->files + x;
    
    if(offset > f->length)
        if(resize_file(((char*)filename)+1, offset + count, raw_helper) == 2)
            return -EDQUOT;     //Out of space on disk

    if(f->offset + offset + count >= h->fsize)
        return -EFBIG;       //Attempting to write past end of max allowed file size        
    
    write_file(((char*)filename)+1, offset, count, (void*)buf, raw_helper);

    //All conditions that are check inside write_file() have been checked above, hence write_file() should never error

    return count;
}

/*  myfuse_release
   
    Unused; does nothing.

    ARGS(
        const char * filename: the name of the file you are doing nothing to.
        struct fuse_file_info * fi: meta data about the file you are doing nothing to
    )

    RETURN: 0 indicating nothing done.
*/
int myfuse_release(const char * filename, struct fuse_file_info * fi){
    return 0;       //Unused
}

/*  myfuse_init
   
    Prepares the whole file system for use
    Creates the helper variable.

    ARGS(
        struct fuse_file_info * fi: meta data about the file you are doing nothing to
    )

    RETURN: void * pointing to the helper variable which is used throughout. This return is sent to fuse_get_context()->private_data.
*/
void * myfuse_init(struct fuse_conn_info * info){ 
    printf("Initializing filesystem\n");
    void * helper = init_fs(file_data_file_name, directory_table_file_name, hash_data_file_name, 4); 
    print_file(helper);
    return helper;      //Only returns the helper pointer, we assume we can trust the input.
}

/*  myfuse_destroy
   
    Closes the entire file system.
    Frees all malloc'd memory, munmaps all files.

    ARGS(
        void * something: something not of use to us.
    )

    RETURN: Nothing, is a void.
*/
void myfuse_destroy(void * something){
    close_fs(raw_helper);
}

/*  myfuse_create
   
    Creates a file.

    ARGS(
        const char * filename: the name of the file you are doing nothing to.
        mode_t mode: ignored.
        struct fuse_file_info: a struct containing data relating to our data structure, ignored.
    )

    RETURN: a positive integer indicating the number of bytes written; sucess, or a negative int describing the fail condition.
*/
int myfuse_create(const char * filename, mode_t mode, struct fuse_file_info * fi){
    printf("Creating file: %s\n", filename);
    int ret = create_file(((char*)filename)+1, 10, raw_helper);
    if(ret == 1){
        return -ENOENT;     //File does not exist.
    }
    else if(ret == 2){
        return -EDQUOT;     //No more space left in file system.
    }
    return 0;       //Success.
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

/*  main
    Starts fuse, defines the variables for our functions.

    ARGS(
        int argc:       the number of arguments passed into the program.
        char * argv[]:  the arguments passed into the program.
    )

    RETURN: 0 on success, non-zero on failure
*/
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
