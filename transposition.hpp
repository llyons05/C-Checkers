#pragma once

#include "board.hpp"

#include <cstdint>

uint64_t rand64();
void set_hash_function();

struct hash_func{
    uint64_t HASH_FUNCTION[4][32];
    uint64_t HASH_COLOR;
}
extern hash;

enum ettflag{
    TT_EXACT,
    TT_ALPHA,
    TT_BETA
};

struct tt_entry{
    uint64_t hash;
    int val;
    uint8_t depth;
    uint8_t flags;
    uint8_t bestmove;
};

class tt_table{
public:
    tt_entry * tt;
    uint32_t tt_size;
    int num_entries = 0;
    int fails = 0;

    tt_table(uint32_t size);
    int probe(uint64_t boardhash, uint8_t depth, int alpha, int beta, char * best);
    void save(uint64_t boardhash, uint8_t depth, int ply, int val, char flags, uint8_t best);
    ~tt_table() {
        free(tt);
    }

private:
    void set_size(uint32_t size);
};

struct tt_eval_entry{
    uint64_t hash;
    int val;
};

class tt_eval_table{
public:
    tt_eval_entry * ett;
    uint32_t ett_size;
    
    tt_eval_table(uint32_t size);
    int probe(uint64_t boardHash);
    void save(uint64_t boardHash, int val);
    ~tt_eval_table() {
        free(ett);
    }

private:
    void set_size(uint32_t size);
};