#include <iostream>
#include <vector>
#include <cstdint>  

/**
 * @class ModArith
 * @brief Класс для модульной арифметики
 */
template <uint32_t MOD>
class ModArith {
public:
  ModArith() : value(0) {}
  ModArith(uint64_t v) : value(v % MOD) {}

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

  uint32_t getValue() const { return value; }

private:
  uint32_t value;
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

  Matrix<T> exponentiate(uint64_t power) const {
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
 * @class WayCntCalculator
 * @brief Класс для вычисления количества способов при помощи матричного возведения в степень
 */
template <typename T>
class WayCntCalculator {
public:
  WayCntCalculator(uint64_t dist, int jumps = 5)
      : distance(dist >= jumps ? dist - jumps : dist), cntJumps(jumps),
        initialSequence({1, 1, 2, 4, 8}), transformationMatrix(jumps) {
    initializeTransformationMatrix();
  }

  uint64_t calculateResult() {
    if (distance < cntJumps) {
      return initialSequence[distance];
    }

    Matrix<T> resultMatrix = transformationMatrix.exponentiate(distance);

    uint64_t finalResult = 0;
    for (int i = 0; i < cntJumps; ++i) {
      finalResult += resultMatrix[0][i].getValue() * initialSequence[cntJumps - i - 1];
      finalResult %= MOD;
    }

    return finalResult;
  }

private:
  std::vector<int> initialSequence;
  Matrix<T> transformationMatrix;
  uint64_t distance;
  int cntJumps;

  static const uint32_t MOD = 1000003; 

  void initializeTransformationMatrix() {
    transformationMatrix[0] = {1, 1, 1, 1, 1};
    transformationMatrix[1] = {1, 0, 0, 0, 0};
    transformationMatrix[2] = {0, 1, 0, 0, 0};
    transformationMatrix[3] = {0, 0, 1, 0, 0};
    transformationMatrix[4] = {0, 0, 0, 1, 0};
  }
};

int main() {
  uint64_t distance;
  std::cin >> distance;

  WayCntCalculator<ModArith<1000003>> solver(distance);
  std::cout << solver.calculateResult() << std::endl;

  return 0;
}
