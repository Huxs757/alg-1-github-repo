#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

// Структуры данных
struct CalcProblemParams 
{
    string filePath1; // Путь к первому файлу
    string filePath2; // Путь ко второму файлу
    enum class operations { vv_sum, vv_sub, vv_scMalt, mm_sum, mm_sub, mm_mul }; 
    operations op; 
};

// Структура для хранения данных вектора
struct VectorData 
{
    vector<double> values; 
    int size; // Размер вектора
};

// Структура для хранения данных матрицы
struct MatrixData 
{
    map<int, vector<double>> diagonals; // Храним только ненулевые элементы по диагоналям
    vector<vector<double>> fullMatrix;  
    int rows; // Количество строк
    int cols; // Количество столбцов
};

// Функция для логирования данных
int logAll(const string& data) 
{
    ofstream logFile("log.txt", ios::app); 
    if (!logFile) {
        return -1; 
    }
    logFile << data << endl; 
    logFile.close(); 
    return 0; 
}

// Функция для чтения вектора из файла
VectorData readVectorFromFile(const string& filepath) 
{
    VectorData vectorData; // Создаем объект для хранения данных вектора
    ifstream dataFile(filepath); 
    if (!dataFile) 
    {
        logAll("Ошибка открытия файла: " + filepath); 
        return vectorData; 
    }

    string type;
    dataFile >> type; // Читаем тип данных
    if (type == "vector") 
    {
        logAll("В файле обнаружено значение vector"); 
        dataFile >> vectorData.size; // Читаем размер вектора
        vectorData.values.resize(vectorData.size); // Резервируем место для значений
        for (int i = 0; i < vectorData.size; ++i) 
        {
            dataFile >> vectorData.values[i]; 
        }
        stringstream ss;
        for (const auto& value : vectorData.values)
        {
            ss << value << " "; 
        }
        logAll(ss.str()); 
    }
    dataFile.close(); // Закрываем файл
    return vectorData; // Возвращаем данные вектора
}

// Функция для чтения матрицы из файла
MatrixData readMatrixFromFile(const string& filepath)
{
    MatrixData matrixData; // Создаем объект для хранения данных матрицы
    ifstream dataFile(filepath); // Открываем файл для чтения
    if (!dataFile) 
    {
        logAll("Ошибка открытия файла: " + filepath); 
        return matrixData; 
    }

    string type;
    dataFile >> type; 
    if (type == "matrix") 
    {
        logAll("В файле обнаружено значение matrix"); 
        string size;
        dataFile >> size; 
        stringstream sizeStream(size);
        char x;
        sizeStream >> matrixData.rows >> x >> matrixData.cols; // Читаем количество строк и столбцов

        matrixData.fullMatrix.resize(matrixData.rows, vector<double>(matrixData.cols)); // Резервируем место для полной матрицы

        for (int i = 0; i < matrixData.rows; ++i) {
            for (int j = 0; j < matrixData.cols; ++j) 
            {
                double value;
                dataFile >> value; // Читаем значение матрицы
                matrixData.fullMatrix[i][j] = value; // Записываем значение в полную матрицу

                if (value != 0) 
                {
                    int diagonalIndex = j - i; // Индекс диагонали
                  matrixData.diagonals[diagonalIndex].push_back(value); // Сохраняем ненулевое значение в соответствующую диагональ
                }
            }
        }

        stringstream ss;
        ss << "Матрица " << matrixData.rows << "x" << matrixData.cols << ":\n"; // Формируем строку для логирования
        for (const auto& row : matrixData.fullMatrix) 
        {
            for (const auto& value : row) 
            {
                ss << value << " "; // Добавляем значения строки в строку логирования
            }
            ss << "\n"; // Переход на новую строку
        }
        logAll(ss.str()); 
    }
    dataFile.close(); // Закрываем файл
    return matrixData; // Возвращаем данные матрицы
}

