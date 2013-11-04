/******************************************************************************
 * @Programmer - Rook
 * @File - Intrusive_list.h
 * @Date - 8/22/2013
 * @Purpose - This file contains the intrusive doubly linked list
 *        implementation to be used along side the nodes declared in
 *        Intrusive.h
 *
 * @Description - The list is implemented using an iterator pattern
 *        similar to that of the STL, and also uses my so called terminator
 *        pattern to strictly avoidout_of_bounds errors.
 *
 *        the hook is implemented to utilize only two pointers, one that points
 *        to the previous element and one that pointer to the next element. the
 *        terminators are setup so that the front terminator's next pointer,
 *        points to the first element in the list, and the end termiantor's
 *        previous pointer points to the last element in the list. Using the
 *        hooks in these terminators, one can get a reference to and iterate
 *        over all the elements in the list.
 *
 *        For an object to be able to be used in this container, all an object
 *        has to do is inherit from one of the node object declared in
 *        Intrusive.h
 *                                                                - Rook
 *
 *****************************************************************************/

#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

#include "Intrusive.h"

namespace intrusive
{

    /**************************************************************************
     * Class Declaration
     *
     * @name - template<class T> list
     *
     * @scope - intrusive
     *
     * @inherits - intrusive::base_container            @see Intrusive.h
     *
     * @desc - this class is my take on an intrusive list. It is implemented
     *       as a doubly linked list, where the hook definition contains
     *       only two pointers, one to the previous and one to the next
     *       element in the list.
     *
     * @property - It utilizes my self-proclaimed "terminator" pattern, where the
     *       list itself contains two nodes for the list, where the
     *       begin_terminator is always at the front and the end_terminator is
     *       always in the back. utilizing there virtual methods, you now have
     *       a safe method of determining if you are trying to iterate past the
     *       ends of the list
     *
     *************************************************************************/

