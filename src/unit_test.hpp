/* 
    Defines a framework for unit testing
*/
#ifndef __UNIT_TEST_HPP
#define __UNIT_TEST_HPP
#include <sstream>

class unit_test
{
public:

    /* Call this to indicate the start of a section of the testing */
    void section(const std::string & name);
    
    /* Call this to record the outcome of each test 
       If outcome is false the test failed 
       returns outcome so you can abort the rest of the test if that makes sense */
    bool check(bool outcome, const std::string & description);
    
    template<typename T>
    bool check_equal(const T & desired, const T & actual, const std::string & description)
    {
        std::stringstream s;
        s << description; 
        s << " (desired = " << desired << ", actual = " << actual << ")";
        return check(desired == actual, s.str());
    }

    /* this method must be overridden in order to execute the test */
    virtual void operator() () = 0;    
    
    /* the constructor is called when the object is instantiated and 
       registers the test with the framework */
    unit_test();
    
    /* call this to run all the tests that have been registered */
    static void run_all();
    
    /* we don't expect tests to be defined in anything else other than the global
       scope, but just in case - this de-registers */
    virtual ~unit_test();
};

#endif
