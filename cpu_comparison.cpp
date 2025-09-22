#define _USE_MATH_DEFINES
#include "cpu_comparison.hpp"

#include "misc.hpp"
#include <math.h>

depth_games::depth_games(int games, int depth, int maximum_pos_advantage, 
                        int verification_search_depth, int piece_diff_for_draw, int min_draw_moves, int rand_pos_depth, int rand_pos_variation){
    this->games = games;
    this->depth = depth;
    pos_advantage_cutoff = maximum_pos_advantage;
    verification_depth = verification_search_depth;
    draw_piece_diff = piece_diff_for_draw;
    draw_move_cutoff = min_draw_moves;
    random_pos_depth = rand_pos_depth;
    random_pos_variation = rand_pos_variation;

    current_version.set_depth(depth);
    new_version.set_depth(depth);
    eval_cpu.set_color(0);
    eval_cpu.set_depth(10);
    eval_cpu.time_limit = INFINITY;
}

void depth_games::run_games(){
    std::cout << "running " << games << " games at depth " << depth << "\n";
    set_hash_function();
    new_version.set_color(1);
    current_version.set_color(0);
    Move arr[64];
    srand(time(NULL));

    for (int i = 0; i < games/2; i++){
        std::cout << "Game " << i << "\n";
        Board board;

        int variation = rand() % random_pos_variation;
        if (variation % 2) variation = -variation;
        board.set_random_pos(random_pos_depth + variation);
        int val = eval_cpu.search_root(board, verification_depth + 2, -10000, 10000);

        while (abs(val) > pos_advantage_cutoff){
            Board board;
            board.set_random_pos(random_pos_depth + variation);
            val = eval_cpu.search_root(board, verification_depth + 2, -10000, 10000);
        }
        Board board2(board);
        board.gen_moves(arr, (char)-1);
        board.print();
        std::cout << "starting pos has a value of: " << val << "\n";

        while (!board.check_win() && !board.check_repetition()){
            if (board.bb.stm == current_version.color){
                Move move = current_version.max_depth_search(board, false);
                board.push_move(move);
            }
            else{
                Move move = new_version.max_depth_search(board, false);
                board.push_move(move);
            }
            board.gen_moves(arr, (char)-1);
        }
        board.print();
        int result = board.check_win();
        if (result == new_version.color + 1){
            new_vs_current[0]++;
            std::cout << "New wins\n";
        }
        else if (result == current_version.color + 1){
            new_vs_current[1]++;
            std::cout << "current wins\n";
        }
        else{
            new_vs_current[2]++;
            std::cout << "draw\n";
        }
        std::cout << new_vs_current[0] << "-" << new_vs_current[1] << "-" << new_vs_current[2] << "\n";

        new_version.set_color(!new_version.color);
        current_version.set_color(!current_version.color);

        board2.gen_moves(arr, (char)-1);
        board2.print();
        std::cout << "starting pos has a value of: " << val << "\n";

        while (!board2.check_win() && !board2.check_repetition()){
            if (board2.bb.stm == current_version.color){
                Move move = current_version.max_depth_search(board2, false);
                board2.push_move(move);
            }
            else{
                Move move = new_version.max_depth_search(board2, false);
                board2.push_move(move);
            }
            board2.gen_moves(arr, (char)-1);
        }
        board2.print();
        result = board2.check_win();
        if (result == new_version.color + 1){
            new_vs_current[0]++;
            std::cout << "New wins\n";
        }
        else if (result == current_version.color + 1){
            new_vs_current[1]++;
            std::cout << "current wins\n";
        }
        else{
            new_vs_current[2]++;
            std::cout << "draw\n";
        }
        std::cout << new_vs_current[0] << "-" << new_vs_current[1] << "-" << new_vs_current[2] << "\n";
    }

    std::cout << "games complete \n";
}

