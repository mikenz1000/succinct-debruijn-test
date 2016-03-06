#include "unit_test.hpp"
#include "debruijn_succinct.hpp"
#include "debruijn_basic.hpp"
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
        debruijn_succinct db(kmers);
                
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
        debruijn_basic db_basic(kmers);
        for (size_t v = 0;v < db.num_nodes();v ++)
        {
            section((std::stringstream() << "checking node " << v).str());
                
            size_t v_basic = db_basic.find_node(db.label(v));
            check_equal(true, v_basic != db.no_node, "found the label in basic");
            check_equal(db_basic.indegree(v_basic), db.indegree(v), "indegree");
            check_equal(db_basic.outdegree(v_basic), db.outdegree(v), "outdegree");

            // and check the outgoing edges
            for (size_t i = 0; i < alphabet.size(); i++)
            {
                section((std::stringstream() << "outgoing(" << v << "," << alphabet[i] << ")").str());
                
                // the edge we will follow
                char x = alphabet[i];
                
                // find out the benchmark result
                size_t outgoing_basic = db_basic.outgoing(v_basic, x);
                if (outgoing_basic == db_basic.no_node)
                {
                    // assert it doesn't exist either in the db graph
                    check_equal(db.no_node, db.outgoing(v, x), "outgoing expecting no_node");
                }
                else
                {
                    size_t v2 = db.outgoing(v,x);
                    check_equal(true, v2 != db.no_node, "outgoing shouldn't return no_node");
                    check_equal(db_basic.label(outgoing_basic), db.label(v2), "outgoing");
                }
            }
            
            // and check the incoming edges
            for (size_t i = 0; i < alphabet.size(); i++)
            {
                section((std::stringstream() << "incoming(" << v << "," << alphabet[i] << ")").str());
                
                // the edge we will follow
                char x = alphabet[i];
                
                // find out the benchmark result
                size_t incoming_basic = db_basic.incoming(v_basic, x);
                if (incoming_basic == db_basic.no_node)
                {
                    // assert it doesn't exist either in the db graph
                    DEBUG_WATCH(incoming_basic)
                    check_equal(db.no_node, db.incoming(v, x), "incoming expecting no_node");
                }
                else
                {
                    DEBUG_WATCH(db_basic.label(incoming_basic))
                    size_t v2 = db.incoming(v,x);
                    check_equal(true, v2 != db.no_node, "incoming shouldn't return no_node");
                    check_equal(db_basic.label(incoming_basic), db.label(v2), "incoming");
                }
            }
        }
        
    }
    
} test_debruijn;