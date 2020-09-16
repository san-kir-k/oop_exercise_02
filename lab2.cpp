#include <iostream>
#include <cmath>
#include <string>

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
private:
    double x_;
    double y_;
public:
    Dot(double x = 0.0, double y = 0.0) {
        x_ = x;
        y_ = y;
    }
    double getX() { return x_; }
    double getY() { return y_; }
    void moveByX(double delta) { x_ += delta; }
    void moveByY(double delta) { y_ += delta; }
};

class Rectangle {
private:
    Dot lowerLeft_;
    Dot upperRight_;
public:
    Rectangle(Dot lowerLeft, Dot upperRight) {
        lowerLeft_ = lowerLeft;
        upperRight_ = upperRight;
    }
    Dot getLowerLeft() { return lowerLeft_; };
    Dot getUpperRight() { return upperRight_; };
    double getLength() { return upperRight_.getX() - lowerLeft_.getY(); }
    double getHeight() { return upperRight_.getX() - lowerLeft_.getY(); }
    double getArea() { return getLength() * getHeight(); }
    double getperimeter() { return 2 * (getLength() + getHeight()); }
    // чтобы передвинуть прямоугольник нужно передвинуть соответствующие координаты двух его точек
    void moveByX(double delta) {
        lowerLeft_.moveByX(delta);
        upperRight_.moveByX(delta);
    }
    void moveByY(double delta) {
        lowerLeft_.moveByY(delta);
        upperRight_.moveByY(delta);
    }
    // !при изменении размеров фиксируется левая нижняя точка!
    bool reduceLength(double delta) {
        double len = getLength();
        if (delta > len)
            return false;
        upperRight_.moveByX(-delta);
        return true;
    }
    bool reduceHeight(double delta) {
        double height = getHeight();
        if (delta > height)
            return false;
        upperRight_.moveByY(-delta);
        return true;
    }
    static int compByArea(Rectangle lhs, Rectangle rhs) {
        double lhsArea = lhs.getArea();
        double rhsArea = rhs.getArea();
        if (lhsArea > rhsArea)
            return MORE;
        else if (lhsArea < rhsArea)
            return LESS;
        return EQ;
    }
    static int compByPerimeter(Rectangle lhs, Rectangle rhs) {
        double lhsPer = lhs.getperimeter();
        double rhsPer = rhs.getperimeter();
        if (lhsPer > rhsPer)
            return MORE;
        else if (lhsPer < rhsPer)
            return LESS;
        return EQ;
    }
    static Intersection getIntersection(Rectangle first, Rectangle second);
    static Union getUnion(Rectangle first, Rectangle second);
};

class Intersection {
private:
    const Rectangle& first;
    const Rectangle& second;
    // поле, которое отвечает за то, существует ли пересечение
    bool valid;
    // исколмое пересечение - прямоугольник
    Rectangle res;
public:
    Intersection(const Rectangle& f, const Rectangle& s,
                 Rectangle r, bool v = true) : first(f), second(s), res(r) {
        valid = v;
    }
    const Rectangle& getFirst() { return first; }
    const Rectangle& getSecond() { return second; }
    bool isValid() { return valid; }
    Rectangle getRes() { return res; }
};

class Union {
private:
    const Rectangle& first;
    const Rectangle& second;
    Rectangle res;
public:
    Union(const Rectangle& f, const Rectangle& s,
          Rectangle r, bool v = true) : first(f), second(s), res(r) {}
    const Rectangle& getFirst() { return first; }
    const Rectangle& getSecond() { return second; }
    Rectangle getRes() { return res; }
};

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
    std::cout << "description            | command \n";
    std::cout << "-----------------------|---------------------\n";
    std::cout << "command's description  | help \n";
    std::cout << "calculate area         | a [1, 2] \n";
    std::cout << "calculate perimeter    | p [1, 2]\n";
    std::cout << "move rectangle x-axis  | mx [1, 2] <number>\n";
    std::cout << "move rectangle y-axis  | my [1, 2] <number>\n";
    std::cout << "change length          | cl [1, 2] <number> \n";
    std::cout << "change height          | ch [1, 2] <number> \n";
    std::cout << "compare by area        | cmpa \n";
    std::cout << "compare by perimeter   | cmpp \n";
    std::cout << "calculate intersection | i \n";
    std::cout << "calculate union        | u \n";
    std::cout << "print rectangle        | print [1, 2] \n";
    std::cout << "end                    | end \n";
}

