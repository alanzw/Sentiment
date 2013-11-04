/******************************************************************************
 * @Programmer - Rook
 * @File - Intrusive_map.h
 * @Date - 8/22/2013
 * @Purpose - This file container the intrusive binary search tree map
 *       implementation to be used along side the nodes declared in
 *       Intrusive.h
 *
 * @Description - The map is implemented using an iterator pattern similar
 *       to that of the STL, and also uses my so called terminator pattern
 *       to strictly avoid out_of_bounds errors.
 *
 *       The hook is implemented to utilize three pointers, one that points
 *       to the left subtree, the right subtree, and the parent of this
 *       element. The terminators are setup so that the begin_terminator is
 *       located at the far left leaf of the tree, and the far right. There
 *       is also a root terminator that is used to access the top of the tree.
 *       the parent pointer of the root node points to the first element in the
 *       tree, the begin_terminator's parent pointer points to the first node
 *       in in-order traversal, and the end_terminator's parent pointer points
 *       to the last element in in-order traversal. Using these terminators,
 *       you can create iterators, and traverse the whole map, and you can use
 *       the root terminator for insertion, deletion, finding, etc.
 *
 *       For an object to be used in this container, all an object needs to do
 *       is inherit from one of the node objects declared in Intrusive.h
 *
 *                                                                   -Rook
 *
 *****************************************************************************/
#ifndef INTRUSIVE_MAP_H
#define INTRUSIVE_MAP_H

#include "Intrusive.h"
#include <iostream>

