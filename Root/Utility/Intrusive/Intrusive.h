/******************************************************************************
 * @Programmer - Rook
 * @File - Intrusive.h
 * @Date - 8/23/2013
 * @Purpose - This file contains an amalgum of abstract classes to be used and
 *       implemented for the purpose of being utilized as an intrusive
 *       container.
 *
 * @Description - The concept of an intrusive container is different than
 *       that of your conventional non-intrusive container, such as the STL
 *       vector. Whereas an STL container uses nodes which contain the
 *       data and the pointers, an intrusive container differs in that the
 *       data itself acts as the node. This means that the pointers must be
 *       members of the object itself.
 *
 *       This provides numerous advantages and disadvantages. The biggest
 *       is that an object can be responsible for removing itself from the list
 *       it belongs to. The data object has scope of the pointers used by the
 *       container, so it can close the gap itself, independant of the
 *       container. Placing this unhooking process within the destructor, you
 *       will find that the object is automatically removed from it's
 *       container when destroyed.
 *
 *       Another advantage is that the container has no ownership of the data.
 *       A vector will push_back or insert copies of data, whereas this type
 *       of container utilizes the instances of data as it's elements.
 *
 *       There were a couple of disadvantages however that i sought to deal
 *       with. First, an object could not belong to more than one list, since
 *       the data itself could contain only one set of pointers to other
 *       objects. Second, the object was only able to belong to a single type
 *       of container at any given time, because the set of pointers was
 *       different for each type of container, (e.g. a list utilizes a next and
 *       previous pointers while a map could utilize left, right, and parent
 *       pointers)
 *
 *       So let's define some terms real quick. First, that set of pointers
 *       relating to other objects, I termed Hooks. The first problem i tackled
 *       was the one of allowing all objects to be able to belong to any
 *       type of container. To contain and manage the hooks, I made an object I
 *       call a Node. Nodes provide a level of abstraction in which all nearby
 *       elements are able to obtain the corresponding hook through.
 *       They all have a method called, get_hook() which returns a hook via the
 *       common interface, base_hook*. When a node is added to a list, the
 *       list will dynamically create a hook of the type needed, to be
 *       contained within the node. Then when an operation within the
 *       container needed to utilize the private members of the hook, the
 *       container knew that the hook had to be one it created, so you can
 *       upcast from there and use the hook. It requires a couple extra
 *       function calls and a static cast, but now you have a uniform way of
 *       obtaining and accessing hooks of any different type.
 *
 *       With the any_hook framework layed out, it was relatively simple to
 *       implement a hook that could belong to multiple containers at a time.
 *       you just needed to set up the base_hook in such a way that the
 *       get_hook() method could uniquely identify the list accessing the node
 *       and then the node would pass back the correct hook belonging to that
 *       list. For the unique id, i decided that the lists object memory
 *       address would suffice, so the keys would be pointers to the
 *       containers. To actually contain multiple hooks dynamically though, i
 *       turned back to the STL and their non-intrusive map container. The map
 *       would use container pointers as the key, and then contain the hooks
 *       at the index. Now whenever a container wanted to use a hook, it would
 *       call get_hook(container*) on the node, pass in a pointer to itself,
 *       and the node would return the hook belonging to that container. For
 *       this reason however, there is no way (yet) to enter an object into
 *       the same list multiple times.
 *
 *       A pleasant surprise was not only that the dynamic_node could belong to
 *       multiple containers, but also to multiple DIFFERENT TYPE containers.
 *       This opened opened up a new realm of possibility for access to lists
 *       of data. For example, let's say you have a non-intrusive list of
 *       component objects in a random order. Then you want to have a bunch of
 *       entities which are comprised of those components. Then you just make
 *       an intrusive list which represents the entity, and add the components
 *       to it. The object is never copied, can belong to multiple containers,
 *       can be iterated and manipulated by any of them, and the best part,
 *       when an object is destroyed in the non-intrusive container, it is
 *       automatically removed from any other intrusive container it belongs
 *       to.
 *
 *       Fucking Golden.
 *                                                                - Rook
 *****************************************************************************/

#ifndef INTRUSIVE_H
#define INTRUSIVE_H

#include <iostream>
#include <stdexcept>
#include <memory>
#include <map>

namespace intrusive
{
    /**
     *Macro Definition
     * @name - GET_HOOK(I, cont_ptr)
     * @scope - intrusive
     * @purpose - used to easily obtain the hook from a given node
     * @pre - the node passed in must belong to the container
     * @param I - this is a pointer to the node of which you need the hook.
     * @param cont_ptr - a pointer to the list who is requesting the hook.
     * @return - hands back a base_hook * pointing to the hook in question
     */
    #define GET_HOOK(I, cont_ptr) static_cast<hook*>(I->get_hook(cont_ptr))

