#define MAX_DEPTH 50
#include <stdlib.h>
#include "functions.h"

typedef struct UndoNode {
    FractalParameters params;
    struct UndoNode* next;
} UndoNode;

static UndoNode* head = NULL;
static int depth = 0;

void clear_undo(void) {
    while (head) {
        UndoNode* temp = head;
        head = head->next;
        free(temp);
    }
    depth = 0;
}


void push(FractalParameters* params) {
    if (!params)
        return;

    UndoNode* node = malloc(sizeof(UndoNode));
    if (!node)
        return;

    node->params = *params;
    node->next = head;
    head = node;
    depth++;

    if (depth > MAX_DEPTH) {
        UndoNode* prev = NULL;
        UndoNode* curr = head;

        while (curr->next != NULL) {
            prev = curr;
            curr = curr->next;
        }

        if (prev)
            prev->next = NULL;

        free(curr);
        depth--;
    }
}

bool pop(FractalParameters* out) {
    if (!head)
        return false;

    *out = head->params;
    UndoNode* temp = head;
    head = head->next;
    free(temp);
    depth--;
    return true;
}