// Функция для выполнения операций с векторами
VectorData performVectorOperation(const VectorData& vec1, const VectorData& vec2, CalcProblemParams::operations op) 
{
    VectorData result; // Создаем объект для хранения результата

    switch (op) 
    {
        case CalcProblemParams::operations::vv_sum: // Сложение векторов
            if (vec1.size != vec2.size) 
            {
                logAll("Ошибка! Размерность векторов не совпадает для сложения."); 
                return result; 
            }
            result.size = vec1.size; // Устанавливаем размер результата
            result.values.resize(result.size); // Резервируем место для значений
            for (int i = 0; i < result.size; ++i) 
            {
                result.values[i] = vec1.values[i] + vec2.values[i]; 
            }
            break;

        case CalcProblemParams::operations::vv_sub: // Вычитание векторов
            if (vec1.size != vec2.size) 
            {
                logAll("Ошибка! Размерность векторов не совпадает для вычитания."); 
                return result; 
            }
            result.size = vec1.size; // Устанавливаем размер результата
            result.values.resize(result.size); 
            for (int i = 0; i < result.size; ++i) 
            {
                result.values[i] = vec1.values[i] - vec2.values[i]; 
            }
            break;

        case CalcProblemParams::operations::vv_scMalt: // Скалярное умножение векторов
            if (vec1.size != vec2.size) 
            {
                logAll("Ошибка! Размерность векторов не совпадает для скалярного умножения."); 
                return result; 
            }
            result.size = 1; // Результат будет скаляром
            result.values.resize(1); // Резервируем место для скалярного значения
            result.values[0] = 0; // Инициализируем скаляр
            for (int i = 0; i < vec1.size; ++i) 
            {
                result.values[0] += vec1.values[i] * vec2.values[i]; // Считаем скалярное произведение
            }
            break;

        default:
            logAll("Ошибка: Неизвестная операция."); 
            break;
    }

    return result; // Возвращаем результат операции
}

// Функция для выполнения операций с матрицами
MatrixData performMatrixOperation(const MatrixData& mat1, const MatrixData& mat2, CalcProblemParams::operations op) 
{
    MatrixData result; // Создаем объект для хранения результата

    // Проверка на совместимость размеров для сложения и вычитания
    if ((op == CalcProblemParams::operations::mm_sum || op == CalcProblemParams::operations::mm_sub) &&
        (mat1.rows != mat2.rows || mat1.cols != mat2.cols)) 
    {
        logAll("Ошибка! Размерности матриц не совпадают."); 
        return result; 
    }

    result.rows = mat1.rows; // Устанавливаем количество строк результата
    result.cols = mat1.cols; // Устанавливаем количество столбцов результата

        stringstream logStream; // Создаем поток для логирования
    switch (op) 
    {
        case CalcProblemParams::operations::mm_sum: // Сложение матриц
            logStream << "Результат сложения матриц:\n"; 
            for (int i = 0; i < mat1.rows; ++i) 
            {
                for (int j = 0; j < mat1.cols; ++j) 
                {
                    double sum = mat1.fullMatrix[i][j] + mat2.fullMatrix[i][j]; // Считаем сумму элементов
                    logStream << sum << " "; 
                    if (sum != 0) 
                    {
                        result.diagonals[j - i].push_back(sum); // Сохраняем ненулевое значение в соответствующую диагональ
                    }
                }
                logStream << "\n"; // Переход на новую строку
            }
            break;

        case CalcProblemParams::operations::mm_sub: // Вычитание матриц
            logStream << "Результат вычитания матриц:\n"; 
            for (int i = 0; i < mat1.rows; ++i) 
            {
                for (int j = 0; j < mat1.cols; ++j) 
                {
                    double diff = mat1.fullMatrix[i][j] - mat2.fullMatrix[i][j]; // Считаем разность элементов
                    logStream << diff << " "; 
                    if (diff != 0) 
                    {
                        result.diagonals[j - i].push_back(diff); // Сохраняем ненулевое значение в соответствующую диагональ
                    }
                }
                logStream << "\n"; // Переход на новую строку
            }
            break;

        case CalcProblemParams::operations::mm_mul: // Умножение матриц
            if (mat1.cols != mat2.rows) 
            {
                logAll("Ошибка! Количество столбцов первой матрицы не совпадает с количеством строк второй."); 
                return result; 
            }
            result.rows = mat1.rows; // Устанавливаем количество строк результата
            result.cols = mat2.cols; // Устанавливаем количество столбцов результата
            logStream << "Результат умножения матриц:\n"; 
            result.fullMatrix.resize(result.rows, vector<double>(result.cols, 0)); // Резервируем место для полной матрицы результата

            for (int i = 0; i < mat1.rows; ++i) 
            {
                for (int j = 0; j < mat2.cols; ++j)
                {
                    for (int k = 0; k < mat1.cols; ++k)
                    {
                        result.fullMatrix[i][j] += mat1.fullMatrix[i][k] * mat2.fullMatrix[k][j]; // Считаем произведение
                    }
                    logStream << result.fullMatrix[i][j] << " "; 
                    if (result.fullMatrix[i][j] != 0) 
                    {
                        result.diagonals[j - i].push_back(result.fullMatrix[i][j]); // Сохраняем ненулевое значение в соответствующую диагональ
                    }
                }
                logStream << "\n"; 
            }
            break;

        default:
            logAll("Ошибка: Неизвестная операция с матрицами."); 
            return result; 
    }

    logAll(logStream.str()); 
    return result; 
}