time_games::time_games(int games, double t, int maximum_pos_advantage, 
                        int verification_search_depth, int piece_diff_for_draw, int min_draw_moves, int rand_pos_depth, int rand_pos_variation){
    this->games = games;
    time_limit = t;
    pos_advantage_cutoff = maximum_pos_advantage;
    verification_depth = verification_search_depth;
    draw_piece_diff = piece_diff_for_draw;
    draw_move_cutoff = min_draw_moves;
    random_pos_depth = rand_pos_depth;
    random_pos_variation = rand_pos_variation;

    current_nodes_searched = 0;
    current_total_time = 0;
    curr_total_depth = 0;
    new_nodes_searched = 0;
    new_total_time = 0;
    new_total_depth = 0;
    eval_cpu.set_color(0);
    eval_cpu.set_depth(verification_depth);
    eval_cpu.time_limit = INFINITY;
}

void time_games::run_games(){
    std::cout << "running " << games << " games with time limit " << time_limit << " seconds per move\n";

    set_hash_function();
    Board board;
    Board start_pos;
    Move arr[64];
    Move m;
    new_cpu new_version(0, 10);
    cpu current_version(0, 10);

    srand(time(NULL));

    for (int i = 0; i < games; i++){
        if (i%2 == 0)
        {
            start_pos.reset();
            int variation = rand() % random_pos_variation;
            if (variation % 2) variation = -variation;
            start_pos.set_random_pos(random_pos_depth + variation);
            int val = eval_cpu.search_root(start_pos, verification_depth, -10000, 10000);

            while (abs(val) >= pos_advantage_cutoff){
                Board start_pos;
                start_pos.set_random_pos(random_pos_depth + variation);
                val = eval_cpu.search(start_pos, verification_depth, 0, -pos_advantage_cutoff, pos_advantage_cutoff, IS_PV);
            }
        }

        Board board(start_pos);
        board.print();

        new_version.set_color(i%2);
        current_version.set_color((i+1)%2);

        board.gen_moves(arr, -1);
        while(!board.check_win() && !board.check_repetition()){
            if (board.bb.stm == new_version.color){
                uint64_t start = get_time();
                m = new_version.time_search(board, time_limit, false);
                uint64_t movetime = get_time() - start;

                if (movetime >= time_limit * 1000){
                    new_total_time += movetime - 1;
                    new_nodes_searched += new_version.nodes_traversed;
                    new_total_depth += new_version.current_depth - 1;
                }
            }
            else{
                uint64_t start = get_time();
                m = current_version.time_search(board, time_limit, false);
                uint64_t movetime = get_time() - start;

                if (movetime >= time_limit * 1000){
                    current_total_time += movetime - 1;
                    current_nodes_searched += current_version.nodes_traversed;
                    curr_total_depth += current_version.current_depth - 1;
                }
            }

            board.push_move(m);
            board.gen_moves(arr, -1);
            // board.print();
        }
        board.print();
        int result = board.check_win();
        if (result == new_version.color + 1){
            new_vs_current[0]++;
        }
        else if (result == current_version.color + 1){
            new_vs_current[1]++;
        }
        else{
            new_vs_current[2]++;
        }
        std::cout << new_vs_current[0] << "-" << new_vs_current[1] << "-" << new_vs_current[2] << "\n";
    }
    std::cout << "games complete \n";
}

int calculate_elo_change(int wins, int losses, int draws){
    double score = wins + (double)draws/2;
    double total = wins + draws + losses;
    double percentage = score / total;
    double eloDifference = -400 * log(1 / percentage - 1) / M_LN10;
    return round(eloDifference);
}

double calculate_inverse_error(double x){
    double a = 8 * (M_PI - 3) / (3 * M_PI * (4 - M_PI));
    double y = log(1 - x*x);
    double z = 2 / (M_PI * a) + y / 2;
    double ret = sqrt(sqrt(z*z - y/a) - z);
    if (x < 0){
        return -ret;
    }
    return ret;
}
double phiInv(double percentage){
    return sqrt(2) * calculate_inverse_error(2 * percentage - 1);
}

double calculate_elo_diff(double percentage){
    return -400 * log(1 / percentage - 1) / M_LN10;
}

