#include <climits>
#include <bitset>
#include <valarray>
#include <vector>
#include <thread>
#include <fstream>
#include <string>
#include <algorithm>
#include <bit>
#include <iostream>

using namespace std;

using ull_type = unsigned long long;

static size_t constexpr PROB_SIZE_REG_BIT = sizeof(uintptr_t) * CHAR_BIT;

using bl_type = bitset<PROB_SIZE_REG_BIT>;
using bit_bl_type = bl_type::reference;

using vec_type = valarray<bl_type>;
using set_vec_type = vector<vec_type>;

using weig_spec_type = valarray<ull_type>;
using set_weig_spec_type = vector<weig_spec_type>;

using set_thr_type = vector<thread>;

static size_t const LOG_2_DEF_NUM_THR = 3;

// Вычисляет количество блоков для хранения вектора.
size_t calc_num_bl(size_t N) {
    return (N + PROB_SIZE_REG_BIT - 1) / PROB_SIZE_REG_BIT;
}

// Возвращает i-ый бит вектора.
bool i_bit(const vec_type &vec, size_t i) {
    return vec[i / PROB_SIZE_REG_BIT][i % PROB_SIZE_REG_BIT];
}

// Обеспечивает доступ к i-ому биту вектора.
bit_bl_type acc_i_bit(vec_type &vec, size_t i) {
    return vec[i / PROB_SIZE_REG_BIT][i % PROB_SIZE_REG_BIT];
}

// Проверяет входную строку на наличие ошибок.
void check_line(string const &line, size_t N, string const &name_inp_file) {
    if (line.length() != N) {
        throw invalid_argument("In input file \"" + name_inp_file + "\" strings have different lengths");
    }

    if (line.find_first_not_of("01\r\n") != string::npos) {
        throw domain_error("In input file \"" + name_inp_file + "\" string \"" + line + "\" has unallowed character");
    }
}

// Конвертирует строку в вектор.
vec_type conv_line_vec(string const &line) {
    vec_type vec(calc_num_bl(line.size()));
    for (size_t ind_ch = 0; ind_ch < line.size(); ++ind_ch) {
        acc_i_bit(vec, ind_ch) = line[ind_ch] == '1';
    }
    return vec;
}

// Добавляет вектор в матрицу базисных (линейно независимых) векторов, если он является базисным - реализует прямой ход аналога метода Гаусса.
bool add_vec_mat_bas_vec(vec_type &vec, set_vec_type &mat_bas_vec, size_t N) {
    for (size_t ind_bit = 0; ind_bit < N; ++ind_bit) {
        if (i_bit(vec, ind_bit)) {
            if (mat_bas_vec[ind_bit].size()) {
                vec ^= mat_bas_vec[ind_bit];
            }
            else {
                mat_bas_vec[ind_bit] = vec;
                return true;
            }
        }
    }
    return false;
}

// Вычисляет вес вектора.
size_t calc_weig_vec(vec_type const &vec) {
    size_t weig_vec = 0;
    for (auto const &bl : vec) {
        weig_vec += bl.count();
    }
    return weig_vec;
}

// Вычисляет множество из B <= min { K, N } базисных (линейно независимых) векторов.
//         Построчно считывает входной файл.
//         Проверяет строку на наличие ошибок.
//         Конвертирует строку в вектор.
//         Добавляет вектор в матрицу базисных (линейно независимых) векторов, если он является базисным - реализует прямой ход аналога метода Гаусса.
//     Конвертирует матрицу базисных (линейно независимых) векторов в множество базисных векторов.
//     Исключает нулевые столбцы.
set_vec_type calc_set_bas_vec(string const &name_inp_file, size_t &N, size_t &K) {
    ifstream inp_file(name_inp_file);

    if (!inp_file.is_open()) {
        throw invalid_argument("Can't open input file \"" + name_inp_file + '\"');
    }

    string line;
    getline(inp_file, line);
    K = 1;
    N = line.length();
    check_line(line, N, name_inp_file);
    
    vec_type vec = conv_line_vec(line);
    set_vec_type mat_bas_vec(N);
    size_t num_bas_vec = add_vec_mat_bas_vec(vec, mat_bas_vec, N);

    while (getline(inp_file, line)) {
        ++K;
        check_line(line, N, name_inp_file);

        vec = conv_line_vec(line);
        num_bas_vec += add_vec_mat_bas_vec(vec, mat_bas_vec, N);
    }

    if (!N) {
        throw invalid_argument("Input file \"" + name_inp_file + "\" is empty");
    }

    set_vec_type set_bas_vec;
    set_bas_vec.reserve(num_bas_vec);
    for (auto const &bas_vec : mat_bas_vec) {
        if (bas_vec.size()) {
            set_bas_vec.push_back(bas_vec);
        }
    }

    vec_type or_bas_vec(set_bas_vec[0].size());
    for (auto const &bas_vec : set_bas_vec) {
        or_bas_vec |= bas_vec;
    }

    size_t num_col = N;
    N = calc_weig_vec(or_bas_vec);
    set_vec_type set_bas_vec_nonz_col(set_bas_vec.size(), vec_type(calc_num_bl(N)));
    size_t ind_nonz_col = 0;
    for (size_t ind_col = 0; ind_col < num_col; ++ind_col) {
        if (i_bit(or_bas_vec, ind_col)) {
            for (size_t ind_bas_vec = 0; ind_bas_vec < set_bas_vec.size(); ++ind_bas_vec) {
                acc_i_bit(set_bas_vec_nonz_col[ind_bas_vec], ind_nonz_col) = i_bit(set_bas_vec[ind_bas_vec], ind_col);
            }
            ++ind_nonz_col;
        }
    }

    return set_bas_vec_nonz_col;
}

