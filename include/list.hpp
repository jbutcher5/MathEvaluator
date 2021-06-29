#pragma once

template <typename T>
class List{
private:
  bool listCheck(){
    return (head != NULL) && (tail != NULL);
  }

  struct Node{
    T* data;
    Node* next = NULL;
  };

public:
  Node* head = NULL;
  Node* tail = NULL;

  void append(const T);
  size_t size();
  Node* getNode(const size_t);
  T getData(const size_t);
  void remove(const size_t);
  void freeAll();
  bool inList(const T);
};

template <class T>
void List<T>::append(const T data){
  Node* new_node = new Node;
  T* new_data = new T;

  *new_data = data;

  new_node->data = new_data;

  if (head == NULL) head = new_node;
  if (tail != NULL)
    tail->next = new_node;

  tail = new_node;
}

template <class T>
size_t List<T>::size(){
  if (!listCheck()) return 0;

  Node* curr = head;

  size_t size = 0;

  while (curr->next != NULL){
    size++;
    curr = curr->next;
  }

  size++;

  return size;
}

template <class T>
typename List<T>::Node* List<T>::getNode(const size_t index){

  Node* curr = head;

  size_t i = 0;

  if (index == size()-1) return tail;

  while (curr->next != NULL){
    if (i == index) return curr;
    curr = curr->next;
    i++;
  }

  return NULL;
}

template <class T>
T List<T>::getData(const size_t index){
  return *(getNode(index)->data);
}

template <class T>
void List<T>::remove(const size_t index){
  if (!listCheck()) return;

  // In-Case HEAD

  if (!index){
    Node* to_link = head->next;

    delete head->data;
    delete head;

    head = to_link;

    return;
  }

  Node* curr = head;

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

  Node* to_link = curr->next->next;

  delete curr->next->data;
  delete curr->next;

  curr->next = to_link;

}

template <class T>
void List<T>::freeAll(){
  while (size() > 0){
    remove(0);
  }

  head = NULL;
  tail = NULL;
}

template <class T>
bool List<T>::inList(const T data){
  for (int i = 0; i < size(); i++){
    if (getData(i) == data){
      return true;
    }
  }

  return false;
}