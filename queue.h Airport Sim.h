// Copyright (c) 2019, Matthew Chalifoux.

#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <cassert>
using namespace std;

template<typename E>
struct Node
{
    // CONSTRUCTOR
    Node(E data1) : data(data1), next(0) {}
    E data;
    Node<E> *next;    
};

template<typename E>
class Queue{
public:
    // CONSTRUCTOR
    Queue();
    // COPY CONSTRUCTOR
    Queue(const Queue &h);
    // ASSIGNMENT CONSTRUCTOR
    Queue & operator =(const Queue & h);
    // MODIFICATION MEMBER FUNCTION
    void Push(E item);
    E Pop();
    bool Empty();
    E Top();    
private:
    Node<E> *head;
    Node<E> *tail;
};
#endif // QUEUE_H

