#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

#define MORE 2
#define LESS 1
#define EQ 0

// классы пересечения и объединения прямоугольников, которые хранят два родительских прямоугольника
// и полученный при пересечении/объединении прямоугольник (в классе Intersection есть поле, показвающее
// существует ли пересечение прямоугольников)
class Intersection;
class Union;

// класс точка, при помощи которого строится класс прмяоугольник
class Dot {
public:
    Dot() {
        x_ = 0.0;
        y_ = 0.0;
    }
    Dot(double x, double y) {
        x_ = x;
        y_ = y;
    }
    double getX() const;
    double getY() const;
    void setX(double val);
    void setY(double val);
    void moveByX(double delta);
    void moveByY(double delta);
private:
    double x_;
    double y_;
};

double Dot::getX() const { return x_; }
double Dot::getY() const { return y_; }
void Dot::setX(double val) { x_ = val; }
void Dot::setY(double val) { y_ = val; }
void Dot::moveByX(double delta) { x_ += delta; }
void Dot::moveByY(double delta) { y_ += delta; }

// перегрузим операторы << и >> для более удобного ввода/вывода
std::istream& operator>>(std::istream& stream, Dot& d) {
    double x, y;
    stream >> x >> y;
    d.setX(x);
    d.setY(y);
    return stream;
}
std::ostream& operator<<(std::ostream& stream, const Dot& d) {
    stream << "(" << d.getX() << ", " << d.getY() << ")";
    return stream;
}

class Rectangle {
public:
    Rectangle() {
        Dot lowerLeft_;
        Dot upperRight_;
    }
    Rectangle(Dot lowerLeft, Dot upperRight) {
        lowerLeft_ = lowerLeft;
        upperRight_ = upperRight;
    }
    void setLowerLeft(const Dot& d);
    void setUpperRight(const Dot& d);
    Dot getLowerLeft() const;
    Dot getUpperRight() const;
    double getLength() const;
    double getHeight() const;
    double getArea() const;
    double getPerimeter() const;
    // чтобы передвинуть прямоугольник нужно передвинуть соответствующие координаты двух его точек
    void moveByX(double delta);
    void moveByY(double delta);
    // !при изменении размеров фиксируется левая нижняя точка!
    bool reduceLength(double delta);
    bool reduceHeight(double delta);
    // методы класса
    static int compByArea(const Rectangle& lhs, const Rectangle& rhs);
    static int compByPerimeter(const Rectangle& lhs, const Rectangle& rhs);
    static Intersection getIntersection(Rectangle first, Rectangle second);
    static Union getUnion(Rectangle first, Rectangle second);
    operator double();
private:
    Dot lowerLeft_;
    Dot upperRight_;
};

void Rectangle::setLowerLeft(const Dot& d) { lowerLeft_ = d; }
void Rectangle::setUpperRight(const Dot& d) { upperRight_ = d; }
Dot Rectangle::getLowerLeft() const { return lowerLeft_; }
Dot Rectangle::getUpperRight() const { return upperRight_; }
double Rectangle::getLength() const { return upperRight_.getX() - lowerLeft_.getY(); }
double Rectangle::getHeight() const { return upperRight_.getX() - lowerLeft_.getY(); }
double Rectangle::getArea() const { return getLength() * getHeight(); }
double Rectangle::getPerimeter() const { return 2 * (getLength() + getHeight()); }
void Rectangle::moveByX(double delta) {
    lowerLeft_.moveByX(delta);
    upperRight_.moveByX(delta);
}
void Rectangle::moveByY(double delta) {
    lowerLeft_.moveByY(delta);
    upperRight_.moveByY(delta);
}
bool Rectangle::reduceLength(double delta) {
    double len = getLength();
    if (delta > len)
        return false;
    upperRight_.moveByX(-delta);
    return true;
}
bool Rectangle::reduceHeight(double delta) {
    double height = getHeight();
    if (delta > height)
        return false;
    upperRight_.moveByY(-delta);
    return true;
}
int Rectangle::compByArea(const Rectangle& lhs, const Rectangle& rhs) {
    double lhsArea = lhs.getArea();
    double rhsArea = rhs.getArea();
    if (lhsArea > rhsArea)
        return MORE;
    else if (lhsArea < rhsArea)
        return LESS;
    return EQ;
}
int Rectangle::compByPerimeter(const Rectangle& lhs, const Rectangle& rhs) {
    double lhsPer = lhs.getPerimeter();
    double rhsPer = rhs.getPerimeter();
    if (lhsPer > rhsPer)
        return MORE;
    else if (lhsPer < rhsPer)
        return LESS;
    return EQ;
}

