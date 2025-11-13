#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

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
    int dataSize;
    int contentSize;
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
    if(index == 0)
    {
        printf("not found command\n");
        free(command);
        return NULL;
    }
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
    if(fileIndex == 0)
    {
        printf("not found file name\n");
        free(fileName);
        return NULL;
    }
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
    if(contentIndex == 0)
    {
        printf("not found file content\n");
        free(fileContent);
        return NULL;
    }
    return fileContent;
}

void createNewFile(fileNode *parent, char *fileName)
{
    if (strstr(fileName, ".txt") == NULL)
    {
        printf("Invalid text file name");
        return;
    }
    char *i = strstr(fileName, ".txt");
    char *j = strchr(fileName, '\0');

    int indexFirst = i - fileName;
    int indexSecond = j - fileName;
    if (indexSecond - indexFirst != 4)
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
    newFile->dataSize = 0;
    newFile->contentSize = 0;
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
    newDir->dataSize = 0;
    newDir->contentSize = 0;
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
        return parent;
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
    if(ptr == NULL)
    {
        printf("Empty directory\n");
        return parent;
    }
    while(ptr->next != parent->child)
    {
        if(strcmp(ptr->fileName, dirName) == 0 && ptr->filetype == 'd')
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    if(strcmp(ptr->fileName, dirName) == 0 && ptr->filetype == 'd')
    {
        return ptr;
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
        if(ptr->filetype == 'd')
        {
            printf("%s/\n", ptr->fileName);
        }
        else
        {
            printf("%s\n", ptr->fileName);
        }
        ptr = ptr->next;
    }
    if(ptr->filetype == 'd')
    {
        printf("%s/\n", ptr->fileName);
    }
    else
    {
        printf("%s\n", ptr->fileName);
    }
}

void retunrPath(fileNode  *currentFile)
{
    if(currentFile->parent == NULL)
    {
        printf("%s", currentFile->fileName);
        return;
    }
    retunrPath(currentFile->parent);
    printf("/%s", currentFile->fileName);
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

freeList *writeToFile(fileNode *file, char *content, byte (*virtualDisk)[BLOCK_SIZE], freeList *freeListHead)
{
    const int contentSize = strlen(content);
    const int dataSize = (contentSize + BLOCK_SIZE - 1)/BLOCK_SIZE;
    if (dataSize > TOTAL_BLOCKS)
    {
        printf("File size exceeds the maximum limit\n");
        return freeListHead;
    }

    file->blockPointer = (int*)malloc(dataSize * sizeof(int));
    if (file->blockPointer == NULL)
    {
        printf("Memory allocation failed for block pointers\n");
        return freeListHead;
    }
    file->dataSize = dataSize;
    file->contentSize = contentSize;
    freeList *ptr = freeListHead;
    freeList *keepPtr = freeListHead;
    int allocatedBlocks = 0;
    while(ptr != NULL && allocatedBlocks < dataSize)
    {
        for (int i = 0; i < contentSize  && i < BLOCK_SIZE; i++)
        {
            virtualDisk[ptr->index][i] = content[i + allocatedBlocks * BLOCK_SIZE];
        }
        file->blockPointer[allocatedBlocks] = ptr->index;
        keepPtr = keepPtr->next;
        if (keepPtr != NULL)
        {
            keepPtr->prev = NULL;
        }
        free(ptr);
        ptr = keepPtr;
        allocatedBlocks++;
    }

    return ptr;
}

void readFile(fileNode *file, byte (*virtualDisk)[BLOCK_SIZE])
{
    for (int i = 0; i < file->dataSize; i++)
    {
        for (int j = 0; j < file->contentSize; j++)
        {
            printf("%c", virtualDisk[file->blockPointer[i]][j]);
        }
        printf("\n");
    }
}

freeList *freeBlocks(fileNode *file, freeList *head, freeList *tail)
{
    for (int i = 0; i < file->dataSize; i++)
    {
        freeList *freeNode = (freeList*)malloc(sizeof(freeList));
        if (freeNode == NULL)
        {
            printf("Memory allocation failed for free list node\n");
            return head;
        }
        freeNode->index = file->blockPointer[i];
        freeNode->next = NULL;
        freeNode->prev = tail;
        tail->next = freeNode;
        tail = freeNode;
    }
    return head;
}

freeList *deleteFile(fileNode *parent, char *fileName, freeList *listHead)
{
    if(strstr(fileName, ".txt") == NULL)
    {
        printf("Invalid text file name");
        return listHead;
    }
    freeList *tail = listHead;
    while (tail->next != NULL)
    {
        tail = tail->next;
    }
    
    fileNode *ptr = parent->child;
    if(ptr == NULL)
    {
        printf("current directory is empty\n");
        return listHead;
    }
    if(ptr->next == parent->child)
    {
        if(strcmp(ptr->fileName, fileName) == 0 && ptr->filetype == 't')
        {
            listHead = freeBlocks(ptr, listHead, tail);
            parent->child = NULL;
            free(ptr->blockPointer);
            free(ptr);
            return listHead;
        }
        printf("No such file exist\n");
        return listHead;
    }
    while(ptr->next != parent->child)
    {
        if(strcmp(ptr->fileName, fileName) == 0 && ptr->filetype == 't')
        {
            listHead = freeBlocks(ptr, listHead, tail);
            fileNode *temp = parent->child;
            while(temp->next != ptr)
            {
                temp = temp->next;
            }
            temp->next = ptr->next;
            free(ptr->blockPointer);
            free(ptr);
            return listHead;
        }
    }
    if(ptr != parent->child)
    {
        if(strcmp(ptr->fileName, fileName) == 0 && ptr->filetype == 't')
        {
            listHead = freeBlocks(ptr, listHead, tail);
            fileNode *temp = parent->child;
            while(temp->next != ptr)
            {
                temp = temp->next;
            }
            temp->next = ptr->next;
            free(ptr->blockPointer);
            free(ptr);
            return listHead;
        }
    }
    printf("No such file exist\n");
    return listHead;
}

void storageDetails(freeList *head)
{
    int countFreeNodes = 0;
    float usage = 0.00;
    freeList *ptr = head;
    while (ptr != NULL)
    {
        countFreeNodes++;
        ptr = ptr->next;
    }
    int usedBlocks = TOTAL_BLOCKS - countFreeNodes;
    usage = (usedBlocks / (float)TOTAL_BLOCKS) * 100;
    printf("Total Blocks: %d\n", TOTAL_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", countFreeNodes);
    printf("Disk Usage: %.2f%%\n", usage);
}

int main() {
    byte virtualDisk[TOTAL_BLOCKS][BLOCK_SIZE];

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
    root->dataSize = 0;
    root->contentSize = 0;
    root->next = NULL;
    root->blockPointer = NULL;
    root->child = NULL;

    cwd = root;
    char input[100];

    printf("$ ./vfs\nCompact VFS - ready. Type 'exit' to quit.\n");
    while(1)
    {
        printf("%s >", cwd->fileName);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char *command = extractCommand(input);
        char *fileName = NULL;
        if(strcmp(command, "ls") != 0 && strcmp(command, "df") != 0 && strcmp(command, "pwd") != 0 && strcmp(command, "exit") != 0)
        {
            fileName = extractFileName(input);
        }

        if (strcmp(command, "exit") == 0)
        {
            break;
        }
        if (strcmp(command, "mkdir") == 0)
    {
        if (fileName != NULL)
        {
            createNewDir(cwd, fileName);
            printf("Dirctory %s created successfully\n", fileName);
            free(fileName);
            free(command);
        }
    }
    else if (strcmp(command, "create") == 0)
    {
        if (fileName != NULL)
        {
            createNewFile(cwd, fileName);
            printf("File %s created successfully\n", fileName);
            free(fileName);
            free(command);
        } 
    }
    else if (strcmp(command, "cd") == 0)
    {
        if (fileName != NULL)
        {
            cwd = moveToDir(cwd, fileName);
            printf("Moved to /%s\n", cwd->fileName);
            free(fileName);
            free(command);
        }
    }
    else if (strcmp(command, "ls") == 0)
    {
        if(fileName == NULL)
        {
            listAllConent(cwd);
            free(command);

        }
    }
    else if (strcmp(command, "pwd") == 0)
    {
        if(fileName == NULL)
        {
            retunrPath(cwd);
            printf("\n");
            free(command);
        }
    }
    else if (strcmp(command, "read") == 0)
    {
        if(fileName != NULL)
        {
            fileNode *ptr = cwd->child;
            bool found = false;
            while(ptr->next != cwd->child)
            {
                if(strcmp(ptr->fileName, fileName) == 0 && ptr->filetype == 't')
                {
                    readFile(ptr, virtualDisk);
                    found = true;
                    break;
                }
                ptr = ptr->next;
            }
            if(!found && strcmp(ptr->fileName, fileName) == 0 && ptr->filetype == 't')
            {
                readFile(ptr, virtualDisk);
                found = true;
            }
            if(!found)
            {
                printf("No such file exist\n");
            }
            free(fileName);
            free(command);
        }
    }
    else if (strcmp(command, "write") == 0)
    {
        char *fileContent = extractFileContent(input);
        if(fileName != NULL && fileContent != NULL)
        {
            fileNode *ptr = cwd->child;
            int found = 0;
            while(ptr->next != cwd->child)
            {
                if(strcmp(ptr->fileName, fileName) == 0 && ptr->filetype == 't')
                {
                    freeList *newhead = writeToFile(ptr, fileContent, virtualDisk, head);
                    head = newhead;
                    found = 1;
                    printf("Data written successfully [size: %d bytes]\n", strlen(fileContent));
                    free(fileContent);
                    break;
                }
                ptr = ptr->next;
            }
            if(!found && strcmp(ptr->fileName, fileName) == 0 && ptr->filetype == 't')
            {
                freeList *newhead = writeToFile(ptr, fileContent, virtualDisk, head);
                head = newhead;
                found = true;
                printf("Data written successfully [size: %d bytes]\n", strlen(fileContent));
                free(fileContent);
            }
            if(!found)
            {
                printf("No such file exist\n");
                free(fileContent);
            }
            free(fileName);
            free(command);
        }
    }
    else if (strcmp(command, "delete") == 0)
    {
        if (fileName != NULL)
        {
            freeList *newhead = deleteFile(cwd, fileName, head);
            head = newhead;
            printf("File deleted successfully\n");
            free(fileName);
            free(command);
        }
    }
    else if (strcmp(command, "rmdir") == 0)
    {
        if (fileName != NULL)
        {
            removeDirectory(cwd, fileName);
            free(fileName);
            free(command);
        }
    }
    else if (strcmp(command, "df") == 0)
    {
        if (fileName == NULL)
        {
            storageDetails(head);
            free(command);
        }
    }
    else
    {
        printf("Invalid command\n");
        if (fileName != NULL) free(fileName);
        fileName = NULL;
        if (command != NULL) free(command);
        command = NULL;
    }
    }

    freeList *freePtr = head;
    freeList *ptr = head;
    while(freePtr != NULL)
    {
        ptr = freePtr->next;
        free(freePtr);
        freePtr = ptr;
    }
    fileNode *filePtr = root;
    fileNode *fileTemp;
    while(filePtr != NULL)
    {
        fileTemp = filePtr->next;
        if(filePtr->blockPointer != NULL)
        {
            free(filePtr->blockPointer);
        }
        free(filePtr);
        filePtr = fileTemp;
    }
    
    return 0;
    
}