    // Forward Declaration
    class base_container;

    /**********************************************************************
     * Abstract Class Declaration
     *
     * @name - base_hook
     *
     * @scope - intrusive
     *
     * @inherits
     *
     * @desc - every hook needs to handle detachment from the list itself
     *      because each hook from different containers will have different
     *      methods of closing that gap. Therefore, the node, at destruction
     *      needs to call the unhook method for each hook. When creating a
     *      new container implementation, the hook definition for your class
     *      must derive from and implement this class.
     *********************************************************************/
    class base_hook
    {
    public:
        /**
         * Virtual Function Declaration
         * @name - ~base_hook()
         * @scope - intrusive::base_hook
         * @purpose - virtual destructor for inherited objects
         */
        virtual ~base_hook() {}

        /**
         * Pure Virtual Function Declaration
         * @name - unhook(base_container*)
         * @scope - intrusive::base_hook
         * @purpose - you call this on any hook to have it detach itself from
         *       the other hooks in the container.
         * @pre - The hook must have been made by the container passed in.
         * @param base_container - the pointer to the container whom should be
         *       unhooked from. This is used as an identifier.
         */
        virtual void unhook(base_container*) = 0;
    };

    /**********************************************************************
     * Abstract Class Declaration
     *
     * @name - base_container
     *
     * @scope - intrusive
     *
     * @inherits
     *
     * @desc - Every container must implement a factory method which is
     *       used to dynamically create that containers hook type so that
     *       a node can create a hook to manage.
     *********************************************************************/
    class base_container
    {
    public:
        /**
         * Virtual Function Declaration
         * @name - ~base_container()
         * @scope - intrusive::base_container
         * @purpose - virtual destructor for inherited objects
         */
        virtual ~base_container() {}

        /**
         * Pure Virtual Function Declaration
         * @name - create_hook()
         * @scope - intrusive::base_container
         * @purpose - Will dynamically create a hook specific to the type of
         *        and return it, pointed to by the common interface base_hook
         * @return - a base_hook * which points to a new instance of the hook
         *       specific to the container type.
         */
        virtual base_hook* create_hook() = 0;
    };

    /**********************************************************************
     * Abstract Class Declaration
     *
     * @name - base_node
     *
     * @scope - intrusive
     *
     * @inherits
     *
     * @desc - This is the public interface that a container will use to
     *       to obtain and manipulate hooks through. The node should have
     *       no knowledge or need to know the type of hook it contains.
     *
     * @warning - These methods are public because they have to be. Yes, you
     *       can call them on the derived objects, but results will vary from
     *       painful to catastrophic in most situations. Leave these to the
     *       professionals, (the containers using them).
     *********************************************************************/
    class base_node
    {
    public:
        /**
         * Virtual Function Declaration
         * @name - ~base_node()
         * @scope - intrusive::base_node
         * @purpose - virtual destructor for inherited objects
         */
        virtual ~base_node() {}

        /**
         * Pure Virtual Function Declaration
         * @name - is_terminal() const
         * @scope - intrusive::base_node
         * @purpose - This is used to determine if the node is a terminal node
         * @pre - the node must be managing a hook
         * @return - will return true if terminal, not true otherwise
         */
        virtual bool is_terminal() const = 0;

        /**
         * Pure Virtual Function Declaration
         * @name - is_terminal_begin() const
         * @scope - intrusive::base_node
         * @purpose - This is used to determine if the node is the terminator
         *       at the beginning of iteration
         * @pre - the node must be managing a hook
         * @return - will return true if it's terminal AND at the beginning of
         *       iteration, and false otherwise.
         */
        virtual bool is_terminal_begin() const = 0;

        /**
         * Pure Virtual Function Declaration
         * @name - is_terminal_end() const
         * @scope - intrusive::base_node
         * @purpose - This is used to determine if the node is the terminator
         *       at the end of iteration.
         * @pre - the node must be managing a hook
         * @return - will return true if it's terminal AND at the end of
         *       iteration, and false otherwise.
         */
        virtual bool is_terminal_end() const = 0;

