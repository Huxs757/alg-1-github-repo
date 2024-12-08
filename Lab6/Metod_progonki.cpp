#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip> // Для управления форматированием

using namespace std;

const int mf = 500;
typedef vector<double> Vector;

int main() {
    int i, N;
    double a, lamda, ro, c, h, tau;
    double Tl, T0, Tr, L, t_end, time;
    Vector T(mf + 1), TT(mf + 1);

    // Ввод входных данных
    cout << "Введите количество узлов по пространственной координате, N: ";
    cin >> N;
    cout << "Введите окончание по времени, t_end: ";
    cin >> t_end;
    cout << "Введите толщину пластины, L: ";
    cin >> L;
    cout << "Введите коэффициент теплопроводности материала пластины, lamda: ";
    cin >> lamda;
    cout << "Введите плотность материала пластины, ro: ";
    cin >> ro;
    cout << "Введите теплоемкость материала пластины, c: ";
    cin >> c;
    cout << "Введите начальную температуру, T0: ";
    cin >> T0;
    cout << "Введите температуру на границе x=0, Tl: ";
    cin >> Tl;
    cout << "Введите температуру на границе x=L, Tr: ";
    cin >> Tr;

    // Вычисление коэффициента температуропроводности
    a = lamda / (ro * c);

    // Расчетный шаг по пространству
    h = L / (N - 1);

    // Шаг по времени, удовлетворяющий условию устойчивости
    tau = 0.25 * pow(h, 2) / a;

    // Задаем начальное поле температуры
    for (i = 1; i <= N; ++i) {
        if (i == 1)
            T[i] = Tl;
        else if (i == N)
            T[i] = Tr;
        else
            T[i] = T0;
    }

    // Интегрирование уравнения теплопроводности
    time = 0;
    while (time < t_end) {
        // Увеличиваем время
        time += tau;

        // Запоминаем предыдущее поле температуры
        for (i = 1; i <= N; ++i)
            TT[i] = T[i];

        // Вычисляем новое поле температуры
        for (i = 2; i < N; ++i)
            T[i] = TT[i] + a * tau / pow(h, 2) * (TT[i + 1] - 2.0 * TT[i] + TT[i - 1]);
    }

    // Вывод результата в файл res.txt
    ofstream f("res.txt");
    f << "Толщина пластины L = " << L << endl;
    f << "Число узлов по пространственной координате N = " << N << endl;
    f << "Коэффициент теплопроводности материала пластины lamda = " << lamda << endl;
    f << "Плотность материала пластины ro = " << ro << endl;
    f << "Теплоемкость материала пластины c = " << c << endl;
    f << "Начальная температура T0 = " << T0 << endl;
    f << "Температура на границе x = 0, Tl = " << Tl << endl;
    f << "Температура на границе x = L, Tr = " << Tr << endl;
    f << "Результат получен с шагом по пространственной координате x, h = " << h << endl;
    f << "Результат получен с шагом по времени tau = " << tau << endl;
    f << "Температурное поле в момент времени t = " << t_end << endl;
    f.close();

    // Вывод температурного поля в файл tempr.txt
    ofstream g("tempr.txt");

    // Подписи столбцов
    g << "Шаг температуры" << setw(35) << "Температура узла" << endl;
    g << "---------------" << setw(20) << "-------------" << endl;

    // Данные температурного поля
    for (i = 1; i <= N; ++i)
        g << setw(7) << setprecision(3) << h * (i - 1) << setw(20) << setprecision(3) << T[i] << endl;

    g.close();

    return 0;
}
