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
void destroy_block(void *block)
{
    free(block);
}


int read_block(int number, void *block)
{
    int result = -1;
    if (number >= 0 && lseek(filesystem_fd, size_of_block * number, SEEK_SET) >= 0)
    {
        if (read(filesystem_fd, block, size_of_block) >= 0)
        {
            result = 0;
        }
    }
    return result;
}

int write_block(int number, void *block)
{
    int result = -1;
    if (number >= 0 && lseek(filesystem_fd, size_of_block * number, SEEK_SET) >= 0)
    {
        if (write(filesystem_fd, block, size_of_block) == size_of_block)
        {
            result = 0;
        }
    }
    return result;
}
int search_free_block()
{
    int number = number_of_root_block + 1;
    char status;
    int read_result;
    while (TRUE)
    {
        if (lseek(filesystem_fd,  size_of_block * number, SEEK_SET) < 0)
        {
            number = -1;
            break;
        }
        read_result = read(filesystem_fd, &status, sizeof(char));
        if (read_result < 0)
        {
            number = -1;
            break;
        }
        if (read_result == 0 || status == BLOCK_STATUS_FREE)
        {
            break;
        }
        number++;
    }
    return number;
}
void *get_block(int number)
{
    void *block = NULL;
    if (number >= 0)
    {
        block = create_block();
        if (block != NULL && read_block(number, block) != 0)
        {
            destroy_block(block);
            block = NULL;
        }
    }
    return block;
}
int set_block_status(int number, char status)
{
    int result = -1;
    if (number >= 0 && lseek(filesystem_fd, size_of_block * number + BLOCK_STATUS_OFFSET, SEEK_SET) >= 0)
    {
        if (write(filesystem_fd, &status, sizeof(char)) == sizeof(char))
        {
            result = 0;
        }
    }
    return result;
}
int remove_block(int number)
{
    int result = 0;
    int status = get_block_status(number);
    switch (status)
    {
        case BLOCK_STATUS_FREE:
            break;
        case BLOCK_STATUS_FOLDER:
            remove_folder(number);
            break;
        case BLOCK_STATUS_FILE:
            remove_file(number);
            break;
        default:
            result = -1;
            break;
    }
    return result;
}
int search_inode(int node_number, char **node_names)
{
    int result = -1;
    if (node_number >= 0 && node_names != NULL)
    {
        if (*node_names == NULL)
        {
            result = node_number;
        }
        else
        {
            int next_node_number = search_inode_in_folder(node_number, *node_names);
            if (next_node_number > 0)
            {
                result = search_inode(next_node_number, node_names + 1);
            }
        }
    }
    return result;
}


