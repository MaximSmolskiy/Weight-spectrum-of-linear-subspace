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

<!---toc end-->

## Задание

a. Назовём вектором строку битов (значения ![](https://latex.codecogs.com/svg.latex?0) или ![](https://latex.codecogs.com/svg.latex?1)) фиксированной длины ![](https://latex.codecogs.com/svg.latex?N): то есть, всего возможно ![](https://latex.codecogs.com/svg.latex?2&space;^&space;N) различных векторов

b. Введём операцию сложения по модулю ![](https://latex.codecogs.com/svg.latex?2) векторов (операция ![](https://latex.codecogs.com/svg.latex?\texttt{xor})), которая по двум векторам ![](https://latex.codecogs.com/svg.latex?a) и ![](https://latex.codecogs.com/svg.latex?b) получает вектор ![](https://latex.codecogs.com/svg.latex?a&space;&plus;&space;b) той же длины ![](https://latex.codecogs.com/svg.latex?N)

c. Пусть задано множество ![](https://latex.codecogs.com/svg.latex?A&space;=&space;\\{&space;a_i&space;\mid&space;i&space;\in&space;1..K&space;\\}) из ![](https://latex.codecogs.com/svg.latex?0&space;\leqslant&space;K&space;\leqslant&space;2&space;^&space;N) векторов. Назовём его порождающим: при помощи сложения ![](https://latex.codecogs.com/svg.latex?a_i) множества ![](https://latex.codecogs.com/svg.latex?A) можно получить ![](https://latex.codecogs.com/svg.latex?2&space;^&space;K) векторов вида ![](https://latex.codecogs.com/svg.latex?\sum_{i&space;=&space;1}^K&space;\beta_i&space;a_i), где ![](https://latex.codecogs.com/svg.latex?\beta_i) равно либо ![](https://latex.codecogs.com/svg.latex?0), либо ![](https://latex.codecogs.com/svg.latex?1)

d. Весом вектора назовём количество единичных (ненулевых) битов в векторе: то есть, вес – это натуральное число от ![](https://latex.codecogs.com/svg.latex?0) до ![](https://latex.codecogs.com/svg.latex?N)

Требуется для заданных порождающего множества векторов и числа ![](https://latex.codecogs.com/svg.latex?N) построить гистограмму (спектр) количества различных векторов по их весу.

### Формат входных данных

Текстовый файл из набора строк одинаковой длины по одному вектору в строке (символы ![](https://latex.codecogs.com/svg.latex?0) или ![](https://latex.codecogs.com/svg.latex?1) без разделителей).

### Формат выходных данных

Текстовый файл строк с парой значений вес/количество разделённых символом табуляции, по одной паре на строку, сортированный по числовому значению веса.

### Дополнительный бонус

1. Если удаётся идентифицировать возможности для распараллеливания, - желательно реализовать параллельные вычисления и/или разметить код для применения параллельных итераций.

2. Оценить ресурсы, указать ограничения реализации, возможные методы дальнейшей алгоритмической и программной оптимизации.

## Идея решения

1. Прямым ходом аналога метода Гаусса найти ![](https://latex.codecogs.com/svg.latex?B) базисных (линейно независимых) векторов.

2. Обратным ходом аналога метода Гаусса уменьшить веса базисных векторов (в среднем).

3. Оптимально (используя код Грея) перебрать и вычислить весовой спектр ![](https://latex.codecogs.com/svg.latex?2&space;^&space;B) линейных комбинаций базисных векторов.

4. Чтобы получить искомый весовой спектр из полученного на шаге `3` - необходимо домножить его на ![](https://latex.codecogs.com/svg.latex?2&space;^&space;{K&space;-&space;B}).

## Реализация

### Ресурсы

1. Память - ![](https://latex.codecogs.com/svg.latex?O&space;(N&space;^&space;2))

2. Время - ![](https://latex.codecogs.com/svg.latex?O&space;(K&space;\cdot&space;N&space;^&space;2&space;&plus;&space;N&space;^&space;3&space;&plus;&space;2&space;^&space;B&space;\cdot&space;\overline{w})&space;=&space;O((K&space;&plus;&space;N)&space;\cdot&space;N&space;^&space;2&space;&plus;&space;2&space;^&space;B&space;\cdot&space;\overline{w})&space;=&space;O&space;(N&space;^&space;2&space;\cdot&space;2&space;^&space;N)), где ![](https://latex.codecogs.com/svg.latex?\overline{w}) - средний вес базисного вектора
