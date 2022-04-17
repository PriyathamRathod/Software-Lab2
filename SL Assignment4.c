//BT20CSE100 Priyatham Rathod - SL Assignment 4.

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
typedef enum{FALSE,TRUE} boolean;

/* for storing metadata of block*/
typedef struct blockMetaData Header;

struct blockMetaData{
    size_t size;
    boolean isFree;
    Header *next;
};

#define META_SIZE sizeof(Header)
#define SBRK_FAILURE (void*)-1

void *header = NULL;

void outOfMemory()
{
    printf("\t[+] Sorry could not allocate requested memory\n");
}

Header *requestSpaceFromOS(Header *lptr,size_t size)
{

    printf("\t[+] Intialising sbrk()....\n");         //sbrk() stands for Space increments after program break address.
    Header *block = sbrk(0);
    printf("\t[+] Creating space of size %ld bytes using the input given by you...\n",size);

    void *request = sbrk(size+META_SIZE);
    if(request==SBRK_FAILURE)
    {
        outOfMemory();
        return NULL;
    }
    printf("\t[+] Done.\n");

    if(lptr!=NULL)
    {
        lptr->next=block;
    }

    block->size = size;
    block->next = NULL;
    block->isFree = FALSE;

    return block;
}

boolean canSplit(Header* block,size_t size)
{
    boolean retVal = FALSE;
    if((block->size) - size > 0) retVal = TRUE;

    return retVal;
}

void* split(Header** block, size_t size)
{
    Header* freePart = (Header*)((*block)+size);
    printf("\t[+] Splitting function has been initialised...\n");
    freePart->size = ( (*block)->size )-size;
    freePart->isFree = TRUE;
    freePart->next=(*block)->next;

    (*block)->size = size;
    (*block)->isFree = FALSE;
    (*block)->next=freePart;
    printf("\t[+] Size of new blocks are freeblock-->%ld bytes and allocated block-->%ld bytes\n",freePart->size,(*block)->size);
}

Header*  getFreeBlock(Header** end,size_t size)
{
    printf("\t[+] Search function has been initialised..\n");
    printf("\t[+] Please wait, finding the free block of size %ld bytes...\n",size);
    Header* current = header;
    while (current!=NULL)
    {
        *end=current;
        if(current->isFree==TRUE&&current->size>=size) return current;
        current=current->next;
    }

    return NULL;
}

void  *MemAlloc(size_t size)
{

    printf("\t[+] Malloc function initialising....\n");
    printf("\t[+] Allocating a memory of size %ld bytes as requested by you.....\n",size );
    if(size<=0)
    {
        printf("\t[+] Done\n");
        return NULL;
    }

    Header *block;
    if(header==NULL)
    {
        printf("\t[+] This is your first allocation, hence we are requesting space from operating system please wait..\n");
        block = requestSpaceFromOS(NULL,size);
        if(block == NULL)
        {
            outOfMemory();
            return NULL;
        }

        header = block;
        printf("\t[+] Done.\n");
    }
    else
    {
        Header *end = header;
        block = getFreeBlock(&end,size);
        if(block == NULL)
        {
            printf("\t[+] Free block is not found in the memory. So we are requesting space from operating system please wait..\n");
            block = requestSpaceFromOS(end,size);

            if(block == NULL)
            {
                outOfMemory();
                return NULL;
            }

            printf("\t[+] Done.\n");
        }
        else
        {
            printf("\t[+] We have found a Free block at %p\n",block);

            if(canSplit(block, size))
            {
                printf("\t[+] But the size is bigger than you requested..\n");
                printf("\t[+] So the splitting is being done(Calling Split function)..\n");
                split(&block,size);
                printf("\t[+] Splitting is done.\n");
            }
            block -> isFree = FALSE;
        }
    }

    return (block+1);
}

boolean canMerge(Header* block)
{
    boolean retVal = FALSE;
    if(block->next!=NULL&&(block->next)->isFree==TRUE)
    {
        retVal=TRUE;
    }

    return retVal;
}

void* mergeBlocks(Header** block)
{
    printf("\t[+] Merge function is initialised. Please wait till the process is completed...\n");
    (*block)->size+=( (*block)->next )->size;
    (*block)->next = ( (*block)->next )->next;
}

void free(void* ptr)
{
    printf("\t[+] Now the free operation will be initialised..\n");
    printf("\t[+] freeing memory located at %p \n",ptr);

    if(ptr==NULL)
    {
        return;
    }
    Header* block = (Header*)ptr - 1;
    printf("\t[+] Freed memory of size %ld bytes located at %p\n",block->size,ptr);

    if(canMerge(block))
    {
        printf("\t[+] We have free block adjacent to it\n");
        printf("\t[+] So now Merge will take place. Please wait till the operationis complete...\n");
        mergeBlocks(&block);
        printf("\t[+] Merging is done.\n");
        printf("\t[+] Size of block which is final is %ld bytes\n",block->size);
    }

    block->isFree = TRUE;
    printf("\t[+] Done.\n");
}

void allocate(size_t size)
{
    void* ptr = MemAlloc(size);
    printf("\t[+] Allocation of memory of size %ld bytes at the address %p on heap is done.\n",size,ptr);
}

int main()
{
    int option;
    size_t size;
    int* ptr;
    printf("Hi there!\nThis program will implement Heap and allocate memomy and also frees them!\n");
    printf("Now input the option according to thier number.\n");
    printf("1.Allocate memory\n2.Free the allocated memory\n3.To exit");
    while(TRUE)
    {
        printf("\nEnter your option:");
        scanf("%d",&option);

        switch (option)
        {

            case 1:
            {
                printf("\nOption 1 selected, Enter the size in bytes:");
                scanf("%ld",&size);
                allocate(size);
                break;
            }

            case 2:
            {
                printf("\nOption 2 selected, Enter the address you wish to free:");
                scanf("%p",&ptr);
                free(ptr);
                break;
            }

            case 3:
            {
                printf("Option 3 selected. Program is terminated!\n");
                exit(0);
            }
        } 
    }

    return 0;
}
