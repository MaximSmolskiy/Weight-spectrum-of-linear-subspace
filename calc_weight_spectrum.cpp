#include <bitset>
#include <valarray>
#include <vector>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

using ull_t = unsigned long long;

static size_t constexpr SIZE_BL = sizeof(uintptr_t) << 3;
using bl_t = bitset<SIZE_BL>;
using vec_t = valarray<bl_t>;
using set_vec_t = vector<vec_t>;

using weig_spec_t = valarray<ull_t>;
using set_weig_spec_t = vector<weig_spec_t>;

using set_thr_t = vector<thread>;

static size_t const LOG_2_DEF_NUM_THR = 3;

void report_error(string const &mes) {
    cerr << mes << ".\n";
    exit(EXIT_FAILURE);
}

set_vec_t calc_set_bas_vec(string const &name_file, size_t &K, size_t &N)
{
    ifstream inp(name_file);

    if (!inp.is_open()) {
        report_error("Can't open input file \"" + name_file + '\"');
    }

    set_vec_t mat_bas_vec;
    size_t num_bas_vec = 0;
    string line;
    bool is_first_line = true;
    while (getline(inp, line)) {
        if (!is_first_line && line.length() != N) {
            report_error("In input file \"" + name_file + "\" strings have different lengths");
        }

        if (line.find_first_not_of("01\r\n") != string::npos) {
            report_error("In input file \"" + name_file + "\" string \"" + line + "\" has unallowed character");
        }

        N = line.length();
        is_first_line = false;
        K++;

        vec_t vec((N + SIZE_BL - 1) / SIZE_BL);
        for (size_t i = 0; i < N; i++) {
            vec[i / SIZE_BL][i % SIZE_BL] = line[i] == '1';
        }

        mat_bas_vec.resize(N);
        for (size_t i = 0; i < N; i++) {
            if (vec[i / SIZE_BL][i % SIZE_BL]) {
                if (mat_bas_vec[i].size()) {
                    vec ^= mat_bas_vec[i];
                } else {
                    mat_bas_vec[i] = vec;
                    num_bas_vec++;
                    break;
                }
            }
        }
    }

    if (!N) {
        report_error("Input file \"" + name_file + "\" is empty");
    }

    set_vec_t set_bas_vec(num_bas_vec);
    size_t i = 0;
    for (auto const &vec : mat_bas_vec) {
        if (vec.size()) {
            set_bas_vec[i] = vec;
            i++;
        }
    }

    return set_bas_vec;
}

weig_spec_t calc_full_bas_weig_spec(size_t N) {
    weig_spec_t full_bas_weig_spec(N + 1);
    full_bas_weig_spec[0] = 1;
    for (size_t i = 1; i <= N; i++) {
        full_bas_weig_spec[i] = full_bas_weig_spec[i - 1] * (N - i + 1) / i;
    }
    return full_bas_weig_spec;
}

size_t calc_weig_vec(vec_t const &vec) {
    size_t weig_vec = 0;
    for (auto const &bl : vec) {
        weig_vec += bl.count();
    }
    return weig_vec;
}

size_t calc_ind_bas_vec(ull_t comb) {
    comb ^= comb + 1;
    size_t ind_bas_vec = 0;
    while (comb) {
        comb &= comb - 1;
        ind_bas_vec++;
    }
    return ind_bas_vec - 1;
}

void calc_part_bas_weig_spec(size_t i, set_vec_t const &set_bas_vec, weig_spec_t &part_bas_weig_spec) {
    vec_t vec(set_bas_vec[0].size());
    size_t log_2_num_thr = min(LOG_2_DEF_NUM_THR, set_bas_vec.size());
    ull_t init_comb = i * (1ULL << (set_bas_vec.size() - log_2_num_thr));
    ull_t init_code_Gray = init_comb ^ (init_comb >> 1);
    size_t j = 0;
    while (init_code_Gray) {
        if (init_code_Gray & 1) {
            vec ^= set_bas_vec[j];
        }
        j++;
        init_code_Gray >>= 1;
    }
    part_bas_weig_spec[calc_weig_vec(vec)]++;

    for (ull_t comb = init_comb; comb < init_comb + (1ULL << (set_bas_vec.size() - log_2_num_thr)) - 1; comb++) {
        vec ^= set_bas_vec[calc_ind_bas_vec(comb)];
        part_bas_weig_spec[calc_weig_vec(vec)]++;
    }
}

weig_spec_t calc_weig_spec(set_vec_t const &set_bas_vec, size_t K, size_t N) {
    weig_spec_t bas_weig_spec(N + 1);

    if (set_bas_vec.size() == N) {
        bas_weig_spec = calc_full_bas_weig_spec(N);
    } else {
        size_t log_2_num_thr = min(LOG_2_DEF_NUM_THR, set_bas_vec.size());
        size_t num_thr = size_t(1) << log_2_num_thr;
        set_thr_t set_thr(num_thr);
        set_weig_spec_t set_part_bas_weig_spec(num_thr, weig_spec_t(N + 1));
        for (size_t i = 0; i < num_thr; i++) {
            set_thr[i] = thread(calc_part_bas_weig_spec, i, cref(set_bas_vec), ref(set_part_bas_weig_spec[i]));
        }

        for (size_t i = 0; i < num_thr; i++) {
            set_thr[i].join();
            bas_weig_spec += set_part_bas_weig_spec[i];
        }
    }

    return bas_weig_spec * (1ULL << (K - set_bas_vec.size()));
}

void write_weig_spec(string const &name_file, weig_spec_t const &weig_spec) {
    ofstream out(name_file);

    if (!out.is_open()) {
        report_error("Can't open output file \"" + name_file + '\"');
    }

    for (size_t i = 0; i < weig_spec.size(); i++)
        out << i << '\t' << weig_spec[i] << '\n';
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        report_error("Wrong number of arguments - expected 3, found " + to_string(argc));
    }

    size_t K = 0;
    size_t N = 0;
    set_vec_t set_bas_vec = calc_set_bas_vec(argv[1], K, N);
    weig_spec_t weig_spec = calc_weig_spec(set_bas_vec, K, N);
    write_weig_spec(argv[2], weig_spec);

    return EXIT_SUCCESS;
}
