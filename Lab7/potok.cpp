#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Глобальная переменная для синхронизации вывода
mutex output_mutex;

// Функция для вычисления суммы части массива
double partial_sum(const vector<double>& arr, size_t start, size_t end) 
{
    return accumulate(arr.begin() + start, arr.begin() + end, 0.0);
}

// Функция, выполняемая в потоке, для вычисления частичной суммы
void thread_function(const vector<double>& arr, size_t start, size_t end, double& result) 
{
    result = partial_sum(arr, start, end);
    
    // Синхронизация вывода
    lock_guard<mutex> lock(output_mutex);
    cout << "Идентификатор потока: " << this_thread::get_id() << ", Частичная сумма с " << start << " по " << end << ": " << result << endl;
}

int main() 
{
    const size_t SIZE = 10'000'000; // Размер массива
    vector<double> arr(SIZE, 1.0); // Инициализация массива значениями 1.0

    int mode;
    cout << "Выберите режим (1 - однопоточный, 2 - многопоточный): ";
    cin >> mode;

    if (mode == 1)
    {
        // Однопоточная работа
        auto start_time = high_resolution_clock::now(); // Начало измерения времени
        double total_sum = partial_sum(arr, 0, SIZE); // Вычисление общей суммы
        auto end_time = high_resolution_clock::now(); // Конец измерения времени

        // Выводим результатов
        cout << "Общая сумма: " << total_sum << endl;
        cout << "Время выполнения (один поток): " << duration_cast<microseconds>(end_time - start_time).count() << " микросекунд" << endl;
    } 
    else if (mode == 2) 
    {
        // Многопоточная работа
        size_t num_threads;
        cout << "Введите количество потоков: ";
        cin >> num_threads;

        // Проверка на корректность ввода
        if (num_threads <= 0) 
        {
            cerr << "Количество потоков должно быть положительным." << endl;
            return 1;
        }

        vector<thread> threads(num_threads); // Вектор для хранения потоков
        vector<double> results(num_threads); // Вектор для хранения результатов
        size_t chunk_size = SIZE / num_threads; // Размер блока для каждого потока

        auto start_time = high_resolution_clock::now(); // Начало измерения времени

        // Запускаем потоки
        for (size_t i = 0; i < num_threads; ++i) 
        {
            size_t start = i * chunk_size; // Начало блока
            size_t end = (i == num_threads - 1) ? SIZE : start + chunk_size; // Конец блока
            threads[i] = thread(thread_function, ref(arr), start, end, ref(results[i])); // Создание потока
        }

        // Ожидание завершения всех потоков
        for (auto& thread : threads) 
        {
            thread.join();
        }

        // Суммируем результаты
        double total_sum = accumulate(results.begin(), results.end(), 0.0);
        auto end_time = high_resolution_clock::now(); // Конец измерения времени

        // Выводим результатов
        cout << "Общая сумма: " << total_sum << endl;
        cout << "Время выполнения (многопоточно): " << duration_cast<microseconds>(end_time - start_time).count() << " микросекунд" << endl;
    } 
    else 
    {
        cerr << "Выбран некорректный режим." << endl; // Обработка некорректного ввода
        return 1;
    }

    return 0;
}
