#pragma once

#include "cpu.hpp"
#include "new_cpu.hpp"

#include <math.h>
#include <cmath>
#include <random>

class time_games{
    int games;
    double time_limit;
    int pos_advantage_cutoff;
    int verification_depth;
    int random_pos_depth;
    int random_pos_variation;

    int new_vs_current[3] = {0, 0, 0};

    uint64_t new_total_time;
    unsigned long new_nodes_searched;
    uint64_t current_total_time;
    unsigned long current_nodes_searched;
    int new_total_pieces;
    int curr_total_pieces;
    int new_total_depth;
    int curr_total_depth;

    cpu eval_cpu;
public:
    time_games(int games, double t, int maximum_pos_advantage = 50, float verification_search_depth = 13, int rand_pos_depth = 12, int rand_pos_variation = 3);
    void run_games();
    void print_results();
    
private:
    void run_game(const Board& start_pos, int new_cpu_color);
    Board get_random_pos();
};