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
class list{
private:
  bool listCheck(){
    return (head != NULL) && (tail != NULL);
  }
public:
  Node<T>* head = NULL;
  Node<T>* tail = NULL;

  void append(const T data){
    Node<T>* new_node = new Node<T>;
    T* new_data = new T;

    *new_data = data;

    new_node->data = new_data;

    if (head == NULL) head = new_node;
    if (tail != NULL) tail->next = new_node;

    tail = new_node;
  }

  size_t size(){
    if (!listCheck()) return 0;

    Node<T>* curr = head;

    size_t size = 0;

    while (curr->next != NULL){
      size++;
      curr = curr->next;
    }

    size++;

    return size;
  }

  Node<T>* getNode(const size_t index){

    Node<T>* curr = head;

    size_t i = 0;

    if (index == size()-1) return tail;

    while (curr->next != NULL){
      if (i == index) return curr;
      curr = curr->next;
      i++;
    }

    return NULL;
  }

  T getData(const size_t index){
    return *(getNode(index)->data);
  }

  void remove(const size_t index){
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

  void freeAll(){
    while (size() > 0){
      remove(0);
    }
  }

  void appendVec(const std::vector<T> vec){
    for (T i : vec) append(i);
  }

  std::vector<T> exposeVec(){
    std::vector<T> ret;
    for (int i = 0; i < size(); i++){
      ret.push_back(getData(i));
    }
    return ret;
  }

  bool inList(const T data){
    for (int i = 0; i < size(); i++){
      if (getData(i) == data){
        return true;
      }
    }

    return false;
  }

};
