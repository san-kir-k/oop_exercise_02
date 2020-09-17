#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

enum class Comp {
    Less,
    More, 
    Eq
};

class Complex {
public:
    // конструкторы
    Complex();
    Complex(const double& r, const double& i);
    Complex(const Complex& val);
    // методы
    static Complex add(const Complex& lhs, const Complex& rhs);
    static Complex sub(const Complex& lhs, const Complex& rhs);
    static Complex mul(const Complex& lhs, const Complex& rhs);
    static Complex div(const Complex& lhs, const Complex& rhs);
    static bool equ(const Complex& lhs, const Complex& rhs);
    static Complex conj(const Complex& val);
    // метод сравнения по модулю
    static Comp moduleComp(const Complex& lhs, const Complex& rhs);
    double getReal() const;
    double getImag() const;
private:
    double real_;
    double imaginary_;
};

Complex::Complex() {
    real_ = 0.0;
    imaginary_ = 0.0;
}
Complex::Complex(const double& r, const double& i) {
    real_ = r;
    imaginary_ = i;
}
// конструктор копирования
Complex::Complex(const Complex& val) {
    real_ = val.real_;
    imaginary_ = val.imaginary_;
}
// Методы - реализация
Complex Complex::add(const Complex& lhs, const Complex& rhs) {
    return Complex(lhs.getReal() + rhs.getReal(), lhs.getImag() + rhs.getImag());
}
Complex Complex::sub(const Complex& lhs, const Complex& rhs) {
    return Complex(lhs.getReal() - rhs.getReal(), lhs.getImag() - rhs.getImag());
}
Complex Complex::mul(const Complex& lhs, const Complex& rhs) {
    return Complex(lhs.getReal() * rhs.getReal() - lhs.getImag() * rhs.getImag(),
                lhs.getReal() * rhs.getImag() + lhs.getImag() * rhs.getReal());
}
Complex Complex::div(const Complex& lhs, const Complex& rhs) {
    double d = rhs.getReal() * rhs.getReal() + rhs.getImag() * rhs.getImag();
    assert(d != 0.0);
    return Complex((lhs.getReal() * rhs.getReal() + lhs.getImag() * rhs.getImag()) / d,
                (lhs.getImag() * rhs.getReal() - lhs.getReal() * rhs.getImag()) / d);
}
bool Complex::equ(const Complex& lhs, const Complex& rhs) {
    return (lhs.getReal() == rhs.getReal()) && (lhs.getImag() == rhs.getImag());
}
Complex Complex::conj(const Complex& val) {
    return Complex(val.real_, -val.imaginary_);
}
Comp Complex::moduleComp(const Complex& lhs, const Complex& rhs) {
    double mL, mR;
    mL = sqrt(lhs.getReal() * lhs.getReal() + lhs.getImag() * lhs.getImag()); 
    mR = sqrt(rhs.getReal() * rhs.getReal() + rhs.getImag() * rhs.getImag()); 
    if (mL == mR)
        return Comp::Eq;
    else {
        return (mL > mR ? Comp::More : Comp::Less);
    } 
}
double Complex::getReal() const { return real_; }
double Complex::getImag() const { return imaginary_; }

// Перегрузка операторов
Complex operator+(const Complex& lhs, const Complex& rhs) {
    return Complex::add(lhs, rhs);
}
Complex operator-(const Complex& lhs, const Complex& rhs) {
    return Complex::sub(lhs, rhs);
}
Complex operator*(const Complex& lhs, const Complex& rhs) {
    return Complex::mul(lhs, rhs);
}
Complex operator/(const Complex& lhs, const Complex& rhs) {
    return Complex::div(lhs, rhs);
}
bool operator==(const Complex& lhs, const Complex& rhs) {
    return Complex::equ(lhs, rhs);
}
// перегрузим оператор << и >> для более удобного вывода/ввода прямоугольника в поток
std::ostream& operator<<(std::ostream& stream, const Complex& v) {
    if (v.getImag() < 0)
        stream << v.getReal() << " - " << abs(v.getImag()) << "i";
    else
        stream << v.getReal() << " + " << v.getImag() << "i";
    return stream;
}
std::istream& operator>>(std::istream& stream, Complex& v) {
    std::string r, i, plus;
    getline(stream, r, ' ');
    getline(stream, plus, ' ');
    getline(stream, i, 'i');
    std::stringstream sr(r);
    std::stringstream si(i);
    double real, imag;
    sr >> real;
    si >> imag;
    if (plus == "-")
        imag *= -1.0;
    v = Complex{real, imag};
    return stream;
}

