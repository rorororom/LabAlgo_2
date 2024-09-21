#include <iostream>
#include <vector>

/**
 * @class AlternatingSequenceCalculator
 * @brief Класс для вычисления самой длинной чередующейся подпоследовательности
 */
class AlternatingSequenceCalculator {
public:
  /**
   * @brief Конструктор класса
   * @param seq Входная последовательность
   */
  AlternatingSequenceCalculator(const std::vector<int>& seq)
      : elementCount(seq.size()), sequence(seq), increasingLength(seq.size(), INCREASE),
        decreasingLength(seq.size(), INCREASE), increasingPrevIndex(seq.size(), DECREASE),
        decreasingPrevIndex(seq.size(), DECREASE), transitions(seq.size(), 0) {}

  /**
   * @brief Метод для вычисления длины самой большой чередующейся подпоследовательности
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
   * @brief Метод для получения самой длинной чередующейся подпоследовательности
   * @return Пара: длина и сама последовательность
   */
  std::pair<int, std::vector<int> > getLongestSequence() {
    int lastIndex = DECREASE;
    int maxLength = 0;

    for (int i = 0; i < elementCount; i++) {
      int currentLength = std::max(increasingLength[i], decreasingLength[i]);
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

    std::reverse(longestSequence.begin(), longestSequence.end());
    return {maxLength, longestSequence};
  }

private:
  const int DECREASE = -1; // Константы для обозначения переходов
  const int INCREASE = 1;

  int elementCount; // Количество элементов в последовательности
  std::vector<int> sequence; // Входная последовательность
  std::vector<int> increasingLength; // Длины возрастающих подпоследовательностей
  std::vector<int> decreasingLength; // Длины убывающих подпоследовательностей
  std::vector<int> increasingPrevIndex; // Индексы предыдущих элементов возрастающей последовательности
  std::vector<int> decreasingPrevIndex; // Индексы предыдущих элементов убывающей последовательности
  std::vector<int> transitions; // Переходы: INCREASE или DECREASE
};

//------------------------------------------------------------------//
//------------------------------------------------------------------//

/**
 * @brief Функция для ввода последовательности
 */
std::vector<int> inputSequence(int count) {
  std::vector<int> sequence(count);
  for (int i = 0; i < count; ++i) {
    std::cin >> sequence[i];
  }
  return sequence;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//

/**
 * @brief Функция для вывода результата
 */
void printSequence(const std::pair<int, std::vector<int> >& result) {
  std::cout << result.first << std::endl;
  for (int num : result.second) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//

int main() {
  int elementCount;
  std::cin >> elementCount;

  std::vector<int> sequence = inputSequence(elementCount);

  AlternatingSequenceCalculator sequenceTask(sequence);
  sequenceTask.calculateLongestSequences();
  auto subsequence = sequenceTask.getLongestSequence();

  printSequence(subsequence);

  return 0;
}
