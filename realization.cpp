#include<iostream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include <SFML/Graphics.hpp>
//подкл. пространство имём sf
using namespace sf;
//объявление константы - количество делений в системе координат
const float lines_num = 10;
//получение ширины и длины экрана в пикселях
VideoMode vector = VideoMode::getDesktopMode();
int h = vector.height, w = vector.width, n;
//вычисление шага - кол-во пикселей, заключённое в одном делении системы координат
float step = h / lines_num;
//объявление объекта точки радиусом 6 пикселей
CircleShape point(6.f);
//счётчик точек, попавших в прямоугольник
int num_of_points = 0;
//объявление структуры описывающей точку
struct coord {
public:
	float x = 0.0;
	float y = 0.0;
};
//объявление структуры, описывающей узел дерева
struct tree_node {
public:
	coord coordinate;       //координата точки, которая хранится в узле
	bool v = true;          //делит плоскость горизонтально или вертикально
	tree_node *left = NULL; //указатели на левый и правый потомки
	tree_node *right = NULL;
};

//объявление прототипов функций
void draw_coordinate_system(RenderWindow &);
coord * generate_points();
void draw_points(RenderWindow &, coord *);
void draw_grid(RenderWindow &, coord *);
bool x_comparator(coord, coord);
bool y_comparator(coord, coord);
void draw_rect(RenderWindow &window, coord, coord);
tree_node * create_tree(tree_node *, coord *, int);
void draw_tree(RenderWindow &, tree_node *);
void calculate_points(tree_node *, coord, coord);
bool in_rect(coord, coord, coord);

int main()
{
	//установка русского шрифта в консоли
	setlocale(LC_ALL, "rus");
	std::cout << "Введите количество точек ";
	std::cin >> n;
	//выделение памяти для массива точек длины n
	coord *pts = new coord[n];
	//генерация случайных точек с помощью вызова соответствующей функции
	pts = generate_points();
	//создание бинарного дерева поиска
	/**в качестве начального параметра
	create_tree() передаётся пустой узел,
	чтобы соблюсти правила работы этой функции*/
	tree_node *null_node = new tree_node;
	tree_node *node = create_tree(null_node, pts, n);
	coord p_1, p_2;
	std::cout << "Введите координату левого нижнего и правого верхнего углов прямоугольника соответственно\n";
	std::cout << "в формате x1 y1 x2 y2:\n";
	//последлвательное считывание координат точек прямоугольника
	std::cin >> p_1.x >> p_1.y >> p_2.x >> p_2.y;
	//вычисление количества точех, попавших в прямоугольник
	calculate_points(node, p_1, p_2);
	std::cout << "В прямоугольник попало " << num_of_points << " точек\n";

	//рисование
	//============================
	//задание уровня сглаживания линий
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	// Объект, который, собственно, является главным окном приложения
	RenderWindow window(vector, "SFML Works!", Style::Default, settings);

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		// Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				// тогда закрываем его
				window.close();
		}

		draw_coordinate_system(window);
		draw_points(window, pts);
		//draw_grid(window, pts);
		draw_rect(window, p_1, p_2);
		//draw_tree(window, node);
		// Отрисовка окна	
		window.display();
	}

	return 0;
}

