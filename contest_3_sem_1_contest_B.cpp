#include <iostream>
#include <vector>

const int MODULE    = 1000003;
const int CNT_JUMPS = 5;

/**
 * @class MatrixExponentiation
 * @brief Класс для выполнения экспоненцирования матриц и для решения дпшки
 */
class MatrixExponentiation {
private:
  std::vector<int> initialSequence; // Начальная последовательность
  std::vector<std::vector<unsigned long long>>
      transformationMatrix; // Основная матрица преобразований
  std::vector<std::vector<unsigned long long>>
      resultMatrix; // Единичная матрица (в итоге результат)
  unsigned long long numberOfSteps; // Количество шагов для вычислений

  /**
   * @brief Инициализация основной матрицы и единичной матрицы
   */
  void initializeMatrices() {
    transformationMatrix = {{1, 1, 1, 1, 1},
                            {1, 0, 0, 0, 0},
                            {0, 1, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 1, 0}};

    resultMatrix         = {{1, 0, 0, 0, 0},
                            {0, 1, 0, 0, 0},
                            {0, 0, 1, 0, 0},
                            {0, 0, 0, 1, 0},
                            {0, 0, 0, 0, 1}};
  }

  /**
   * @brief Умножение двух матриц
   * @param matrixA Первая матрица
   * @param matrixB Вторая матрица
   * @return Результат умножения матриц matrixA и matrixB
   */
  std::vector<std::vector<unsigned long long>> multiplyMatrices(
      const std::vector<std::vector<unsigned long long>> &matrixA,
      const std::vector<std::vector<unsigned long long>> &matrixB) {

    std::vector<std::vector<unsigned long long>> productMatrix(
        CNT_JUMPS, std::vector<unsigned long long>(CNT_JUMPS, 0));

    for (int row = 0; row < CNT_JUMPS; ++row) {
      for (int col = 0; col < CNT_JUMPS; ++col) {
        for (int k = 0; k < CNT_JUMPS; ++k) {
          productMatrix[row][col] =
              (productMatrix[row][col] +
               matrixA[row][k] * matrixB[k][col] % MODULE) %
              MODULE;
        }
      }
    }

    return productMatrix;
  }

  /**
   * @brief Возведение матрицы в степень
   * @param power Степень возведения
   */
  void exponentiateMatrix(unsigned long long power) {
    while (power > 0) {
      if (power % 2 == 1) {
        resultMatrix = multiplyMatrices(resultMatrix, transformationMatrix);
      }
      transformationMatrix =
          multiplyMatrices(transformationMatrix, transformationMatrix);
      power /= 2;
    }
  }

public:
  /**
   * @brief Конструктор для инициализации начальных значений и шагов
   * @param steps Количество шагов для вычисления
   */
  MatrixExponentiation(unsigned long long steps)
      : initialSequence({1, 1, 2, 4, 8}), numberOfSteps(steps - CNT_JUMPS) {
    initializeMatrices();
  }

  /**
   * @brief Основная функция для вычисления результата
   * @return Результат вычисления дпшки
   */
  unsigned long long calculateResult() {
    if (numberOfSteps + CNT_JUMPS <= CNT_JUMPS) {
      return initialSequence[numberOfSteps + CNT_JUMPS - 1];
    }

    exponentiateMatrix(numberOfSteps);

    unsigned long long finalResult = 0;
    for (size_t i = 0; i < CNT_JUMPS; i++) {
      finalResult =
          (finalResult +
           resultMatrix[0][i] * initialSequence[CNT_JUMPS - i - 1] % MODULE) %
          MODULE;
    }

    return finalResult;
  }
};

int main() {
  unsigned long long steps;
  std::cin >> steps;

  if (steps == 0) {
    std::cout << 1 << std::endl;
    return 0;
  }

  MatrixExponentiation solver(steps);
  std::cout << solver.calculateResult() << std::endl;

  return 0;
}
