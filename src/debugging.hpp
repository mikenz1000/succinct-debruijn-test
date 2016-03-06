/*
    Some utilities to help with debugging because I don't have an IDE that lets me step through
*/
#pragma once
#ifndef __DEBUGGING_HPP
#define __DEBUGGING_HPP
#include <iostream>

/* haven't set up with an IDE that lets me step through, so... */
#if true
#define DEBUG_WATCH(v) std::cout << #v << " = " << v << std::endl;
#define DEBUG_OUT(msg) std::cout << msg << std::endl;
#else
#define DEBUG_WATCH(v)
#define DEBUG_OUT(msg)
#endif

#endif