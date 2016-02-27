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
        check_equal<debruijn::edge_index_t>(db.forward(2),5, "forward(2)");
        check_equal<debruijn::edge_index_t>(db.backward(5),2, "backward(5)");
        check_equal<debruijn::edge_index_t>(db.outdegree(6),2, "outdegree(6)");
        
        
    }
    
} test_debruijn;