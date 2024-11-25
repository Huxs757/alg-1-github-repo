#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

// Структуры данных
struct CalcProblemParams {
    string filePath1;
    string filePath2;
    enum class operations { vv_sum, vv_sub, vv_scMalt, mm_sum, mm_sub, mm_mul };
    operations op;
};

struct VectorData {
    vector<double> values;
    int size;
};

struct MatrixData {
    map<int, vector<double>> diagonals; // Храним только ненулевые элементы
    vector<vector<double>> fullMatrix;  // Для логирования полной матрицы
    int rows;
    int cols;
};

// Логирование
int logAll(const string& data) {
    ofstream logFile("log.txt", ios::app);
    if (!logFile) {
        return -1;
    }
    logFile << data << endl;
    logFile.close();
    return 0;
}

// Чтение вектора из файла
VectorData readVectorFromFile(const string& filepath) {
    VectorData vectorData;
    ifstream dataFile(filepath);
    if (!dataFile) {
        logAll("Ошибка открытия файла: " + filepath);
        return vectorData;
    }

    string type;
    dataFile >> type;
    if (type == "vector") {
        logAll("В файле обнаружено значение vector");
        dataFile >> vectorData.size;
        vectorData.values.resize(vectorData.size);
        for (int i = 0; i < vectorData.size; ++i) {
            dataFile >> vectorData.values[i];
        }
        stringstream ss;
        for (const auto& value : vectorData.values) {
            ss << value << " ";
        }
        logAll(ss.str());
    }
    dataFile.close();
    return vectorData;
}

// Чтение матрицы из файла
MatrixData readMatrixFromFile(const string& filepath) {
    MatrixData matrixData;
    ifstream dataFile(filepath);
    if (!dataFile) {
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
        sizeStream >> matrixData.rows >> x >> matrixData.cols;

        matrixData.fullMatrix.resize(matrixData.rows, vector<double>(matrixData.cols));

        for (int i = 0; i < matrixData.rows; ++i) {
            for (int j = 0; j < matrixData.cols; ++j) {
                double value;
                dataFile >> value;
                matrixData.fullMatrix[i][j] = value;

                if (value != 0) {
                    int diagonalIndex = j - i;
                    matrixData.diagonals[diagonalIndex].push_back(value);
                }
            }
        }

        stringstream ss;
        ss << "Матрица " << matrixData.rows << "x" << matrixData.cols << ":\n";
        for (const auto& row : matrixData.fullMatrix) {
            for (const auto& value : row) {
                ss << value << " ";
            }
            ss << "\n";
        }
        logAll(ss.str());
    }
    dataFile.close();
    return matrixData;
}

// Операции с векторами
VectorData performVectorOperation(const VectorData& vec1, const VectorData& vec2, CalcProblemParams::operations op) {
    VectorData result;

    switch (op) {
        case CalcProblemParams::operations::vv_sum:
            if (vec1.size != vec2.size) {
                logAll("Ошибка! Размерность векторов не совпадает для сложения.");
                return result;
            }
            result.size = vec1.size;
            result.values.resize(result.size);
            for (int i = 0; i < result.size; ++i) {
                result.values[i] = vec1.values[i] + vec2.values[i];
            }
            break;

        case CalcProblemParams::operations::vv_sub:
            if (vec1.size != vec2.size) {
                logAll("Ошибка! Размерность векторов не совпадает для вычитания.");
                return result;
            }
            result.size = vec1.size;
            result.values.resize(result.size);
            for (int i = 0; i < result.size; ++i) {
                result.values[i] = vec1.values[i] - vec2.values[i];
            }
            break;

        case CalcProblemParams::operations::vv_scMalt:
            if (vec1.size != vec2.size) {
                logAll("Ошибка! Размерность векторов не совпадает для скалярного умножения.");
                return result;
            }
            result.size = 1;
            result.values.resize(1);
            result.values[0] = 0;
            for (int i = 0; i < vec1.size; ++i) {
                result.values[0] += vec1.values[i] * vec2.values[i];
            }
            break;

        default:
            logAll("Ошибка: Неизвестная операция с векторами.");
            break;
    }

    return result;
}

