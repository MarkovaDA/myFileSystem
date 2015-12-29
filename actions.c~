#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "actions.h"

const int size_of_block = SIZE_OF_BLOCK;
int filesystem_fd = -1;
const int number_of_root_block = NUMBER_OF_ROOT_BLOCK;

int init()
{
    int result = 0;
    // пытаемся открыть существующий файл с файловой системой
    filesystem_fd = open(FILESYSTEM, O_RDWR, 0666);
    if (filesystem_fd < 0)
    {
        // создаем новый
        filesystem_fd = open(FILESYSTEM, O_CREAT | O_RDWR, 0666);
        if (filesystem_fd < 0 || create_root() != 0)
        {
            result = -1;
        }
    }
    return result;
}
int create_root()
{
    int result = -1;
    inode_t *root = (inode_t *)create_block();
    if (root != NULL)
    {
        root->status = BLOCK_STATUS_FOLDER;
        root->name[0] = '\0';
        root->stat.st_mode = S_IFDIR | 0777;
        root->stat.st_nlink = 2;
        if (write_block(number_of_root_block, root) == 0)
        {
            result = 0;
        }
        destroy_block(root);
    }
    return result;
}


void *create_block()
{
    return calloc(size_of_block, sizeof(char));
}
