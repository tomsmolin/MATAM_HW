#ifndef HW3_LINKED_LIST_H
#define HW3_LINKED_LIST_H
#include <iostream>
#include <cstdbool>
namespace mtm
{
    template <class T>
    class Node
    {
        public:
            T data;
            Node<T>* next;
            Node(const T& new_data) : data(new_data), next(nullptr) {}  
            Node(const Node<T>& node) = default;
            ~Node() = default;  
            Node<T>& operator=(const Node<T>&) = delete;
    };

    template <class T>
    class LinkedList
    {
        private:
            Node<T>* first;
            Node<T>* last;
            int size;
        
        public:

            LinkedList() : first(nullptr), last(nullptr), size(0) {}
            LinkedList(const LinkedList<T>& linked_list);
            ~LinkedList();
            LinkedList<T>& operator=(const LinkedList<T>& list);
            void insert(const T data); 
            void erase(T data); 
            bool listContains(T data);
            std::ostream& printData(std::ostream& out);

            int getSize();
            Node<T>* getFirst() const; 
            Node<T>* getLast() const; 

            T& operator[](int index);

        private:
            //'insert' helpers fuction:
            void insertTheFirstElement(Node<T>* new_node);
            void insertElementToTheHeadOfTheList(Node<T>* new_node);
            void insertElementToTheEndOfTheList(Node<T>* new_node);
            void insertElementBetween(Node<T>* new_node);
            
            //'erase' helpers function:
            void eraseHead();
            void eraseLast(Node<T>* previous_node, Node<T>* inner_iterator);
            void eraseMiddle(Node<T>* previous_node, Node<T>* inner_iterator);

            void cloneList(Node<T> * node, Node<T> *& new_first, Node<T> *& new_last, int& new_size);
            void clearList(Node<T>* inner_iterator, bool reset = true); 

    };
    

//***************************************
// Access:    public 
// Explanation: LinkedList copy ctor.
//             
//***************************************
    template <class T>
    LinkedList<T>::LinkedList(const LinkedList<T>& linked_list) : first(nullptr), last(nullptr), size(0)
    {
        cloneList(linked_list.first, first, last, size);
    }


//***************************************
// Access:    public 
// Explanation: LinkedList dtor.
//             
//***************************************
    template <class T>
    LinkedList<T>::~LinkedList()
    {
        clearList(first);
    }

    template <class T>
    LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& list)
    {
        if (this == &list)
        {
            return *this;
        }

        // Saving current state
        Node<T> * tmp_first = first;
        Node<T> * tmp_last = last;
        int tmp_size = size;

        try {
            cloneList(list.first, first, last, size);
            clearList(tmp_first, false);
        }
        catch(std::bad_alloc& ba) {
            first = tmp_first;
            last = tmp_last;
            size = tmp_size;
        }

        return *this;
    }

//***************************************
// Access:    public 
// Explanation: Create new node with 'date' and insert it to the right place according to it priority.           
//***************************************
    template <class T>
    void LinkedList<T>::insert(const T data)
    {
        Node<T>* new_node = new Node<T>(data);
  
        new_node->next = nullptr;
        //The queue is empty.
        if(first == nullptr)
        {
            insertTheFirstElement(new_node);
            return;
        }
        
        //The new element's priority is higher than the head element.
        if (data < first->data)
        {
            insertElementToTheHeadOfTheList(new_node);
            return;
        }
        //The new element's priority is lower or equal to the last.
        if (data > last->data)
        {
            insertElementToTheEndOfTheList(new_node);
            return;
        }

        insertElementBetween(new_node);
        
    }


//***************************************
// Access:    public 
// Explanation: Remove the node that contain the entered 'data'.           
//***************************************
    template <class T>
    void LinkedList<T>::erase(T data)
    {
        Node<T>* previous_node = nullptr;
        Node<T>* inner_iterator = first;
        while (inner_iterator)
        {
            if (data == inner_iterator->data)
            {
                if (inner_iterator == first)
                {
                    eraseHead();
                    return;
                }
                if (inner_iterator == last)
                {
                    eraseLast(previous_node, inner_iterator);
                    return;
                }
                eraseMiddle(previous_node, inner_iterator);
                return;
            }
            previous_node = inner_iterator;
            inner_iterator = inner_iterator->next;
        }
    }

//***************************************
// Access:    public 
// Explanation: Returns true if the list contains a node with the entered 'data'.  
//              Returns false otherwise.         
//***************************************
    template <class T>
    bool LinkedList<T>::listContains(T data)
    {
        if (size == 0)
        {
            return false;
        }
        Node<T>* inner_iterator = first;
        while (inner_iterator)
        {
            if(data == inner_iterator->data)
            {
                return true;
            }
            inner_iterator  = inner_iterator->next;
        }
        return false;
    }