//определение функций
//============================
//отрисовка системы координат
void draw_coordinate_system(RenderWindow &window) {
	float x_pos = 0.0, y_pos = 0.0;
	Font font;
	font.loadFromFile("arial.ttf");
	Text text("", font, 20);
	for (int i = 0; i < lines_num; i++) {
		text.setString(std::to_string(i));
		text.setPosition(x_pos + 1, 0.0);
		window.draw(text);
		x_pos += step;
	}
	for (int i = 0; i < lines_num; i++) {
		text.setString(std::to_string(i));
		text.setPosition(0.0, y_pos + 1);
		window.draw(text);
		y_pos += step;
	}
}
//генерация точек
coord * generate_points() {
	srand(time(NULL));
	coord *points = new coord[n];
	for (int i = 0; i < n; i++) {
		points[i].x = rand() % (int)(lines_num)+0.001 * (rand() % 1000);
		points[i].y = rand() % (int)(lines_num)+0.001 * (rand() % 1000);
	}
	return points;
}
//отрисовка точек
void draw_points(RenderWindow &window, coord *points) {
	point.setFillColor(Color(255, 255, 0));
	for (int i = 0; i < n; i++, points++) {
		point.setPosition(points->x * step - 6, points->y * step - 6);
		window.draw(point);
	}
}
/*отрисовка сетки, которая получается при проведении
горизонтальной и вертикальной линий через каждую точку*/
void draw_grid(RenderWindow &window, coord *points) {
	RectangleShape line(Vector2f(1.f, h));
	line.setFillColor(Color(255, 100, 0));
	for (int i = 0; i < n; i++) {
		line.setPosition(points[i].x * step, 0);
		window.draw(line);
	}
	line.rotate(-90.f);
	for (int i = 0; i < n; i++) {
		line.setPosition(0, points[i].y * step);
		window.draw(line);
	}
}
//компаратор для сортировки по х
bool x_comparator(coord a, coord b) {
	return a.x < b.x;
}
//компаратор для сортировки по y
bool y_comparator(coord a, coord b) {
	return a.y < b.y;
}
//отрисовка прямоугольника
void draw_rect(RenderWindow &window, coord p_1, coord p_2) {
	float height = p_2.y - p_1.y;
	float width = p_2.x - p_1.x;
	RectangleShape h_line(Vector2f(4.f, height * step));
	RectangleShape w_line(Vector2f(width * step, 4.f));
	h_line.setFillColor(Color(255, 255, 0));
	w_line.setFillColor(Color(255, 255, 0));
	h_line.setPosition(p_1.x * step, p_1.y * step);
	window.draw(h_line);
	h_line.setPosition(p_2.x * step, p_1.y * step);
	window.draw(h_line);
	w_line.setPosition(p_1.x * step, p_2.y * step);
	window.draw(w_line);
	w_line.setPosition(p_1.x * step, p_1.y * step);
	window.draw(w_line);
}
//создание двоичного дерева
tree_node * create_tree(tree_node * pr_node, coord *points, int n) {
	tree_node * node = new tree_node;
	if (pr_node->v) {
		node->v = !pr_node->v;
		std::sort(points, points + n, x_comparator);
		node->coordinate = points[n / 2];
	}
	else {
		node->v = !pr_node->v;
		std::sort(points, points + n, y_comparator);
		node->coordinate = points[n / 2];
	}
	if (n == 1) {
		return node;
	}
	if (n == 2) {
		node->left = create_tree(node, points, n / 2);
	}
	else if (n % 2) {
		node->right = create_tree(node, points + n / 2 + 1, n / 2);
		node->left = create_tree(node, points, n / 2);
	}
	else {
		node->right = create_tree(node, points + n / 2 + 1, n / 2 - 1);
		node->left = create_tree(node, points, n / 2);
	}
	return node;
}
//изображение структуры дерева
void draw_tree(RenderWindow &window, tree_node *node) {
	if (node->right) {
		Vertex line_without_thickness[] =
		{
			// Координата первой вершины
			Vertex(Vector2f(node->coordinate.x * step, node->coordinate.y * step)),
			// Координата второй вершины
			Vertex(Vector2f(node->right->coordinate.x * step, node->right->coordinate.y * step))
		};
		window.draw(line_without_thickness, 2, Lines);
		draw_tree(window, node->right);
	}
	if (node->left) {
		Vertex line_without_thickness[] =
		{
			// Координата первой вершины
			Vertex(Vector2f(node->coordinate.x * step, node->coordinate.y * step)),
			// Координата второй вершины
			Vertex(Vector2f(node->left->coordinate.x * step, node->left->coordinate.y * step))
		};
		window.draw(line_without_thickness, 2, Lines);
		draw_tree(window, node->left);
	}
}
//подсчёт колличества точек путём рекурсивного обхода дерева
void calculate_points(tree_node *node, coord p1, coord p2) {
	if (!node->left && !node->right) {//если вершина терминальная
		if (in_rect(node->coordinate, p1, p2)) {
			num_of_points++;
		}
		return;
	}
	else if (node->v) {//если делелин на верхнее и нижнее
		if (p2.y <= node->coordinate.y) {//если ниже
			if (node->left) {
				calculate_points(node->left, p1, p2);
			}
		}
		else if (p1.y >= node->coordinate.y) {//если выше
			if (node->right) {
				calculate_points(node->right, p1, p2);
			}
		}
		else {//если пересекается
			coord left1, left2;
			left1 = p1;
			left2.y = node->coordinate.y;
			left2.x = p2.x;
			coord right1, right2;
			right2 = p2;
			right1.y = node->coordinate.y;
			right1.x = p1.x;
			if (node->left) {
				calculate_points(node->left, left1, left2);
			}
			if (node->right) {
				calculate_points(node->right, right1, right2);
			}
			if (in_rect(node->coordinate, p1, p2)) {
				num_of_points++;
			}
		}
	}
	else {//если деление на левое и правое
		if (p2.x <= node->coordinate.x) {//если левее
			if (node->left) {
				calculate_points(node->left, p1, p2);
			}
		}
		else if (p1.x >= node->coordinate.x) {//если правее
			if (node->right) {
				calculate_points(node->right, p1, p2);
			}
		}
		else {//если пересекается
			coord left1, left2;
			left1 = p1;
			left2.x = node->coordinate.x;
			left2.y = p2.y;
			coord right1, right2;
			right2 = p2;
			right1.x = node->coordinate.x;
			right1.y = p1.y;
			if (node->left) {
				calculate_points(node->left, left1, left2);
			}
			if (node->right) {
				calculate_points(node->right, right1, right2);
			}
			if (in_rect(node->coordinate, p1, p2)) {
				num_of_points++;
			}
		}
	}

}
/*проверка принаджлежности точки прямоугольнику,
который задан левой нижней и правой верхней точками p1 и p2*/
bool in_rect(coord p, coord p1, coord p2) {
	return (p.x >= p1.x) && (p.x <= p2.x) && (p.y >= p1.y) && (p.y <= p2.y);
}
