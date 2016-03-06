/*
    A basic de Bruijn graph implementation that stores data using non-succinct structures, 
    (for validating the succinct implementation)
    
    See http://alexbowe.com/succinct-debruijn-graphs/  
*/
    
#ifndef __DEBRUIJN_BASIC_HPP
#define __DEBRUIJN_BASIC_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <string>
#include <list>
#include "debugging.hpp"


class debruijn_basic
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
    /* the length of the kmers */
    int k;
    
    struct node;
    
    struct edge {
        edge_t symbol;
        node * next;
    };
    
    struct node {
        std::list<edge*> outgoing_edges;
        std::list<edge*> incoming_edges;
        std::string label;
        
        edge * find_outgoing(edge_t symbol)
        {
            for (auto i = outgoing_edges.begin(); i != outgoing_edges.end(); i++)
                if ((*i)->symbol == symbol) return *i;
            return NULL;
        }
        edge * find_incoming(edge_t symbol)
        {
            for (auto i = incoming_edges.begin(); i != incoming_edges.end(); i++)
                if ((*i)->next->label[0] == symbol) return *i;
            return NULL;
        }
    };   
    
    std::vector<node*> nodes;
    
    /* returns the node of the given label, or null if it doesn't exist */
    node * _find_node(const std::string & label)
    {
        for (size_t i = 0;i < nodes.size();i ++)
            if (nodes[i]->label == label) return nodes[i];
        return NULL;
    }
    node * _find_or_create_node(const std::string & label)
    {
        node * result = _find_node(label);
        if (!result)
        {
            result = new node();
            result->label = label;   
            nodes.push_back(result);         
        }
        return result;
    }
    
public:  
    debruijn_basic(const std::vector<std::string> & kmers)
    {             
        k = kmers[0].length();      
        
        // convert the kmers into nodes 
        for (auto i = kmers.begin(); i != kmers.end(); i ++)
        {
            // extract the label for this node
            std::string label = i->substr(0, i->length()-1);
            node * n = _find_or_create_node(label);
            
            // extract the label for the next node
            std::string next_label = i->substr(1);
            
            // and find the edge
            edge_t edge_symbol = (*i)[k-1];
            
            // we don't store the outgoing $ edge though
            if (edge_symbol == '$') continue;
            
            if (n->find_outgoing(edge_symbol))
            {
                // we have seen the same kmer multiple times
            }
            else
            {
                edge * e = new edge();
                e->symbol = edge_symbol;
                node * next_node = _find_or_create_node(next_label);
                e->next = next_node;
                n->outgoing_edges.push_back(e);
                edge * reverse_edge = new edge();
                reverse_edge->symbol = edge_symbol;
                reverse_edge->next = n;
                next_node->incoming_edges.push_back(reverse_edge);
            }                     
        }
    }      
    
    node_index_t find_node(std::string label)
    {
        for (node_index_t i = 0;i < nodes.size();i ++)
            if (nodes[i]->label == label) return i;
        return no_node;
    }
    
    /* return the number of outgoing edges from node v */
    int outdegree(node_index_t v)
    {
        return nodes[v]->outgoing_edges.size();        
    }
    
    /* return the node you get to by following edge labelled by symbol c,
       or no_node if there is no edge */
    node_index_t outgoing(node_index_t v, edge_t C)
    {
        edge * e = nodes[v]->find_outgoing(C);
        if (!e) return no_node;
        return find_node(e->next->label);        
    }
    
    /* returns the k-1 length label of the node pointed to by v */
    std::string label(node_index_t v)
    {
        return nodes[v]->label;        
    }
    
    int indegree(node_index_t v)
    {
        return nodes[v]->incoming_edges.size();        
    }
    
    /* return the predecessing node starting with the given symbol */
    node_index_t incoming(node_index_t v, edge_t X)
    {
        edge * e = nodes[v]->find_incoming(X);
        if (!e) return no_node;
        return find_node(e->next->label);        
    }  
};

#endif