#include <string.h>

#include "functions.h"

void split_list(ImageNode* source, ImageNode** frontRef, ImageNode** backRef) {
    // создаем два указателя. slow двигается на 1 узел вперед, fast на 2
    ImageNode* slow = source;
    ImageNode* fast = source->next;

    // двигаем указатели пока fast не дойдет до конца
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // начало левой части совпадает с исходной головой
    *frontRef = source;

    // на начало второй части указывает slow.next
    *backRef = slow->next;

    // отделяем правую часть
    slow->next = NULL;
}

ImageNode* merge(ImageNode* node_a, ImageNode* node_b) {
    // создаем головной элемент для создания списка
    ImageNode dummy;
    ImageNode* tail = &dummy;
    dummy.next = NULL;

    // двигаем указатели списков пока один не дойдет до конца своего списка
    while (node_a != NULL && node_b != NULL) {
        // добавляем элемент первого списка, если он меньше
        if (strcmp(node_a->fract_name, node_b->fract_name) <= 0) {
            tail->next = node_a;
            node_a = node_a->next;
        }
        // иначе второго
        else {
            tail->next = node_b;
            node_b = node_b->next;
        }

        // двигаем указатель полученного списка
        tail = tail->next;
    }

    // если дошли до конца одного списка, то добавляем конец другого
    if (node_a != NULL)
        tail->next = node_a;
    else
        tail->next = node_b;

    return dummy.next;
}

ImageNode* merge_sort(ImageNode* head) {
    if (!head || !head->next)
        return head;

    // разделяем список на две части
    ImageNode* front;
    ImageNode* back;
    split_list(head, &front, &back);

    // рекурсивно сортируем левую и правую части
    front = merge_sort(front);
    back = merge_sort(back);

    // получим отсортированный подсписок слиянием
    return merge(front, back);
}

void update_gallery_positions(ImageNode* head) {
    ImageNode* current = head->next;
    int i = 0;

    while (current != NULL) {
        // Используем точно такие же формулы и константы, как при первой загрузке
        const int img_height = 280, img_width = 330;
        const float y_start = 90, x_start = 115, x_indent = img_width + 70, y_indent = img_height + 35;

        current->field = (Rectangle) {
            x_start + (float) (i % 4) * x_indent,
            y_start + (i / 4) * y_indent,
            (float)img_width,
            (float)img_height
        };

        current = current->next;
        i++;
    }
}