#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std; // Добавлено пространство имен std

const double TwoPi = 6.283185307179586;

void FFTAnalysis(double *AVal, double *FTvl, int Nvl, int Nft) 
{
    int i, j, n, m, Mmax, Istp;
    double Tmpr, Tmpi, Wtmp, Theta;
    double Wpr, Wpi, Wr, Wi;
    double *Tmvl;

    n = Nvl * 2; 
    Tmvl = new double[n];

    // Заполнение временного массива
    for (i = 0; i < n; i += 2) 
    {
        Tmvl[i] = 0; // Мнимая часть
        Tmvl[i + 1] = AVal[i / 2]; // Действительная часть
    }
    i = 1; 
    j = 1;
    while (i < n) 
    {
        if (j > i) 
        {
            Tmpr = Tmvl[i]; Tmvl[i] = Tmvl[j]; Tmvl[j] = Tmpr;
            Tmpr = Tmvl[i + 1]; Tmvl[i + 1] = Tmvl[j + 1]; Tmvl[j + 1] = Tmpr;
        }
        i = i + 2; 
        m = Nvl;
        while ((m >= 2) && (j > m)) 
        {
            j = j - m; 
            m = m >> 1;
        }
        j = j + m;
    }

    // Основной цикл FFT
    Mmax = 2;
    while (n > Mmax)
     {
        Theta = -TwoPi / Mmax; 
        Wpi = sin(Theta);
        Wtmp = sin(Theta / 2); 
        Wpr = Wtmp * Wtmp * 2;
        Istp = Mmax * 2; 
        Wr = 1; 
        Wi = 0; 
        m = 1;

        while (m < Mmax) 
        {
            i = m; 
            m = m + 2; 
            Tmpr = Wr; 
            Tmpi = Wi;
            Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
            Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;

            while (i < n) 
            {
                j = i + Mmax;
                Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j - 1];
                Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j - 1];

                Tmvl[j] = Tmvl[i] - Tmpr; 
                Tmvl[j - 1] = Tmvl[i - 1] - Tmpi;
                Tmvl[i] = Tmvl[i] + Tmpr; 
                Tmvl[i - 1] = Tmvl[i - 1] + Tmpi;
                i = i + Istp;
            }
        }

        Mmax = Istp;
    }

    // Вычисление амплитуд
    for (i = 0; i < Nft; i++) 
    {
        j = i * 2; 
        FTvl[i] = 2 * sqrt(pow(Tmvl[j], 2) + pow(Tmvl[j + 1], 2)) / Nvl;
    }

    delete[] Tmvl; // Освобождение памяти
}

int main() 
{
    const int Nvl = 8; // Длина массива (должна быть кратной к двойке)
    double AVal[Nvl] = {0.0, 0.5, 1.0, 0.5, 0.0, -0.5, -1.0, -0.5}; // Входные данные (синусоида)
    double FTvl[Nvl]; // Выходной массив

    // Вызов функции FFT
    FFTAnalysis(AVal, FTvl, Nvl, Nvl);

    // Вывод результатов
    cout << fixed << setprecision(4);
    cout << "Результаты FFT:" << endl;
    for (int i = 0; i < Nvl; i++) 
    {
           cout << "FTvl[" << i << "] = " << FTvl[i] << endl;
    }

    return 0;
}
