#pragma once

#include "cpu.hpp"
#include "new_cpu.hpp"

#include <math.h>
#include <cmath>
#include <random>

class depth_games{
    int games;
    int depth;
    int pos_advantage_cutoff;
    int verification_depth;
    int draw_piece_diff;
    int draw_move_cutoff;
    int random_pos_depth;
    int random_pos_variation;

    int new_vs_current[3] = {0, 0, 0};

    cpu current_version;
    new_cpu new_version;
    cpu eval_cpu;

    public:
        depth_games(int games, int depth, int maximum_pos_advantage = .5, int verification_search_depth = 8, 
                    int piece_diff_for_draw = 1, int min_draw_moves = 200, int rand_pos_depth = 12, int rand_pos_variation = 3);
        void run_games();
        void print_results();
};

class time_games{
    int games;
    double time_limit;
    int pos_advantage_cutoff;
    int verification_depth;
    int draw_piece_diff;
    int draw_move_cutoff;
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
        time_games(int games, double t, int maximum_pos_advantage = .5, int verification_search_depth = 8,
                   int piece_diff_for_draw = 1, int min_draw_moves = 200, int rand_pos_depth = 12, int rand_pos_variation = 3);
        void run_games();
        void print_results();
};