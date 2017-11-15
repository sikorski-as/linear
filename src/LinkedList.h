#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
    using const_reference = const Type&;

    class ConstIterator;
    class Iterator;
    friend ConstIterator;
    friend Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
private:
    size_type count;
    struct Node
    {
        Type data;
        Node * next;
        Node * prev;
    };
    Node * head;
    Node * tail;

    void initSentinel() // inits container, creates sentinel
    {
        head = new Node;
        head -> next = nullptr;
        head -> prev = nullptr;
        tail = head;
        count = 0;
    }

    void clear() // removes sentinel as well
    {
        Node * nextBuffer;
        Node * current = head;
        while(current != tail)
        {
            nextBuffer = current -> next;
            delete current;
            current = nextBuffer;
        }
        delete tail;
        count = 0;
    }

    void add(const Node * passedNodePointer, const Type & item)
    {
        Node * nodePointer = const_cast<Node *>(passedNodePointer);
        Node * newNode = new Node;
        if(nodePointer == head)
        {
            newNode -> prev = nullptr;
            head -> prev = newNode;
            newNode -> next = head;
            head = newNode;
        }
        else
        {
            std::cout << "A\n";
            newNode -> prev = nodePointer -> prev;
            std::cout << "B\n";
            if(nodePointer->prev == nullptr)
                std::cout << "POINTER 1 is nullptr\n";
            if((nodePointer->prev)->next == nullptr)
                std::cout << "POINTER 2 is nullptr\n";
            (nodePointer->prev)->next = newNode;
        }
        std::cout << "C\n";
        newNode -> next = nodePointer;
        std::cout << "D\n";
        newNode -> data = item;
        std::cout << "E\n";
        nodePointer -> prev = newNode;
        count++;
    }

    void remove(const Node * nodePointer) // const??
    {
        Node * followingNode = nodePointer->next;
        if(nodePointer == head)
        {
            head = followingNode;
        }
        followingNode->prev = nullptr;
        delete nodePointer;
        count--;
    }

public:
    LinkedList()
    {
        initSentinel();
    }

    LinkedList(std::initializer_list<Type> initList)
    {
        initSentinel();
        for(const_reference listElement: initList)
        {
            append(listElement);
        }
    }

    LinkedList(const LinkedList& other)
    {
        Node * current = other.head;
        while(current != other.tail)
        {
            std::cout << "Pre-append\n";
            append(current->data);
            std::cout << "After-append\n";
            current = current->next;
        }
    }

    LinkedList(LinkedList&& other) : count(other.count), head(other.head), tail(other.tail)
    {
        other.initSentinel();
    }

    ~LinkedList()
    {
        clear();
    }

    LinkedList& operator=(const LinkedList& other)
    {
        clear();
        initSentinel();

        Node * current = other.head;
        while(current != other.tail)
        {
            append(current->data);
            current = current->next;
        }

        return *this;
    }

    LinkedList& operator=(LinkedList&& other)
    {
        clear();

        head = other.head;
        tail = other.tail;
        count = other.count;

        other.initSentinel();
        return *this;
    }

    bool isEmpty() const
    {
        return head == tail;
    }

    size_type getSize() const
    {
        return count;
    }

    void append(const Type& item)
    {
        add(tail, item);
    }

    void prepend(const Type& item)
    {
        add(head, item);
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
        add(insertPosition.elementPointer, item);
    }

    Type popFirst()
    {
        if(isEmpty())
            throw std::logic_error("List is empty");
        Type tempCopy = head->data;
        remove(head);
        return tempCopy;
    }

    Type popLast()
    {
        if(isEmpty())
            throw std::logic_error("List is empty");
        Node * that = tail->prev;
        Type tempCopy = that->data;
        remove(that);
        return tempCopy;
    }

    void erase(const const_iterator& position)
    {
        if(isEmpty())
            throw std::out_of_range("Bad iterator");
        remove(position.elementPointer);
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded) // may need a range-check
    {
        if(isEmpty())
            throw std::out_of_range("Bad iterator");
        Node * first = const_cast<Node*>(firstIncluded.elementPointer);
        Node * last = const_cast<Node*>(lastExcluded.elementPointer);
        if(first == head)
        {
            head = last;
            last -> prev = nullptr;
        }
        else
        {
            (first -> prev) -> next = last;
            last -> prev = first -> prev;
        }
        Node * nextBuffer;
        Node * current = first;
        while(current != last)
        {
            nextBuffer = current -> next;
            delete current;
            current = nextBuffer;
        }
    }

    iterator begin()
    {
        return ConstIterator(this, head);
    }

    iterator end()
    {
        return ConstIterator(this, tail);
    }

    const_iterator cbegin() const
    {
        return ConstIterator(this, head);
    }

    const_iterator cend() const
    {
        return ConstIterator(this, tail);
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }

};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
    friend LinkedList<Type>;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename LinkedList::value_type;
    using difference_type = typename LinkedList::difference_type;
    using pointer = typename LinkedList::const_pointer;
    using reference = typename LinkedList::const_reference;
protected:
    const LinkedList<Type> * listPointer;
    const LinkedList<Type>::Node * elementPointer;
    ConstIterator(const LinkedList<Type> * initList, const LinkedList<Type>::Node * initElementPointer) : listPointer(initList), elementPointer(initElementPointer)
    {

    }

    bool isItTail() const
    {
        return listPointer->tail == elementPointer;
    }

    bool isItHead() const
    {
        return listPointer->head == elementPointer;
    }

public:
    explicit ConstIterator() : listPointer(nullptr), elementPointer(nullptr)
    {}

    ConstIterator(const ConstIterator& other) : listPointer(other.listPointer), elementPointer(other.elementPointer) // can be removed
    {

    }

    reference operator*() const
    {
        if(isItTail())
            throw std::out_of_range("Bad iterator");
        return elementPointer->data;
    }

    ConstIterator& operator++()
    {
        if(isItTail())
            throw std::out_of_range("Bad iterator");
        elementPointer = elementPointer->next;
        return *this;
    }

    ConstIterator operator++(int)
    {
        auto tempCopy = *this;
        operator++();
        return tempCopy;
    }

    ConstIterator& operator--()
    {
        if(isItHead())
            throw std::out_of_range("Bad iterator");
        elementPointer = elementPointer->prev;
        return *this;
    }

    ConstIterator operator--(int)
    {
        auto tempCopy = *this;
        operator--();
        return tempCopy;
    }

    ConstIterator operator+(difference_type d) const
    {
        (void)d;
        throw std::runtime_error("WHAT IS IT SUPPOSED TO DO?");
    }

    ConstIterator operator-(difference_type d) const
    {
        (void)d;
        throw std::runtime_error("WHAT IS IT SUPPOSED TO DO?");
    }

    bool operator==(const ConstIterator& other) const
    {
        return other.listPointer == listPointer && other.elementPointer == elementPointer;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return other.listPointer != listPointer || other.elementPointer != elementPointer;
    }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
    friend LinkedList<Type>;
public:
    using pointer = typename LinkedList::pointer;
    using reference = typename LinkedList::reference;

    explicit Iterator()
    {}

    Iterator(const ConstIterator& other)
        : ConstIterator(other)
    {}

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--()
    {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    Iterator operator+(difference_type d) const
    {
        return ConstIterator::operator+(d);
    }

    Iterator operator-(difference_type d) const
    {
        return ConstIterator::operator-(d);
    }

    reference operator*() const
    {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
