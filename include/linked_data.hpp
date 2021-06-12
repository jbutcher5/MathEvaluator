#pragma once

#include <cstddef>
#include <stdlib.h>
#include <string>
#include <iostream>

template <typename T>
struct Node{
  T* data;
  Node<T>* next = NULL;
};

template <typename T>
struct ll{
  Node<T>* head = NULL;
  Node<T>* tail = NULL;
};

template <typename T>
void appendItem(T data, ll<T>& list){

  Node<T>* new_node = new Node<T>;
  T* new_data = new T;

  *new_data = data;

  new_node->data = new_data;

  if (list.head == NULL) list.head = new_node;
  if (list.tail != NULL) list.tail->next = new_node;

  list.tail = new_node;
}

template <typename T>
void removeItem(size_t index, ll<T>& list){
  // In-Case HEAD

  if (!index){
    Node<T>* to_link = list.head->next;

    delete list.head->data;
    delete list.head;

    list.head = to_link;

    return;
  }

  Node<T>* curr = list.head;

  size_t i = 0;

  while (i != index-1) {
    if (curr->next == NULL && i != index){
      return;
    }

    curr = curr->next;

    i++;
  }

  // In-Case TAIL

  if (curr->next->next == NULL){
    delete curr->next->data;
    delete curr->next;
    curr->next = NULL;
    list.tail = curr;
    return;
  }

  Node<T>* to_link = curr->next->next;

  delete curr->next->data;
  delete curr->next;

  curr->next = to_link;
}
