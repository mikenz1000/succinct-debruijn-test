/*
    A simple (non-efficient) data structure that supports rank & succint.
    a.k.a a Succinct Indexable Dictionary
    see https://en.wikipedia.org/wiki/Succinct_data_structure    
*/
#pragma once
#ifndef __RANK_SELECT_HPP
#define __RANK_SELECT_HPP
#include <vector>

/*
    T is the type of the element in the data structure
*/
template<typename T, typename index_t = size_t>
class rank_select {
    /* The actual data list, named B as per the wikipedia definition */
    std::vector<T> B;
public:
    /* Range constructor accepts iterators e.g. std::vector.begin() and end() */
    template<class InputIterator>
    rank_select(InputIterator begin, InputIterator end) 
        : B(begin,end)
    {        
    }
    
    /* Constructs a blank - the built-in element-wise assignment will be fine */
    rank_select() {}
    
    /* Rank function returns the number of occurances of x on the range [0,i] */
    index_t rank(const T & x, const index_t i) const
    {
        if ((i < 0) || (i >= size())) throw std::runtime_error("rank parameter i out of range");
        index_t result = 0;
        for (size_t j = 0;j <= i;j ++)
            if (B[j] == x) result ++;
        return result;
    }
    
    /* Select returns the position of the ith occurance of c
       if there are less than i occurances then it throws a runtime error */
    size_t select(const T & x, const index_t i) const
    {
        size_t counter = 0;
        for (size_t j = 0;j < B.size(); j++)
        {
            if (B[j] == x)
            {
                counter++;
                if (counter == i) return j;
            }
        }        
        throw std::runtime_error("select out of range");
    }   
    
    /* Returns the number of elements in the structure */
    index_t size() const
    { 
        return B.size();
    }
    
    /* Element-wise access */
    const T operator[](index_t i)
    {
        return B[i];
    }
};


#endif