// Вычисляет полный базисный весовой спектр - { C(N, i) | i = 0..N }, где C(N, i) - биномиальный коэффициент из N по i.
weig_spec_type calc_full_bas_weig_spec(size_t N) {
    weig_spec_type C_N(N + 1);
    C_N[0] = 1;
    for (size_t i = 1; i <= N; ++i) {
        C_N[i] = C_N[i - 1] * (N - i + 1) / i;
    }
    return C_N;
}

// Вычисляет индекс меняющегося бита в (i + 1)-ом коде Грея - количество единичных битов в числе (i xor (i + 1)) без единицы.
size_t calc_ind_ch_bit_next_code_Gray(ull_type ind_code_Gray) {
    ull_type num = ind_code_Gray ^ (ind_code_Gray + 1);
    return popcount(num) - 1;
}

// Вычисляет частичный базисный весовой спектр, используя код Грея.
void calc_part_bas_weig_spec(size_t ind_part, set_vec_type const &set_bas_vec, weig_spec_type &part_bas_weig_spec) {
    size_t const log_2_num_thr = min(LOG_2_DEF_NUM_THR, set_bas_vec.size());
    ull_type const num_ind_code_Gray_part = 1ULL << (set_bas_vec.size() - log_2_num_thr);
    ull_type const init_ind_code_Gray = ind_part * num_ind_code_Gray_part;
    ull_type init_code_Gray = init_ind_code_Gray ^ (init_ind_code_Gray >> 1);
    vec_type vec(set_bas_vec[0].size());
    size_t ind_bit_init_code_Gray = 0;
    while (init_code_Gray) {
        if (init_code_Gray & 1) {
            vec ^= set_bas_vec[ind_bit_init_code_Gray];
        }
        ++ind_bit_init_code_Gray;
        init_code_Gray >>= 1;
    }
    ++part_bas_weig_spec[calc_weig_vec(vec)];

    for (ull_type ind_code_Gray = init_ind_code_Gray; ind_code_Gray < init_ind_code_Gray + num_ind_code_Gray_part - 1; ++ind_code_Gray) {
        vec ^= set_bas_vec[calc_ind_ch_bit_next_code_Gray(ind_code_Gray)];
        ++part_bas_weig_spec[calc_weig_vec(vec)];
    }
}

// Вычисляет базисный весовой спектр.
// Если B == N, то вычисляет полный базисный весовой спектр.
// Если B != N, то:
//     по возможности, параллельно вычисляет частичные базисные весовые спектры;
//     вычисляет базисный весовой спектр - сумму частичных базисных весовых спектров.
weig_spec_type calc_bas_weig_spec(set_vec_type const &set_bas_vec, size_t N) {
    if (set_bas_vec.size() == N) {
        return calc_full_bas_weig_spec(N);
    }

    size_t const log_2_num_thr = min(LOG_2_DEF_NUM_THR, set_bas_vec.size());
    size_t const num_thr = size_t(1) << log_2_num_thr;
    set_thr_type set_thr;
    set_thr.reserve(num_thr);
    set_weig_spec_type set_part_bas_weig_spec(num_thr, weig_spec_type(N + 1));
    for (size_t ind_part = 0; ind_part < num_thr; ++ind_part) {
        set_thr.emplace_back(calc_part_bas_weig_spec, ind_part, cref(set_bas_vec), ref(set_part_bas_weig_spec[ind_part]));
    }

    weig_spec_type bas_weig_spec(N + 1);
    for (size_t ind_part = 0; ind_part < num_thr; ++ind_part) {
        set_thr[ind_part].join();
        bas_weig_spec += set_part_bas_weig_spec[ind_part];
    }
    return bas_weig_spec;
}

// Вычисляет весовой спектр - произведение базисного весового спектра и 2 ^ (K - B).
weig_spec_type calc_weig_spec(set_vec_type const &set_bas_vec, size_t N, size_t K) {
    return calc_bas_weig_spec(set_bas_vec, N) * (1ULL << (K - set_bas_vec.size()));
}

// Записывает весовой спектр в выходной файл.
void write_weig_spec(string const &name_out_file, weig_spec_type const &weig_spec) {
    ofstream out_file(name_out_file);

    if (!out_file.is_open()) {
        throw invalid_argument("Can't open output file \"" + name_out_file + '\"');
    }

    for (size_t weig = 0; weig < weig_spec.size(); ++weig)
        out_file << weig << '\t' << weig_spec[weig] << '\n';
}

// Главная функция, аргументы - имена входного и выходного файлов
int main(int argc, char *argv[]) {
    try {
        if (argc != 3) {
            throw invalid_argument("Wrong number of arguments - expected 3, found " + to_string(argc));
        }

        size_t N, K;
        set_vec_type const set_bas_vec = calc_set_bas_vec(argv[1], N, K);
        weig_spec_type const weig_spec = calc_weig_spec(set_bas_vec, N, K);
        write_weig_spec(argv[2], weig_spec);
        return EXIT_SUCCESS;
    } catch (exception const& exc) {
        cerr << exc.what() << ".\n";
        return EXIT_FAILURE;
    }
}
