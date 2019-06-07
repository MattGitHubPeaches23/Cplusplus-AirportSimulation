// Copyright (c) 2019, Matthew Chalifoux.
#include "queue.h"

template<typename E>
bool Queue<E>::Empty()
{
    if(head == NULL)
    {
        return true;
    }
    else if(head != NULL)
    {
        return false;
    }
}

template<typename E>
E Queue<E>::Top()
{
    assert(Empty() != true);

    if(Empty() != true)
    {
        return head->data;
    }
}

template<typename E>
E Queue<E>::Pop()
{
    assert(Empty() != true);
    Node<E> *pnt;
    pnt = head;
    E sub;
    if(head != NULL)
    {
        sub = head->data;
        head = head->next;
        delete pnt;
        return sub;
    }
}

template<typename E>
void Queue<E>::Push(E item)
{
    Node<E> *insert = new Node<E>(item);
    if(head == NULL)
    {
        head = insert;
        tail = insert;
    }
    else if(head != NULL)
    {
        tail->next = insert;
        tail = insert;
    }
}
template<typename E>
Queue<E> &Queue<E>::operator =(const Queue & a)
{
    Queue<E> test;
    E sub;
    Node<E> *i, *ipt, *ipt2;
    i = new Node<E>(sub);
    i->data = a.head->data;
    i->next = a.head->next;
    ipt = i;

    while(ipt != NULL)
    {
        test.Push(ipt->data);
        ipt = ipt->next;
    }

    head = test.head;
}
// ADD CODE 4065
template<typename E>
Queue<E>::Queue(const Queue &a)
{
    Queue<E> test;
    E sub;
    Node<E> *i, *ipt;
    i = new Node<E>(sub);
    i->data = a.head->data;
    i->next = a.head->next;
    ipt = i;

    while(ipt != NULL)
    {
        test.Push(ipt->data);
        ipt = ipt->next;
    }
    head = test.head;
}

template<typename E>
Queue<E>::Queue()
{
    head = NULL;
    tail = NULL;
}
