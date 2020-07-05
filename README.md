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

1. Считывание входных данных - набора ![](https://latex.codecogs.com/svg.latex?K) строк одинаковой длины ![](https://latex.codecogs.com/svg.latex?N) по одному вектору в строке (символы ![](https://latex.codecogs.com/svg.latex?0) или ![](https://latex.codecogs.com/svg.latex?1) без разделителей).

2. Конвертация набора строк в порождающее множество векторов ![](https://latex.codecogs.com/svg.latex?A).

3. 1. Нахождение ![](https://latex.codecogs.com/svg.latex?B&space;\leqslant&space;N) базисных (линейно независимых) векторов прямым ходом аналога метода Гаусса.

   2. Уменьшение в среднем весов базисных векторов обратным ходом аналога метода Гаусса.
   
   3. Хранение базисных векторов в виде массивов индексов единиц.

4. 1. Параллельное нахождение ![](https://latex.codecogs.com/svg.latex?T&space;\leqslant&space;2&space;^&space;B) (![](https://latex.codecogs.com/svg.latex?T) - степень двойки) частичных базисных весовых спектров, используя код Грея.

   2. Нахождение базисного весового спектра - суммы частичных базисных весовых спектров.
   
   3. Нахождение весового спектра - произведения базисного весового спектра и ![](https://latex.codecogs.com/svg.latex?2&space;^&space;{K&space;-&space;B}).
   
5. Запись выходных данных - весового спектра.

## Реализация

### Ресурсы

#### Время

1. ![](https://latex.codecogs.com/svg.latex?O&space;(K&space;\cdot&space;N)&space;=&space;O&space;(N&space;\cdot&space;2&space;^&space;N)).

2. ![](https://latex.codecogs.com/svg.latex?O&space;(K&space;\cdot&space;N)&space;=&space;O&space;(N&space;\cdot&space;2&space;^&space;N)).

3. 1. ![](https://latex.codecogs.com/svg.latex?O&space;(K&space;\cdot&space;N&space;^&space;2)&space;=&space;O&space;(N&space;^&space;2&space;\cdot&space;2&space;^&space;N)).

   2. ![](https://latex.codecogs.com/svg.latex?O&space;(N&space;^&space;3)).
   
   3. ![](https://latex.codecogs.com/svg.latex?O&space;(N&space;^&space;2)).

4. 1. ![](https://latex.codecogs.com/svg.latex?O&space;(T&space;\cdot&space;B&space;\cdot&space;\overline{w}&space;&plus;&space;2&space;^&space;B&space;\cdot&space;\overline{w})&space;=&space;O(N&space;^&space;2&space;\cdot&space;2&space;^&space;N)), где ![](https://latex.codecogs.com/svg.latex?\overline{w}) - средний вес базисных векторов.

   2. ![](https://latex.codecogs.com/svg.latex?O&space;(T&space;\cdot&space;N)&space;=&space;O&space;(N&space;\cdot&space;2&space;^&space;N)).
   
   3. ![](https://latex.codecogs.com/svg.latex?O&space;(N)).
   
5. ![](https://latex.codecogs.com/svg.latex?O&space;(N)).

Итого - ![](https://latex.codecogs.com/svg.latex?O&space;(K&space;\cdot&space;N&space;^&space;2&space;&plus;&space;N&space;^&space;3&space;&plus;&space;T&space;\cdot&space;B&space;\cdot&space;\overline{w}&space;&plus;&space;2&space;^&space;B&space;\cdot&space;\overline{w}&space;&plus;&space;T&space;\cdot&space;N)&space;=&space;O(N&space;^&space;2&space;\cdot&space;2&space;^&space;N)).

#### Память

1. ![](https://latex.codecogs.com/svg.latex?O&space;(K&space;\cdot&space;N)&space;=&space;O&space;(N&space;\cdot&space;2&space;^&space;N)).

2. ![](https://latex.codecogs.com/svg.latex?O&space;(K&space;\cdot&space;N)&space;=&space;O&space;(N&space;\cdot&space;2&space;^&space;N)).

3. 1. ![](https://latex.codecogs.com/svg.latex?O&space;(N&space;^&space;2)).

   2. ![](https://latex.codecogs.com/svg.latex?O&space;(1)).
   
   3. ![](https://latex.codecogs.com/svg.latex?O&space;(N&space;\cdot&space;\overline{w})&space;=&space;O&space;(N&space;^&space;2)), где ![](https://latex.codecogs.com/svg.latex?\overline{w}) - средний вес базисных векторов.

4. 1. ![](https://latex.codecogs.com/svg.latex?O&space;(T&space;\cdot&space;N)&space;=&space;O&space;(N&space;\cdot&space;2&space;^&space;N)).
   
   2. ![](https://latex.codecogs.com/svg.latex?O&space;(N)).
   
   3. ![](https://latex.codecogs.com/svg.latex?O&space;(1)).
   
5. ![](https://latex.codecogs.com/svg.latex?O&space;(1)).

Итого - ![](https://latex.codecogs.com/svg.latex?O&space;(K&space;\cdot&space;N&space;&plus;&space;N&space;^&space;2&space;&plus;&space;T&space;\cdot&space;N)&space;=&space;O&space;(N&space;\cdot&space;2&space;^&space;N)).
