# Вычисление весового спектра линейного подпространства

## Содержание

<!---toc start-->

* [Задание](#задание)
  * [Формат входных данных](#формат-входных-данных)
  * [Формат выходных данных](#формат-выходных-данных)
  * [Дополнительный бонус](#дополнительный-бонус)
* [Идея решения](#идея-решения)
* [Реализация](#реализация)
  * [Ресурсы](#ресурсы)
    * [Время](#время)
    * [Память](#память)

<!---toc end-->

## Задание

a. Назовём вектором строку битов (значения <img src="https://render.githubusercontent.com/render/math?math=0"> или <img src="https://render.githubusercontent.com/render/math?math=1">) фиксированной длины <img src="https://render.githubusercontent.com/render/math?math=N">: то есть, всего возможно <img src="https://render.githubusercontent.com/render/math?math=2%20%5E%20N"> различных векторов

b. Введём операцию сложения по модулю <img src="https://render.githubusercontent.com/render/math?math=2"> векторов (операция <img src="https://render.githubusercontent.com/render/math?math=%5Ctexttt%7Bxor%7D">), которая по двум векторам <img src="https://render.githubusercontent.com/render/math?math=a"> и <img src="https://render.githubusercontent.com/render/math?math=b"> получает вектор <img src="https://render.githubusercontent.com/render/math?math=a%20%2B%20b"> той же длины <img src="https://render.githubusercontent.com/render/math?math=N">

c. Пусть задано множество <img src="https://render.githubusercontent.com/render/math?math=A%20%3D%20%5C%7B%20a_i%20%5Cmid%20i%20%5Cin%201..K%20%5C%7D"> из <img src="https://render.githubusercontent.com/render/math?math=0%20%5Cleqslant%20K%20%5Cleqslant%202%20%5E%20N"> векторов. Назовём его порождающим: при помощи сложения <img src="https://render.githubusercontent.com/render/math?math=a_i"> множества <img src="https://render.githubusercontent.com/render/math?math=A"> можно получить <img src="https://render.githubusercontent.com/render/math?math=2%20%5E%20K"> векторов вида <img src="https://render.githubusercontent.com/render/math?math=%5Cdisplaystyle%20%5Csum_%7Bi%20%3D%201%7D%5EK%20%5Cbeta_i%20a_i">, где <img src="https://render.githubusercontent.com/render/math?math=%5Cbeta_i"> равно либо <img src="https://render.githubusercontent.com/render/math?math=0">, либо <img src="https://render.githubusercontent.com/render/math?math=1">

d. Весом вектора назовём количество единичных (ненулевых) битов в векторе: то есть, вес – это натуральное число от <img src="https://render.githubusercontent.com/render/math?math=0"> до <img src="https://render.githubusercontent.com/render/math?math=N">

Требуется для заданных порождающего множества векторов и числа <img src="https://render.githubusercontent.com/render/math?math=N"> построить гистограмму (спектр) количества различных векторов по их весу.

### Формат входных данных

Текстовый файл из набора строк одинаковой длины по одному вектору в строке (символы <img src="https://render.githubusercontent.com/render/math?math=0"> или <img src="https://render.githubusercontent.com/render/math?math=1"> без разделителей).

### Формат выходных данных

Текстовый файл строк с парой значений вес/количество разделённых символом табуляции, по одной паре на строку, сортированный по числовому значению веса.

### Дополнительный бонус

1. Если удаётся идентифицировать возможности для распараллеливания, - желательно реализовать параллельные вычисления и/или разметить код для применения параллельных итераций.

2. Оценить ресурсы, указать ограничения реализации, возможные методы дальнейшей алгоритмической и программной оптимизации.

## Идея решения

1. 1. Вычисление множества из <img src="https://render.githubusercontent.com/render/math?math=B%20%5Cleqslant%20%5Cmin%20%5C%7B%20K%2C%20N%20%5C%7D"> базисных (линейно независимых) векторов.

   2. 1. Построчное считывание входного файла.

      2. Проверка строки на наличие ошибок.

      3. Конвертация строки в вектор.

      4. Добавление вектора в матрицу базисных векторов, если он является базисным - реализация прямого хода аналога метода Гаусса.

   3. Конвертация матрицы в множество базисных векторов.

   4. Исключение нулевых столбцов.

2. 1. Если <img src="https://render.githubusercontent.com/render/math?math=B%20%3D%20N">, то вычисление полного базисного весового спектра <img src="https://render.githubusercontent.com/render/math?math=%5C%7B%20C_N%5Ei%20%5Cmid%20i%20%5Cin%200..N%20%5C%7D">, где <img src="https://render.githubusercontent.com/render/math?math=C_N%5Ei"> - биномиальный коэффициент из <img src="https://render.githubusercontent.com/render/math?math=N"> по <img src="https://render.githubusercontent.com/render/math?math=i">.

   2. Если <img src="https://render.githubusercontent.com/render/math?math=B%20%5Cneq%20N">, то вычисление базисного весового спектра - использование кода Грея.

3. Вычисление весового спектра - произведения базисного весового спектра и <img src="https://render.githubusercontent.com/render/math?math=2%20%5E%20%7BK%20-%20B%7D">.

## Реализация

### Ресурсы

#### Время

1. <img src="https://render.githubusercontent.com/render/math?math=O%20(K%20%5Ccdot%20N%20%5E%202)">.

2. <img src="https://render.githubusercontent.com/render/math?math=O%20(2%20%5E%20B%20%5Ccdot%20N)%20%3D%20O%20(N%20%5Ccdot%202%20%5E%7B%5Cmin%20%5C%7B%20K%2C%20N%20%5C%7D%7D)">.

3. <img src="https://render.githubusercontent.com/render/math?math=O%20(N)">.

Итого - <img src="https://render.githubusercontent.com/render/math?math=O%20(K%20%5Ccdot%20N%20%5E%202%20%2B%202%20%5E%20B%20%5Ccdot%20N)%20%3D%20O%20(K%20%5Ccdot%20N%20%5E%202%20%2B%20N%20%5Ccdot%202%20%5E%20%7B%5Cmin%20%5C%7B%20K%2C%20N%20%5C%7D%7D)">.

#### Память

1. <img src="https://render.githubusercontent.com/render/math?math=O%20(N%20%5E%202)">.

2. <img src="https://render.githubusercontent.com/render/math?math=O%20(N)">.

3. <img src="https://render.githubusercontent.com/render/math?math=O%20(N)">.

Итого - <img src="https://render.githubusercontent.com/render/math?math=O%20(N%20%5E%202)">.
