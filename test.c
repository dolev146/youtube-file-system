#include <stdio.h>
#include "fs.h"
#include <string.h>

void main()
{
    /**
     * @brief first check this
     *
     */
    // create_fs();
    // sync_fs();

    /***/
    // mount_fs();
    // // allocate_file("first");
    // sync_fs();
    // print_fs();
    // printf("done\n");

    /*****
     * * another example
     * *
     * ****/

    // mount_fs();
    // // alocate_file("first");

    // set_filesize(0, 1000);
    // print_fs();

    // set_filesize(0, 5000);
    // print_fs();

    // set_filesize(0, 1000);
    // print_fs();

    // sync_fs();
    // print_fs();
    // printf("done\n");

    /*****
     * * another example
     * *
     * ****/

     mount_fs();
    // allocate_file("first");
    int file = allocate_file("another");
    set_filesize(file, 5000);
    char yakov[] = "why are you gay?";
    
    char data = 'b';
    int i;
    for (i = 0; i < 16; i++)
    {
        write_byte(file, i , &yakov[i]);
    }

    sync_fs();
    printf("done\n");
} // main
