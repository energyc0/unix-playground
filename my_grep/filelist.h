#ifndef FILELIST_H
#define FILELIST_H

//adds filename to the list
void flist_add(const char* filename);
//gets filename from the list, moves iterator
//if end of the list, return NULL
char* flist_get();
//return 1 if root != NULL
int flist_is_empty();
//frees the list
void flist_free();

#endif