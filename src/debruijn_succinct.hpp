/*
    An educational Succint de Bruijn implementation
    
    See http://alexbowe.com/succinct-debruijn-graphs/
    
    This is an exercise in understanding the algorithm.  Not optimised, doesn't use underlying succinct data structures
 
    alphabet must always start with the 'terminator' char (normally $)
*/
    
#ifndef __DEBRUIJN_SUCCINCT_HPP
#define __DEBRUIJN_SUCCINCT_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_set>
#include "rank_select.hpp"
#include "debugging.hpp"

class debruijn_succinct
{
public:
    /* for clarity, variables that store indexes of edges and nodes use these two typdefs */
    typedef size_t edge_index_t;
    typedef size_t node_index_t;
    typedef uint8_t edge_t;
    
    /* the node index value for no node (possible return value from some traversal functions) */
    const node_index_t no_node = (node_index_t)(-1);
    const edge_index_t no_edge = (edge_index_t)(-1);
    
 protected:
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
    
    /* returns the character corresponding to the position i in the array described by F */
    size_t Findex(edge_index_t i)
    {
        size_t alphabet_index = F.size()-1;
        while (F[alphabet_index] > i) alphabet_index--;
        return alphabet_index; 
    }
   
    /* The edge letter array */
    rank_select<edge_t> W;
    
    /* The array of bits indicating if this edge is the last for the node, stored as '0' and '1' */
    rank_select<bool> L;
    
    /* the length of the kmers */
    int k;
    
    /* the edge flag is stored in the high bit of an 8 bit char */
    edge_t edge_flag(edge_t x, bool flag)
    {
        x = x & 0x7f;
        if (flag) return x | 0x80;
        else return x;
    }
    
    /* the alphabet - the first char must be the terminator.  example alphabet $ACGT */
    std::string alphabet;
    
    /* the first character of the alphabet is always the terminator */
    edge_t terminator()
    {
        return alphabet[0];
    }
public:  
    
