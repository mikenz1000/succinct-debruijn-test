/*
    Very simple (easy to follow?) Succint de Bruijn implementation using strings that should be easy to follow
    
    See http://alexbowe.com/succinct-debruijn-graphs/
    
    This is an exercise in understanding the algorithm.  Not in any way meant to be efficient...
 
*/
    
#ifndef __DEBRUIJN_HPP
#define __DEBRUIJN_HPP

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

/* haven't set up with an IDE that lets me step through, so... */
#if false
#define DEBUG_WATCH(v) std::cout << #v << " = " << v << std::endl;
#define DEBUG_OUT(msg) std::cout << msg << std::endl;
#else
#define DEBUG_WATCH(v)
#define DEBUG_OUT(msg)
#endif

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
    static bool edge_sort_function (const edge & i, const edge & j) { 
        if (i.node_rev == j.node_rev) 
            // i observed in the paper that W is sorted in increasing order too whenever the nodes are the same
            // probably makes no difference but making the implementation match the paper exactly
            return i.edge < j.edge;
        else 
            return i.node_rev < j.node_rev; 
    }
    
    
    /* the indexes of the start of each letter in the last column of the nodes list */
    std::vector<int> F;
    std::string alphabet = "$ACGT";
    
    /* The edge letter array */
    std::vector<std::string> W;
    
    /* The array of bits indicating if this edge is the last for the node, stored as '0' and '1' */
    std::vector<std::string> L;
    
    /* the length of the kmers */
    int k;
    
