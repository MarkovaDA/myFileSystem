#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "actions.h"

// получаем атрибуты файла
int my_getattr(const char *path, struct stat *stbuf);
// получаем сдержимое папки
int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi);
// определяем опции открытия файла
int my_open(const char *path, struct fuse_file_info *fi);
// читаем данные из открытого файла
int my_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
// предоставляет возможность записать в открытый файл
int my_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
// создаём папку
int my_mkdir(const char *path, mode_t mode);
// создаём файл
int my_mknod(const char *path, mode_t mode, dev_t dev);
// переименование
int my_rename(const char *old_path, const char *new_path);
// удалям папку
int my_rmdir(const char *path);
// удаляем файл
int my_unlink(const char *path);
// изменить размер файла
int my_truncate(const char *path, off_t size);

int my_getattr(const char *path, struct stat *stbuf)
{
    int result = -ENOENT;
    char **node_names = split_path(path);
    if (node_names != NULL)
    {
        int number = search_inode(number_of_root_block, node_names);
        if (number >= 0 && get_inode_stat(number, stbuf) == 0)
        {
            result = 0;
        }
        destroy_node_names(node_names);
    }
    return result;
}
int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    int result = -ENOENT;
    char **node_names = split_path(path);
    if (node_names != NULL)
    {
        int number = search_inode(number_of_root_block, node_names);
        if (number >= 0)
        {
            inode_t *folder = (inode_t *)get_block(number);
            if (folder != NULL)
            {
                if (folder->status == BLOCK_STATUS_FOLDER)
                {
                    result = 0;
                    filler(buf, ".", NULL, 0);
                    filler(buf, "..", NULL, 0);
                    char name[NODE_NAME_MAX_SIZE];
                    stat_t stat;
                    int *start = (int *)folder->content;
                    int *end = (int *)((void *)folder + size_of_block);
                    while (start < end)
                    {
                        if (*start > 0 && get_inode_name(*start, name) == 0 && get_inode_stat(*start, &stat) == 0)
                        {
                            if (filler(buf, name, &stat, 0) != 0)
                            {
                                break;
                            }
                        }
                        start++;
                    }
                }
                destroy_block(folder);
            }
        }
        destroy_node_names(node_names);
    }
    return result;
}
int my_open(const char *path, struct fuse_file_info *fi)
{
	return 0;
}

int my_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int result = -ENOENT;
    char **node_names = split_path(path);
    if (node_names != NULL)
    {
        int number = search_inode(number_of_root_block, node_names);
        if (number >= 0)
        {
            inode_t *file = (inode_t *)get_block(number);
            if (file != NULL)
            {
                if (file->status == BLOCK_STATUS_FILE)
                {
                    if (offset < NODE_CONTENT_MAX_SIZE)
                    {
                        if (offset + size > NODE_CONTENT_MAX_SIZE)
                        {
                            size = NODE_CONTENT_MAX_SIZE - offset;
                        }
                        memcpy(buf, file->content + offset, size);
                        result = size;
                    }
                    else
                    {
                        result = 0;
                    }
                }
                destroy_block(file);
            }
        }
        destroy_node_names(node_names);
    }
    return result;
}