    template<class T>
    class list :
        base_container
    {
    public:
        /**********************************************************************
         * Class Declaration
         *
         * @name - hook
         *
         * @scope - template<class T> ::intrusive::list<T>
         *
         * @inherits - intrusive::base_hook                 @see Intrusive.h
         *
         * @desc - this is the object that a node will have to contain to be
         *       able to be a member of the intrusive::list object. This is the
         *       object that actually contains the next and prev pointer for
         *       the doubly linked list and should never be manipulated outside
         *       the list object.
         *
         * @warning - do not derive from this directly, your object will NOT
         *       be compatable with this list. You must derive from a node.
         *       Only node and containers should have direct access to hooks
         *
         *********************************************************************/
        class hook :
            public base_hook
        {
        public:
            /**
             * Function Declaration
             * @name - void unhook(base_container* listptr)
             * @scope - template<class T> intrusive::list<T>::hook::unhook
             * @purpose - use this function to detach this hook from a list by
             *            by closing the gap
             * @pre - this hook must have been claimed by the container calling
             *        this function
             * @post - the hook will close the gap in the list, effectively
             *         removing itself from the list
             * @param listptr - required to obtain the hooks from the next and
             *                  previous nodes.
             * @return
             */
            void unhook(base_container* listptr)
            {

                hook * prev = GET_HOOK(m_prev, listptr);
                hook * next = GET_HOOK(m_next, listptr);

                prev->m_next = m_next;
                next->m_prev = m_prev;
            }

            base_node * m_prev;
            base_node * m_next;
        };

        /**********************************************************************
         * Class Declaration
         *
         * @name - iterator
         *
         * @scope - template<class T> ::intrusive::list<T>
         *
         * @inherits
         *
         * @desc - This is your standard implementation of the iterator pattern
         *       for a double linked list. Traverse bidirectionally, and
         *       utilizes the terminators to determine whether you have
         *       traveled past the end or not
         *********************************************************************/
        class iterator
        {
        public:

            /**
             * Function Declaration
             * @name - iterator()
             * @scope - template<class T> intrusive::list<T>::iterator
             * @purpose - default constructor
             * @pre - list must point to a valid existing object.
             * @post - the object is initialized
             * @param listptr - a pointer to the list you will be iterating through
             * @param init - sets the point
             */
            iterator(base_container * listptr, base_node * init = 0) :
                m_listptr(listptr),
                m_current(init)
                {}

            /**
             * Function Declaration
             * @name - operator*()
             * @scope - template<class T> intrusive::list<T>::iterator
             * @purpose - obtins a reference to the object at the current
             *       position of the iterator
             * @pre - the current position cannot be at a terminator
             * @return - returns a reference to the object at the current
             *       position of the iterator
             */
            T& operator*()
            {
                if(m_current->is_terminal())
                    throw std::out_of_range("Iterator out of range.");

                return *static_cast<T*>(m_current);
            }

            /**
             * Function Declaration
             * @name - operator*() const
             * @scope - template<class T> intrusive::list<T>::iterator
             * @purpose - obtains a const declared reference to the object
             *       at the current position of the iterator
             * @pre - the current position cannot be at a terminator
             * @return - returns a const declared reference to the object
             *       at the current position of the iterator
             */
            const T& operator*() const
            {
                if(m_current->is_terminal())
                    throw std::out_of_range("Iterator out of range.");

                return *static_cast<T*>(m_current);
            }

            /**
             * Function Declaration
             * @name - operator++()
             * @scope - template<class T> intrusive::list<T>::iterator
             * @purpose - advances to the next element in the list. (pre-increment)
             * @post - if the current node is not a terminator, then the iterator
             *       will have advanced to the next node.
             *       else, nothing will happen
             * @return - a reference to this iterator is returned
             */
            iterator& operator++()
            {
                if(!m_current->is_terminal_begin())
                {
                    hook * current = static_cast<hook*>(m_current->get_hook(m_listptr));
                    m_current = current->m_next;
                }

                return *this;
            }

            /**
             * Function Declaration
             * @name - operator++(int)
             * @scope - template<class T> intrusive::list<T>::iterator
             * @purpose - advances to the next element in the list. (post-increment)
             * @post - if the current node is not an end terminator, then the iterator
             *       will have advanced to the next node.
             *       else, nothing will happen.
             * @return - a COPY of this iterator is returned by VALUE
             */
            iterator operator++(int)
            {
                iterator temp(*this);
                ++*this;
                return temp;
            }

            /**
             * Function Declaration
             * @name - operator--()
             * @scope - template<class T> intrusive::list<T>::iterator
             * @purpose - advances to the previous element in the list. (pre-decrement)
             * @post - if the current node is not a begin terminator, then the iterator
             *       will have advanced to the previous node.
             *       else, nothing will happen.
             * @return - a reference to this iterator is returned
             */
            iterator& operator--()
            {
                if(!m_current->is_terminal_end())
                {
                    hook * current = m_current->get_hook(m_listptr);
                    m_current = current->m_prev;
                }

                return *this;
            }

            /**
             * Function Declaration
             * @name - operator--(int)
             * @scope - template<class T> intrusive::list<T>::iterator
             * @purpose - advances to the previous element in the list. (post-decrement)
             * @post - if the current node is not a begin terminator, then the iterator
             *       will have advanced to the previous node.
             *       else, nothing will happen.
             * @return - a COPY of this iterator is returned by VALUE
             */
            iterator& operator--(int)
            {
                iterator temp(*this);
                --*this;
                return temp;
            }

            /**
             * Function Declaration
             * @name - operator==(const iterator& x)
             * @scope - template<class T> intrusive::list<T>::iterator
             * @purpose - determines whether iterator x is at the same position as this
             *       iterator
             * @param x - the iterator to compare to this one
             * @return - returns true if they are at the same position, false otherwise
             */
            bool operator==(const iterator& x) const {return m_current == x.m_current;}

            /**
             * Function Declaration
             * @name - operator!=(const iterator& x)
             * @purpose - determines whether iterator x is NOT at the same position as this
             *       iterator
             * @param x - the iterator to compare to this one
             * @return - returns true if they are NOT at the same position, false otherwise
             */
            bool operator!=(const iterator& x) const {return m_current != x.m_current;}

        private:
            base_container * m_listptr;
            base_node * m_current;
        };

    private:
        /**********************************************************************
         * Abstract Class Declaration
         *
         * @name - terminator
         *
         * @scope - template<class T> intrusive::list<T>
         *
         * @inherits - intrusive::base_node                  @see Intrusive.h
         *
         * @desc - This is an abstract class that represents a node which
         *         terminates the list, either at the beginning or end.
         *         Pure Virtual Abstracts:
         *         is_terminal_begin()
         *         is_terminal_end()
         *
         *********************************************************************/
        class terminator :
            public base_node
        {
        public:
            /**
             * Function Declaration
             * @name - ~terminator()
             * @scope - template<class T> intrusive::list<T>::terminator
             * @purpose - virtual destructor for polymorphic destruction
             */
            virtual ~terminator() {}

            /**
             * Function Declaration
             * @name - is_terminal() const final
             * @scope - template<class T> intrusive::list<T>::terminator
             * @purpose - to determine if this node is a terminator
             * @return - returns true
             */
            bool is_terminal() const final {return true;}

            /**
             * Function Declaration
             * @name - attach(base_container* listptr)
             * @scope - template<class T> intrusive::list<T>::terminator
             * @purpose - unused, defined to satisfy abstract class.
             * @param listptr - unused
             */
            void attach(base_container * listptr) {}

            /**
             * Function Declaration
             * @name - detach()
             * @scope - template<class T> intrusive::list<T>::terminator
             * @purpose - unused, defined to satisfy abstract class
             */
            void detach() {}

            /**
             * Function Declaration
             * @name - get_hook(base_container*)
             * @scope - template<class T> intrusive::list<T>::terminator
             * @purpose - obtain this terminators hook
             * @param listptr - unused
             * @return - a pointer to the hook this terminator owns
             */
            base_hook* get_hook(base_container* listptr) final {return &m_hook;}

        private:
            hook m_hook;
        };

        /**********************************************************************
         * Class Declaration
         *
         * @name - begin_terminator
         *
         * @scope - template<class T> intrusive::list<T>
         *
         * @inherits - intrusive::list<T>::terminator
         *
         * @desc - This node represents the beginning of the list and contains
         *         a hook. The prev element is always null and the next element
         *         always points to the first node in the list.
         *
         *********************************************************************/
        class begin_terminator final:
            public terminator
        {
        public:
            /**
             * Function Declaration
             * @name - is_terminal_begin() const
             * @scope - template<class T> intrusive::list<T>::begin_terminator
             * @purpose - used to determine if this node is the front terminator
             * @return - returns true
             */
            bool is_terminal_begin() const {return true;}

            /**
             * Function Declaration
             * @name - is_terminal_end() const
             * @scope - template<class T> intrusive::list<T>::begin_terminator
             * @purpose - used to determine if this node is the end terminator
             * @return - returns false
             */
            bool is_terminal_end() const {return false;}
        };

        /**********************************************************************
         * Class Declaration
         *
         * @name - end_terminator
         *
         * @scope - template<class T> intrusive::list<T>
         *
         * @inherits - intrusive::list<T>::terminator
         *
         * @desc - This node represents the end of the list and contains
         *         a hook. The prev element points to the last node in the list
         *         and the next element is always null
         *
         *********************************************************************/
        class end_terminator final:
            public terminator
        {
        public:
            /**
             * Function Declaration
             * @name - is_terminal_begin() const
             * @scope - template<class T> intrusive::list<T>::end_terminator
             * @purpose - used to determine if this node is the front terminator
             * @return - returns false
             */
            bool is_terminal_begin() const {return false;}

            /**
             * Function Declaration
             * @name - is_terminal_end() const
             * @scope - template<class T> intrusive::list<T>::end_terminator
             * @purpose - used to determine if this node is the front terminator
             * @return - returns true
             */
            bool is_terminal_end() const {return true;}
        };

    public:

        /**
         * Function Declaration
         * @name - list()
         * @scope - template<class T> intrusive::list<T>
         * @purpose - default constructor
         * @post - the object initializes the terminators to point at themselves
         */
        list()
        {
            hook * front = static_cast<hook*>(m_Begin.get_hook(this));
            hook * back = static_cast<hook*>(m_End.get_hook(this));

            front->m_prev = nullptr;
            front->m_next = &m_End;
            back->m_prev = &m_Begin;
            back->m_next = nullptr;
        }

        /**
         * Function Declaration
         * @name - create_hook()
         * @scope - template<class T> intrusive::list<T>
         * @purpose - dynamically creates the hook object defined in this
         *       container
         * @return - the pointer to a new hook instance is returned
         * @warning - this may be public, but there should be no reason
         *       for you to ever use this function. It is for internal use
         */
        base_hook* create_hook()
        {
            return new hook;
        }

        /**
         * Function Declaration
         * @name - is_empty() const
         * @scope - template<class T> intrusive::list<T>
         * @purpose - determines whether the list is empty
         * @return - returns true if empty, false if not
         */
        bool is_empty() const
        {
            hook * front = m_Begin.get_hook(this);

            return (front->m_next = &m_End)?true:false;
        }

        /**
         * Function Declaration
         * @name - push_back(base_node & val)
         * @scope - template<class T> intrusive::list<T>
         * @purpose - attaches val to this list and inserts it at the end
         * @pre - if val is a static hook, it should belong to no other list
         * @post - val is at the end of the list
         * @param val - the object to be added by reference
         */
        void push_back(base_node & val)
        {
            val.attach(this);

            hook * end = static_cast<hook*>(m_End.get_hook(this));
            hook * left = static_cast<hook*>(end->m_prev->get_hook(this));
            hook * middle = static_cast<hook*>(val.get_hook(this));

            left->m_next = &val;
            middle->m_prev = end->m_prev;
            middle->m_next = &m_End;
            end->m_prev = &val;

            return;
        }

        /**
         * Function Declaration
         * @name - push_front(base_node & val)
         * @scope - template<class T> intrusive::list<T>
         * @purpose - attaches val to this list and inserts it at the front
         * @pre - if val is a static hook, it should belong to no other list
         * @post - val is at the end of the list
         * @param val - the object to be added by reference
         */
        void push_front(base_node & val)
        {
            return;
        }

        /**
         * Function Declaration
         * @name - begin()
         * @scope - template<class T> intrusive::list<T>
         * @purpose - This returns an iterator that starts at the first
         *       element in the list
         * @return - returns an iterator to the first element in the list
         */
        iterator begin()
        {
            hook * front = static_cast<hook*>(m_Begin.get_hook(this));

            return iterator(this, front->m_next);
        }

        /**
         * Function Declaration
         * @name - end()
         * @scope - template<class T> intrusive::list<T>
         * @purpose - This returns an iterator that points to the element
         *        beyond the last in the list
         * @return - returns an iterator to the element beyond the last in
         *        the list
         * @warning - this will always be a terminator so do not dereference.
         */
        iterator end()
        {
            return iterator(this, &m_End);
        }

        /**
         * Function Declaration
         * @name - rbegin()
         * @scope - template<class T> intrusive::list<T>
         * @purpose - This returns an iterator that points to the last element
         *        in the list
         * @return - returns an interator to the last element in the list
         */
        iterator rbegin()
        {
            hook * end = m_End.get_hook(this);

            return iterator(this, end->m_prev);
        }

        /**
         * Function Declaration
         * @name - rend()
         * @scope - template<class T> intrusive::list<T>
         * @purpose - this returns an iterator that points to the element
         *      beyond the first in the list
         * @return - returns an iterator to the element beyond the first
         *      element
         * @warning - this will always be a terminator so do not dereference.
         */
        iterator rend()
        {
            return iterator(this, &m_Begin);
        }

    private:
        begin_terminator m_Begin;
        end_terminator m_End;
    };
}

#endif

/*     __________________________________________
     //     ____        ________       ____      \\
    ||  ...|MMMM|......|MMMMMMMM|.....|MMMM|...   ||
    ||  ...|MMMM|______|MMMMMMMM|_____|MMMM|....  ||
    ||  ...|MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM|....  ||
    ||  ...\.............................../....  ||
    ||  ....\............................./.....  ||
    ||  .....\.MMMMMMMMMMMMMMMMMMMMMMMMM./......  ||
    ||  ......\__.....................__/.......  ||
    ||  ....... |MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.......... ||
    ||  ........|MMMMMMMMMMMMMMMMMMMMM|.........  ||
    ||  ........|.....................|.........  ||
    ||  ......./.MMMMMMMMMMMMMMMMMMMMM.\........  ||
    ||  ....../MMMMMMMMMMMMMMMMMMMMMMMMM\ ......  ||
    ||  ..... |..........................|......  ||
    ||  ...../............................\.....  ||
    ||  ....|MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM|....  ||
    ||  ..../oooooooooooooooooooooooooooooo\....  ||
    ||  .._/................................\_..  ||
    ||  _/MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\_  ||
    || |MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM| ||
    || |MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM| ||
    || |----------------------------------------| ||
     \\__________________________________________//
*/
