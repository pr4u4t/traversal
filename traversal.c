#include <dirent.h>
#include <errno.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

int traverse(const char* dpath, int depth);

int handle_dirent(struct dirent *dirent, const char* path, int depth);

//-----------------------------------------------------------------------//

int main(int argc, char **argv) {
    if (argc < 2) {
        printf ("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }
    
    return traverse(argv[1],0);
}

int traverse(const char* dpath, int depth){
    //This is from readdir example
    struct dirent *dirent;
    DIR *parentDir;
    char path[4096]; //Max path length
    
    if (!dpath) {
        printf ("Directory path invalid '%s'\n", dpath);
        return -1;
    }
    
    parentDir = opendir (dpath);
    if (parentDir == NULL) {
        printf ("Error opening directory '%s'\n", dpath);
        return -2;
    }
    
    
    while((dirent = readdir(parentDir)) != NULL){
        *path = 0; //set path[0] element to 0 making 0 length str
        strcat(path,dpath);
        strcat(path,"/");
        strcat(path,dirent->d_name);
        
        if(handle_dirent(dirent,path,depth) == 1){
            traverse(path,depth+1);
        }
    }
    
    closedir (parentDir);
    return 0;
}

int handle_dirent(struct dirent *dirent, const char *path, int depth){
    //This is from example 1
    struct stat buf;
 
    //skip if directory entry name '.' which is same directory, or '..' which is upper directory 
    if((strcmp(dirent->d_name,".") == 0 && strlen(dirent->d_name) == 1) 
        || (strcmp(dirent->d_name,"..") == 0 && strlen(dirent->d_name) == 2 )){
        return 0;
    }
    
    if (lstat(path, &buf) < 0) {
        return -1;
    }
     
    printf("%*s%s\r\n",depth,"",dirent->d_name);
    
    //this is if-else-if from example that checks if this is directory
    return (S_ISDIR(buf.st_mode)) ? 1 : 0;
}
