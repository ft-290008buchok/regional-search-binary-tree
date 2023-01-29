# Бинарное дерево поиска
В данном репозитории представлено решение на C++ задачи регионального поиска точек на плоскости, оптимизированная с помощью бинарного дерева поиска

# Пример работы программы

<p align="center">
    <img width="600" src="https://user-images.githubusercontent.com/71639489/215331718-245777c9-4dc3-4c54-8129-0f630a0bae0f.png">
</p>

<p align="center">
Графический вывод
</p>

<p align="center">
    <img width="600" src="https://user-images.githubusercontent.com/71639489/215332130-5119034b-565d-4120-a323-1be2c98aec0e.png">
</p>

<p align="center">
Консольный вывод
</p>

# Постановка задачи
На плоскости 10 х 10 случайно сгенерировано n точек (равномерное распределение), пользователь задаёт прямоугольник, лежащий в этой плоскости, в формате (x, y координаты левого верхнего угла, x, y координаты правого нижнего угла). Требуется найти сколько точекпопадает в заданный прямоугольник за время log(n).
# Решение
Для ускорения поиска производится предобработка массива точек - они представляются в виде бинарного дерева поиска по следующему алгоритму:   
Все точки сортируются по X координате   
Средняя точка в отсортированном массиве кладётся в корневой узел дерева   
Отсортированный массив делится на 2 подмассива по средней точке   
Оба подмассива сортируются по Y координате   
Средняя точка левого подмассива кладётся в узел левого потомка   
Средняя точка правого подмассива кладётся в узел правого потомка   
Потом левый подмассив делится на 2 подмассива по своей средней точке и каждый из них сортируется по X   
Аналогичная процедура проделывается и с правым подмассивом   
Средние точки подмассивов левого подмассива кладутся в узлы потомков левого потомка   
Средние точки подмассивов правого подмассива кладутся в узлы потомков правого потомка   
И так далее, пока не закончатся точки   

После предобработки можно приступать к поиску, для этого выполняется рекурсивный обход бинарного дерева, и при каждом спуске по дереву на один уровень вниз происходит выбор, пойти по левой ветке, по правой или  рассмотреть обе в зависимости от координат прямоугольника, заданного пользователем.   
Таким образом, при поиске обходятся не все точки, и задача поиска решается за O(log(n)), что  оптимальнее, чем прямой перебор, который занимает O(n^2).   
В realization.cpp представлена реализация данного алгоритма с подробными комментариями на русском языке.