        /**
         * Pure Virtual Function Declaration
         * @name - attach(base_container * listptr)
         * @scope - intrusive::base_node
         * @purpose - This is used by the container to attach it to itself, or
         *       rather to claim it, and create a hook for itself within the
         *       node. Any insert operations in the container should call this
         *       before performing ANY operations to make sure there is a valid
         *       hook for the container.
         * @post - the object should have a valid hook to be used by the
         *       container.
         * @param listptr - a pointer to the container which will be used to
         *       call it's create_hook() method and obtain a hook from it.
         */
        virtual void attach(base_container * listptr) = 0;

        /**
         * Pure Virtual Function Declaration
         * @name - detach()
         * @scope - intrusive::base_node
         * @purpose - This is used to detach the node from any and all
         *       containers it belongs to. This can be done uniformly by
         *       calling the unhook method on all it's managed hooks.
         * @post - All managed hooks will be disconnected, and in the case
         *       of dynamically allocated hooks, all hooks will be deleted
         */
        virtual void detach() = 0;

        /**
         * Pure Virtual Function Declaration
         * @name - get_hook(base_container*)
         * @scope - intrusive::base_node
         * @purpose - the method used to obtain the hook for the given
         *       container. The pointer is used as a unique identifier.
         * @return - a hook is returned for the given container.
         */
        virtual base_hook* get_hook(base_container*) = 0;
    };

    /**********************************************************************
     * Class Declaration
     *
     * @name - template<class TContainer> static_node
     *
     * @scope - intrusive
     *
     * @inherits - intrusive::base_node
     *
     * @desc - this object represents the absolute bare minimum needed for an
     *       object to to be used in an intrusive container. A static node
     *       contains only one type of hook, and can therefore, belong to only
     *       one type of container, and only one container at a time. The
     *       tradeoff is that there is no overhead at all in obtaining the
     *       hook besides the function call and static cast, and there is
     *       no use or need for dynamic allocation of hooks. The template
     *       parameter to this class is the container type it is to belong
     *       to.
     *
     *********************************************************************/
    template<template<class> class TContainer>
    class static_node :
        public base_node
    {
    public:
        /**
         * Virtual Function Declaration
         * @name - ~static_node()
         * @scope - template< template<class> class TContainer>
         *          intrusive::static_node<TContainer>
         * @purpose - virtual destructor for inherited objects
         */
        virtual ~static_node() {detach();}

        /**
         * Function Declaration
         * @name - is_terminal() const final
         * @scope - template< template<class> class TContainer>
         *          intrusive::static_node<TContainer>
         * @purpose - used to determine whether this node is
         *       terminal. Since it is a static_node, it will
         *       always return false. This overwrites the
         *       method declared in intrusive::base_node.
         * @return - always returns false.
         * -- INLINE --
         */
        bool is_terminal() const final {return false;}

        /**
         * Function Declaration
         * @name - is_terminal_begin() const final
         * @scope - template< template<class> class TContainer>
         *          intrusive::static_node<TContainer>
         * @purpose - used to determine whether this node is a terminator
         *       and at the beginning of the container. Since it is a static
         *       node, it will always return false. This overwrites the method
         *       declared in intrusive::base_node.
         * @return - always returns false.
         * -- INLINE --
         */
        bool is_terminal_begin() const final {return false;}

        /**
         * Function Declaration
         * @name - is_terminal_end() const final
         * @scope - template< template<class> class TContainer>
         *          intrusive::static_node<TContainer>
         * @purpose - used to determine whether this node is a terminator and
         *       at the end of the container. Since it is a static_node, it
         *       will always return false. This overwrites the method declared
         *       in intrusive::base_node.
         * @return - always returns false
         * -- INLINE --
         */
        bool is_terminal_end() const final {return false;}

        /**
         * Function Declaration
         * @name - attach()
         * @scope - template< template<class> class TContainer>
         *          intrusive::static_node<TContainer>
         * @purpose - simply sets the owner of this node. This overwrites the
         *       method declared in intrusive::base_node
         * @pre - listptr cannot be null and must point to a valid container
         * @post - the node will be owned by the container.
         * -- INLINE --
         */
        void attach(base_container* listptr) {m_listptr = listptr;}

        /**
         * Function Declaration
         * @name - detach()
         * @scope - template< template<class> class TContainer>
         *          intrusive::static_node<TContainer>
         * @purpose - tells the hook to remove itself from the container,
         *       effectively freeing ownership of this node. This overwrites
         *       the method declared in intrusive::base_node
         * @post - the hook close the gap in the container, and the node will
         *       free to be part of another container.
         * -- INLINE --
         */
        void detach() {m_hook.unhook(m_listptr);}

        /**
         * Function Declaration
         * @name - get_hook(base_container*)
         * @scope - template< template<class> class TContainer>
         *          intrusive::static_node<TContainer>
         * @purpose - retrieves the hook owned by the static_node. Since it
         *       only ever owns one, there is no need to search for it, and
         *       therefore, does not need the pointer to the container needing
         *       it. This overwrites the method declared in
         *       intrusive::base_node.
         * @pre - the object must be attached or belong to the container before
         *       this method is called
         * @param - unused
         * @return - a pointer to the hook owned by the node is returned
         * -- INLINE --
         */
        base_hook* get_hook(base_container*) {return &m_hook;}

    private:
        base_container* m_listptr;
        // since the hook class member of the container class should never be
        // be dependant upon the template parameter, the template parameter
        // used does not matter to make the hook.
        typename TContainer<base_node>::hook m_hook;
    };

