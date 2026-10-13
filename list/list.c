// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

node_t * getNode(elem value) {
  node_t *mynode = (node_t *) malloc(sizeof(node_t));
  if (mynode == NULL) {
    return NULL; // Allocation failed
  }
  mynode->value = value;
  mynode->next = NULL;

  return mynode;
}

list_t *list_alloc() { 
  list_t* mylist =  (list_t *) malloc(sizeof(list_t)); 
  if (mylist != NULL) {
    mylist->head = NULL;
  }
  return mylist;
}

void list_free(list_t *l) {
  if (l == NULL) return;

  node_t *curr = l->head;
  while (curr != NULL) {
    node_t *tmp = curr;
    curr = curr->next;
    free(tmp);
  }
  free(l);
}

void list_print(list_t *l) {
  if (l == NULL) {
    printf("NULL\n");
    return;
  }
  char *str = listToString(l);
  if (str != NULL) {
    printf("%s\n", str);
    free(str);
  }
}

/*
 * Converts the list into a string representation.
 * Allocates a fixed buffer size suitable for standard test list lengths.
 */
char * listToString(list_t *l) {
  char* buf = (char *) malloc(sizeof(char) * 10024);
  if (buf == NULL) return NULL;
  buf[0] = '\0';

  char tbuf[20];
	node_t* curr = l->head;

  while (curr != NULL) {
    sprintf(tbuf, "%d->", curr->value);
    strcat(buf, tbuf);
    curr = curr->next;
  }
  strcat(buf, "NULL");
  return buf;
}

int list_length(list_t *l) {
  if (l == NULL) return 0;

  int len = 0;
  node_t *curr = l->head;
  while (curr != NULL) {
    len++;
    curr = curr->next;
  }
  return len;
}

void list_add_to_back(list_t *l, elem value) {
  if (l == NULL) return;

  node_t *mynode = getNode(value);
  if (mynode == NULL) return;

  if (l->head == NULL) {
    l->head = mynode;
    return;
  }

  node_t *curr = l->head;
  while (curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = mynode;
}

void list_add_to_front(list_t *l, elem value) {
     if (l == NULL) return;

     node_t *cur_node = getNode(value);
     if (cur_node == NULL) return;

     cur_node->next = l->head;
     l->head = cur_node;
}

void list_add_at_index(list_t *l, elem value, int index) {
  if (l == NULL || index < 1) return;

  if (index == 1) {
    list_add_to_front(l, value);
    return;
  }
  node_t *curr = l->head;
  // Traverse to the node immediately preceding the target index
  for (int i = 1; curr != NULL && i < index - 1; i++) {
    curr = curr->next;
  }

  if (curr == NULL) return;

  node_t *mynode = getNode(value);
  if (mynode == NULL) return;

  mynode->next = curr->next;
  curr->next = mynode;
}

elem list_remove_from_back(list_t *l) {
  if (l == NULL || l->head == NULL) return -1;

  if (l->head->next == NULL) {
    elem val = l->head->value;
    free(l->head);
    return val;
  }

  node_t *curr = l->head;
  while (curr->next->next != NULL) {
    curr = curr->next;
  }

  elem val = curr->next->value;
  free(curr->next);
  curr->next = NULL;

  return val;
}

elem list_remove_from_front(list_t *l) {
  if (l == NULL || l->head == NULL) return -1;
  node_t *tmp = l->head;
  elem val = tmp->value;
  l->head = l->head->next;
  free(tmp);
  return val;
}

elem list_remove_at_index(list_t *l, int index) { 
  if (l ==NULL || l->head == NULL || index < 1) return -1; 

  if (index == 1){
    return list_remove_from_front(l);
  }

  node_t *curr = l->head;
  for (int i = 1; curr != NULL && i < index - 1; i++) {
    curr = curr->next;
  }

  if (curr == NULL || curr->next == NULL) return -1;

  node_t *tmp = curr->next;
  elem val = tmp->value;
  curr->next = tmp->next;
  free(tmp);

  return val;
}

bool list_is_in(list_t *l, elem value) {
  if (l == NULL) return false;
  node_t *curr = l->head;
  while (curr != NULL) {
    if (curr->value == value) {
      return true;
    }
    curr = curr->next;
  }
  return false;
}

elem list_get_elem_at(list_t *l, int index) {
  if(l == NULL || index < 1) return -1;
  node_t *curr = l->head;
  for (int i = 1; curr != NULL; i++) {
    if (i == index) {
      return curr->value;
    }
    curr = curr->next;
  }
  return -1;
}

int list_get_index_of(list_t *l, elem value) {
  if (l == NULL) return -1;
  node_t *curr = l->head;
  int index = 1;

  while (curr != NULL) {
    if (curr->value == value) {
      return index;
    }
    curr = curr->next;
    index++;
  }
  return -1;
}

