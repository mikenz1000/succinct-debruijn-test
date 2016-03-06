/* 
    Test util for comparing a succinct de bruijn for correctness against a basic de bruijn
*/
#pragma once
#ifndef __DEBRUIJN_TEST_HPP
#define __DEBRUIJN_TEST_HPP
#include "debruijn_succinct.hpp"
#include "debruijn_basic.hpp"
#include "unit_test.hpp"

class debruijn_comparer
{
public:
    debruijn_succinct & db;
    debruijn_basic & db_basic;
    std::string alphabet;
    debruijn_comparer(debruijn_basic & db_basic, debruijn_succinct & db, const std::string & alphabet)
    : db(db), db_basic(db_basic),alphabet(alphabet)
    {
        
    }
    
    void run(unit_test * ut)
    {
        for (size_t v = 0;v < db.num_nodes();v ++)
        {
            size_t v_basic = db_basic.find_node(db.label(v));
            //section((std::stringstream() << "checking node " << v << " " << db.label(v)).str());
                
            ut->check_equal(true, v_basic != db.no_node, "found the label in basic");
            ut->check_equal(db_basic.indegree(v_basic), db.indegree(v), (std::stringstream() << "indegree(" << v << ")").str());
            ut->check_equal(db_basic.outdegree(v_basic), db.outdegree(v), (std::stringstream() << "outdegree(" << v << ")").str());

            // and check the outgoing edges
            for (size_t i = 0; i < alphabet.size(); i++)
            {
                //section((std::stringstream() << "outgoing(" << v << "," << alphabet[i] << ")").str());
                
                // the edge we will follow
                char x = alphabet[i];
                
                // find out the benchmark result
                size_t outgoing_basic = db_basic.outgoing(v_basic, x);
                if (outgoing_basic == db_basic.no_node)
                {
                    // assert it doesn't exist either in the db graph
                    ut->check_equal(db.no_node, db.outgoing(v, x), (std::stringstream() << "outgoing(" << v << "," << alphabet[i] << ") [no node]").str());
                }
                else
                {
                    size_t v2 = db.outgoing(v,x);
                    ut->check_equal(true, v2 != db.no_node, "outgoing shouldn't return no_node");
                    ut->check_equal(db_basic.label(outgoing_basic), db.label(v2), (std::stringstream() << "outgoing(" << v << "," << alphabet[i] << ")").str());
                }
            }
            
            // and check the incoming edges
            for (size_t i = 0; i < alphabet.size(); i++)
            {
                //section((std::stringstream() << "incoming(" << v << "," << alphabet[i] << ")").str());
                
                // the edge we will follow
                char x = alphabet[i];
                
                // find out the benchmark result
                size_t incoming_basic = db_basic.incoming(v_basic, x);
                if (incoming_basic == db_basic.no_node)
                {
                    // assert it doesn't exist either in the db graph
                    ut->check_equal(db.no_node, db.incoming(v, x), (std::stringstream() << "incoming(" << v << "," << alphabet[i] << ") [no node]").str());
                }
                else
                {
                    size_t v2 = db.incoming(v,x);
                    ut->check_equal(true, v2 != db.no_node, "incoming shouldn't return no_node");
                    ut->check_equal(db_basic.label(incoming_basic), db.label(v2), (std::stringstream() << "incoming(" << v << "," << alphabet[i] << ")").str());
                }
            }
        }
    }
};

#endif