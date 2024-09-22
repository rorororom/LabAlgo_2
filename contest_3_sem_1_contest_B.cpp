#include <iostream>
#include <vector>

/**
 * @class MatrixOperations
 * @brief Класс для работы с матрицами - умножение и возведение в степень
 */
class MatrixOperations {
private:
  int size;
  unsigned long long mod;
  std::vector<std::vector<unsigned long long>> matrix;

public:
  MatrixOperations(int n, unsigned long long module, bool identity = false)
      : size(n), mod(module) {
    matrix.resize(size, std::vector<unsigned long long>(size, 0));
    if (identity) {
      for (int i = 0; i < size; ++i) {
        matrix[i][i] = 1;
      }
    }
  }

  std::vector<unsigned long long> &operator[](int i) { return matrix[i]; }

  const std::vector<unsigned long long> &operator[](int i) const {
    return matrix[i];
  }

  // Умножение матриц
  MatrixOperations operator*(const MatrixOperations &other) const {
    MatrixOperations product(size, mod);
    for (int row = 0; row < size; ++row) {
      for (int col = 0; col < size; ++col) {
        for (int k = 0; k < size; ++k) {
          product[row][col] =
              (product[row][col] + matrix[row][k] * other[k][col]) % mod;
        }
      }
    }
    return product;
  }

  // Возведение матрицы в степень
  MatrixOperations exponentiate(unsigned long long power) const {
    MatrixOperations result(size, mod, true); // Единичная матрица
    MatrixOperations base = *this;
    while (power > 0) {
      if (power % 2 == 1) {
        result = result * base;
      }
      base = base * base;
      power /= 2;
    }
    return result;
  }
};

/**
 * @class MatrixSolver
 * @brief Класс для решения задачи, находит ответ
 */
class MatrixSolver {
public:
  /**
   * @brief Конструктор с параметрами
   * @param dist Дистанция
   * @param jumps Количество состояний
   * @param mod Модуль для вычислений
   */
  MatrixSolver(unsigned long long dist, int jumps = 5,
               unsigned long long mod = 1000003)
      : distance(dist >= jumps ? dist - jumps : dist), cntJumps(jumps),
        module(mod), initialSequence({1, 1, 2, 4, 8}),
        transformationMatrix(jumps, mod) {
    initializeTransformationMatrix();
  }

public:
  /**
   * @brief Основная функция для вычисления результата
   * @return Результат вычисления задачи
   */
  unsigned long long calculateResult() {
    if (distance < cntJumps) {
      return initialSequence[distance];
    }

    MatrixOperations resultMatrix = transformationMatrix.exponentiate(distance);

    unsigned long long finalResult = 0;
    for (int i = 0; i < cntJumps; ++i) {
      finalResult = (finalResult +
                     resultMatrix[0][i] * initialSequence[cntJumps - i - 1]) %
                    module;
    }

    return finalResult;
  }

private:
  std::vector<int> initialSequence;
  MatrixOperations transformationMatrix;
  unsigned long long distance;
  int cntJumps;
  unsigned long long module;

    /**
   * @brief Инициализация матрицы преобразований
   */
  void initializeTransformationMatrix() {
    transformationMatrix[0] = {1, 1, 1, 1, 1};
    transformationMatrix[1] = {1, 0, 0, 0, 0};
    transformationMatrix[2] = {0, 1, 0, 0, 0};
    transformationMatrix[3] = {0, 0, 1, 0, 0};
    transformationMatrix[4] = {0, 0, 0, 1, 0};
  }

};

int main() {
  unsigned long long distance;
  std::cin >> distance;

  MatrixSolver solver(distance);
  std::cout << solver.calculateResult() << std::endl;

  return 0;
}