    /**********************************************************************
     * Class Declaration
     *
     * @name - any_node
     *
     * @scope - intrusive
     *
     * @inherits - intrusive::base_node
     *
     * @desc - this object, when inherited, allows an object to belong to
     *       any type of container, regardless of the type of hook it needs.
     *       when inserted into any container, this node will obtain a hook
     *       dynamically via the create_hook method of the container that wants
     *       it. The only difference in performance is that, rather then having
     *       hook statically, it must be dynamically allocated and deallocated
     *       when inserted into a different container. It can still only belong
     *       to one container at a time however, so it will only use one hook,
     *       making it still lightweight and efficient.
     *
     *********************************************************************/
    class any_node :
        public base_node
    {
    public:
        /**
         * Function Declaration
         * @name - any_node()
         * @scope - intrusive::any_node
         * @purpose - default constructor, nulls out the elements
         * -- INLINE --
         */
        any_node() :
            m_listptr(nullptr),
            m_hook(nullptr)
            {}

        /**
         * Virtual Function Declaration
         * @name - ~any_node()
         * @scope - intrusive::any_node
         * @purpose - virtual destructor for inherited objects
         * -- INLINE --
         */
        virtual ~any_node() { detach();}

        /**
         * Function Declaration
         * @name - is_terminal() const final
         * @scope - intrusive::any_node
         * @purpose - used to determine if this node is a terminator. Since
         *       it can never be, it will always return false. This overwrites
         *       the method declared in intrusive::base_node.
         * @return - ALWAYS returns false
         * -- INLINE --
         */
        bool is_terminal() const final {return false;}

        /**
         * Function Declaration
         * @name - is_terminal_begin() const final {return false;}
         * @scope - intrusive::any_node
         * @purpose - used to determine if this node is a terminator and at the
         *       beginning of the container. Since it can never be, it will
         *       always return false. This overwrites the method declared in
         *       intrusive::base_node
         * @return - ALWAYS returns false
         * -- INLINE --
         */
        bool is_terminal_begin() const final {return false;}

        /**
         * Function Declaration
         * @name - is_terminal_end() const final {return false;}
         * @scope - intrusive::any_node
         * @purpose - used to determine if this node is a terminator and at the
         *       end of the container. Since it can never be, it will always
         *       return false. This overwrites the method declared in
         *       intrusive::base_node.
         * @return - ALWAYS returns false
         * -- INLINE --
         */
        bool is_terminal_end() const final {return false;}

        /**
         * Function Declaration
         * @name - attach(base_container*)
         * @scope - intrusive::any_node
         * @purpose - this function is used by a container to claim it, and
         *       more importantly, create a valid hook for the container to
         *       use.
         * @pre - listptr CANNOT be null and must point to a valid container
         * @post - the node will belong to the container passed in, and there
         *       will be a valid hook for the container to obtain and use
         * @param - a pointer to the container to which is claiming this node
         */
        void attach(base_container* listptr);

        /**
         * Function Declaration
         * @name - detach()
         * @scope - intrusive::any_node
         * @purpose - this will use the hooks unhook method to close the gap in
         *       the container and then deallocate the hook, freeing this node
         *       to used by any another container
         * @pre - must be attached to a container already
         * @post - the node will be detached entirely, and the node will be free
         *       to use in another container
         */
        void detach();

        /**
         * Function Declaration
         * @name - get_hook(base_container*)
         * @scope - intrusive::any_hook
         * @purpose - retrieves the hook owned by the any_node. Since it only
         *       ever owns one, there is no need to search for it, and
         *       therefore, it does not need the pointer to the container
         *       needing it. This overwrites the method declared in
         *       intrusive::base_node.
         * @return - a pointer to the hook owned by the node is returned
         * -- INLINE --
         */
        base_hook* get_hook(base_container*) { return m_hook.get();}

    private:
        base_container * m_listptr;
        std::shared_ptr<base_hook> m_hook;
    };


