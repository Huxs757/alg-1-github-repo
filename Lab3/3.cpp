#include <iostream> 
#include <fstream> // Для работы с файлами. Комбинация ofstream и ifstream: создание, чтение и запись в файлы.
#include <string> // для std::string
#include <sstream>
#include <vector> // Для работы с векторами

// Подключаем пространство имен std 
using namespace std; 

struct CalcProblemParams
{
   string filePath1;
   string filePath2;
   enum class operations {vv_sum, vv_sub, vv_scMalt};
   operations op;
};

// Структура для хранения значений векторов и их размерностей:
struct VectorData 
{
    vector<double> values;
    int size;
};

int logAll(const string& data)
{
    ofstream logFile("log.txt", ios::app); // Открываем файл в режиме добавления
    if (!logFile) 
    {
        return -1;
    }
    logFile << data << std::endl; // Записываю данные в файл
    logFile.close(); // Закрываю файл
    return 0; 
}

// Функция для чтения из файлов и проверки значений
VectorData readDataFromFile(const string& filepath)
 {
    VectorData vectorData;
    ifstream dataFile(filepath); // Открываем файл в режиме чтения
    if (!dataFile) 
    {
        logAll("Ошибка открытия файла: " + filepath);
        return vectorData; // Возвращаем пустой вектор
    }

    string type;
    dataFile >> type; // Читаем тип вектора
    if (type == "vector")
    {
        logAll("В файле обнаружено значение vector");
        // Читаем размер вектора
        dataFile >> vectorData.size;
        vectorData.values.resize(vectorData.size);
        logAll(to_string(vectorData.size));
        // Читаем значения вектора
        for (int i = 0; i < vectorData.size; ++i)
        {
            dataFile >> vectorData.values[i];
        }
        // Логируем загруженные значения вектора в одной строке
        stringstream ss;
        for (const auto& value : vectorData.values) 
        {
            ss << value << " "; // Добавляем значение и пробел
        }
        logAll(ss.str()); // Записываем всю строку в log.txt
    }
    dataFile.close(); // Закрываем файл
    return vectorData; 
}

// Функция для сложения двух векторов
VectorData performVectorOperation(const VectorData& vec1, const VectorData& vec2, CalcProblemParams::operations op) 
{
    VectorData result;

    switch (op) 
    {
        case CalcProblemParams::operations::vv_sum:
            if (vec1.size != vec2.size) 
            {
                logAll("Ошибка! Размерность векторов не совпадает для сложения");
                return VectorData(); // Возвращаем пустой вектор в случае ошибки
            }
            result.size = vec1.size;
            result.values.resize(result.size);
            for (int i = 0; i < result.size; ++i) 
            {
                result.values[i] = vec1.values[i] + vec2.values[i];
            }
            break;

        case CalcProblemParams::operations::vv_sub:
            // Проверка на равенство размерностей
            if (vec1.size != vec2.size) 
            {
                logAll("Ошибка! Размерность векторов не совпадает для вычитания");
                return VectorData(); // Возвращаем пустой вектор в случае ошибки
            }
            result.size = vec1.size;
            result.values.resize(result.size);
            // Вычитание векторов
            for (int i = 0; i < result.size; ++i) 
            {
                result.values[i] = vec1.values[i] - vec2.values[i];
            }
            break;

        case CalcProblemParams::operations::vv_scMalt:
            // Проверка на равенство размерностей
            if (vec1.size != vec2.size) 
            {
                logAll("Ошибка! Размерность векторов не совпадает для скалярного умножения");
                return VectorData(); // Возвращаем пустой вектор в случае ошибки
            }
            result.size = 1; // Скалярное произведение - это одно значение
            result.values.resize(result.size);
            result.values[0] = 0; // Инициализируем скалярное произведение
            // Скалярное умножение векторов
            for (int i = 0; i < vec1.size; ++i) 
            {
                result.values[0] += vec1.values[i] * vec2.values[i];
            }
            break;

        default:
            logAll("Ошибка: Неизвестная операция");
            break;
    }

    return result;
}
 
int main(int argc, char* argv[]) 
{ 
    VectorData vector1, vector2;
    CalcProblemParams calcParams;
    for (int i = 1; i < argc; i++) 
    {
        if (string(argv[i]) == "--fp1") 
        {
            if (i + 1 < argc) 
            { // Проверка границ
                calcParams.filePath1 = argv[i + 1];
                logAll("File Path 1: " + calcParams.filePath1);
                vector1 = readDataFromFile(calcParams.filePath1);
            }
        }
        if (string(argv[i]) == "--fp2") 
        {
            if (i + 1 < argc) 
            { // Проверка границ
                calcParams.filePath2 = argv[i + 1];
                logAll("File Path 2: " + calcParams.filePath2);
                vector2 = readDataFromFile(calcParams.filePath2);
            }
        }
        if (string(argv[i]) == "--op") 
        {
            if (i + 1 < argc) 
            {
                string operation = argv[i + 1];
                if (operation == "vv_sum") 
{
    calcParams.op = CalcProblemParams::operations::vv_sum;
    VectorData result = performVectorOperation(vector1, vector2, calcParams.op);
    if (result.size > 0) {
        cout << "Результат суммирования: ";
        string resultString = "Результат суммирования: ";
        for (const auto& value : result.values)
        {
            cout << value << " ";
            resultString += to_string(value) + " "; // Формируем строку для логирования
        }
        cout << std::endl;
        logAll(resultString); // Записываем результат в log.txt
    }
}    
      else if (operation == "vv_sub") 
      {
        calcParams.op = CalcProblemParams::operations::vv_sub;
        VectorData result = performVectorOperation(vector1, vector2, calcParams.op);
        if (result.size > 0) {
            cout << "Результат вычитания: ";
            string resultString = "Результат вычитания: ";
            for (const auto& value : result.values)
            {
                cout << value << " ";
                resultString += to_string(value) + " "; // Формируем строку для логирования
            }
            cout << std::endl;
            logAll(resultString); // Записываем результат в log.txt
    }
} 
        else if (operation == "vv_scMalt") 
        {
            calcParams.op = CalcProblemParams::operations::vv_scMalt;
            VectorData result = performVectorOperation(vector1, vector2, calcParams.op);
            if (result.size > 0) 
            {
                 cout << "Результат скал. умнож: " << result.values[0] << std::endl;
                 logAll("Результат скал. умнож: " + to_string(result.values[0])); // Записываем результат в log.txt
            }
}
        else 
                {
                    logAll("Ошибка. Неизвестная операция");
                }
            } 
            else 
            {
                logAll("Ошибка. Не указана операция");
            }
        }
    }
    return 0;
} 
