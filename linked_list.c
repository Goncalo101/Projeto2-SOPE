#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

void push(node_t* head, tlv_request_t val)
{
    node_t* current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (node_t*)malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
}

int list_size_empty(node_t* head)
{

    node_t* current = head;
    if (head == NULL) {
        return 1;
    } else
        return 0;
}

void pop(node_t** head)
{
    node_t* next = NULL;

    if (*head == NULL) {
        return;
    }

    next = (*head)->next;
    free(*head);
    *head = next;
}