    /**********************************************************************
     * Class Declaration
     *
     * @name - dynamic_node
     *
     * @scope - intrusive
     *
     * @inherits - intrusive::base_node
     *
     * @desc - This is, in my opinion, what makes these intrusive containers
     *       worth it. A dynamic_node lifts the restriction of a node,
     *       belonging to only one container at once. This required that the
     *       node be able to hold more than one hook at a time, and to do that
     *       i used the std::map to hold the hooks with indexes to the
     *       container that they belong to. When a container needs the hook
     *       it just calls the get_hook method like any other node, except the
     *       container pointer passed in is utilized as an index to look up the
     *       corresponding node. Even better, using the base_hook pointers, the
     *       map can hold any type of hook, which, in summary, means that a
     *       dynamic node can belong to any container of any type at any time.
     *
     *       The only real tradeoff here is that depending on the number of
     *       containers the node belongs to, the node will grow quite large,
     *       and the operations within the containers can become quite
     *       quite expensive as the calls to get_hook() stack up. You can use
     *       them in a couple containers, but things can become quite slow if
     *       a node belongs to too many containers, so use them wisely, and
     *       they in turn will show you their true potential. Give and take.
     *
     *********************************************************************/
    class dynamic_node :
        public base_node
    {
        // too lazy to type this type a lot
        typedef std::map<base_container* ,std::shared_ptr<base_hook> > map_type;

    public:
        /**
         * Virtual Function Declaration
         * @name - ~dynamic_node()
         * @scope - intrusive::dynamic_node
         * @purpose - detach itself from all containers
         *       and is virtual for inheriting classes.
         * -- INLINE --
         */
        virtual ~dynamic_node() { detach();}

        /**
         * Function Declaration
         * @name - is_terminal() const final
         * @scope - intrusive::dynamic_node
         * @purpose - used to determine if this node is a terminator. Since
         *       it can never be, it will always return false. This overwrites
         *       the method declared in intrusive::base_node
         * @return - ALWAYS returns false
         * -- INLINE --
         */
        bool is_terminal() const final {return false;}

        /**
         * Function Declaration
         * @name - is_terminal() const final
         * @scope - intrusive::dynamic_node
         * @purpose - used to determine if this node is a terminator and at the
         *       beginning of the container. Since it can never be, it will
         *       always return false. This overwrites the method declared in
         *       intrusive::base_node
         * @return - ALWAYS returns false
         * -- INLINE --
         */
        bool is_terminal_begin() const final {return false;}

        /**
         * Function Declaration
         * @name - is_terminal_end() const final
         * @scope - intrusive::dynamice_node
         * @purpose - used to determine if this node is a terminator and at the
         *       end of the container. Since it can never be, it will always
         *       return false. This overwrites the method declared in
         *       intrusive::base_node.
         * @return - ALWAYS returns false
         * -- INLINE --
         */
        bool is_terminal_end() const final {return false;}

        /**
         * Function Declaration
         * @name - attach(base_container* listptr)
         * @scope - intrusive::dynamic_node
         * @purpose - This function is used to attach this node to the
         *       container and create a valid hook for it to use, adding it
         *       in the map of hooks, to be recalled later. The pointer to the
         *       container is used as an index to the obtain it later.
         * @pre - listptr CANNOT be null and must point to a valid container
         * @post - a hook will be added to the hook map to be used by later by
         *       the container.
         * @param listptr - a pointer to a valid container to obtain a hook
         *       from
         */
        void attach(base_container* listptr);

        /**
         * Function Declaration
         * @name - detach()
         * @scope - intrusive::dynamic_node
         * @purpose - used to close the gap in all the containers the node
         *       belongs to, and then destroy all the hooks currently in the
         *       map. Becauset the hooks are managed by smart pointers, all
         *       the hooks are destroyed automatically when the map is cleared.
         * @post - the node will be removed from all containers, and the hooks
         *       destroyed
         */
        void detach();

        /**
         * Function Declaration
         * @name - get_hook(base_container*)
         * @scope - intrusive::dynamic_node
         * @purpose - This is used to obtain the hook corresponding to the
         *       given container. The pointer is used to navigate the map
         *       and locate the hook used by the container.
         * @pre - listptr CANNOT be null and must point to a valid container
         * @return - a pointer to the hook used by the container is returned.
         * -- INLINE --
         */
        base_hook* get_hook(base_container* listptr) { return m_hooks[listptr].get();}

    private:
        map_type m_hooks;
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
