/* 
    A simple framework for unit testing
*/
#ifndef __UNIT_TEST_HPP
#define __UNIT_TEST_HPP
#include <sstream>

/* 
    Create global-scope instances of this class to auto-register the unit tests and
    override the () operator to implement the test
    
    e.g. 
    class my_test : public unit_test {
        virtual void operator() () {
            ...
        }
    } my_test_instance;
*/
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
    
    /* we don't expect tests to be defined in anything else other than the global
       scope, but just in case - this de-registers */
    virtual ~unit_test();
};


/*
    Instantiate this and call run() to run all the unit tests registered in the program 
*/
class unit_test_runner
{
protected:
    /* allow unit_test to access the protected methods in this class */
    friend class unit_test;
    
    /* the current runner - used for the call backs */
    static __thread unit_test_runner * current; 
    
    int test_count = 0;
    int fail_count = 0;
    
public:
    /* determines whether passed tests are reported to the output */
    bool show_passed = false;
    
    /* runs all registered tests */
    void run();
};

#endif
