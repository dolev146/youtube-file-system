#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "fs.h"
/*
struct superblock
{
    int num_inodes;
    int num_blocks;
    int size_blocks;
};

struct inode
{
    int size;
    char name[8];
};

struct disk_block
{
    int next_block_num;
    char data[512];
};

*/

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

int find_empty_inode()
{
    int i;
    for (i = 0; i < sb.num_inodes; i++)
    {
        if (inodes[i].first_block == -1)
        {
            return i;
        }
    }
    return -1;
} // return empty inode

int get_block_num(int file, int offset)
{

    int togo = offset;
    int bn = inodes[file].first_block;
    while (togo > 0)
    {
        bn = dbs[bn].next_block_num;
        togo--;
    }
    return bn;
} // get_block_num

void shorten_file(int bn)
{
    int nn = dbs[bn].next_block_num;
    if (nn >= 0)
    {
        shorten_file(nn);
    }
    dbs[bn].next_block_num = -1;
} // shorten_file

int find_empty_block()
{
    int i;
    for (i = 0; i < sb.num_blocks; i++)
    {
        if (dbs[i].next_block_num == -1)
        {
            return i;
        }
    }
    return -1;
} // return empty block

// initialize new filesystem
void create_fs()
{
    sb.num_inodes = 10;
    sb.num_blocks = 100;
    sb.size_blocks = sizeof(struct disk_block);

    int i;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "emptyfi");

    } // init inodes

    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);

    for (i = 0; i < sb.num_blocks; i++)
    {
        dbs[i].next_block_num = -1;
    } // init disk_block

} // create filesystem

// load a file system
void mount_fs()
{
    FILE *file;
    file = fopen("fs_data", "r");

    // superblock
    fread(&sb, sizeof(struct superblock), 1, file);

    // inodes
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);

    fread(inodes, sizeof(struct inode), sb.num_inodes, file);
    fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);

    fclose(file);
} // mount_fs()

// write the file system
void sync_fs()
{
    FILE *file;
    file = fopen("fs_data", "w+");
    // superblock
    fwrite(&sb, sizeof(struct superblock), 1, file);

    fwrite(inodes, sizeof(struct inode), sb.num_inodes, file);
    fwrite(dbs, sizeof(struct disk_block), sb.num_blocks, file);

    fclose(file);

} // sync_fs

void print_fs()
{
    printf("Superblock info \n");
    printf("\tnum inodes: %d\n", sb.num_inodes);
    printf("\tnum blocks: %d\n", sb.num_blocks);
    printf("\tsize blocks: %d\n", sb.size_blocks);

    printf("inodes\n");
    int i;
    for (i = 0; i < sb.num_inodes; i++)
    {
        printf("\tsize: %d block: %d name: %s\n", inodes[i].size, inodes[i].first_block, inodes[i].name);
    } // init inodes

    for (i = 0; i < sb.num_blocks; i++)
    {
        printf("\tblock num: %d next block %d\n", i, dbs[i].next_block_num);
    } // init disk_block

} // print_fs

int allocate_file(char name[8])
{
    // find an empty inode
    int in = find_empty_inode();
    // find / claim a disk block
    int block = find_empty_block();

    // claim them
    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;

    strcpy((inodes[in]).name, name);
    // return the file descriptor
    return in;

} // allocate_file

// add / delet blocks
void set_filesize(int filenum, int size)
{

    // how many blocks should we have
    int tmp = size + BLOCKSIZE - 1;
    int num = tmp / BLOCKSIZE;
    int bn = inodes[filenum].first_block;
    num--;
    // grow the file if necessary
    while (num > 0)
    {
        // check next block number
        int next_num = dbs[bn].next_block_num;
        if (next_num == -1)
        {
            int empty = find_empty_block();
            dbs[bn].next_block_num = empty;
            dbs[empty].next_block_num = -2;
        }
        bn = dbs[bn].next_block_num;
        num--;
    }

    // shoten if necessary
    shorten_file(bn);
    dbs[bn].next_block_num = -2;

} // set_filesize
void write_byte(int filenum, int pos, char *data)
{
    // calculate which block
    int relative_block = pos / BLOCKSIZE;

    // find th eblock number
    int bn = get_block_num(filenum, relative_block);
    // calculate the offset in the block
    int offset = pos % BLOCKSIZE;
    // write the data
    dbs[bn].data[offset] = (*data);

} // write_byte
