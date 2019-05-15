#include <stdlib.h>

#include "linked_list.h"

void push(node_t *head, tlv_request_t val) {
  node_t *current = head;

  while (current->next != NULL) {
    current = current->next;
  }

  current->next = (node_t *) malloc(sizeof(node_t));
  current->next->val = val;
  current->next->next = NULL;
}

size_t list_size(node_t *head) {
  node_t *current = head;
  size_t size = 1;
  
  if (head == NULL) return 0;

  while (current->next != NULL) {
    current = current->next;
    ++size;
  }

  return size;
}

void pop(node_t **head) {
  node_t *next = NULL;

  if (*head == NULL) {
    return;
  }

  next = (*head)->next;
  free(*head);
  *head = next;
}