int calculate_error_margin(int wins, int losses, int draws){
    double total = wins + losses + draws;
    double winP = wins/total;
    double lossP = losses / total;
    double drawP = draws / total;
    double percentage = (wins + draws * .5) / total;
    double winsDev = winP * pow(1 - percentage, 2);
    double drawsDev = drawP * pow(0.5 - percentage, 2);
    double lossesDev = lossP * pow(0 - percentage, 2);
    double stdDev = sqrt(winsDev + lossesDev + drawsDev) / sqrt(total);
    double confidenceP = 0.95;
    double minConfidenceP = (1 - confidenceP) / 2;
    double maxConfidenceP = 1 - minConfidenceP;
    double devMin = std::max(percentage + phiInv(minConfidenceP) * stdDev, -.99999);
    double devMax = std::min(percentage + phiInv(maxConfidenceP) * stdDev, .99999);
    double difference = calculate_elo_diff(devMax) - calculate_elo_diff(devMin);

    return round(difference / 2);
}

void time_games::print_results(){
    std::cout << "Result of " << games << " games with " << time_limit << " seconds per move:\n";
    std::cout << "New vs Current:\n";
    std::cout << new_vs_current[0] << " wins, " << new_vs_current[1] << " losses, " << new_vs_current[2] << " draws\n";

    int new_current_change = calculate_elo_change(new_vs_current[0], new_vs_current[1], new_vs_current[2]);
    int new_current_error = calculate_error_margin(new_vs_current[0], new_vs_current[1], new_vs_current[2]);
    char new_curr_sign = (new_current_change > 0) ? '+' : ' ';
    std::cout << "Estimated Elo Difference (New vs Current): " << new_curr_sign 
                << new_current_change << " (Error Margin: +/-" << new_current_error << ")\n\n";

    int new_nps = new_nodes_searched / (new_total_time/1000);
    int curr_nps = current_nodes_searched / (current_total_time/1000);
    std::cout << "New version nodes per second: " << new_nps << "\n";
    std::cout << "Current version nodes per second: " << curr_nps << "\n\n";

    double new_margin_wins = (double) new_total_pieces / (double) new_vs_current[0];
    double curr_margin_wins = (double) curr_total_pieces / (double) new_vs_current[1];
    std::cout << "Margins of victory: \n" << "New: " << new_margin_wins << "    Current: " << curr_margin_wins << "\n";

    double new_npd = new_nodes_searched/curr_total_depth;
    double curr_npd = current_nodes_searched / curr_total_depth;
    std::cout << "Nodes per depth:\nNew: " << new_npd << "    Current: " << curr_npd << "\n\n";
}

void depth_games::print_results(){
    std::cout << "Result of " << games << " games with depth " << depth << ":\n";
    std::cout << "New vs Current:\n";
    std::cout << new_vs_current[0] << " wins, " << new_vs_current[1] << " losses, " << new_vs_current[2] << " draws\n";

    int new_current_change = calculate_elo_change(new_vs_current[0], new_vs_current[1], new_vs_current[2]);
    int new_current_error = calculate_error_margin(new_vs_current[0], new_vs_current[1], new_vs_current[2]);
    char new_curr_sign = (new_current_change > 0) ? '+' : ' ';
    std::cout << "Estimated Elo Difference (New vs Current): " << new_curr_sign 
                << new_current_change << " (Error Margin: +/-" << new_current_error << ")\n\n";
}

int main(){
    int games = 50;
    int depth = 7;
    double time_limit = .1;
    //depth_games depth_comp(games, depth, 50, 7, 1, 150, 7, 2);
    time_games time_comp(games, time_limit, 50, 15, 1, 150, 13, 9);

    //depth_comp.run_games(false);
    time_comp.run_games();
    //depth_comp.print_results();
    time_comp.print_results();
}
/*
Result of 100 games with 0.1 seconds per move:
New vs Current:
39 wins, 26 losses, 35 draws
Estimated Elo Difference (New vs Current): +45 (Error Margin: +/-56)

New version nodes per second: 3794078
Current version nodes per second: 4216617

Nodes per depth:
New: 24230    Current: 27205
*/