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

void push_with_id(node_t *head, tlv_request_t val, size_t id) {
  node_t *current = head;

  while (current->next != NULL) {
    current = current->next;
  }

  current->next = (node_t *) malloc(sizeof(node_t));
  current->next->val = val;
  current->next->id = id;
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

tlv_request_t value_at(node_t *head, size_t index) {
  node_t *current = head;
  size_t i = 0, size = list_size(current);

  if (index >= size) {
    tlv_request_t tlv_request_t;
    return tlv_request_t;
  }

  while (i != index) {
    current = current->next;
    ++i;
  }

  return current->val;
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