public:  
     /* for clarity, variables that store indexes of edges and nodes use these two typdefs */
    typedef size_t edge_index_t;
    typedef size_t node_index_t;
    
    /* the node index value for no node (possible return value from some traversal functions) */
    const node_index_t no_node = (node_index_t)(-1);

    /* constructor builds the graph */
    debruijn(const std::vector<std::string> & kmers)
    {             
        k = kmers[0].length();      
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
        for (int ix = edges.size()-1;ix >= 0;ix --)
        {
            edge * i = &edges[ix];
            DEBUG_WATCH(last_node)
            DEBUG_WATCH(i->node_rev)
            
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
            std::cout << (*i).node_rev << " " << (*i).edge << std::endl;
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
    
    /* return the position of the ith occurance of c
       or list.size() if there are only i-1 occurances */
    size_t select(const std::vector<std::string> & list, const std::string & c, size_t i)
    {
//        if (i == 0) return 0;
        DEBUG_OUT("select")
        DEBUG_WATCH(i)
        DEBUG_WATCH(list.size())
        size_t counter = 0;
        for (size_t j = 0;j < list.size(); j++)
        {
            if (list[j] == c)
            {
                counter++;
                if (counter == i) return j;
            }
        }
        DEBUG_WATCH(counter)
        
        // if we try to select the n+1'th when there are only n, return the index one past the end of the array
        if (counter == i-1) return list.size();
        
        throw std::runtime_error("select out of range");
    }
    
    /* return index of the last edge of the node pointed to by edge i */
    edge_index_t forward(edge_index_t i)
    {
        DEBUG_OUT("forward")
        DEBUG_WATCH(i)
        
        // find the edge label
        std::string C = W[i];
        
        // step 1 - determine the rank
        size_t r = rank(W,C,i);
        
        // step 2 - find first occurance
        std::string::size_type alphabet_index = alphabet.find(C[0]);    
        DEBUG_WATCH(alphabet_index)
        if (alphabet_index == std::string::npos) 
            throw std::runtime_error("alphabet not recognised"); 
            
        // can't follow a $ edge  
        if (alphabet_index == 0)
            return no_node; 
            
        size_t first_occurance = F[alphabet_index];
        DEBUG_WATCH(first_occurance)
        
        // step 3 - find rank of the edge just before the base (hence the minus 1)
        size_t rank_to_base = (first_occurance == 0) ? 0 : rank(L,"1",first_occurance-1);
        
        // step 4 - add r and "select" to find the edge index of the r'th edge of the node
        DEBUG_WATCH(C)
        DEBUG_WATCH(r);
        DEBUG_WATCH(rank_to_base);
        
        size_t result = select(L,"1",rank_to_base + r); 
        // -1 because if r is 1 we want the first edge of the set not the second..
        DEBUG_WATCH(result);
        
        return result;        
    }
    
    /* return index of the first edge that points to the node that the edge at i exists */
    edge_index_t backward(edge_index_t i)
    {
        DEBUG_OUT("backward")
        DEBUG_WATCH(i)
        // find the letter of the node edge i comes from...
        
        // step 1 - reverse lookup into F to find which letter must be in the last column of the node list
        size_t alphabet_index = F.size()-1;
        while (F[alphabet_index] > i) alphabet_index--;
        std::string C;
        C.push_back(alphabet[alphabet_index]);    
        DEBUG_WATCH(C)
        
        // can't go backwards from $
        if (alphabet_index == 0) return no_node;
        
        // steps 2 & 3.1 - find the rank in L to the base and the current edge 
        size_t rank_to_base = (alphabet_index == 0) ? 0 : rank(L, "1", F[alphabet_index]-1);
        
        // we don't know if we point to an edge that has L=1 or L=0 so rank to the edge BELOW it, which will either
        // be the same node and L=0 OR the node below, 
        size_t rank_to_current_edge = (i == 0) ? 0 : rank(L, "1", i-1);
        DEBUG_WATCH(rank_to_base)
        DEBUG_WATCH(rank_to_current_edge)
        
        // step 3.2 "we are at second C" or whatever
        size_t r = rank_to_current_edge - rank_to_base + 1;
        
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
    node_index_t outgoing(node_index_t v, std::string C)
    {
        // step 0 (not in the paper)
        // find the last edge of the node
        edge_index_t first_edge = select(L,"1",v)+1;
        edge_index_t last_edge = select(L,"1",v+1);
        
        // step 1 - find the number of C's before first edge with matching C
        int C_index = rank(W,C,last_edge);
        
        // then select the C_index'th C 
        edge_index_t C_edge = select(W,C,C_index);
        
        // is it within the range of this node?    
        if (C_edge < first_edge) return no_node;
        
        // return the last edge of the node that C_edge points to
        edge_index_t outgoing_edge = forward(C_edge);
        DEBUG_WATCH(outgoing_edge)
        
        // and convert to a node index
        // (subtract 1 because if the rank is 1 it is the first node = 0th)
        node_index_t outgoing_node = rank(L,"1",outgoing_edge) - 1;
        return outgoing_node;
    }
    
    /* returns the k-1 length label of the node pointed to by v */
    std::string label(node_index_t v)
    {
        DEBUG_OUT("label");
        DEBUG_WATCH(v);

        std::string result;
        // convert to edge index
        // add one because using zero-based node index
        edge_index_t i = select(L,"1",v+1);
        
        // k-1 because we print node labels, not entire kmers
        for (int todo = k-1; todo > 0; todo --)
        {
            // lookup in F
            int alphabet_index = 4;
            while (F[alphabet_index] > i) alphabet_index--;
            DEBUG_WATCH(todo)
            DEBUG_WATCH(i)
            DEBUG_WATCH(alphabet_index)
            
            // and add to the front of the string        
            result.insert(result.begin(), alphabet[alphabet_index]);
            
            // and move backwards unless it's the last step
            // or it's a $ in which case the rest of the node to the left must be $
            // so don't go backwards
            if ((todo > 1) && (alphabet_index != 0))
                i = backward(i);
            
        }
        return result;
    }
    
    int indegree(node_index_t v)
    {
        DEBUG_OUT("indegree")
        DEBUG_WATCH(v)
        
        // find the index of the first edge of the node`
        edge_index_t edge = select(L,"1",v+1); 
        DEBUG_WATCH(edge)
        
        // and go backwards to the first edge which can't be a flagged edge (because it's the first)
        edge_index_t first_edge = backward(edge);
        DEBUG_WATCH(first_edge)
        
        if (first_edge == no_node) return 0; // indegree is zero if backward fails
        
        // find the position of this symbol in W
        std::string C;
        C.push_back(W[first_edge][0]);
        edge_index_t symbol_pos = rank(W,C,first_edge);
        DEBUG_WATCH(symbol_pos)

        // advance one to find the position of the next non-flagged version of this symbol
        edge_index_t last_edge = select(W,C,symbol_pos+1);
        DEBUG_WATCH(last_edge)

        // now count the number of flagged symbols between these two positions
        C.push_back('-');
        edge_index_t flagged_below_first = rank(W,C,first_edge);
        edge_index_t flagged_below_last = rank(W,C,last_edge);
        DEBUG_WATCH(flagged_below_first)
        DEBUG_WATCH(flagged_below_last)
        
        // and the difference + 1 is the number of incoming edges
        return flagged_below_last - flagged_below_first + 1;
    }
    
    /* return the predecessing node starting with the given symbol */
    node_index_t incoming(node_index_t, std::string C)
    {
        
    }
    
    
};

#endif