// Основная функция
int main(int argc, char* argv[]) 
{
    VectorData vector1, vector2; // Объекты для хранения векторов
    MatrixData matrix1, matrix2; // Объекты для хранения матриц
    CalcProblemParams calcParams; // Объект для хранения параметров задачи

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) 
    {
        if (string(argv[i]) == "--fp1") 
        {
            if (i + 1 < argc) 
            {
                calcParams.filePath1 = argv[i + 1]; // Сохраняем путь к первому файлу
                vector1 = readVectorFromFile(calcParams.filePath1); // Читаем вектор из первого файла
                matrix1 = readMatrixFromFile(calcParams.filePath1); // Читаем матрицу из первого файла
            }
        }
        if (string(argv[i]) == "--fp2") 
        {
            if (i + 1 < argc) 
            {
                calcParams.filePath2 = argv[i + 1]; // Сохраняем путь ко второму файлу
                vector2 = readVectorFromFile(calcParams.filePath2); // Читаем вектор из второго файла
                matrix2 = readMatrixFromFile(calcParams.filePath2); // Читаем матрицу из второго файла
            }
        }
        if (string(argv[i]) == "--op") // Если аргумент --op
        {
            if (i + 1 < argc) 
            {
                string operation = argv[i + 1]; // Сохраняем операцию
                if (operation == "vv_sum") // Сложение векторов
                {
                    calcParams.op = CalcProblemParams::operations::vv_sum; // Устанавливаем операцию
                    VectorData result = performVectorOperation(vector1, vector2, calcParams.op); // Выполняем операцию
                    if (result.size > 0) 
                    {
                        cout << "Результат сложения: "; // Выводим результат
                        string resultString = "Результат сложения: "; // Формируем строку для логирования
                        for (const auto& value : result.values) 
                        {
                            cout << value << " "; // Выводим значения результата
                            resultString += to_string(value) + " "; // Добавляем значения в строку логирования
                        }
                        cout << endl; 
                        logAll(resultString); 
                    }
                } 
                else if (operation == "vv_sub") // Вычитание векторов
                {
                    calcParams.op = CalcProblemParams::operations::vv_sub; // Устанавливаем операцию
                    VectorData result = performVectorOperation(vector1, vector2, calcParams.op); // Выполняем операцию
                    if (result.size > 0) 
                    {
                        cout << "Результат вычитания: "; // Выводим результат
                        string resultString = "Результат вычитания: "; // Формируем строку для логирования
                        for (const auto& value : result.values) 
                        {
                            cout << value << " "; // Выводим значения результата
                            resultString += to_string(value) + " "; // Добавляем значения в строку логирования
                        }
                        cout << endl; 
                        logAll(resultString); 
                    }
                } 
                else if (operation == "vv_scMalt") // Скалярное умножение векторов
                {
                    calcParams.op = CalcProblemParams::operations::vv_scMalt; 
                    VectorData result = performVectorOperation(vector1, vector2, calcParams.op); // Выполняем операцию
                    if (result.size > 0) 
                    {
                        cout << "Результат скал. умнож: " << result.values[0] << endl; // Выводим результат
                        logAll("Результат скал. умнож: " + to_string(result.values[0])); // Логируем результат
                    }
                } 
                else if (operation == "mm_sum") // Сложение матриц
                {
                    calcParams.op = CalcProblemParams::operations::mm_sum; 
                    MatrixData result = performMatrixOperation(matrix1, matrix2, calcParams.op); 
                } 
                else if (operation == "mm_sub") // Вычитание матриц
                {
                    calcParams.op = CalcProblemParams::operations::mm_sub; 
                    MatrixData result = performMatrixOperation(matrix1, matrix2, calcParams.op); 
                } 
                else if (operation == "mm_mul") // Умножение матриц
                {
                    calcParams.op = CalcProblemParams::operations::mm_mul; 
                    MatrixData result = performMatrixOperation(matrix1, matrix2, calcParams.op); 
                } 
                else 
                {
                    logAll("Ошибка. Неизвестная операция."); 
                }
            }
        }
    }
    return 0; 
}


