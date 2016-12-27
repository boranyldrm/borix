//
// Created by boran on 15.10.2016.
//

#include "file.h"
#include "../mm/alloc.h"
#include "../include/string.h"
#include "../include/util.h"
#include "../include/screen.h"

static uint32 inode_sys = 0;

// construct
void file_init (File *file, string name, uint8 type) {
    uint32 length = strLength(name);
    file->f_name = malloc(length + 1);
    memory_set(name, 0, length);
    memcpy(file->f_name, name, length);
    file->f_name[length] = 0;

    check_name(file);
    //todo master will be added to current process
    file->f_inode = inode_sys;
    inode_sys++;
    file->f_size = 0;
    file->f_type = type;
    file->f_parent = NULL;
    file->f_child = NULL;
    file->f_next = NULL;
    file->f_prev = NULL;
    file->f_link = NULL;
    file->f_mem_map = NULL;

}

// deconstruct
void file_delete (File *file) {
    free(file);

    if (file -> f_prev == NULL) {
        file -> f_parent -> f_child = file -> f_next;
        file -> f_next = NULL;
    }
    else if (file -> f_next == NULL)
        file -> f_prev -> f_next = NULL;
    else if (file -> f_next == NULL && file -> f_prev == NULL)
        file -> f_parent -> f_child = NULL;
    else {
        printk("prev ");
        printk(file -> f_prev -> f_name);
        printk(" next is now ");
        printk(file -> f_next -> f_name);
        printk("\n");

        printk("next ");
        printk(file -> f_next -> f_name);
        printk(" prev is now ");
        printk(file -> f_prev -> f_name);
        printk("\n");

        file -> f_prev -> f_next = file -> f_next;
        file -> f_next -> f_prev = file -> f_prev;
    }
    File *n = file -> f_child;
    File *nn = NULL;
    while (n != NULL) {
        nn = n -> f_next;
        free(n);
        n = nn;
    }
}


void check_name (File *file) {
    strReplace(file -> f_name, '/', '_');
    strReplace(file -> f_name, '\\', '_');
    strReplace(file -> f_name, '?', '_');
    strReplace(file -> f_name, '>', '_');
    strReplace(file -> f_name, '<', '_');
    strReplace(file -> f_name, '*', '_');
    strReplace(file -> f_name, '"', '_');
    strReplace(file -> f_name, ':', '_');
}


bool add_child (File *parent, File *child) {
    if (child == NULL || parent == NULL)
        return false;
    child -> f_parent = parent;
    child -> f_prev = NULL;
    child -> f_next = parent -> f_child;
    if (parent -> f_child != NULL)
        parent -> f_child -> f_prev = child;
    parent -> f_child = child;
    return true;
}

//child enter NULL, return as a file
void *create_child (File *parent, File *child, string name, uint8 type) {
    file_init(child, name, type);
    add_child(parent, child);
}

File *find (File *file, string name) {
    File *tmp = file -> f_child;
    while (tmp != NULL) {
        if (strEquals(tmp -> f_name, name))
            return tmp;
        tmp = tmp -> f_next;
    }
    return NULL;
}

uint32 fs_mmap (File *file, uint32 size, uint32 flags, uint32 offset, uint32 prot) {
    /* Wll be Implemented After paging
     */
    return NULL;
}

Stat_fs stat() {
    Stat_fs statFs;
    return statFs;
}
