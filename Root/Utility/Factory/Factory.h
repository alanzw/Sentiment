#ifndef FACTORY_H
#define FACTORY_H

// library tools
#include <map>
#include <string>
#include <typeinfo>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cxxabi.h>

const std::string demangle(const char* name);

template<class base>
class Factory
{
protected:
	template<class T>
	static base * createT() { return new T;}

    typedef std::map<std::string, base*(*)()> map_type;

    static map_type& GetMap()
    {
        static map_type map;
        return map;
    }

public:
	virtual ~Factory(){}

	static base * createInstance(const std::string & s)
    {
    	if(!GetMap().count(s))
    		return nullptr;
		typename map_type::iterator it = GetMap().find(s);
    	return it->second();
    }

	template <class TDerived>
	struct DerivedRegister :
	    public Factory<base>
	{
	    DerivedRegister()
        {
            std::cout << "wakka\n";
            std::string name = demangle(typeid(TDerived).name());
            GetMap().insert(std::pair<std::string, base*(*)()>(name, &createT<TDerived>));
        }

        DerivedRegister(const std::string& name)
        {
            GetMap().insert(std::pair<std::string, base*(*)()>(name, &createT<TDerived>));
        }

        void func() {}

	};

	static void GetNames(std::vector<std::string>& names)
	{
	    names.clear();

	    for(auto it = GetMap().begin(); it != GetMap().end(); ++it)
	        names.push_back(it->first);

	}
};


#endif
