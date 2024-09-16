#include <iostream>
#include <vector>

// Константы для обозначения переходов между убывающей и возрастающей
// последовательностями
const int DECREASE = -1;
const int INCREASE = 1;

/// Макрос для вычисления максимального значения между двумя аргументами
#define MAX(a, b) ((a) >= (b)) ? (a) : (b)

/**
 * @class AlternatingSequence
 * @brief Класс для работы с чередующейся последовательностью
 */
class AlternatingSequence {
private:
  int elementCount; // Количество элементов в последовательности
  std::vector<int> sequence; // Входная последовательность
  std::vector<int>
      increasingLength; // Длины возрастающих подпоследовательностей
  std::vector<int> decreasingLength; // Длины убывающих подпоследовательностей
  std::vector<int> increasingPrevIndex; // Индексы предыдущих элементов
                                        // возрастающей последовательности
  std::vector<int> decreasingPrevIndex; // Индексы предыдущих элементов
                                        // убывающей последовательности
  std::vector<int> transitions; // Переходы: INCREASE или DECREASE

public:
  /**
   * @brief Конструктор класса.
   * @param n Количество элементов в последовательности.
   */
  AlternatingSequence(int n)
      : elementCount(n), sequence(n, 0), increasingLength(n, INCREASE),
        decreasingLength(n, INCREASE), increasingPrevIndex(n, DECREASE),
        decreasingPrevIndex(n, DECREASE), transitions(n, 0) {}

  /**
   * @brief Метод для ввода последовательности чисел
   */
  void inputSequence() {
    for (int i = 0; i < elementCount; i++) {
      std::cin >> sequence[i];
    }
  }

  /**
   * @brief Метод для вычисления длин чередующихся подпоследовательностей
   */
  void calculateLongestSequences() {
    for (int i = 1; i < elementCount; i++) {
      for (int j = 0; j < i; j++) {
        if (sequence[i] > sequence[j] &&
            increasingLength[i] < decreasingLength[j] + 1) {
          increasingLength[i] = decreasingLength[j] + 1;
          increasingPrevIndex[i] = j;
          transitions[i] = INCREASE;
        } else if (sequence[i] < sequence[j] &&
                   decreasingLength[i] < increasingLength[j] + 1) {
          decreasingLength[i] = increasingLength[j] + 1;
          decreasingPrevIndex[i] = j;
          transitions[i] = DECREASE;
        }
      }
    }
  }

  /**
   * @brief Метод для нахождения и вывода самой длинной чередующейся
   * последовательности.
   */
  void findAndPrintLongestSequence() {
    int lastIndex = DECREASE;
    int maxLength = 0;

    for (int i = 0; i < elementCount; i++) {
      int currentLength = MAX(increasingLength[i], decreasingLength[i]);
      if (maxLength < currentLength) {
        maxLength = currentLength;
        lastIndex = i;
      }
    }

    std::vector<int> longestSequence;
    while (lastIndex != DECREASE) {
      longestSequence.push_back(sequence[lastIndex]);
      if (transitions[lastIndex] == INCREASE) {
        lastIndex = increasingPrevIndex[lastIndex];
      } else {
        lastIndex = decreasingPrevIndex[lastIndex];
      }
    }

    std::cout << maxLength << std::endl;
    for (int i = maxLength - 1; i >= 0; i--) {
      std::cout << longestSequence[i] << " ";
    }
    std::cout << std::endl;
  }
};

/**
 * @brief Точка входа в программу. Выполняет ввод данных и запускает расчет
 * чередующихся последовательностей.
 * @return Код завершения программы.
 */
int main() {
  int elementCount;

  std::cin >> elementCount;
  AlternatingSequence sequenceTask(elementCount);

  sequenceTask.inputSequence();
  sequenceTask.calculateLongestSequences();
  sequenceTask.findAndPrintLongestSequence();

  return 0;
}
