/* 
    Defines a framework for unit testing
*/
#ifndef __UNIT_TEST_HPP
#define __UNIT_TEST_HPP

class unit_test
{
protected:
    /* Call this to indicate the start of a section of the testing */
    void section(const char * name);
    
    /* Call this to record the outcome of each test 
       If outcome is false the test failed */
    void check(bool outcome, const char * description);
public:
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
