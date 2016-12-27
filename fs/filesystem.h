//
// Created by boran on 21.10.2016.
//

#ifndef BORIX_FILESYSTEM_H
#define BORIX_FILESYSTEM_H

#include "file.h"

typedef struct FILE_SYSTEM {
    File *root;
    File *dev;
    File *proc;
    File *mnt;
    File *sys;
    //children of sys
    File *env;
    File *usr;
    File *mods;
    File *sockets;
} File_System;

void fs_init (File_System *fs);
void fs_delete (File_System *fs);

void mknod ();  //will be implemented later

File *path (File_System *fs, string path);
File *parent_path (File_System *fs, string pp_name, string file_name);

bool link (File_System *fs, string file_name, string new_file);

bool addFile (File_System *fs, string dir, File *file);

File *piv_root (File_System *fs, File *target_dir);


#endif //BORIX_FILESYSTEM_H
