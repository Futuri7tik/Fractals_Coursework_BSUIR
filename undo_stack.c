#define MAX_DEPTH 50
#include <stdlib.h>
#include "functions.h"

// структура узла стека
typedef struct UndoNode {
    FractalParameters params;
    struct UndoNode* next;
} UndoNode;

// статическая переменная вершины стека
static UndoNode* head = NULL;

// глубина стека
static int depth = 0;

// очистка стека
void clear_undo(void) {
    while (head) {
        UndoNode* temp = head;
        head = head->next;
        free(temp);
    }
    depth = 0;
}

// добавление элемента в стек
void push(FractalParameters* params) {
    // проверка на неинициализированный стек
    if (!params)
        return;

    // выделение памяти под новый узел
    UndoNode* node = malloc(sizeof(UndoNode));
    if (!node)
        return;

    // занесение данных в новый узел
    node->params = *params;
    node->next = head;

    // теперь вершина - новый узел
    head = node;
    depth++;

    // удаление последнего элемента при превышении максимальной глубины стека
    if (depth > MAX_DEPTH) {
        // сохраняем текущий и прошлый узел
        UndoNode* prev = NULL;
        UndoNode* curr = head;

        // двигаем указатели до конца стека
        while (curr->next != NULL) {
            prev = curr;
            curr = curr->next;
        }

        // отделяем предпоследний узел от конца
        if (prev)
            prev->next = NULL;

        // очищаем дно стека
        free(curr);
        depth--;
    }
}

// удаление верхнего элемента стека
bool pop(FractalParameters* out) {
    // если нет головы, то стек пуст
    if (!head)
        return false;

    // заносим вершину стека
    *out = head->params;

    // очищаем верхний элемент
    UndoNode* temp = head;
    head = head->next;
    free(temp);
    depth--;
    return true;
}