#pragma once

#include <cstddef>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <array>

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
bool listCheck(const ll<T>& list){
  return (list.head != NULL) && (list.tail != NULL);
}

template <typename T>
void appendItem(const T data, ll<T>& list){

  Node<T>* new_node = new Node<T>;
  T* new_data = new T;

  *new_data = data;

  new_node->data = new_data;

  if (list.head == NULL) list.head = new_node;
  if (list.tail != NULL) list.tail->next = new_node;

  list.tail = new_node;
}

template <typename T>
void removeItem(const size_t index, ll<T>& list){
  if (!listCheck<T>(list)) return;

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

template <typename T>
size_t getIndex(const T data, const ll<T> list){
  if (!listCheck<T>(list)) return 0;

  Node<T>* curr = list.head;

  size_t i = 0;

  while (curr->next != NULL){
    if (*(curr->data) == data) return i;

    curr = curr->next;

    i++;
  }

  if (*(list.tail->data) == data) return i++;

  return 0;
}

template <typename T>
bool inList(const T data, const ll<T> list){
  if(!listCheck<T>(list)) return false;

  if (*(list.head->data) == data) return true;

  if (getIndex<T>(data, list)) return true;

  return false;
}

template <typename T>
size_t getLength(ll<T>& list){
  if (!listCheck<T>(list)) return 0;

  size_t total = 0;

  Node<T>* curr = list.head;

  while (curr->next == NULL){
    total++;
    curr = curr->next;
  }

  return total++;

}

template <typename T>
void freeAll(ll<T>& list){
  if (!listCheck<T>(list)) return;

  Node<T>* curr = list.head;

  while (curr->next != NULL){
    Node<T>* next = curr->next;
    delete curr->data;
    delete curr;

    curr = next;
  }

  delete curr->data;
  delete curr;

  list.head = NULL;
  list.tail = NULL;
}

template <typename T, size_t S>
void convertArr(const std::array<T,S> array, ll<T>& list){
  for (int i = 0; i < array.size(); i++){
    appendItem<T>(array[i], list);
  }
}
