#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct CacheNode
{
    int key;
    char *token;
    struct CacheNode *next;
    struct CacheNode *previous;
} CacheNode;

typedef struct hashNode
{
    int key;
    CacheNode *cachePtr;
    struct hashNode *next;
} hashNode;

typedef struct hashTable
{
    hashNode **tableBoxes;
    int spaceFilled;
    int capacity;
} hashTable;

int Capacity = 0;
CacheNode *MRU = NULL;
CacheNode *LRU = NULL;

int hashFunction(int key)
{
    if (Capacity == 0) return 0;
    return key % Capacity;
}


hashTable createCache(int capacity)
{
    hashTable cache;
    cache.tableBoxes = malloc(capacity * sizeof(hashNode*));
    if(cache.tableBoxes == NULL)
    {
        printf("Memory allocation failed.\n");
        cache.tableBoxes = NULL;
        cache.spaceFilled = 0;
        cache.capacity = 0;
        return cache;
    }
    for (int i = 0; i < capacity; i++)
    {
        cache.tableBoxes[i] = NULL;
    }
    cache.capacity = capacity;
    cache.spaceFilled = 0;

    return cache;
}

void insertRecent(int key, char *token) 
{
    CacheNode *newNode = malloc(sizeof(CacheNode));
    if(newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }
    newNode->key = key;
    newNode->token = token;
    newNode->previous = NULL;
    newNode->next = MRU;

    if(MRU == NULL)
    {
        MRU = LRU = newNode;
    }
    else
    {
        MRU->previous = newNode;
        MRU = newNode;
    }
}

int removeLeast()
{
    if(MRU == NULL || LRU == NULL)
    {
        printf("No recent key!\n");
        return -1;
    }

    int key = LRU->key;
    CacheNode *temp = LRU;
    LRU = temp->previous;
    if(LRU == NULL)
    {
        MRU = NULL;
    }
    else
    {
    LRU->next = NULL;
    }
    free(temp->token);
    free(temp);
    return key;
}

hashNode *searchHash(hashTable *cache, int key)
{
    if(cache == NULL || cache->tableBoxes == NULL) return NULL;

    int index = hashFunction(key);
    hashNode *current = cache->tableBoxes[index];

    while(current)
    {
        if(current->key == key)
            return current;
        current = current->next;
    }

    return NULL;
}

void insertHash(hashTable *cache, int key, CacheNode *node) {
    if (cache == NULL || cache->tableBoxes == NULL) return;
    int index = hashFunction(key);
    hashNode *ptr = malloc(sizeof(hashNode));
    if (!ptr) {
        printf("Memory allocation failed\n");
        return;
    }
    ptr->key = key;
    ptr->cachePtr = node;
    ptr->next = cache->tableBoxes[index];
    cache->tableBoxes[index] = ptr;
}

void deleteHash(hashTable *cache, int key)
{
    if(cache == NULL || cache->tableBoxes == NULL) return;

    int index = hashFunction(key);
    hashNode *current = cache->tableBoxes[index];
    hashNode *previousHashNode = NULL;

    while(current)
    {
        if(current->key == key)
        {
            if(previousHashNode)
                previousHashNode->next = current->next;
            else
                cache->tableBoxes[index] = current->next;

            free(current);
            return;
        }

        previousHashNode = current;
        current = current->next;
    }
}

void put(int key, char *token)
{
    extern hashTable cache;
    hashNode *node = searchHash(&cache, key);

    if(node)
    {
        CacheNode *cacheNode = node->cachePtr;
        free(cacheNode->token);
        cacheNode->token = strdup(token);
        moveToRecent(cacheNode);
        return;
    }

    if(cache.spaceFilled >= cache.capacity)
    {
        int removedKey = removeLeast();
        if(removedKey != -1)
        {
            deleteHash(&cache, removedKey);
            cache.spaceFilled--;
        }
    }

    insertRecent(key, token);
    insertHash(&cache, key, MRU);
    cache.spaceFilled++;
}

char* getValue(int key)
{
    extern hashTable cache;

    hashNode *node = searchHash(&cache, key);
    if(node == NULL)
        return NULL;

    moveToRecent(node->cachePtr);
    return node->cachePtr->token;
}

void moveToRecent(CacheNode *node) {
    if (node == NULL || node == MRU) return;

    if (node->previous)
        node->previous->next = node->next;
    if (node->next)
        node->next->previous = node->previous;

    if (node == LRU) {
        LRU = node->previous;
    }

    node->previous = NULL;
    node->next = MRU;
    if (MRU)
        MRU->previous = node;
    MRU = node;

    if (LRU == NULL) LRU = MRU;
}

hashTable cache;

int main()
{
    int capacityInput;
    printf("Enter cache size: ");
    scanf("%d", &capacityInput);

    Capacity = capacityInput;
    cache = createCache(capacityInput);

    int choice;
    while(1)
    {
        printf("\n1. Put data\n2. Get data\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if(choice == 1)
        {
            int keyInput;
            char tokenInput[100];

            printf("Enter key: ");
            scanf("%d", &keyInput);

            printf("Enter token: ");
            scanf("%s", tokenInput);

            put(keyInput, tokenInput);
            printf("Data stored successfully.\n");
        }
        else if(choice == 2)
        {
            int searchKey;
            printf("Enter key to search: ");
            scanf("%d", &searchKey);

            char *result = getValue(searchKey);

            if(result == NULL)
                printf("NULL\n");
            else
                printf("%s\n", result);
        }
        else if(choice == 3)
        {
            printf("Program ended.\n");
            break;
        }
        else
        {
            printf("Invalid input.\n");
        }
    }

    return 0;
}