    /* constructor builds the graph */
    debruijn_succinct(const std::vector<std::string> & kmers, const std::string & alphabet)
        : alphabet(alphabet)
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
        std::vector<bool> _L(edges.size(), false);
        for (int ix = edges.size()-1;ix >= 0;ix --)
        {
            edge * i = &edges[ix];
            
            if (last_node != i->node_rev)
            {
                last_node = i->node_rev;
                _L[ix] = true;
            }           
            // else the false value is what we want
        }
        L = rank_select<bool>(_L.begin(), _L.end());
             
        
        // and output
        F.push_back(0); // $ always starts 
        int alphabet_index = 0;
        int edge_index = 0;
        std::vector<edge_t> _W;
        for (auto i = edges.begin();i != edges.end();i ++)
        {
            while (alphabet[alphabet_index] != i->node_rev[0]) 
            {
                F.push_back(edge_index);
                alphabet_index++;                
            }
            edge_index ++;
            
            // if the length of edge is 2 then it's got the minus flag
            _W.push_back(edge_flag(i->edge[0], i->edge.length() == 2));
        }
        W = rank_select<edge_t>(_W.begin(), _W.end());
    }    
    
    /* return index of the last edge of the node pointed to by edge i */
    edge_index_t forward(edge_index_t i)
    {
        // find the edge label
        edge_t C = edge_flag(W[i],false);
        
        // step 1 - determine the rank
        size_t r = W.rank(C,i);
        
        // step 2 - find first occurance
        std::string::size_type alphabet_index = alphabet.find(C);    
        if (alphabet_index == std::string::npos) 
            throw std::runtime_error("alphabet not recognised"); 
            
        // can't follow a $ edge  
        if (alphabet_index == 0)
            return no_node; 
            
        size_t first_occurance = F[alphabet_index];
        
        // step 3 - find rank of the edge just before the base (hence the minus 1)
        size_t rank_to_base = (first_occurance == 0) ? 0 : L.rank(true,first_occurance-1);
        
        // step 4 - add r and "select" to find the edge index of the r'th edge of the node
        size_t result = L.select(true, rank_to_base + r); 
        
        return result;        
    }
        
    /* return index of the first edge that points to the node that the edge at i exists */
    edge_index_t backward(edge_index_t i)
    {
        // step 1 - reverse lookup into F to find which letter must be in the last column of the node list
        size_t alphabet_index = Findex(i);
        std::string C = std::string(1, alphabet[alphabet_index]);
        
        // can't go backwards from $
        if (alphabet_index == 0) return no_node;
        
        // steps 2 & 3.1 - find the rank in L to the base and the current edge 
        size_t rank_to_base = (alphabet_index == 0) ? 0 : L.rank(true, F[alphabet_index]-1);
        
        // we don't know if we point to an edge that has L=1 or L=0 so rank to the edge BELOW it, which will either
        // be the same node and L=0 OR the node below, 
        size_t rank_to_current_edge = (i == 0) ? 0 : L.rank(true, i-1);
        
        // step 3.2 "we are at second C" or whatever
        size_t r = rank_to_current_edge - rank_to_base + 1;
        
        // and select to find the position
        return W.select(C[0],r);
    }
    
    /* returns the zero-based index of the first edge of the node v (also zero-based) */
    edge_index_t node_to_edge(node_index_t v)
    {
        // add one because for the 0th node we want to find the 1st '1' in L
        return L.select(true, v+1);
    }
    
    /* returns the zero-based index of the node that edge i belongs to */
    node_index_t edge_to_node(edge_index_t i)
    {
        // subtract BEFORE doing the rank because we don't know whether this is the last edge
        // of the node or not. 
        // if it's the 0th edge we know it's the 0th node
        if (i == 0) return 0;
        else return L.rank(true, i - 1);
    }
    
    /* return the number of outgoing edges from node v */
    int outdegree(node_index_t v)
    {
        // select to find the position of the v'th 1
        // add one because nodes indexes are zero-based
        // position will then point to the last edge of the node
        edge_index_t position = L.select(true, v+1);
        
        // select to find the position of the previous node (v-1)th */
        edge_index_t previous = (v == 0) ? -1 : L.select(true, v);      
        
        // and ignore any outgoing edges that are $
        edge_index_t terminators = W.rank(terminator(),position) - ((previous == -1) ? 0 : W.rank(terminator(),previous));
        
        // also check for flagged $
        terminators += W.rank(edge_flag(terminator(),true),position) - ((previous == -1) ? 0 : W.rank(edge_flag(terminator(),true),previous));
 
        // "boom!" the difference between the edge indexes tells us how many edges leave that node (minus one)
        // slight typo in the text it should be select(7) - select(6) + 1 = 7 - 6 + 1 = 2 */
        return position - previous - terminators;  
    }
    
    /* return the node you get to by following edge labelled by symbol c,
       or no_node if there is no edge */
    node_index_t outgoing(node_index_t v, edge_t C)
    {
        // if we follow the $ we are at the end of the graph
        if (C == terminator()) return no_node;
        
        // step 0 (not in the paper)
        // convert from node index to a range of edge indexes
        
        // first_edge is the first edge of the set of edges comprising this node
        // we effectively add one to the last edge of the node below because L lets us find 
        // last edges of nodes only
        // for v == 0 we know the first edge is edge 0 of course
        edge_index_t first_edge = (v == 0) ? 0 : (L.select(true,v)+1);
        
        // returns zero for v = 0 if first node only has one edge
        edge_index_t last_edge = L.select(true,v+1);
        
        // try it for both C and C- the flagged edge character
        for (int flag = 0; flag <= 1; flag ++)
        {
            // determine the character we are looking for in this test
            edge_t Cflagged = edge_flag(C, flag == 1);
                
            // step 1 - find the number of C's before or including the last edge
            edge_index_t C_index = W.rank(Cflagged,last_edge);
            
            // if C_index is zero we didn't find ANY Cs in the range so we know this edge can't be followed
            if (C_index == 0) continue;
            
            // then select the C_index'th C
            edge_index_t C_edge = W.select(Cflagged,C_index);
            
            // is it within the range of this node?    
            if (C_edge >= first_edge) 
            {
                // return the last edge of the node that C_edge points to
                edge_index_t outgoing_edge = forward(C_edge);
                
                // and convert to a node index
                // (subtract 1 because if the rank is 1 it is the first node = 0th)
                node_index_t outgoing_node = L.rank(true,outgoing_edge) - 1;
                return outgoing_node;
            }
        }
        return no_node;
    }
    
    /* returns the k-1 length label of the node pointed to by v */
    std::string label(node_index_t v)
    {
        std::string result;
        // convert to edge index
        // add one because using zero-based node index
        edge_index_t i = L.select(true,v+1);
        
        // k-1 because we print node labels, not entire kmers
        for (int todo = k-1; todo > 0; todo --)
        {
            // lookup in F
            int alphabet_index = alphabet.size()-1;
            while (F[alphabet_index] > i) alphabet_index--;
            
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
    
    /* returns the number of edges leading into node v */
    int indegree(node_index_t v)
    {
        // find the index of the first edge of the node`
        edge_index_t edge = L.select(true,v+1); 
        
        // and go backwards to the first edge which can't be a flagged edge (because it's the first)
        edge_index_t first_edge = backward(edge);
        
        if (first_edge == no_node) return 0; // indegree is zero if backward fails
        
        // find the position of this symbol in W
        edge_t C = edge_flag(W[first_edge],false);
        edge_index_t symbol_pos = W.rank(C,first_edge);
        
        // advance one to find the position of the next non-flagged version of this symbol
        edge_index_t last_edge = W.select(C,symbol_pos+1);
        
        // now count the number of flagged symbols between these two positions
        C = edge_flag(C, true);
        edge_index_t flagged_below_first = W.rank(C,first_edge);
        edge_index_t flagged_below_last = W.rank(C,last_edge);
        
        // and the difference + 1 is the number of incoming edges
        return flagged_below_last - flagged_below_first + 1;
    }
    
    /* finds the value of i where F(i) = desired, where i is in the range [first,last]
       and where F[i] is increasing with i, i.e. F(i+1) >= F(i) 
       returns fail_value if not found 
       result_type is the type of F(i)
       index_type is the type of i */
    template <typename result_type, typename index_type, class F>
    index_type binary_search(index_type first, index_type last, result_type desired, index_type fail_value, F f) {
        while ( first <= last ) {
            size_t mid = (last + first) / 2;

            result_type temp = f(mid);
            
            if (temp == desired) return mid;
            else if (temp < desired) first = mid + 1;
            else 
                // must be temp > desired
                last = mid - 1;
        }
        return fail_value;
    }
    
    /* return the predecessing node starting with the given symbol */
    node_index_t incoming(node_index_t i, edge_t X)
    {
        // find the last letter of the node pointed to by i, which is the letter of the incoming edge
        size_t alphabet_index = Findex(node_to_edge(i));
        
        // if the last char of the node is $ then we must be on the node with no incoming edges
        if (alphabet_index == 0) return no_node;
        edge_t C = alphabet[alphabet_index];
        
        // find the first of the incoming edges
        edge_index_t first_edge = backward(node_to_edge(i));
        
        // if backward failed then there are no incoming edges
        if (first_edge == no_edge) return no_node;
        
        node_index_t first_node = edge_to_node(first_edge);
        
        // check the label of the node belonging to first edge in case this is the one we are looking for
        std::string first_label = label(first_node);
        if (first_label[0] == X) return first_node;
        
        // find the next occurance of C, which gives an upper bound on the edge indexes
        // makes use of the special return value of W.size() if we select for one greater than the number of C's
        edge_index_t next_C = W.select(C, W.rank(C, first_edge) + 1);
        
        // find the range of Cminuses we need to test
        edge_t Cminus = edge_flag(C,true);
        edge_index_t first_Cminus = W.rank(Cminus, first_edge)+1;
        edge_index_t last_Cminus = W.rank(Cminus, next_C);
        
        // note that last_Cminus may end up less than first_Cminus if first_Cminus was in fact the 
        // last one before the end of the array.  If so it will drop straight through the binary search
        // below
        
        // binary search to find the rank of the Cminus that gives us a label with the desired starting letter
        // the first letter of the label will be in ascending order within the range of nodes that 
        // have the same remaining letter, because of the sort.
        edge_index_t found_index = no_edge;
        
        while ( first_Cminus <= last_Cminus ) {
            size_t mid = (last_Cminus + first_Cminus) / 2;

            node_index_t test_node = edge_to_node(W.select(Cminus, mid));
            std::string test_label = label(test_node);
            edge_t first_char = test_label[0];    
            
            if (first_char == X) 
            {
                found_index = mid;
                first_Cminus = last_Cminus + 1; // force exit of the loop.
            }
            else 
                if (first_char < X) first_Cminus = mid + 1;
            else 
                // must be temp > desired
                last_Cminus = mid - 1;
        }
        if (found_index == no_edge) return no_node;
        else return edge_to_node(W.select(Cminus, found_index));
    }
    
    /* returns the number of nodes i.e. value node indexes are between 0 and the result of this-1 inclusive */
    node_index_t num_nodes()
    {
        return L.rank(true,L.size()-1);
    }
};

#endif