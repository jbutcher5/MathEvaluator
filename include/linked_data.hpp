#pragma once

#include <cstddef>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

template <typename T>
struct Node{
  T* data;
  Node<T>* next = NULL;
};

template <typename T>
class ll{
public:
  Node<T>* head = NULL;
  Node<T>* tail = NULL;

  bool listCheck();
  void appendItem(const T);
  void removeItem(const size_t);
  size_t getIndex(const T);
  bool inList(const T);
  size_t getLength();
  void freeAll();
  void convertVec(const std::vector<T>);
};

template <typename T>
bool ll<T>::listCheck(){
  return (head != NULL) && (tail != NULL);
}

template <typename T>
void ll<T>::appendItem(const T data){

  Node<T>* new_node = new Node<T>;
  T* new_data = new T;

  *new_data = data;

  new_node->data = new_data;

  if (head == NULL) head = new_node;
  if (tail != NULL) tail->next = new_node;

  tail = new_node;
}

template <typename T>
void ll<T>::removeItem(const size_t index){
  if (!listCheck()) return;

  // In-Case HEAD

  if (!index){
    Node<T>* to_link = head->next;

    delete head->data;
    delete head;

    head = to_link;

    return;
  }

  Node<T>* curr = head;

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
    tail = curr;
    return;
  }

  Node<T>* to_link = curr->next->next;

  delete curr->next->data;
  delete curr->next;

  curr->next = to_link;
}

template <typename T>
size_t ll<T>::getIndex(const T data){
  if (!listCheck()) return 0;

  Node<T>* curr = head;

  size_t i = 0;

  while (curr->next != NULL){
    if (*(curr->data) == data) return i;

    curr = curr->next;

    i++;
  }

  if (*(tail->data) == data) return i++;

  return 0;
}

template <typename T>
bool ll<T>::inList(const T data){
  if(!listCheck()) return false;

  if (*(head->data) == data) return true;

  if (getIndex(data)) return true;

  return false;
}

template <typename T>
size_t ll<T>::getLength(){
  if (!listCheck()) return 0;

  size_t total = 0;

  Node<T>* curr = head;

  while (curr->next == NULL){
    total++;
    curr = curr->next;
  }

  return total++;

}

template <typename T>
void ll<T>::freeAll(){
  if (!listCheck()) return;

  Node<T>* curr = head;

  while (curr->next != NULL){
    Node<T>* next = curr->next;
    delete curr->data;
    delete curr;

    curr = next;
  }

  delete curr->data;
  delete curr;

  head = NULL;
  tail = NULL;
}

template <typename T>
void ll<T>::convertVec(const std::vector<T> array){
  for (int i = 0; i < array.size(); i++){
    appendItem(array[i]);
  }
}
