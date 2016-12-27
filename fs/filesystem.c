//
// Created by boran on 21.10.2016.
//

#include "filesystem.h"
#include "../mm/alloc.h"
#include "../include/util.h"

void fs_init (File_System *fs) {
    fs -> root = malloc(sizeof(File));

    //children of root
    create_child(fs -> root, fs -> dev, "dev", T_DIRECTORY);    //dev is child of root
    create_child(fs -> root, fs -> proc, "proc", T_DIRECTORY);  //proc is child of root
    create_child(fs -> root, fs -> mnt, "mnt", T_DIRECTORY);   //mnt is child of root

    create_child(fs -> root, fs -> sys, "sys", T_DIRECTORY);    //sys is child of root

    //children of sys
    create_child(fs -> sys, fs -> env, "env", T_DIRECTORY);     //env is child of sys
    create_child(fs -> sys, fs -> usr, "usr", T_DIRECTORY);     //usr is child of sys
    create_child(fs -> sys, fs -> mods, "mods", T_DIRECTORY);     //mods is child of sys
    create_child(fs -> sys, fs -> sockets, "sockets", T_DIRECTORY);      //sockets is child of sys

}

void fs_delete (File_System *fs) {
    free(fs -> root);
}

//void mknod () {}  //will be implemented later

File *path (File_System *fs, string path) {
    if (path == NULL)
        return NULL;

    File *tmp = fs->root;
    string name, beg, end;

    beg = path;
    while (beg == (string) '/')
        beg++;
    end = beg + 1;

    while (beg != NULL) {
        if (tmp->f_type != T_DIRECTORY)
            return NULL;

        while (end != NULL && end != (string) '/')
            end++;
        name = malloc((uint32) (end - beg + 1));
        memcpy(name, beg, (int) (end - beg));
        name[end - beg] = NULL;

        if (strEquals("..", name))
            tmp = tmp->f_parent;
        else if (strEquals(".", name)) {/*do nothing*/}
        else {
            tmp = find(tmp, name);
            if (tmp == NULL) {
                free(name);
                return NULL;
            }
            if (tmp->f_type == T_LINK && tmp->f_link != NULL)
                tmp = tmp->f_link;
        }
        beg = end;
        while (beg == (string) '/')
            beg++;
        end = beg + 1;
        free(name);
    }

    return tmp;
}

File *parent_path (File_System *fs, string pp_name, string file_name) {
    if (pp_name == NULL)
        return NULL;

    File *tmp;
    File *par = fs->root;
    string name, beg, end;

    beg = pp_name;
    while (beg == (string) '/')
        beg++;
    end = beg + 1;

    while (beg != NULL) {
        if (par->f_type != T_DIRECTORY)
            return NULL;

        while (end != NULL && end != (string) '/')
            end++;
        name = malloc((uint32) (end - beg + 1));
        memcpy(name, beg, (int) (end - beg));
        name[end - beg] = NULL;

        if (strEquals("..", name))
            par = par->f_parent;
        else if (strEquals(".", name)) { /*Do nothing*/ }
        else {
            tmp = par;
            if (par->f_type == T_LINK && par->f_link != NULL)
                par = par->f_link;

            par = find(par, name);
            if(par == NULL) {
                strCopy(file_name, name);
                free(name);
                return tmp;
            }
        }
        beg = end;
        while (beg == (string) '/')
            beg++;
        end = beg + 1;

        free(name);
    }

    return par;
}

bool link (File_System *fs, string file_name, string new_file) {
    File *to_link = path(fs, file_name);
    if (to_link == NULL)
        return false;

    string name = malloc(255);
    File *par = parent_path(fs, new_file, name);
    File *tmp;
    file_init(tmp, name, T_LINK);
    tmp->f_link = to_link;
    add_child(par, tmp);
    return true;

}

bool addFile (File_System *fs, string dir, File *file) {
    File *fdir = path(fs, dir);
    if (fdir == NULL)
        return false;
    else
        return add_child(fdir, file);
}

File *piv_root (File_System *fs, File *target_dir) {
    if (target_dir ==  NULL)
        return NULL;
    else {
        File *new_root = NULL;
        file_init(new_root, "/", T_DIRECTORY);

        File *main_child = target_dir->f_child;
        add_child(new_root, main_child);

        uint8 i = 0;
        File *tmp_ch = main_child->f_prev;
        do {
            if (tmp_ch != NULL) {
                add_child(new_root, tmp_ch);
                tmp_ch = tmp_ch->f_prev;
            }
            else
                i = 0;
        } while (i == 1);

        tmp_ch = main_child->f_next;
        i = 1;
        do {
            if (tmp_ch != NULL) {
                add_child(new_root, tmp_ch);
                tmp_ch = tmp_ch->f_next;
            }
            else
                i = 0;
        } while (i == 1);

        return new_root;
    }
}
