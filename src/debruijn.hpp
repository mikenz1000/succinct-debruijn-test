/*
    Very primitive/simple succint de Bruijn implementation using strings that should be easy to follow
    
    See http://alexbowe.com/succinct-debruijn-graphs/
    
    This is an exercise in understanding how these things work.  Next version will be more optimised!
    
    The kmer length is k.  the node length is therefore k-1.
*/
    
#ifndef __DEBRUIJN_HPP
#define __DEBRUIJN_HPP

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>


class debruijn
{
    /* when we build the graph we need to store this edge data
       it corresponds to one row in the tables in the paper 
       e.g. the second row of the table in the paper is CGA C */
    struct edge {
        /* e.g. CGA (or $$$ or $$T), but reversed order (first element is last character) */
        std::string node_rev;
        
        /* e.g. C
           suffixed with - flag if necessary e.g. T or G- */
        std::string edge;
    };
    static bool edge_sort_function (const edge & i, const edge & j) { return i.node_rev < j.node_rev; }
    
    
    /* the indexes of the start of each letter in the last column of the nodes list */
    std::vector<int> F;
    std::string alphabet = "$ACGT";
    
    /* The edge letter array */
    std::vector<std::string> W;
    
    /* The array of bits indicating if this edge is the last for the node, stored as '0' and '1' */
    std::vector<std::string> L;
    
public:  
     /* for clarity, variables that store indexes of edges and nodes use these two typdefs */
    typedef size_t edge_index_t;
    typedef size_t node_index_t;
    
    /* the node index value for no node (possible return value from some traversal functions) */
    const node_index_t no_node = (node_index_t)(-1);

    /* constructor builds the graph */
    debruijn(const std::vector<std::string> & kmers)
    {                   
        std::vector<edge> edges;
        
        // avoid processing kmers more than once
        std::unordered_set<std::string> done;
        
        // the incoming edge detection e.g. for the kmer GACG the incoming edge detector is ACG
        std::unordered_set<std::string> incoming;
        
        // convert the kmers into nodes 
        for (auto i = kmers.begin(); i != kmers.end(); i ++)
        {
            // uniqueness enforcement
            if (done.count(*i))
                continue;
            done.insert(*i);
            
            edge e;
            
            // work from the back of the string to the front
            int j = (*i).length()-1;
            e.edge = (*i)[j--];
            while (j >= 0)
                e.node_rev.push_back((*i)[j--]);            
            
            edges.push_back(e);            
        }
        
        // sort edges by node_rev
        std::sort(edges.begin(), edges.end(), edge_sort_function);
        
        for (auto i = edges.begin(); i != edges.end(); i++)
        {
            // determine the incoming edge detector e.g. for kmer GACT node_rev is CAG so detector is CA+T
            std::string inc = (*i).node_rev.substr(0, (*i).node_rev.length()-1) + (*i).edge;
            
            // if we saw it already then add the minus
            // this may be in the wrong place if strictly the first occurance in W should be flagged (since the sort happens after)
            if (incoming.count(inc) > 0)
            {
                (*i).edge.push_back('-');
            }
            else
                incoming.insert(inc);
        }
        
        // generate L (by going backwards through the nodes)
        std::string last_node = "";
        for (auto  i = edges.rend(); i != edges.rbegin(); i--)
        {
            if (last_node != i->node_rev)
            {
                last_node = i->node_rev;
                L.insert(L.begin(), "1");
            }           
            else 
                L.insert(L.begin(), "0");
        }
             
        
        // and output
        F.push_back(0); // $ always starts 
        int alphabet_index = 0;
        int edge_index = 0;
        for (auto i = edges.begin();i != edges.end();i ++)
        {
            while (alphabet[alphabet_index] != i->node_rev[0]) 
            {
                F.push_back(edge_index);
                alphabet_index++;                
            }
            edge_index ++;
            ///std::cout << (*i).node_rev << " " << (*i).edge << std::endl;
            W.push_back(i->edge);
        }
    }    
    
    /* write the graph data to cout */
    void dump()
    {
        std::cout << "L\tW" << std::endl;
        {
            for (auto i = L.begin(), j = W.begin();i != L.end(); i++,j++)
                std::cout << *i << "\t" << *j << std::endl;
        }
        for (int i = 0;i < F.size();i ++)
        {
            std::cout << "F[" << alphabet[i] << "] = " << F[i] << std::endl;
        }
    } 
    
    /* returns the number of occurances of c on the range [0,i] */
    size_t rank(const std::vector<std::string> & list, const std::string & c, size_t i)
    {
        size_t result = 0;
        for (size_t j = 0;j <= i;j ++)
            if (list[j] == c) result++;
        return result;
    }
    
    /* return the position of the ith occurance of c */
    size_t select(const std::vector<std::string> & list, const std::string & c, size_t i)
    {
//        if (i == 0) return 0;
        size_t counter = 0;
        for (size_t j = 0;j < list.size(); j++)
        {
            if (list[j] == c)
            {
                counter++;
                if (counter == i) return j;
            }
        }
        throw std::runtime_error("select out of range");
    }
    
    /* return index of the last edge of the node pointed to by edge i */
    edge_index_t forward(edge_index_t i)
    {
        // find the edge label
        std::string C = W[i];
        
        // step 1 - determine the rank
        size_t r = rank(W,C,i);
        
        // step 2 - find first occurance
        std::string::size_type alphabet_index = alphabet.find(C[0]);    
        if (alphabet_index == std::string::npos) 
            throw std::runtime_error("alphabet not recognised");    
        size_t first_occurance = F[alphabet_index];
        
        // step 3 - count the Cs before the first occurange
        size_t Cs_before = rank(L,"1",first_occurance);
        
        // step 4 - add r and "select" to find the edge index of the r'th edge of the node
        size_t result = select(L,"1",Cs_before + r);
        
        return result;        
    }
    
    /* return index of the first edge that points to the node that the edge at i exists */
    edge_index_t backward(edge_index_t i)
    {
        // find the letter of the node edge i comes from...
        
        // step 1 - reverse lookup into F to find which letter must be in the last column of the node list
        size_t alphabet_index = F.size()-1;
        while (F[alphabet_index] > i) alphabet_index--;
        std::string C;
        C.push_back(alphabet[alphabet_index]);    
        
        // steps 2 & 3.1 - find the rank in L to the base and the current edge 
        size_t rank_to_base = rank(L, "1", F[alphabet_index]);
        size_t rank_to_current_edge = rank(L, "1", i);
        
        // step 3.2 "we are at second C" or whatever
        size_t r = rank_to_current_edge - rank_to_base;
        
        // and select to find the position
        return select(W,C,r);
    }
    
    /* return the number of outgoing edges from node v */
    int outdegree(node_index_t v)
    {
        // select to find the position of the v'th 1
        edge_index_t position = select(L, "1", v);
        
        // select to find the position of the previous node (v-1)th */
        edge_index_t previous = (v == 0) ? 0 : select(L, "1", v-1);      
        
        // "boom!" the difference between the edge indexes tells us how many edges leave that node (minus one)
        // slight typo in the text it should be select(7) - select(6) + 1 = 7 - 6 + 1 = 2 */
        return position - previous + 1;  
    }
    
    /* return the node you get to by following edge labelled by symbol c,
       or no_node if there is no edge */
    node_index_t outgoing(node_index_t v, char c)
    {
        return no_node;
    }
};

#endif