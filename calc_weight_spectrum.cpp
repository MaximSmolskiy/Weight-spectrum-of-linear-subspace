#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <omp.h>

using namespace std;

// Двоичный логарифм количества итераций параллельного цикла
const size_t LOG_2_T = 7;

// Считывание входных данных
vector<string> read_input_data(const string &filename)
{
  freopen(filename.c_str(), "r", stdin);
  vector<string> set_lines;
  string line;
  while (cin >> line)
    set_lines.push_back(line);
  fclose(stdin);
  return set_lines;
}

// Конвертация набора строк в порождающее множество векторов
vector<vector<bool>> convert_set_lines_to_generating_set_vectors(const vector<string> &set_lines)
{
  vector<vector<bool>> generating_set_vectors(set_lines.size(), vector<bool>(set_lines[0].size()));
  for (size_t i = 0; i < set_lines.size(); i++)
    for (size_t j = 0; j < set_lines[i].size(); j++)
      generating_set_vectors[i][j] = set_lines[i][j] == '1';
  return generating_set_vectors;
}

// Операция сложения по модулю 2 векторов (операция xor),
vector<bool> operator^(const vector<bool> &vector_1, const vector<bool> &vector_2)
{
  vector<bool> xor_vectors(vector_1.size());
  for (size_t i = 0; i < xor_vectors.size(); i++)
    xor_vectors[i] = vector_1[i] != vector_2[i];
  return xor_vectors;
}

// Нахождение индексов единиц базисных векторов аналогом метода Гаусса - прямой и обратный ход
vector<vector<size_t>> find_indices_1_basis_vectors(vector<vector<bool>> &generating_set_vectors, size_t K, size_t N)
{
  vector<vector<bool>> set_basis_vectors(N, vector<bool>(N));
  vector<bool> presence_basis_vectors(N);
  for (size_t i = 0; i < K; i++)
    for (size_t j = 0; j < N; j++)
      if (generating_set_vectors[i][j])
        if (presence_basis_vectors[j])
          generating_set_vectors[i] = generating_set_vectors[i] ^ set_basis_vectors[j];
        else
        {
          set_basis_vectors[j] = generating_set_vectors[i];
          presence_basis_vectors[j] = true;
          break;
        }
  for (size_t i = N - 1; i != SIZE_MAX; i--)
    if (presence_basis_vectors[i])
      for (size_t j = N - 1; j != SIZE_MAX; j--)
        if (i != j && set_basis_vectors[i][j] && presence_basis_vectors[j])
          set_basis_vectors[i] = set_basis_vectors[i] ^ set_basis_vectors[j];
  vector<vector<size_t>> indices_1_basis_vectors;
  for (size_t i = 0; i < N; i++)
    if (presence_basis_vectors[i])
    {
      indices_1_basis_vectors.push_back(vector<size_t>());
      for (size_t j = 0; j < N; j++)
        if (set_basis_vectors[i][j])
          indices_1_basis_vectors.back().push_back(j);
    }
  return indices_1_basis_vectors;
}

// Степень с основанием 2
unsigned long long power_2(size_t exponent)
{
  unsigned long long power_2_exponent = 1;
  for (size_t i = 0; i < exponent; i++)
    power_2_exponent *= 2;
  return power_2_exponent;
}

// Вычисление таблицы двоичного логарифма минус 1
unordered_map<unsigned long long, size_t> calculate_log_2_table_minus_1()
{
  unordered_map<unsigned long long, size_t> log_2_table_minus_1;
  unsigned long long power_2_i = 2;
  for (size_t i = 1; i < 64; i++)
  {
    log_2_table_minus_1[power_2_i] = i - 1;
    power_2_i *= 2;
  }
  return log_2_table_minus_1;
}

