#include "types.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

extern struct NODE* root;
extern struct NODE* cwd;

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW

    size_t length = strlen(pathName);

    if (length == 1 && pathName[0] == '/') {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char* baseName = (char*) malloc(sizeof(char) * length + 1);
    char* dirName = (char*) malloc(sizeof(char) * length + 1);

    struct NODE* parentNode = splitPath(pathName, baseName, dirName);

    if (parentNode == NULL) {
        return;
    }

    // Check if the dir already exists
    struct NODE* lastNode = parentNode->childPtr;
    for (struct NODE* node = parentNode->childPtr; node != NULL; node = node->siblingPtr) {
        if (strcmp(baseName, node->name) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        lastNode = node;
    }

    // Initialize the new node
    struct NODE* newNode = malloc(sizeof(struct NODE));
    strcpy(newNode->name, baseName);
    newNode->fileType = 'D';
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = parentNode;

    // Link the new node into the sibling linked list
    if (lastNode == NULL) {
        parentNode->childPtr = newNode;
    } else {
        lastNode->siblingPtr = newNode;
    }

    // Print success message
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
    //

    // Detect absolute path or not
    bool isAbsolue = pathName[0] == '/';
    struct NODE* currentNode = isAbsolue ? root : cwd;

    // Fill dirName properly
    size_t lastCopyEnd = 0;
    for (size_t i = isAbsolue /* skip no-op on first slash */; pathName[i] != '\0'; i++) {
        if (pathName[i] == '/') {
            // Check if directory exists in children of `current_node`
            if (currentNode->fileType != 'D' || currentNode->childPtr == NULL) {
                pathName[i] = '\0';
                printf("ERROR: directory %s does not exist\n", pathName + lastCopyEnd + 1);
                return NULL;
            } else {
                bool foundChild = false;
                for (struct NODE* node = currentNode->childPtr; node != NULL; node = node->siblingPtr) {
                    if (strncmp(node->name, pathName + lastCopyEnd + isAbsolue, i - lastCopyEnd - isAbsolue) == 0) {
                        foundChild = true;
                        currentNode = node;
                        break;
                    }
                }

                if (!foundChild) {
                    pathName[i] = '\0';
                    printf("ERROR: directory %s does not exist\n", pathName + lastCopyEnd + isAbsolue);
                    return NULL;
                }
            }

            // Flush to dirName
            strncpy(dirName + lastCopyEnd, pathName + lastCopyEnd, i - lastCopyEnd);
            lastCopyEnd = i;
        }
    }
    dirName[lastCopyEnd] = '\0';

    // Fill baseName with the rest
    strcpy(baseName, pathName + lastCopyEnd + !(!isAbsolue && lastCopyEnd == 0));

    return currentNode;
}
