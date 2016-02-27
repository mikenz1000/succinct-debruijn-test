#include "unit_test.hpp"
#include <list>
#include "terminal.hpp"
#include <iostream>
#include <typeinfo>

using namespace std;

/* we can't use an instance type directly because of 
   https://isocpp.org/wiki/faq/ctors#static-init-order */
list<unit_test*> & tests()
{
    static list<unit_test*> * tests = new list<unit_test*>();
    return *tests;    
}

void unit_test::section(const char * name)
{
    std::cout << "[section] " << name << terminal::reset << endl;
}

bool unit_test::check(bool outcome, const char * description)
{
    if (!outcome)
    {
        std::cout << terminal::red << "[failed] " << description << terminal::reset << endl;
    }    
    else
    {
        std::cout << terminal::light_gray << "[passed] " << description << terminal::reset << endl;
    }
    return outcome;
}

unit_test::unit_test()
{
    tests().push_back(this);
}

void unit_test::run_all()
{
    for (auto i = tests().begin(); i != tests().end(); i++)
    {
        try {
            (*i)->operator()();
        } catch (std::exception & e)
        {
            cout << terminal::red << "[failed with exception] " << e.what() << terminal::reset << endl;
        }
    }
}

unit_test::~unit_test()
{
    tests().remove(this);
}