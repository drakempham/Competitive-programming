#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <array>
#include <cstdio>

using namespace std;

const int INF = 1000000000;

// Executes a shell command and returns its stdout
string exec(const string& cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// Simulates the problem
struct GridState {
    int N, M, T;
    vector<vector<bool>> v_walls;
    vector<vector<bool>> h_walls;
    vector<pair<int, int>> balls;
    vector<pair<int, int>> baskets;
};

GridState parse_input(const string& input_str) {
    GridState state;
    stringstream ss(input_str);
    ss >> state.N >> state.M >> state.T;
    
    state.v_walls.assign(state.N, vector<bool>(state.N - 1, false));
    for (int i = 0; i < state.N; ++i) {
        string row;
        ss >> row;
        for (int j = 0; j < state.N - 1; ++j) {
            state.v_walls[i][j] = (row[j] == '1');
        }
    }
    
    state.h_walls.assign(state.N - 1, vector<bool>(state.N, false));
    for (int i = 0; i < state.N - 1; ++i) {
        string row;
        ss >> row;
        for (int j = 0; j < state.N; ++j) {
            state.h_walls[i][j] = (row[j] == '1');
        }
    }
    
    state.balls.resize(state.M);
    state.baskets.resize(state.M);
    for (int i = 0; i < state.M; ++i) {
        ss >> state.balls[i].first >> state.balls[i].second;
        ss >> state.baskets[i].first >> state.baskets[i].second;
    }
    
    return state;
}

// Helper to check for walls
bool has_wall(const GridState& state, int r1, int c1, int r2, int c2) {
    if (r1 == r2) {
        return state.v_walls[r1][min(c1, c2)];
    } else {
        return state.h_walls[min(r1, r2)][c1];
    }
}

// Expands macro sequence (F, R, L, S, M, P) to basic operations
vector<char> expand_operations(const string& op_sequence) {
    vector<char> registered_macro;
    bool recording = false;
    vector<char> current_recording;
    vector<char> expanded;
    
    for (char op : op_sequence) {
        if (op == 'M') {
            if (!recording) {
                recording = true;
                current_recording.clear();
            } else {
                registered_macro = current_recording;
                recording = false;
            }
        } else if (op == 'P') {
            if (registered_macro.empty()) continue;
            for (char basic_op : registered_macro) {
                expanded.push_back(basic_op);
                if (recording) {
                    current_recording.push_back(basic_op);
                }
            }
        } else if (op == 'F' || op == 'R' || op == 'L' || op == 'S') {
            expanded.push_back(op);
            if (recording) {
                current_recording.push_back(op);
            }
        } else if (op == ' ' || op == '\n' || op == '\r') {
            continue;
        } else {
            throw runtime_error("Invalid operation character: " + string(1, op));
        }
    }
    return expanded;
}

struct SimResult {
    int N, M, T;
    int V;
    int A;
    int score;
    double elapsed;
    string error;
};

SimResult run_test_on_solver(const string& solver_cmd, int seed) {
    // Generate test case using python3 test_generator.py --seed S
    string test_case;
    try {
        test_case = exec("python3 test_generator.py --seed " + to_string(seed));
    } catch (...) {
        return {0, 0, 0, 0, 0, INF, 0.0, "Failed to run test_generator.py"};
    }
    
    GridState state = parse_input(test_case);
    
    // Write test case to a temporary file
    string temp_in = "temp_input_" + to_string(seed) + ".txt";
    ofstream out(temp_in);
    out << test_case;
    out.close();
    
    // Measure time and execute solver
    auto t0 = chrono::high_resolution_clock::now();
    string output;
    try {
        output = exec(solver_cmd + " < " + temp_in + " 2>/dev/null");
    } catch (const exception& e) {
        remove(temp_in.c_str());
        return {state.N, state.M, state.T, 0, 0, state.T * state.M, 0.0, e.what()};
    }
    auto t1 = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(t1 - t0).count();
    
    remove(temp_in.c_str());
    
    // Parse output lines into a single string
    string op_sequence = "";
    for (char c : output) {
        if (c == 'F' || c == 'R' || c == 'L' || c == 'S' || c == 'M' || c == 'P') {
            op_sequence += c;
        }
    }
    
    int A = op_sequence.length();
    if (A > state.T) {
        return {state.N, state.M, state.T, 0, A, state.T * state.M, elapsed, "Output length " + to_string(A) + " exceeds T=" + to_string(state.T)};
    }
    
    vector<char> basic_ops;
    try {
        basic_ops = expand_operations(op_sequence);
    } catch (const exception& e) {
        return {state.N, state.M, state.T, 0, A, state.T * state.M, elapsed, e.what()};
    }
    
    // Grid ball positions: -1 means empty, >= 0 is ball index
    vector<vector<int>> ball_at(state.N, vector<int>(state.N, -1));
    for (int k = 0; k < state.M; ++k) {
        ball_at[state.balls[k].first][state.balls[k].second] = k;
    }
    
    int robot_r = 0, robot_c = 0;
    int robot_dir = 0; // 0=right, 1=down, 2=left, 3=up
    int held_ball = -1;
    
    const int DR[4] = {0, 1, 0, -1};
    const int DC[4] = {1, 0, -1, 0};
    
    int ops_executed = 0;
    for (char op : basic_ops) {
        if (ops_executed >= state.T) break;
        
        if (op == 'F') {
            int nr = robot_r + DR[robot_dir];
            int nc = robot_c + DC[robot_dir];
            if (nr >= 0 && nr < state.N && nc >= 0 && nc < state.N) {
                if (!has_wall(state, robot_r, robot_c, nr, nc)) {
                    robot_r = nr;
                    robot_c = nc;
                }
            }
        } else if (op == 'R') {
            robot_dir = (robot_dir + 1) % 4;
        } else if (op == 'L') {
            robot_dir = (robot_dir + 3) % 4;
        } else if (op == 'S') {
            int cell_ball = ball_at[robot_r][robot_c];
            if (held_ball == -1 && cell_ball == -1) {
                // Nothing
            } else if (held_ball == -1 && cell_ball != -1) {
                held_ball = cell_ball;
                ball_at[robot_r][robot_c] = -1;
            } else if (held_ball != -1 && cell_ball == -1) {
                ball_at[robot_r][robot_c] = held_ball;
                held_ball = -1;
            } else {
                int tmp = ball_at[robot_r][robot_c];
                ball_at[robot_r][robot_c] = held_ball;
                held_ball = tmp;
            }
        }
        ops_executed++;
    }
    
    int V = 0;
    for (int k = 0; k < state.M; ++k) {
        int dr = state.baskets[k].first;
        int dc = state.baskets[k].second;
        if (ball_at[dr][dc] == k) {
            V++;
        }
    }
    
    int score = (V == state.M) ? A : state.T * (state.M - V);
    return {state.N, state.M, state.T, V, A, score, elapsed, ""};
}

void print_summary(const vector<SimResult>& results, const string& label) {
    long long total_score = 0;
    double total_time = 0.0;
    double max_time = 0.0;
    int perfect_count = 0;
    int error_count = 0;
    int total_balls = 0;
    int total_possible = 0;
    
    int min_score = INF;
    int max_score = -1;
    
    for (const auto& r : results) {
        total_score += r.score;
        total_time += r.elapsed;
        max_time = max(max_time, r.elapsed);
        if (r.V == r.M) perfect_count++;
        if (!r.error.empty()) error_count++;
        total_balls += r.V;
        total_possible += r.M;
        
        min_score = min(min_score, r.score);
        max_score = max(max_score, r.score);
    }
    
    double avg_score = (double)total_score / results.size();
    double avg_time = total_time / results.size();
    
    vector<int> sorted_scores;
    for (const auto& r : results) sorted_scores.push_back(r.score);
    sort(sorted_scores.begin(), sorted_scores.end());
    int median_score = sorted_scores[sorted_scores.size() / 2];
    
    cout << "\n================================================================================\n";
    cout << "  Summary: " << label << "\n";
    cout << "================================================================================\n";
    cout << "  Test cases:        " << results.size() << "\n";
    cout << "  Errors:            " << error_count << "\n";
    cout << "  Perfect (V=M):     " << perfect_count << "/" << results.size() 
         << " (" << fixed << setprecision(1) << 100.0 * perfect_count / results.size() << "%)\n";
    if (total_possible > 0) {
        cout << "  Balls delivered:   " << total_balls << "/" << total_possible 
             << " (" << 100.0 * total_balls / total_possible << "%)\n";
    }
    cout << "  Score (avg):       " << fixed << setprecision(1) << avg_score << "\n";
    cout << "  Score (min):       " << min_score << "\n";
    cout << "  Score (max):       " << max_score << "\n";
    cout << "  Score (total):     " << total_score << "\n";
    cout << "  Score (median):    " << median_score << "\n";
    cout << "  Time (avg):        " << fixed << setprecision(2) << avg_time << "s\n";
    cout << "  Time (max):        " << max_time << "s\n";
    cout << "================================================================================\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <solver_cmd> [count] [seed_start] [--compare <other_solver>]\n";
        return 1;
    }
    
    string solver_a = argv[1];
    int count = 20;
    int seed_start = 0;
    
    string solver_b = "";
    
    int arg_idx = 2;
    if (argc > arg_idx && string(argv[arg_idx]) != "--compare") {
        count = stoi(argv[arg_idx++]);
    }
    if (argc > arg_idx && string(argv[arg_idx]) != "--compare") {
        seed_start = stoi(argv[arg_idx++]);
    }
    if (argc > arg_idx && string(argv[arg_idx]) == "--compare") {
        arg_idx++;
        if (argc > arg_idx) {
            solver_b = argv[arg_idx];
        }
    }
    
    cout << "\n==========================================================================\n";
    cout << "   AH066 C++ Benchmark Utility \n";
    cout << "  Solver A: " << solver_a << "\n";
    if (!solver_b.empty()) {
        cout << "  Solver B: " << solver_b << "\n";
    }
    cout << "  Tests: " << count << " | Seeds: " << seed_start << " to " << (seed_start + count - 1) << "\n";
    cout << "==========================================================================\n";
    
    vector<SimResult> results_a;
    vector<SimResult> results_b;
    
    if (solver_b.empty()) {
        // Single solver benchmark
        cout << setw(6) << "Seed" << " | "
             << setw(3) << "N" << " " << setw(3) << "M" << " " << setw(7) << "T" << " | "
             << setw(3) << "V" << "/" << setw(2) << "M" << " | "
             << setw(10) << "Score" << " | "
             << setw(7) << "Time" << " | Status\n";
        cout << string(68, '-') << "\n";
        
        for (int i = 0; i < count; ++i) {
            int seed = seed_start + i;
            SimResult r = run_test_on_solver(solver_a, seed);
            results_a.push_back(r);
            
            string status = (r.error.empty()) ? "OK" : ("ERR: " + r.error.substr(0, 20));
            char perfect = (r.V == r.M) ? '*' : ' ';
            cout << setw(6) << seed << " | "
                 << setw(3) << r.N << " " << setw(3) << r.M << " " << setw(7) << r.T << " | "
                 << setw(3) << r.V << "/" << setw(2) << r.M << perfect << " | "
                 << setw(10) << r.score << " | "
                 << setw(6) << fixed << setprecision(2) << r.elapsed << "s | "
                 << status << "\n";
        }
        
        print_summary(results_a, solver_a);
    } else {
        // Comparison benchmark
        cout << setw(6) << "Seed" << " | "
             << setw(3) << "N" << " " << setw(3) << "M" << " " << setw(7) << "T" << " | "
             << setw(3) << "V_A" << " " << setw(10) << "Score_A" << " | "
             << setw(3) << "V_B" << " " << setw(10) << "Score_B" << " | "
             << "Winner\n";
        cout << string(78, '-') << "\n";
        
        int a_wins = 0, b_wins = 0, ties = 0;
        vector<double> log_ratios;
        
        for (int i = 0; i < count; ++i) {
            int seed = seed_start + i;
            SimResult ra = run_test_on_solver(solver_a, seed);
            SimResult rb = run_test_on_solver(solver_b, seed);
            
            results_a.push_back(ra);
            results_b.push_back(rb);
            
            string winner = "TIE";
            if (ra.score < rb.score) {
                winner = "A \u25c0";
                a_wins++;
            } else if (rb.score < ra.score) {
                winner = "\u25b6 B";
                b_wins++;
            } else {
                ties++;
            }
            
            cout << setw(6) << seed << " | "
                 << setw(3) << ra.N << " " << setw(3) << ra.M << " " << setw(7) << ra.T << " | "
                 << setw(3) << ra.V << " " << setw(10) << ra.score << " | "
                 << setw(3) << rb.V << " " << setw(10) << rb.score << " | "
                 << winner << "\n";
            
            if (ra.score > 0 && rb.score > 0) {
                log_ratios.push_back(log((double)ra.score / rb.score));
            }
        }
        
        print_summary(results_a, solver_a);
        print_summary(results_b, solver_b);
        
        cout << "\n  Head-to-head: A wins " << a_wins << ", B wins " << b_wins << ", Ties " << ties << "\n";
        if (!log_ratios.empty()) {
            double sum_log = 0.0;
            for (double val : log_ratios) sum_log += val;
            double geo_mean_ratio = exp(sum_log / log_ratios.size());
            if (geo_mean_ratio < 1.0) {
                cout << "  A is " << fixed << setprecision(2) << 1.0 / geo_mean_ratio << "x better than B on average (geometric mean)\n";
            } else {
                cout << "  B is " << fixed << setprecision(2) << geo_mean_ratio << "x better than A on average (geometric mean)\n";
            }
        }
    }
    
    return 0;
}
