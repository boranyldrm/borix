//
// Created by boran on 15.10.2016.
//

#include "../include/types.h"

#ifndef BORIX_FILE_H
#define BORIX_FILE_H

enum {
    // T_ represents TYPE
    T_FILE,
    T_DIRECTORY,
    T_DEVICE,
    T_PROCESS,
    T_LINK
};

typedef struct STAT_FS {
    int32 st_dev;
    int32 ino_t;
    int32 st_mode;
    int32 st_nlink;
    int32 st_uid;
    int32 st_gid;
    int32 st_rdev;
    int32 st_size;
    int32 st_block_size;
    int32 st_blocks;
    int32 st_atime;
    int32 st_mtime;
    int32 st_ctime;
} Stat_fs;


typedef struct FILE {
    char *f_mem_map;
    string f_name;
    uint32 f_size;
    uint8 f_type;
    uint32 f_inode;

    struct FILE *f_dev;   // master device
    struct FILE *f_link;  // real file, if this file is a link

    struct FILE *f_master;

    struct FILE *f_parent;
    struct FILE *f_child;
    struct FILE *f_next;
    struct FILE *f_prev;

    struct FILE *f_device;    // this file is the device master of current file
} File;


void file_init (File *file,string name, uint8 type);
void file_delete (File *file);

void check_name (File *file);

bool add_child (File *parent, File *child);
void *create_child (File *parent, File *child, string name, uint8 type);
File *find (File *file, string name);
uint32 fs_mmap (File *file, uint32 size, uint32 flags, uint32 offset, uint32 prot);

Stat_fs stat();


#endif //BORIX_FILE_H
