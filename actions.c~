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
