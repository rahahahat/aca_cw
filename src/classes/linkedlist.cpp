#include "linkedlist.h"

#include "pipeline.h"
template <class T>
LLNode<T>::LLNode()
{
    payload = NULL;
    next = NULL;
    prev = NULL;
};

template <class T>
LLNode<T>::LLNode(T *value)
{
    payload = value;
    next = NULL;
    prev = NULL;
};

template <class T>
int LLNode<T>::isTail()
{
    return next == NULL;
};

template <class T>
int LLNode<T>::isHead()
{
    return prev == NULL;
};


template <class T>
T* LLNode<T>::get()
{
    return payload;
};

template <class T>
LinkedList<T>::LinkedList()
{
    head = NULL;
    tail = NULL;
    size = 0;
}

template <class T>
void LinkedList<T>::add(T *value)
{
    LLNode<T> *node = new LLNode<T>(value);
    if (head == NULL && size == 0)
    {
        head = node;
        tail = node;
        size++;
        return;
    }
    tail->next = node;
    node->prev = tail;
    node->next = NULL;
    tail = node;
    size++;
    return;
};

template <class T>
T* LinkedList<T>::pop()
{
    if (size == 0) return NULL;
    LLNode<T>* temp = head->next;
    T* temp_val = head->payload;
    head->next = NULL;
    head->prev = NULL;
    head->payload = NULL;
    if (temp != NULL)
    {
        temp->prev = NULL;
    }
    if (head == tail) {
        tail = NULL;
    }
    delete head;
    head = temp;
    size--;
    if (size <= 1)
    {
        tail = head;
    }
    return temp_val;
}

template <class T>
T* LinkedList<T>::remove(LLNode<T>* node)
{
    if (node == NULL)
    {
        std::cout << "LLNode<T> is NULL" << std::endl;
        return NULL;
    }
    LLNode<T>* prev = node->prev;
    LLNode<T>* next = node->next;
    T* ret_val;
    if (prev != NULL)
    {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    if (node == tail)
    {
        tail = prev;
    }
    if (node == head)
    {
        head = next;
    }
    ret_val = node->payload;
    node->prev = NULL;
    node->next = NULL;
    node->payload = NULL;
    size--;
    delete node;
    return ret_val;
}

template <class T>
T* LinkedList<T>::remove(int i)
{
    if (i >= size)
    {
        std::cout << "LinkedList<T> index out of range" << std::endl;
        return NULL;
    }
    int count = 0;
    LLNode<T>* prev;
    LLNode<T>* next;
    T* ret_val;
    LLNode<T>* curr = head;
    while(count < i)
    {
        curr = curr->next;
        count++;
    }
    prev = curr->prev;
    next = curr->next;
    if (prev != NULL)
    {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    if (i == 0)
    {
        head = next;
    }
    if (i == size - 1)
    {
        tail = prev;
    }
    ret_val = curr->payload;
    curr->prev = NULL;
    curr->next = NULL;
    curr->payload = NULL;
    size--;
    delete curr;
    return ret_val;
}

template <class T>
void LinkedList<T>::removeAndDestroy(int i)
{
    T* ret_val = remove(i);
    delete ret_val;
    return;
}

template <class T>
void LinkedList<T>::removeAndDestroy(LLNode<T> *node)
{
    T* ret_val = remove(node);
    delete ret_val;
    return;
}

template <class T>
void LinkedList<T>::flushAfterNode(LLNode<T> *node)
{
    LLNode<T> *start_node = node->next;
    LLNode<T> *next = NULL;
    while(start_node != NULL)
    {   
        next = start_node->next;
        removeAndDestroy(start_node);
        start_node = next;
    }
    return;
}