// Нахождение частичного весового спектра
vector<unsigned long long> find_partial_weight_spectrum(size_t N, int i, unsigned long long power_2_B_minus_log_2_T, const vector<vector<size_t>> &indices_1_basis_vectors, unordered_map<unsigned long long, size_t> &log_2_table_minus_1)
{
  vector<unsigned long long> partial_weight_spectrum(N + 1);
  vector<bool> binary_vector(N);
  size_t weight_vector = 0;
  unsigned long long first_combination = i * power_2_B_minus_log_2_T, first_Gray_code = first_combination ^ (first_combination >> 1);
  for (size_t j = 0; first_Gray_code > 0; first_Gray_code /= 2, j++)
    if (first_Gray_code % 2 == 1)
    {
      size_t weight_changed_0 = 0;
      for (size_t k = 0; k < indices_1_basis_vectors[j].size(); k++)
      {
        binary_vector[indices_1_basis_vectors[j][k]] = !binary_vector[indices_1_basis_vectors[j][k]];
        weight_changed_0 += binary_vector[indices_1_basis_vectors[j][k]];
      }
      weight_vector += 2 * weight_changed_0 - indices_1_basis_vectors[j].size();
    }
  partial_weight_spectrum[weight_vector]++;
  for (unsigned long long j = first_combination; j < first_combination + power_2_B_minus_log_2_T - 1; j++)
  {
    size_t index_basis_vector = log_2_table_minus_1[(j ^ (j + 1)) + 1], weight_changed_0 = 0;
    for (size_t k = 0; k < indices_1_basis_vectors[index_basis_vector].size(); k++)
    {
      binary_vector[indices_1_basis_vectors[index_basis_vector][k]] = !binary_vector[indices_1_basis_vectors[index_basis_vector][k]];
      weight_changed_0 += binary_vector[indices_1_basis_vectors[index_basis_vector][k]];
    }
    weight_vector += 2 * weight_changed_0 - indices_1_basis_vectors[index_basis_vector].size();
    partial_weight_spectrum[weight_vector]++;
  }
  return partial_weight_spectrum;
}

// Нахождение весового спектра
vector<unsigned long long> find_weight_spectrum(size_t K, size_t N, size_t B, const vector<vector<size_t>> &indices_1_basis_vectors, unordered_map<unsigned long long, size_t> &log_2_table_minus_1)
{
  size_t log_2_T = min(LOG_2_T, B), T = size_t(power_2(log_2_T));
  unsigned long long power_2_B_minus_log_2_T = power_2(B - log_2_T);
  vector<vector<unsigned long long>> partial_weight_spectra(T);
#pragma omp parallel
  {
#pragma omp for
    for (int i = 0; i < int(T); i++)
      partial_weight_spectra[i] = find_partial_weight_spectrum(N, i, power_2_B_minus_log_2_T, indices_1_basis_vectors, log_2_table_minus_1);
  }
  vector<unsigned long long> weight_spectrum(N + 1);
  for (size_t i = 0; i < T; i++)
    for (size_t j = 0; j <= N; j++)
      weight_spectrum[j] += partial_weight_spectra[i][j];
  unsigned long long power_2_K_minus_B = power_2(K - B);
  for (size_t i = 0; i <= N; i++)
    weight_spectrum[i] *= power_2_K_minus_B;
  return weight_spectrum;
}

// Запись выходных данных
void write_output_data(const string &filename, const vector<unsigned long long> &weight_spectrum, size_t N)
{
  freopen(filename.c_str(), "w", stdout);
  for (size_t i = 0; i <= N; i++)
    cout << i << '\t' << weight_spectrum[i] << '\n';
  fclose(stdout);
}

int main()
{
  vector<string> set_lines = read_input_data("example/input.txt");
  vector<vector<bool>> generating_set_vectors = convert_set_lines_to_generating_set_vectors(set_lines);
  size_t K = generating_set_vectors.size(), N = generating_set_vectors[0].size();
  vector<vector<size_t>> indices_1_basis_vectors = find_indices_1_basis_vectors(generating_set_vectors, K, N);
  size_t B = indices_1_basis_vectors.size();
  unordered_map<unsigned long long, size_t> log_2_table_minus_1 = calculate_log_2_table_minus_1();
  vector<unsigned long long> weight_spectrum = find_weight_spectrum(K, N, B, indices_1_basis_vectors, log_2_table_minus_1);
  write_output_data("example/output.txt", weight_spectrum, N);
  return 0;
}