// перегрузим оператор << и >> для более удобного вывода/ввода прямоугольника в поток
std::ostream& operator<<(std::ostream& stream, const Rectangle& r) {
    stream << "left lower dot: " << r.getLowerLeft() << std::endl;
    stream << "right upper dot: " << r.getUpperRight();
    return stream;
}
std::istream& operator>>(std::istream& stream, Rectangle& r) {
    Dot lLeft, uRight;
    stream >> lLeft >> uRight;
    r.setLowerLeft(lLeft);
    r.setUpperRight(uRight);
    return stream;
}
// приведение к double вычисляет площадь
Rectangle::operator double() {
    return this->getArea();
}
Rectangle& operator++(Rectangle& r) {
    r.reduceHeight(-1.0);
    r.reduceLength(-1.0);
    return r;
}
// параметр post указывает компилятору на то, что инкремент постфиксный
Rectangle operator++(Rectangle& r, int post) {
    Rectangle copy = r; 
    r.reduceHeight(-1.0);
    r.reduceLength(-1.0);
    return copy;
}
// сравнивать прямоугольники будем по площади
bool operator<(const Rectangle& lhs, const Rectangle& rhs) {
    return lhs.getArea() < rhs.getArea();
}
bool operator>(const Rectangle& lhs, const Rectangle& rhs) {
    return lhs.getArea() > rhs.getArea();
}
bool operator==(const Rectangle& lhs, const Rectangle& rhs) {
    return lhs.getArea() == rhs.getArea();
}
Rectangle operator "" _r(const char* str, long unsigned int size) {
    std::string s;
    for (auto i = 0; i < size; ++i) {
        if (str[i] == ')' || str[i] == '(' || str[i] == ',')
            s.push_back(' ');
        else 
            s.push_back(str[i]);
    }
    std::stringstream ss(s);
    Rectangle res;
    ss >> res;
    return res;
}

class Intersection {
public:
    Intersection(const Rectangle& f, const Rectangle& s,
                 Rectangle r, bool v = true) : first(f), second(s), res(r) {
        valid = v;
    }
    const Rectangle& getFirst() const;
    const Rectangle& getSecond() const;
    bool isValid() const;
    Rectangle getRes() const;
private:
    const Rectangle& first;
    const Rectangle& second;
    // поле, которое отвечает за то, существует ли пересечение
    bool valid;
    // искомое пересечение - прямоугольник
    Rectangle res;
};

const Rectangle& Intersection::getFirst() const { return first; }
const Rectangle& Intersection::getSecond() const { return second; }
bool Intersection::isValid() const { return valid; }
Rectangle Intersection::getRes() const { return res; }

class Union {
public:
    Union(const Rectangle& f, const Rectangle& s,
          Rectangle r, bool v = true) : first(f), second(s), res(r) {}
    const Rectangle& getFirst() const;
    const Rectangle& getSecond() const;
    Rectangle getRes() const;
private:
    const Rectangle& first;
    const Rectangle& second;
    Rectangle res;
};

const Rectangle& Union::getFirst() const { return first; }
const Rectangle& Union::getSecond() const { return second; }
Rectangle Union::getRes() const { return res; }

Intersection Rectangle::getIntersection(Rectangle first, Rectangle second) {
    // для получения правой верхней точки пересечения находим наименьшую из двух
    // правых координат х и наименьшую из двух верхних координат y
    double xMax = fmin(first.getUpperRight().getX(), second.getUpperRight().getX());
    double yMax = fmin(first.getUpperRight().getY(), second.getUpperRight().getY());
    // для получения левой нижней точки пересечения находим наибольшую из двух
    // левых координат х и наибольшую из двух нижних координат y
    double xMin = fmax(first.getLowerLeft().getX(), second.getLowerLeft().getX());
    double yMin = fmax(first.getLowerLeft().getY(), second.getLowerLeft().getY());
    Dot resUpperRight(xMax, yMax);
    Dot resLowerLeft(xMin, yMin);
    Rectangle res(resLowerLeft, resUpperRight);
    // проверка на валидность пересечения: если левая нижняя точка лежит правее/выше
    // правой верхней точки, то пересечение пустое
    Intersection iRes(first, second, res, yMin < yMax || xMin < xMax);
    return iRes;
}
Union Rectangle::getUnion(Rectangle first, Rectangle second) {
    // для получния координат точек прямоугольника нужно взять самую правую верхнюю и
    // самую левую нижнюю точки
    double xMax = fmax(first.getUpperRight().getX(), second.getUpperRight().getX());
    double yMax = fmax(first.getUpperRight().getY(), second.getUpperRight().getY());
    double xMin = fmin(first.getLowerLeft().getX(), second.getLowerLeft().getX());
    double yMin = fmin(first.getLowerLeft().getY(), second.getLowerLeft().getY());
    Dot resUpperRight(xMax, yMax);
    Dot resLowerLeft(xMin, yMin);
    Rectangle res(resLowerLeft, resUpperRight);
    Union uRes(first, second, res);
    return uRes;
}