//***************************************
//  Access:    public. 
//  Explanation: Printing the list 'date' in lines.         
//***************************************
    template <class T>
    std::ostream& LinkedList<T>::printData(std::ostream& out)
    {
        Node<T>* inner_iterator = first;
        for (int i = 0; i < size; i++)
        {
        out << inner_iterator->data << std::endl;
        inner_iterator = inner_iterator->next;
        }
        return out;
    }

    template <class T>
    int LinkedList<T>::getSize() 
    {
        return size;
    }
//***************************************
// Access:    public 
// Explanation: get the first node in the list.           
//**************************************
    template <class T>
    Node<T>* LinkedList<T>::getFirst() const
    {
        return first;
    }

//***************************************
// Access:    public 
// Explanation: get the last node in the list.    
//***************************************
    template <class T>
    Node<T>* LinkedList<T>::getLast() const
    {
        return last;
    }
    
    template <class T>
    T& LinkedList<T>::operator[](int index)
    {
        Node<T>* iterator = first;
        for (int i = 0 ; i < index ; i++)
        {
            iterator = iterator->next;
        }

        return iterator->data;
    }

//***************************************
// 'insert' helper function imlementaion:
//  Access:    private 
//  Explanation: insert the node the it required place.           
//***************************************
    template <class T>
    void LinkedList<T>::insertTheFirstElement(Node<T>* new_node)
    {
        first = new_node;
        last = new_node;
        size++;
    }

    template <class T>
    void LinkedList<T>::insertElementToTheHeadOfTheList(Node<T>* new_node)
    {
        new_node->next = first;
        first = new_node;
        size++;
    }

    template <class T>
    void LinkedList<T>::insertElementToTheEndOfTheList(Node<T> *new_node)
    {
        last->next = new_node;
        last = new_node;
        size++;
    }

    template <class T>
    void LinkedList<T>::insertElementBetween(Node<T> *new_node)
    {
        Node<T> *previous_node = first;
        Node<T> *inner_iterator = first->next;

        while(inner_iterator)
        {
            if (new_node->data > previous_node->data && new_node->data < inner_iterator->data)
            {
                new_node->next = inner_iterator;
                previous_node->next = new_node;
                size++;
                return;
            }
            previous_node = inner_iterator;
            inner_iterator = inner_iterator->next;
        }
    }




//***************************************
// 'erase' helper function imlementaion:
//  Access:    private 
//  Explanation: Erasing the node according to it place in the list.           
//***************************************
    template <class T>
    void LinkedList<T>::eraseHead()
    {
        Node<T>* to_delete = first;
        first = to_delete->next; 
        delete to_delete;
        size--;
        if (size == 0)
        {
            last = nullptr;
        }
    }

    template <class T>
    void LinkedList<T>::eraseLast(Node<T>* previous_node, Node<T>* inner_iterator)
    {
        previous_node->next = nullptr;
        delete inner_iterator;
        last = previous_node;
        size--;   
    }

    template <class T>
    void LinkedList<T>::eraseMiddle(Node<T>* previous_node, Node<T>* inner_iterator)
    {
        previous_node->next = inner_iterator->next;
        delete inner_iterator;
        size--;
    }

//***************************************
// Access:    priate 
// Explanation: duplicate the list.           
//***************************************
    template <class T>
    void LinkedList<T>::cloneList(Node<T> * node, Node<T> *& new_first, Node<T> *& new_last, int& new_size)
    {
        new_first = nullptr;
        new_last = nullptr;
        new_size = 0;

        while (node)
        {
            Node<T> * new_node = new Node<T>(node->data);
            if (!new_first)
            {
                new_first = new_node;
                new_last = new_node;
            }
            else
            {
                new_last->next = new_node;
                new_last = new_node;
            }
            node = node->next;
            new_size++;
        }
    }
    
//***************************************
// Access:    private 
// Explanation: Delete all the node in the list.
//             If 'reset' is set to true well initialze 'LinkedList' fields  to an empty list fileds.
//            'reset' is a default 'true' parameter.
//***************************************
    template <class T>
    void LinkedList<T>::clearList(Node<T>* inner_iterator, bool reset)
    {
        while (inner_iterator)
        {
            Node<T>* tmp = inner_iterator->next;
            delete inner_iterator;
            inner_iterator = tmp;
        }
        if(reset)
        {
            first = nullptr;
            last = nullptr;
            size = 0;
        }
    }



}
#endif //HW3_LINKED_LIST_H
