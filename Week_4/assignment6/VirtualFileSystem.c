#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int BLOCK_SIZE = 512;
const int TOTAL_BLOCKS = 1024;
typedef unsigned char byte;

typedef struct freeList {
    int index;
    struct freeList *next;
    struct freeList *prev;
} freeList;

typedef struct fileNode {
    char fileName[50];
    char filetype;
    struct fileNode *parent;
    struct fileNode *next;
    int *blockPointer;
    struct fileNode *child;

} fileNode;

freeList *createFreeList()
{
    freeList *head = (freeList*)malloc(sizeof(freeList));
    if (head == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    head->index = 0;
    head->prev = NULL;
    head->next = NULL;

    freeList *ptr = head;
    
    for (int i = 1; i < TOTAL_BLOCKS; i++)
    {
        freeList *newNode = (freeList*)malloc(sizeof(freeList));
        if (newNode == NULL)
        {
            printf("Memory allocation failed for the particualr block in free list\n");
            freeList *freePtr = head;
            ptr = head;
            while(freePtr != NULL)
            {
                ptr = ptr->next;
                free(freePtr);
                freePtr = ptr;
            }
            return NULL;
        }
        
        newNode->index = i;
        newNode->prev = ptr;
        newNode->next = NULL;
        ptr->next = newNode;
        ptr = newNode;
    }
    return head;
}

char *extractCommand(char *input)
{
    char *command = (char*)malloc(10 * sizeof(char));
    if(command == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int index = 0;
    while(input[index] != ' ' && input[index] != '\0')
    {
        command[index] = input[index];
        index++;
    }
    command[index] = '\0';
    return command;
}

char *extractFileName(char *input)
{
    char *fileName = (char*)malloc(50 * sizeof(char));
    if(fileName == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int index = 0;
    int fileIndex = 0;
    while(input[index] != ' ' && input[index] != '\0')
    {
        index++;
    }

    if(input[index] == '\0')
    {
        printf("not found file name\n");
        free(fileName);
        return NULL;
    }
    index++;
    
    while (input[index] != ' ' && input[index] != '\0')
    {
        fileName[fileIndex] = input[index];
        index++;
        fileIndex++;
    }
    fileName[fileIndex] = '\0';
    return fileName;
}

char *extractFileContent(char *input)
{
    char *fileContent = (char*)malloc(100 * sizeof(char));
    if(fileContent == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    int index = 0;
    int contentIndex = 0;
    int spaceCount = 0;
    while(input[index] != '\0')
    {
        if(input[index] == ' ')
        {
            spaceCount++;
            if(spaceCount == 2)
            {
                index++;
                continue;
            }
        }
        if(spaceCount >= 2)
        {
            fileContent[contentIndex] = input[index];
            contentIndex++;
        }
        index++;
    }
    fileContent[contentIndex] = '\0';
    return fileContent;
}

void createNewFile(fileNode *parent, char *fileName)
{
    if (strstr(fileName, ".txt") == NULL)
    {
        printf("Invalid text file name");
        return;
    }
    int i = strstr(fileName, ".txt");
    int j = strchr(fileName, '\0');

    if (j - i != 4)
    {
        printf("Invalid file name");
        return;
    }
    fileNode *newFile = (fileNode*)malloc(sizeof(fileNode));
    if (newFile == NULL) {
        printf("Memory allocation failes\n");
        return;
    }
    strcpy(newFile->fileName, fileName);
    newFile->blockPointer = NULL;
    newFile->parent = parent;
    newFile->child = NULL;
    newFile->filetype = 't';
    if(parent->child == NULL)
    {
        parent->child = newFile;
        newFile->next = newFile;
        return;
    }
    fileNode *ptr = parent->child;
    while(ptr->next != parent->child)
    {
        ptr = ptr->next;
    }
    ptr->next = newFile;
    newFile->next = parent->child;
}

void createNewDir(fileNode *parent, char *dirName)
{
    if (strstr(dirName, ".txt") != NULL || strchr(dirName, '.') != NULL)
    {
        printf("Invalid directory name");
        return;
    }

    fileNode *newDir = (fileNode*)malloc(sizeof(fileNode));
    if (newDir == NULL) {
        printf("Memory allocation failes\n");
        return;
    }
    strcpy(newDir->fileName, dirName);
    newDir->blockPointer = NULL;
    newDir->parent = parent;
    newDir->child = NULL;
    newDir->filetype = 'd';
    if(parent->child == NULL)
    {
        parent->child = newDir;
        newDir->next = newDir;
        return;
    }
    fileNode *ptr = parent->child;
    while(ptr->next != parent->child)
    {
        ptr = ptr->next;
    }
    ptr->next = newDir;
    newDir->next = parent->child;
}

fileNode *moveToDir(fileNode *parent, char *dirName)
{
    if (strstr(dirName, ".txt") != NULL && strstr(dirName, "..") == NULL)
    {
        printf("Invalid directory name");
        return;
    }
    if(strstr(dirName, "..") != NULL)
    {
        if(parent->parent != NULL)
        {
            parent = parent->parent;
            return parent;
        }
        printf("You are in the root directory");
        return NULL;
    }
    fileNode *ptr = parent->child;
    while(ptr->next != parent->child)
    {
        if(strcmp(ptr->fileName, dirName) == 0 && ptr->filetype == 'd')
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    printf("No such dirctory exist\n");
    return parent;
}

void listAllConent(fileNode *parent)
{
    if(parent->child == NULL)
    {
        printf("(empty)\n");
        return;
    }
    fileNode *ptr = parent->child;
    while(ptr->next != parent->child)
    {
        printf("%s\n", ptr->fileName);
        ptr = ptr->next;
    }
    printf("%s\n", ptr->fileName);
}

void removeDirectory(fileNode *parent, char *dirName)
{
    if (strstr(dirName, ".txt") != NULL || strchr(dirName, '.') != NULL)
    {
        printf("Invalid directory name");
        return;
    }
    if(parent->child == NULL)
    {
        printf("current directory is empty\n");
        return;
    }
    fileNode *ptr = parent->child;
    fileNode *lastNode = parent->child;
    while (lastNode->next != parent->child)
    {
        lastNode = lastNode->next;
    }

    if (strcmp(ptr->fileName, dirName) == 0)
        {
            if(ptr->filetype == 'd' && ptr->child == NULL && ptr == lastNode)
            {
                parent->child = NULL;
                free(ptr);
                return;
            }
            if (ptr->filetype == 'd' && ptr->child == NULL)
            {
                parent->child = ptr->next;
                lastNode->next = parent->child;
                free(ptr);
                return;
            }
        }

    while (ptr->next != parent->child)
    {
        if (strcmp(ptr->fileName, dirName) == 0)
        {
            if(ptr->filetype == 'd' && ptr->child == NULL)
            {
                fileNode *temp = parent->child;
                while(temp->next != ptr)
                {
                    temp = temp->next;
                }
                temp->next = ptr->next;
                free(ptr);
                return;
            }
        }
    }
}

// void writeToFile(fileNode *file, char *content, byte *virtualDisk, freeList *freeListHead)
// {
//     const int contentSize = strlen(content);
//     const int dataSize = (contentSize + BLOCK_SIZE - 1)/BLOCK_SIZE;
//     if (dataSize > TOTAL_BLOCKS)
//     {
//         printf("File size exceeds the maximum limit\n");
//         return;
//     }

//     freeList *ptr = freeListHead;
//     freeList *keepPtr = freeListHead;
//     int allocatedBlocks = 0;

//     for (int i = 0; i < dataSize; i++)
//     {
//         int blockIndex = ptr->index;
//     }

//     while(ptr != NULL && allocatedBlocks < dataSize)
//     {
//         keepPtr = keepPtr->next;
//         keepPtr->prev = NULL;
//         free(ptr);
//         ptr = keepPtr;
//         allocatedBlocks++;
//     }
// }

int main() {
    byte *virtualDisk = (byte*)calloc(TOTAL_BLOCKS * BLOCK_SIZE, sizeof(byte));
    if (virtualDisk == NULL) {
        printf("Memory allocation failes\n");
        return 1;
    }

    freeList *head = createFreeList();

    fileNode *root = (fileNode*)malloc(sizeof(fileNode));
    if(root == NULL) {
        printf("Memory allocation failes\n");
        return 1;
    }
    fileNode *cwd = NULL;
    strcpy(root->fileName, "/");
    if(strstr(root->fileName, "txt") != NULL){
        root->filetype = 't';
    }
    else {
        root->filetype = 'd';
    }
    root->parent = NULL;
    root->next = NULL;
    root->blockPointer = NULL;
    root->child = NULL;

    cwd = root;
    char input[100];
    fgets(input, sizeof(input), stdin);
    char *command = extractCommand(input);
    char *fileName = extractFileName(input);

    if (strcmp(command, "mkdir") == 0)
    {
        if (fileName != NULL)
        {
            createNewDir(cwd, fileName);
        }
    }
    else if (strcmp(command, "create") == 0)
    {
        if (fileName != NULL)
        {
            createNewFile(cwd, fileName);
        } 
    }
    else if (strcmp(command, "cd") == 0)
    {
        if (fileName != NULL)
        {
            cwd = moveToDir(cwd, fileName);
        }
    }
    else if (strcmp(command, "ls") == 0)
    {
        if(fileName == NULL)
        {
            listAllConent(cwd);
        }
    }
    else if (strcmp(command, "write") == 0)
    {
    

    }
    else if (strcmp(command, "delete") == 0)
    {

    }
    else if (strcmp(command, "rmdir") == 0)
    {
        if (fileName != NULL)
        {
            removeDirectory(cwd, fileName);
        }
    }
    else
    {

    }

    return 0;
    
}