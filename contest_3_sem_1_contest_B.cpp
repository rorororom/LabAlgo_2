#include <iostream>
#include <vector>

/**
 * @class ModArith
 * @brief Класс для модульной арифметики
 */
template <unsigned long long MOD>
class ModArith {
public:
  ModArith() : value(0) {}
  ModArith(unsigned long long v) : value(v % MOD) {}

  ModArith operator+(const ModArith &other) const {
    return ModArith((value + other.value) % MOD);
  }

  ModArith operator*(const ModArith &other) const {
    return ModArith((value * other.value) % MOD);
  }

  ModArith &operator+=(const ModArith &other) {
    value = (value + other.value) % MOD;
    return *this;
  }

  ModArith &operator*=(const ModArith &other) {
    value = (value * other.value) % MOD;
    return *this;
  }

  unsigned long long getValue() const { return value; }

private:
  unsigned long long value;
};

/**
 * @class Matrix
 * @brief Класс для работы с матрицами произвольного типа данных
 */
template <typename T>
class Matrix {
public:
  Matrix(int n) : size(n) {
    matrix.resize(size, std::vector<T>(size, 0));
  }

  static Matrix<T> createIdentityMatrix(int n) {
    Matrix<T> identityMatrix(n);
    for (int i = 0; i < n; ++i) {
      identityMatrix[i][i] = 1;
    }
    return identityMatrix;
  }

  std::vector<T> &operator[](int i) { return matrix[i]; }
  const std::vector<T> &operator[](int i) const { return matrix[i]; }

  // Умножение матриц
  Matrix<T> operator*(const Matrix<T> &other) const {
    Matrix<T> product(size);
    for (int row = 0; row < size; ++row) {
      for (int col = 0; col < size; ++col) {
        for (int k = 0; k < size; ++k) {
          product[row][col] += matrix[row][k] * other[k][col];
        }
      }
    }
    return product;
  }

  // Возведение матрицы в степень
  Matrix<T> exponentiate(unsigned long long power) const {
    Matrix<T> result = Matrix<T>::createIdentityMatrix(size);
    Matrix<T> base = *this;
    while (power > 0) {
      if (power % 2 == 1) {
        result = result * base;
      }
      base = base * base;
      power /= 2;
    }
    return result;
  }

private:
  int size;
  std::vector<std::vector<T>> matrix;
};

/**
 * @class MatrixExponentiationSolver
 * @brief Класс для решения задачи с возведением матрицы в степень
 */
template <typename T>
class MatrixExponentiationSolver {
public:
  MatrixExponentiationSolver(unsigned long long dist, int jumps = 5)
      : distance(dist >= jumps ? dist - jumps : dist), cntJumps(jumps),
        initialSequence({1, 1, 2, 4, 8}), transformationMatrix(jumps) {
    initializeTransformationMatrix();
  }

  unsigned long long calculateResult() {
    if (distance < cntJumps) {
      return initialSequence[distance];
    }

    Matrix<T> resultMatrix = transformationMatrix.exponentiate(distance);

    unsigned long long finalResult = 0;
    for (int i = 0; i < cntJumps; ++i) {
      finalResult += resultMatrix[0][i].getValue() * initialSequence[cntJumps - i - 1];
    }

    return finalResult;
  }

private:
  std::vector<int> initialSequence;
  Matrix<T> transformationMatrix;
  unsigned long long distance;
  int cntJumps;

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

  MatrixExponentiationSolver<ModArith<1000003>> solver(distance);
  std::cout << solver.calculateResult() << std::endl;

  return 0;
}
