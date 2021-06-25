#pragma once 

#include <cstddef>
#include <iostream>

template <typename T>
class Stack{
private:
    struct Node{
        T* data;
        Node* prev = NULL;
    };

public:
    Node* tail = NULL;
    Node* head = NULL;

    void push(T data){
        Node* new_node = new Node;
        T* new_data = new T;

        *new_data = data;

        new_node->data = new_data;

        if (head == NULL) head = new_node;
        if (tail != NULL) new_node->prev = tail;

        tail = new_node;
    }

    T pop(){
        Node* to_delete = tail;
        T data = *(to_delete->data);
        
        bool is_final = head==tail;

        if (!is_final) tail = to_delete->prev;

        delete to_delete->data;
        delete to_delete;

        if (is_final){
            head = NULL;
            tail = NULL;
        }

        return data;
    }

    T peak(){
        return *(tail->data);
    }

    size_t size(){

        if (head == NULL) return 0;

        size_t result = 0;
        
        Node* curr = tail;

        while (curr->prev != NULL){
            curr = curr->prev;
            result++;
        }

        result++;

        return result;
    }
};