/* 
    Tests the debruijn succinct implementation vs basic, using the graph in the paper 
    http://alexbowe.com/succinct-debruijn-graphs/
*/
#include "unit_test.hpp"
#include "debruijn_succinct.hpp"
#include "debruijn_basic.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "debruijn_comparer.hpp"

class test_debruijn_t : public unit_test
{
    void operator() ()
    {
        section("simple debruijn test based on the example data in the paper");
        std::string test = "TACGTCGACGACT";
        std::string alphabet = "$ACGT";
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
        debruijn_succinct db(4,alphabet,kmers.begin(),kmers.end());
                
        // and .. test it
        check_equal(5UL, db.forward(2), "forward(2)");
        check_equal(2UL, db.backward(5), "backward(5)");
        check_equal(7UL, db.backward(1), "backward(1)");
        check_equal(3UL, db.backward(7), "backward(1)");
        check_equal(db.no_node, db.backward(0), "backward(1)");
        
        check_equal(2, db.outdegree(6), "outdegree(6)");
        check_equal(1, db.outdegree(0), "outdegree(0)");
        check_equal(12UL, db.forward(8), "forward(8)");
        check_equal(10UL, db.outgoing(6,'T'), "outgoing(6,T)");
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
        check_equal(std::string("$$$"), db.label(0), "label(0)");
        check_equal(std::string("CGA"), db.label(1), "label(1)");
        check_equal(std::string("$TA"), db.label(2), "label(2)");
        check_equal(std::string("GAC"), db.label(3), "label(3)");
        check_equal(std::string("TAC"), db.label(4), "label(4)");
        check_equal(std::string("GTC"), db.label(5), "label(5)");
        check_equal(std::string("ACG"), db.label(6), "label(6)");
        check_equal(std::string("TCG"), db.label(7), "label(7)");
        check_equal(std::string("$$T"), db.label(8), "label(8)");
        check_equal(std::string("ACT"), db.label(9), "label(9)");
        check_equal(std::string("CGT"), db.label(10), "label(10)");
        
        check_equal(0,db.indegree(0), "indegree(0)");
        check_equal(2,db.indegree(1), "indegree(1)");
        check_equal(2,db.indegree(6), "indegree(6)");
        check_equal(1,db.indegree(2), "indegree(2)");
        
        check_equal(4UL,db.incoming(6,'T'), "incoming(6,T)");
        
        check_equal(11UL, db.num_nodes(), "num_nodes");
        
        section("test succinct against basic debruijn");
        debruijn_basic db_basic(4,kmers.begin(),kmers.end());
        debruijn_comparer dc(db_basic,db,alphabet);
        dc.run(this);
       
    }
    
} test_debruijn;