void helper() {
    std::cout << "description              | command \n";
    std::cout << "-------------------------|---------------------\n";
    std::cout << "command's description    | help \n";
    std::cout << "calculate area           | a [1, 2] \n";
    std::cout << "calculate perimeter      | p [1, 2]\n";
    std::cout << "move rectangle x-axis    | mx [1, 2] <number>\n";
    std::cout << "move rectangle y-axis    | my [1, 2] <number>\n";
    std::cout << "change length            | cl [1, 2] <number> \n";
    std::cout << "change height            | ch [1, 2] <number> \n";
    std::cout << "compare by area          | cmpa \n";
    std::cout << "compare by perimeter     | cmpp \n";
    std::cout << "calculate intersection   | i \n";
    std::cout << "calculate union          | u \n";
    std::cout << "print rectangle          | print [1, 2] \n";
    std::cout << "compare rectangles       | cmp \n";
    std::cout << "postfix increment        | posti [1, 2] \n";
    std::cout << "prefix increment         | prefi [1, 2] \n";
    std::cout << "rectangle to double      | double [1, 2] \n";
    std::cout << "literal for rectangle    | \n";
    std::cout << "(print created rectangle | lit \n";
    std::cout << " ((1, 2.9),(1.07, 0.0)) )| \n";
    std::cout << "end                      | end \n";
}

// главный цикл интерактивной программы
void mainLoop(Rectangle& f, Rectangle& s) {
    std::string cmd;
    int which = 1;
    double num = 0.0;
    while (cmd != "end") {
        std::cin >> cmd;
        // вызываем соответ-ую команду, проверяем ее на валидность
        if (cmd == "help")
            helper();
        else if (cmd == "a") {
            std::cin >> which;
            if (which == 1)
                std::cout << f.getArea() << std::endl;
            else if (which == 2)
                std::cout << s.getArea() << std::endl;
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "p") {
            std::cin >> which;
            if (which == 1)
                std::cout << f.getPerimeter() << std::endl;
            else if (which == 2)
                std::cout << s.getPerimeter() << std::endl;
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "mx") {
            std::cin >> which >> num;
            if (which == 1)
                f.moveByX(num);
            else if (which == 2)
                s.moveByX(num);
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "my") {
            std::cin >> which >> num;
            if (which == 1)
                f.moveByY(num);
            else if (which == 2)
                s.moveByY(num);
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "cl") {
            std::cin >> which >> num;
            if (which == 1)
                f.reduceLength(-num);
            else if (which == 2)
                s.reduceLength(-num);
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "ch") {
            std::cin >> which >> num;
            if (which == 1)
                f.reduceHeight(-num);
            else if (which == 2)
                s.reduceHeight(-num);
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "cmpa") {
            int res = Rectangle::compByArea(f, s);
            if (res == LESS)
                std::cout << "first rectangle's area is less than second" << std::endl;
            else if (res == MORE)
                std::cout << "second rectangle's area is less than first" << std::endl;
            else
                std::cout << "rectangle's areas are equal" << std::endl;
        } else if (cmd == "cmpp") {
            int res = Rectangle::compByPerimeter(f, s);
            if (res == LESS)
                std::cout << "first rectangle's perimeter is less than second" << std::endl;
            else if (res == MORE)
                std::cout << "second rectangle's perimeter is less than first" << std::endl;
            else
                std::cout << "rectangle's perimeters are equal" << std::endl;
        } else if (cmd == "i") {
            Intersection res = Rectangle::getIntersection(f, s);
            if (res.isValid())
                std::cout << res.getRes() << std::endl;
            else
                std::cout << "Intersection is empty!" << std::endl;
        } else if (cmd == "u") {
            Union res = Rectangle::getUnion(f, s);
            std::cout << res.getRes() << std::endl;
        } else if (cmd == "print") {
            std::cin >> which;
            if (which == 1)
                std::cout << f << std::endl;
            else if (which == 2)
                std::cout << s << std::endl;
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "cmp") {
            std::cout << "f < s: " << (f < s) <<
                        ", f > s: " << (f > s) << 
                        ", f == s: " << (f == s) << std::endl;
        } else if (cmd == "prefi") {
            std::cin >> which;
            if (which == 1)
                ++f;
            else if (which == 2)
                ++s;
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "posti") {
            std::cin >> which;
            if (which == 1)
                f++;
            else if (which == 2)
                s++;
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "double") {
            std::cin >> which;
            if (which == 1)
                std::cout << (double)f << std::endl;
            else if (which == 2)
                std::cout << (double)s << std::endl;
            else
                std::cout << "Wrong arguments!" << std::endl;
        } else if (cmd == "lit") {
            std::cout << "((1, 2.9),(1.07, 0.0))"_r << std::endl;
        }
        else if (cmd == "end")
            continue;
        else
            std::cout << "Wrong comand!" << std::endl;
    }
}

int main() {
    // создаем два прямоугольника
    std::cout << "First rectangle: " << std::endl;
    std::cout << "Enter coordinates of lower left dot: " << std::endl;
    Dot fLowerLeft;
    std::cin >> fLowerLeft;
    std::cout << "Enter coordinates of upper right dot: " << std::endl;
    Dot fUpperRight;
    std::cin >> fUpperRight;
    Rectangle f(fLowerLeft, fUpperRight);
    std::cout << "Second rectangle: " << std::endl;
    std::cout << "Enter coordinates of lower left dot: " << std::endl;
    Dot sLowerLeft;
    std::cin >> sLowerLeft;
    std::cout << "Enter coordinates of upper right dot: " << std::endl;
    Dot sUpperRight;
    std::cin >> sUpperRight;
    Rectangle s(sLowerLeft, sUpperRight);
    helper();
    mainLoop(f, s);
    return 0;
}