// Пользовательский литерал
Complex operator "" _c(const char* str, long unsigned int size) {
    std::stringstream ss(str);
    Complex res;
    ss >> res;
    return res;
}

// Вспомогательные функции
void helper() {
    std::cout << "description              | command \n";
    std::cout << "-------------------------|---------------------\n";
    std::cout << "сложить два числа        | add  \n";
    std::cout << "вычисть из левого правое | sub \n";
    std::cout << "умножить два числа       | mul \n";
    std::cout << "поделить левое на правое | div \n";
    std::cout << "сравнить два числа       | equ \n";
    std::cout << "получить сопряженное     | conj [1, 2] \n";
    std::cout << "сравнить числа по модулю | module \n";
    std::cout << "пользовательский литерал | lit \n";
    std::cout << "(      12.3 + 6.78i     )| \n";
    std::cout << "вывести число на экран   | print [1, 2] \n";
    std::cout << "end                      | end \n";
}

// главный цикл интерактивной программы
void mainLoop(Complex& f, Complex& s) {
    std::string cmd;
    int which = 1;
    while (cmd != "end") {
        std::cin >> cmd;
        // вызываем соответ-ую команду, проверяем ее на валидность
        if (cmd == "help")
            helper();
        else if (cmd == "add") {
            Complex tmp = f + s;
            std::cout << tmp << std::endl;
        } else if (cmd == "sub") {
            Complex tmp = f - s;
            std::cout << tmp << std::endl;
        } else if (cmd == "mul") {
            Complex tmp = f * s;
            std::cout << tmp << std::endl;
        } else if (cmd == "div") {
            Complex tmp = f / s;
            std::cout << tmp << std::endl;
        } else if (cmd == "equ") {
            std::cout << (f == s) << std::endl;
        } else if (cmd == "conj") {
            std::cin >> which;
            if (which == 1) {
                std::cout << Complex::conj(f) << std::endl;
            } else if (which == 2) {
                std::cout << Complex::conj(s) << std::endl;
            } else {
                std::cout << "Wrong args!" << std::endl;
            }
        } else if (cmd == "module") {
            Comp res = Complex::moduleComp(f, s);
            if (res == Comp::Eq)
                std::cout << "Eq" << std::endl;
            else if (res == Comp::Less)
                std::cout << "First number is less than second" << std::endl;
            else
                std::cout << "Second number is less than first" << std::endl;
        } else if (cmd == "lit") {
            std::cout << "12.3 + 6.78i"_c << std::endl; 
        } else if (cmd == "print") {
            std::cin >> which;
            if (which == 1) {
                std::cout << f << std::endl;
            } else if (which == 2) {
                std::cout << s << std::endl;
            } else {
                std::cout << "Wrong args!" << std::endl;
            }
        } else if (cmd == "end")
            continue;
        else
            std::cout << "Wrong comand!" << std::endl;
    }
}

int main() {
    // создаем два числа
    Complex f, s;
    std::cout << "First complex (x + yi): " << std::endl;
    std::cin >> f;
    std::cout << "Second complex: " << std::endl;
    std::cin >> s;
    helper();
    // запускаем цикл
    mainLoop(f, s);
    return 0;
}
