#pragma once

#include "misc.hpp"
#include "board.hpp"
#include "cpu.hpp"
#include "transposition.hpp"

#include <algorithm>
#include <iostream>
#include <time.h>
#include <vector>

/*
This class is for testing new features for the cpu
*/
class new_cpu : public cpu {
public:
    new_cpu(int cpu_color) : 
    cpu(cpu_color) {}
};