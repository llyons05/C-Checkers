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
    new_cpu(int cpu_color = 0, int cpu_depth = 10) : 
    cpu(cpu_color, cpu_depth, 0x4000000, 0x4000000) {}
};