/******************************************************************************
 * @Programmer - Rook
 * @File - Intrusive.cpp
 * @Date - 8/23/2013
 * @Purpose - This file container all the definitions to the classes declared
 *     declared in Intrusive.h
 *****************************************************************************/

#include "Intrusive.h"

namespace intrusive
{
    void any_node::attach(base_container* listptr)
    {
        m_listptr = listptr;
        m_hook.reset(listptr->create_hook());
    }

    void any_node::detach()
    {
        m_hook->unhook(m_listptr);
        m_listptr = nullptr;
        m_hook.reset();
    }

    void dynamic_node::attach(base_container* listptr)
    {
        if(m_hooks.count(listptr) == 0)
        {
            m_hooks.insert(map_type::value_type(listptr, std::shared_ptr<base_hook>(listptr->create_hook())));
        }

        return;
    }

    void dynamic_node::detach()
    {
        for(auto it = m_hooks.begin(); it != m_hooks.end(); ++it)
        {
            it->second->unhook(it->first);
        }

        m_hooks.clear();
    }
}
