#include "unit_test.hpp"
#include "debruijn.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class test_debruijn_t : public unit_test
{
    void operator() ()
    {
        section("simple debruijn test based on the example data in the paper");
        std::string test = "TACGTCGACGACT";
        const char * i = test.c_str();
        std::vector<std::string> kmers;
        std::string kmer("$$$$");
        
        while (*i)
        {
            // shift kmer to the left by one
            kmer.erase(0,1);
            
            // append the next nucleotide
            kmer.push_back(*i);
            
            // and store
            kmers.push_back(kmer);
            i++;
        }
        
        // and shift it once more and add the $
        kmer.erase(0,1);
        kmer.push_back('$');
        kmers.push_back(kmer);
        
        // build the graph!
        debruijn db(kmers);
        db.dump();
                
        // and .. test it
        check_equal(5UL, db.forward(2), "forward(2)");
        check_equal(2UL, db.backward(5), "backward(5)");
        check_equal(2, db.outdegree(6), "outdegree(6)");
        check_equal(12UL, db.forward(8), "forward(8)");
        check_equal(10UL, db.outgoing(6,"T"), "outgoing(6,T)");
        const unsigned long forward_expected[13] = 
        {
            10, //0 $$$T -> $$TA
            4,  //1 CGAC -> GACT
            5,  //2 $TAC -> TACG-
            8,  //3 GACG -> ACGT
            11, //4 GACT -> ACT$
            
            8,  //5 TACG- -> ACGT
            9,  //6 GTCG -> TCGA-
            1,  //7 ACGA -> CGAC   
            12, //8 ACGT -> CGTC
            1,  //9 TCGA -> CGAC
            2, //10 $$TA -> $TAC
            (unsigned long)-1,//11 ACT$ -> -1
            6, //12 CGTC -> GTCG
        };
        for (int i = 0; i < 13;i ++)
            check_equal(forward_expected[i], db.forward(i), "forward(i)");
        
    }
    
} test_debruijn;