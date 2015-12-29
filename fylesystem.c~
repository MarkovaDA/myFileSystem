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
