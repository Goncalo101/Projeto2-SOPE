#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "types.h"

typedef struct node {
  tlv_request_t val;
  size_t id;
  struct node *next;
} node_t;

void push(node_t* head, tlv_request_t val);
void push_with_id(node_t* head, tlv_request_t val, size_t id);
size_t list_size(node_t* head);
tlv_request_t value_at(node_t* head, size_t index);
void pop(node_t **head);

#endif /* _LINKED_LIST_H */