namespace intrusive
{
    /**************************************************************************
     *
     */
    template<class TKey, class T>
    class map :
        public base_container
    {
    public:
        class hook :
            public base_hook
        {
        public:
            void unhook(base_container* mapptr)
            {
                base_node * swap = nullptr;
                hook * swapHook = nullptr;
                hook * parentHook = nullptr;
                hook * leftHook = nullptr;
                hook * rightHook = nullptr;

                if(m_left != nullptr && m_right != nullptr)
                {
                    if(!m_left->is_terminal_begin())
                    {

                        swap = m_left;
                        swapHook = GET_HOOK(swap, mapptr);

                        while(swapHook->m_right != nullptr)
                        {
                            swap = swapHook->m_right;
                            swapHook = GET_HOOK(swap, mapptr);
                        }

                        if(swap != m_left)
                        {
                            parentHook = GET_HOOK(swapHook->m_parent, mapptr);
                            parentHook->m_right = swapHook->m_left;
                        }
                    }
                    else if(!m_right->is_terminal_end())
                    {

                        swap = m_right;
                        swapHook = GET_HOOK(swap, mapptr);

                        while(swapHook->m_left != nullptr)
                        {
                            swap = swapHook->m_left;
                            swapHook = GET_HOOK(swap, mapptr);
                        }

                        if(swap != m_right)
                        {
                            parentHook = GET_HOOK(swapHook->m_parent, mapptr);
                            parentHook->m_left = swapHook->m_right;
                        }
                    }

                    if(swap != nullptr)
                    {
                        swapHook->m_parent = m_parent;

                        if(swap != m_left)
                            swapHook->m_left = m_left;

                        if(swap != m_right)
                            swapHook->m_right = m_right;

                        parentHook = GET_HOOK(m_parent, mapptr);

                        if(m_parent->is_terminal())
                            parentHook->m_parent = swap;
                        else
                        {
                            if(parentHook->m_left != nullptr && GET_HOOK(parentHook->m_left, mapptr) == this)
                                parentHook->m_left = swap;
                            else
                                parentHook->m_right = swap;
                        }

                        if(m_left != nullptr && m_left != swap)
                        {
                            leftHook = GET_HOOK(m_left, mapptr);
                            leftHook->m_parent = swap;
                        }

                        if(m_right != nullptr && m_right != swap)
                        {
                            rightHook = GET_HOOK(m_right, mapptr);
                            rightHook->m_parent = swap;
                        }
                    }
                    else
                    {
                           parentHook = GET_HOOK(m_parent, mapptr);
                           leftHook = GET_HOOK(m_left, mapptr);
                           rightHook = GET_HOOK(m_right, mapptr);

                           parentHook->m_parent = m_right;
                           rightHook->m_parent = m_parent;
                           rightHook->m_left = m_left;
                           leftHook->m_parent = m_right;
                    }
                }
                else if(m_left != nullptr)
                {
                    parentHook = GET_HOOK(m_parent, mapptr);
                    leftHook = GET_HOOK(m_left, mapptr);


                    if(parentHook->m_left != nullptr && GET_HOOK(parentHook->m_left, mapptr) == this)
                        parentHook->m_left = m_left;
                    else
                        parentHook->m_right = m_left;

                    leftHook->m_parent = m_parent;
                }
                else if(m_right != nullptr)
                {
                    parentHook = GET_HOOK(m_parent, mapptr);
                    rightHook = GET_HOOK(m_right, mapptr);

                    if(parentHook->m_left != nullptr && GET_HOOK(parentHook->m_left, mapptr) == this)
                        parentHook->m_left = m_right;
                    else
                        parentHook->m_right = m_right;

                    rightHook->m_parent = m_parent;
                }
                else
                {
                    parentHook = GET_HOOK(m_parent, mapptr);

                    if(GET_HOOK(parentHook->m_left, mapptr) == this)
                        parentHook->m_left = nullptr;
                    else
                        parentHook->m_right = nullptr;
                }

            }

            TKey m_key;
            base_node * m_parent;
            base_node * m_left;
            base_node * m_right;
        };

        class iterator
        {
        public:
            iterator(base_container * mapptr = 0, base_node * init = 0) :
                m_mapptr(mapptr),
                m_current(init)
                {}

            T& operator*()
            {
                if(m_current->is_terminal())
                    throw std::out_of_range("Iterator out of range.");

                return *static_cast<T*>(m_current);
            }

            const T& operator*() const
            {
                if(m_current->is_terminal())
                    throw std::out_of_range("Iterator out of range.");

                return *static_cast<T*>(m_current);
            }

            iterator& operator++()
            {
                if(!m_current->is_terminal_end())
                {
                    hook * current_hook = GET_HOOK(m_current, m_mapptr);

                    if(current_hook->m_right != nullptr)
                    {

                        m_current = current_hook->m_right;
                        current_hook = GET_HOOK(m_current, m_mapptr);

                        while(current_hook->m_left != nullptr)
                        {
                            m_current = current_hook->m_left;
                            current_hook = GET_HOOK(m_current, m_mapptr);
                        }
                    }
                    else
                    {
                        hook * parent_hook = GET_HOOK(current_hook->m_parent, m_mapptr);

                        while(m_current != parent_hook->m_left)
                        {
                            m_current = current_hook->m_parent;
                            current_hook = GET_HOOK(m_current, m_mapptr);
                            parent_hook = GET_HOOK(current_hook->m_parent, m_mapptr);
                        }

                        m_current = current_hook->m_parent;
                    }
                }

                return *this;
            }

            iterator operator++(int)
            {
                iterator temp(*this);
                ++*this;
                return temp;
            }

            iterator& operator--()
            {
                if(!m_current->is_terminal_begin())
                {
                    hook * current_hook = GET_HOOK(m_current, m_mapptr);

                    if(current_hook->m_left != nullptr)
                    {
                        m_current = current_hook->m_left;
                        current_hook = GET_HOOK(m_current, m_mapptr);

                        while(current_hook->m_right != nullptr)
                        {
                            m_current = current_hook->m_right;
                            current_hook = GET_HOOK(m_current, m_mapptr);
                        }
                    }
                    else
                    {
                        hook * parent_hook = GET_HOOK(current_hook->m_parent, m_mapptr);

                        while(m_current != parent_hook->m_right)
                        {
                            m_current = current_hook->m_parent;
                            current_hook = GET_HOOK(m_current, m_mapptr);
                            parent_hook = GET_HOOK(current_hook->m_parent, m_mapptr);
                        }

                        m_current = current_hook->m_parent;
                    }
                }

                return *this;
            }

            iterator& operator--(int)
            {
                iterator temp(*this);
                --*this;
                return temp;
            }

            bool operator==(const iterator& x) const {return m_current == x.m_current;}
            bool operator!=(const iterator& x) const {return m_current != x.m_current;}

        private:
            base_container * m_mapptr;
            base_node * m_current;
        };

    private:
        class terminator :
            public base_node
        {
        public:
            virtual ~terminator() {}

            bool is_terminal() const final {return true;}

            void attach(base_container * listptr) {}

            void detach() {}

            base_hook* get_hook(base_container*) final {return &m_hook;}

        private:
            hook m_hook;
        };

        class begin_terminator final :
            public terminator
        {
        public:
            bool is_terminal_begin() const {return true;}

            bool is_terminal_end() const {return false;}
        };

        class end_terminator final :
            public terminator
        {
        public:
            bool is_terminal_begin() const {return false;}

            bool is_terminal_end() const {return true;}
        };

        class root_terminator final :
            public terminator
        {
        public:
            bool is_terminal_begin() const {return false;}

            bool is_terminal_end() const {return false;}
        };

    public:
        map()
        {
            m_Root.attach(this);
            m_Begin.attach(this);
            m_End.attach(this);

            hook * rootHook = GET_HOOK((&m_Root), this);
            hook * beginHook = GET_HOOK((&m_Begin), this);
            hook * endHook = GET_HOOK((&m_End), this);

            rootHook->m_parent = &m_End;
            rootHook->m_left = nullptr;
            rootHook->m_right = nullptr;

            endHook->m_parent = &m_Root;
            endHook->m_left = &m_Begin;
            endHook->m_right = nullptr;

            beginHook->m_parent = &m_End;
            beginHook->m_left = nullptr;
            beginHook->m_right = nullptr;
        }

        base_hook * create_hook()
        {
            return new hook;
        }

        bool is_empty()
        {
            hook * rootHook = GET_HOOK( (&m_Root), this);

            return (rootHook->m_parent == &m_End)?true:false;
        }

        void insert(TKey key, base_node & val)
        {
            hook * rootHook = GET_HOOK((&m_Root), this);

            base_node * topNode = rootHook->m_parent;

            val.attach(this);

            if(topNode == &m_End)
            {
                hook * valHook = GET_HOOK((&val), this);
                valHook->m_parent = &m_Root;
                valHook->m_left = &m_Begin;
                valHook->m_right = &m_End;
                valHook->m_key = key;

                rootHook->m_parent = &val;
                GET_HOOK((&m_Begin), this)->m_parent = &val;
                GET_HOOK((&m_End), this)->m_parent = &val;
                GET_HOOK((&m_End), this)->m_left = nullptr;
            }
            else
                insert(key, &val, topNode);
        }

        void remove(TKey key)
        {
            hook * rootHook = GET_HOOK( (&m_Root), this);

            base_node * currentNode = rootHook->m_parent;

            if(currentNode != &m_End)
                remove(key, currentNode);
        }


        long size()
        {
            return 0;
        }

        iterator begin()
        {
            hook * first = GET_HOOK( (&m_Begin), this);

            return iterator(this, first->m_parent);
        }

        iterator end()
        {
            return iterator(this, &m_End);
        }

        iterator find(TKey key)
        {

        }

        void print()
        {
            hook * rootHook = GET_HOOK( (&m_Root), this);

            base_node * topNode = rootHook->m_parent;

            int level = 0;
            print(topNode, level);
        }

    private:

        void print(base_node * current, int & height)
        {
            hook * currentHook = GET_HOOK(current, this);

            if(currentHook->m_right != nullptr)
            {
                print(currentHook->m_right, ++height);
            }

            for(int i = 0; i < height; i++)
            {
                std::cout << "     ";
            }

            if(current->is_terminal_begin())
                std::cout << "B";
            else if(current->is_terminal_end())
                std::cout << "E";
            else
                std::cout << static_cast<T *>(current)->m_meh << std::endl;

            std::cout << "\n";

            if(currentHook->m_left != nullptr)
            {
                print(currentHook->m_left, ++height);
            }
            height--;
        }

        void insert(TKey &key, base_node * insertNode, base_node * current)
        {
            hook* currentHook = GET_HOOK(current, this);

            if(key < currentHook->m_key)
            {
                if(currentHook->m_left != nullptr && !currentHook->m_left->is_terminal())
                    insert(key, insertNode, currentHook->m_left);
                else
                {
                    hook * insertHook = GET_HOOK(insertNode, this);
                    hook * leftHook;

                    insertHook->m_parent = current;
                    insertHook->m_left = currentHook->m_left;
                    insertHook->m_right = nullptr;
                    insertHook->m_key = key;

                    if(currentHook->m_left != nullptr)
                    {
                           leftHook = GET_HOOK(currentHook->m_left, this);
                           leftHook->m_parent = insertNode;
                    }

                    currentHook->m_left = insertNode;
                }
            }
            else if(key > currentHook->m_key)
            {


                if(currentHook->m_right != nullptr && !currentHook->m_right->is_terminal())
                    insert(key, insertNode, currentHook->m_right);
                else
                {

                    hook * insertHook = GET_HOOK(insertNode, this);
                    hook * rightHook;

                    insertHook->m_parent = current;
                    insertHook->m_left = nullptr;
                    insertHook->m_right = currentHook->m_right;
                    insertHook->m_key = key;



                    if(currentHook->m_right != nullptr)
                    {
                           rightHook = GET_HOOK(currentHook->m_right, this);
                           rightHook->m_parent = insertNode;
                    }

                    currentHook->m_right = insertNode;
                }
            }
        }

        bool remove(TKey & key, base_node * current)
        {
            if(current != nullptr && !current->is_terminal())
            {
                hook * currentHook = GET_HOOK(current, this);

                if(key < currentHook->m_key)
                {
                    return remove(key, currentHook->m_left);
                }
                else if(key > currentHook->m_key)
                {
                    return remove(key, currentHook->m_right);
                }

                current->detach();
                return true;
            }

            return false;
        }

    private:
        root_terminator m_Root;
        begin_terminator m_Begin;
        end_terminator m_End;
    };
}
#endif
