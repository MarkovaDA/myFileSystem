#ifndef FSACTIONS
#define FSACTIONS

#define _FILE_OFFSET_BITS 64
#define FUSE_USE_VERSION 26
#include <fuse.h>

//упаковка структур по одному байту
#pragma pack(1)

//#define DEBUG
#ifdef DEBUG
#define TRACE printf("[DEBUG] FILE:%s LINE:%d\n", __FILE__, __LINE__);
#else
#define TRACE
#endif

#define FILESYSTEM "filesystem"
#define NUMBER_OF_ROOT_BLOCK 0
#define SIZE_OF_BLOCK 4096
#define NODE_NAME_MAX_SIZE 256
#define BLOCK_STATUS_OFFSET 0
#define NODE_NAME_OFFSET (BLOCK_STATUS_OFFSET + sizeof(char))
#define NODE_STAT_OFFSET (NODE_NAME_OFFSET + NODE_NAME_MAX_SIZE)
#define NODE_CONTENT_OFFSET (NODE_STAT_OFFSET + sizeof(stat_t))
#define NODE_CONTENT_MAX_SIZE (size_of_block - NODE_CONTENT_OFFSET)

// информация о файле
typedef struct stat stat_t;
// файл или папка
typedef struct node inode_t;

enum boolean
{
    FALSE = 0,
    TRUE = 1,
};

enum block_status
{
    BLOCK_STATUS_FREE = 0,
    BLOCK_STATUS_FOLDER = 1,
    BLOCK_STATUS_FILE = 2,
};

struct node
{
    char status;
    char name[NODE_NAME_MAX_SIZE];
    stat_t stat;
    char content[0];
};




