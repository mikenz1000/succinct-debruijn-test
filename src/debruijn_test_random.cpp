/*
    Generates a random graph and compares the succinct and basic debruijn implementations on it
*/
#include "unit_test.hpp"
#include "debruijn_succinct.hpp"
#include "debruijn_basic.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include "debruijn_comparer.hpp"

class test_debruijn_random_t : public unit_test
{
    void operator() ()
    {
        section("randomly generated tests for debruijn graph");
        std::string alphabet = "$ACGT";
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator (seed);
        std::uniform_int_distribution<int> distribution(1,alphabet.size()-1);
        section((std::stringstream() << "random number seed = " << seed).str());
        
        std::vector<std::string> kmers;
        size_t length = generator() % 5 + 20;
        size_t kmer_length = 4;
        
        for (int sequence_repeat = generator() % 4 + 2; sequence_repeat > 0; sequence_repeat--)
        {
            std::string test;
            for (size_t k = 0;k < length;k ++)
                test.push_back(alphabet[distribution(generator)]);
                
            section((std::stringstream() << "sequence = " << test).str());
            const char * i = test.c_str();
            std::string kmer(kmer_length, '$');
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
        }
        // build the graphs
        debruijn_succinct db(kmers,alphabet);
        debruijn_basic db_basic(kmers);
        
        debruijn_comparer dc(db_basic,db,alphabet);
        dc.run(this);
        
    }
    
} test_debruijn_random;