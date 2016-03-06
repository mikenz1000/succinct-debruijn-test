#include "unit_test.hpp"
#include "rank_select.hpp"
#include <vector>

class test_rank_select_t : public unit_test
{
    void operator() ()
    {
        section("rank select tests");
        std::vector<bool> B { false,true,true,false,true,false,false,true,false,false };
        rank_select<bool> rs(B.begin(), B.end());
        check_equal(3UL,rs.rank(true,5),"rank(5)");
        check_equal(7UL,rs.select(true,4),"select(4)");
    }
    
} test_rank_select;