#include "unit_test.hpp"
#include <list>
#include "terminal.hpp"
#include <iostream>
#include <typeinfo>

using namespace std;

__thread unit_test_runner * unit_test_runner::current; 
    
/* we can't use an instance type directly because of 
   https://isocpp.org/wiki/faq/ctors#static-init-order */
list<unit_test*> & tests()
{
    static list<unit_test*> * tests = new list<unit_test*>();
    return *tests;    
}

void unit_test::section(const std::string & name)
{
    std::cout << "[section] " << name << terminal::reset << endl;
}

bool unit_test::check(bool outcome, const std::string & description)
{
    unit_test_runner::current->test_count++;
    if (!outcome)
    {
        unit_test_runner::current->test_count++;
        std::cout << terminal::red << "[failed] " << description << terminal::reset << endl;
    }    
    else
    {
        if (unit_test_runner::current->show_passed)
            std::cout << terminal::light_gray << "[passed] " << description << terminal::reset << endl;
    }
    return outcome;
}

unit_test::unit_test()
{
    tests().push_back(this);
}

void unit_test_runner::run()
{
    test_count = 0;
    fail_count = 0;
    current = this;
    for (auto i = tests().begin(); i != tests().end(); i++)
    {
        try {
            (*i)->operator()();
        } catch (std::exception & e)
        {
            cout << terminal::red << "[failed with exception] " << e.what() << terminal::reset << endl;
        }
    }
    cout << (test_count - fail_count) << " tests passed" << endl;
    if (fail_count) cout << terminal::red;
    cout << fail_count << " tests failed" << endl;
    cout << terminal::reset;
}

unit_test::~unit_test()
{
    tests().remove(this);
}