// Операции с матрицами
MatrixData performMatrixOperation(const MatrixData& mat1, const MatrixData& mat2, CalcProblemParams::operations op) {
    MatrixData result;

    if ((op == CalcProblemParams::operations::mm_sum || op == CalcProblemParams::operations::mm_sub) &&
        (mat1.rows != mat2.rows || mat1.cols != mat2.cols)) {
        logAll("Ошибка! Размерности матриц не совпадают для сложения или вычитания.");
        return result;
    }

    result.rows = mat1.rows;
    result.cols = mat1.cols;

    stringstream logStream;
    switch (op) {
        case CalcProblemParams::operations::mm_sum:
            logStream << "Результат сложения матриц:\n";
            for (int i = 0; i < mat1.rows; ++i) {
                for (int j = 0; j < mat1.cols; ++j) {
                    double sum = mat1.fullMatrix[i][j] + mat2.fullMatrix[i][j];
                    logStream << sum << " ";
                    if (sum != 0) {
                        result.diagonals[j - i].push_back(sum);
                    }
                }
                logStream << "\n";
            }
            break;

        case CalcProblemParams::operations::mm_sub:
            logStream << "Результат вычитания матриц:\n";
            for (int i = 0; i < mat1.rows; ++i) {
                for (int j = 0; j < mat1.cols; ++j) {
                    double diff = mat1.fullMatrix[i][j] - mat2.fullMatrix[i][j];
                    logStream << diff << " ";
                    if (diff != 0) {
                        result.diagonals[j - i].push_back(diff);
                    }
                }
                logStream << "\n";
            }
            break;

        case CalcProblemParams::operations::mm_mul:
            if (mat1.cols != mat2.rows) {
                logAll("Ошибка! Количество столбцов первой матрицы не совпадает с количеством строк второй.");
                return result;
            }
            result.rows = mat1.rows;
            result.cols = mat2.cols;
            logStream << "Результат умножения матриц:\n";
            result.fullMatrix.resize(result.rows, vector<double>(result.cols, 0));

            for (int i = 0; i < mat1.rows; ++i) {
                for (int j = 0; j < mat2.cols; ++j) {
                    for (int k = 0; k < mat1.cols; ++k) {
                        result.fullMatrix[i][j] += mat1.fullMatrix[i][k] * mat2.fullMatrix[k][j];
                    }
                    logStream << result.fullMatrix[i][j] << " ";
                    if (result.fullMatrix[i][j] != 0) {
                        result.diagonals[j - i].push_back(result.fullMatrix[i][j]);
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
int main(int argc, char* argv[]) {
    VectorData vector1, vector2;
    MatrixData matrix1, matrix2;
    CalcProblemParams calcParams;

    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "--fp1") {
            if (i + 1 < argc) {
                calcParams.filePath1 = argv[i + 1];
                vector1 = readVectorFromFile(calcParams.filePath1);
                matrix1 = readMatrixFromFile(calcParams.filePath1);
            }
        }
        if (string(argv[i]) == "--fp2") {
            if (i + 1 < argc) {
                calcParams.filePath2 = argv[i + 1];
                vector2 = readVectorFromFile(calcParams.filePath2);
                matrix2 = readMatrixFromFile(calcParams.filePath2);
            }
        }
        if (string(argv[i]) == "--op") {
            if (i + 1 < argc) {
                string operation = argv[i + 1];
                if (operation == "vv_sum") {
                    calcParams.op = CalcProblemParams::operations::vv_sum;
                    VectorData result = performVectorOperation(vector1, vector2, calcParams.op);
                    if (result.size > 0) {
                        cout << "Результат сложения: ";
                        string resultString = "Результат сложения: ";
                        for (const auto& value : result.values) {
                            cout << value << " ";
                            resultString += to_string(value) + " ";
                        }
                        cout << std::endl;
                        logAll(resultString);
                    }
                } else if (operation == "vv_sub") {
                    calcParams.op = CalcProblemParams::operations::vv_sub;
                    VectorData result = performVectorOperation(vector1, vector2, calcParams.op);
                    if (result.size > 0) {
                        cout << "Результат вычитания: ";
                        string resultString = "Результат вычитания: ";
                        for (const auto& value : result.values) {
                            cout << value << " ";
                            resultString += to_string(value) + " ";
                        }
                        cout << std::endl;
                        logAll(resultString);
                    }
                } else if (operation == "vv_scMalt") {
                    calcParams.op = CalcProblemParams::operations::vv_scMalt;
                    VectorData result = performVectorOperation(vector1, vector2, calcParams.op);
                    if (result.size > 0) {
                        cout << "Результат скал. умнож: " << result.values[0] << std::endl;
                        logAll("Результат скал. умнож: " + to_string(result.values[0]));
                    }
                } else if (operation == "mm_sum") {
                    calcParams.op = CalcProblemParams::operations::mm_sum;
                    MatrixData result = performMatrixOperation(matrix1, matrix2, calcParams.op);
                    if (result.rows > 0) {
                        cout << "Результат сложения матриц записан в лог.\n";
                    }
                } else if (operation == "mm_sub") {
                    calcParams.op = CalcProblemParams::operations::mm_sub;
                    MatrixData result = performMatrixOperation(matrix1, matrix2, calcParams.op);
                    if (result.rows > 0) {
                        cout << "Результат вычитания матриц записан в лог.\n";
                    }
                } else if (operation == "mm_mul") {
                    calcParams.op = CalcProblemParams::operations::mm_mul;
                    MatrixData result = performMatrixOperation(matrix1, matrix2, calcParams.op);
                    if (result.rows > 0) {
                        cout << "Результат умножения матриц записан в лог.\n";
                    }
                } else {
                    logAll("Ошибка. Неизвестная операция.");
                }
            }
        }
    }
    return 0;
}