void printRect(Rectangle r) {
    std::cout << "left lower dot: (" << r.getLowerLeft().getX() << ", " <<
    r.getLowerLeft().getY() << ")" << std::endl;
    std::cout << "right upper dot: (" << r.getUpperRight().getX() << ", " <<
    r.getUpperRight().getY() << ")" << std::endl;
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
        }
        else if (cmd == "p") {
            std::cin >> which;
            if (which == 1)
                std::cout << f.getperimeter() << std::endl;
            else if (which == 2)
                std::cout << s.getperimeter() << std::endl;
            else
                std::cout << "Wrong arguments!" << std::endl;
        }
        else if (cmd == "mx") {
            std::cin >> which >> num;
            if (which == 1)
                f.moveByX(num);
            else if (which == 2)
                s.moveByX(num);
            else
                std::cout << "Wrong arguments!" << std::endl;
        }
        else if (cmd == "my") {
            std::cin >> which >> num;
            if (which == 1)
                f.moveByY(num);
            else if (which == 2)
                s.moveByY(num);
            else
                std::cout << "Wrong arguments!" << std::endl;
        }
        else if (cmd == "cl") {
            std::cin >> which >> num;
            if (which == 1)
                f.reduceLength(-num);
            else if (which == 2)
                s.reduceLength(-num);
            else
                std::cout << "Wrong arguments!" << std::endl;
        }
        else if (cmd == "ch") {
            std::cin >> which >> num;
            if (which == 1)
                f.reduceHeight(-num);
            else if (which == 2)
                s.reduceHeight(-num);
            else
                std::cout << "Wrong arguments!" << std::endl;
        }
        else if (cmd == "cmpa") {
            int res = Rectangle::compByArea(f, s);
            if (res == LESS)
                std::cout << "first rectangle's area is less than second" << std::endl;
            else if (res == MORE)
                std::cout << "second rectangle's area is less than first" << std::endl;
            else
                std::cout << "rectangle's areas are equal" << std::endl;
        }
        else if (cmd == "cmpp") {
            int res = Rectangle::compByPerimeter(f, s);
            if (res == LESS)
                std::cout << "first rectangle's perimeter is less than second" << std::endl;
            else if (res == MORE)
                std::cout << "second rectangle's perimeter is less than first" << std::endl;
            else
                std::cout << "rectangle's perimeters are equal" << std::endl;
        }
        else if (cmd == "i") {
            Intersection res = Rectangle::getIntersection(f, s);
            if (res.isValid())
                printRect(res.getRes());
            else
                std::cout << "Intersection is empty!" << std::endl;
        }
        else if (cmd == "u") {
            Union res = Rectangle::getUnion(f, s);
            printRect(res.getRes());
        }
        else if (cmd == "print") {
            std::cin >> which;
            if (which == 1)
                printRect(f);
            else if (which == 2)
                printRect(s);
            else
                std::cout << "Wrong arguments!" << std::endl;
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
    double x, y;
    std::cin >> x >> y;
    Dot fLowerLeft(x, y);
    std::cout << "Enter coordinates of upper right dot: " << std::endl;
    std::cin >> x >> y;
    Dot fUpperRight(x, y);
    Rectangle f(fLowerLeft, fUpperRight);
    std::cout << "Second rectangle: " << std::endl;
    std::cout << "Enter coordinates of lower left dot: " << std::endl;
    std::cin >> x >> y;
    Dot sLowerLeft(x, y);
    std::cout << "Enter coordinates of upper right dot: " << std::endl;
    std::cin >> x >> y;
    Dot sUpperRight(x, y);
    Rectangle s(sLowerLeft, sUpperRight);
    helper();
    mainLoop(f, s);
    return 0;
}
