#include <iostream>
#include <string>
#include <vector>
#include <tuple>

/**
 * @class LcsCalculator
 * @brief Класс для вычисления наибольшей общей подпоследовательности (LCS)
 */
class LcsCalculator {
public:
  /**
   * @brief Конструктор класса
   * @param s1 Первая строка
   * @param s2 Вторая строка
   */
  LcsCalculator(const std::string &s1, const std::string &s2)
      : str1(s1), str2(s2) {
    lcsTable.resize(str1.size() + 1, std::vector<int>(str2.size() + 1, 0));
  }

  /**
   * @brief Метод для вычисления и восстановления наибольшей общей подпоследовательности
   * @return Кортеж, содержащий длину LCS и индексы символов, входящих в LCS, для обеих строк
   */
  std::tuple<int, std::vector<int>, std::vector<int>> calculate() {
    computeLcsTable();
    auto [str1Indexes, str2Indexes] = reconstructLcs();

    int lcsLength = lcsTable[str1.size()][str2.size()];
    return {lcsLength, str1Indexes, str2Indexes};
  }

private:
  /**
   * @brief Вычисление таблицы LCS
   */
  void computeLcsTable() {
    for (size_t i = 1; i <= str1.size(); ++i) {
      for (size_t j = 1; j <= str2.size(); ++j) {
        if (str1[i - 1] == str2[j - 1]) {
          lcsTable[i][j] = lcsTable[i - 1][j - 1] + 1;
        } else {
          lcsTable[i][j] = std::max(lcsTable[i - 1][j], lcsTable[i][j - 1]);
        }
      }
    }
  }

  /**
   * @brief Восстановление индексов символов LCS для обеих строк
   * @return Массивы индексов для первой и второй строки
   */
  std::pair<std::vector<int>, std::vector<int>> reconstructLcs() {
    std::vector<int> str1Indexes, str2Indexes;
    size_t i = str1.size(), j = str2.size();

    while (i > 0 && j > 0) {
      if (str1[i - 1] == str2[j - 1]) {
        str1Indexes.push_back(i);
        str2Indexes.push_back(j);
        --i;
        --j;
      } else if (lcsTable[i - 1][j] > lcsTable[i][j - 1]) {
        --i;
      } else {
        --j;
      }
    }

    return {str1Indexes, str2Indexes};
  }

  // Приватные поля класса
  std::string str1, str2;
  std::vector<std::vector<int>> lcsTable;
};

/**
 * @brief Функция для вывода результатов LCS
 * @param length Длина наибольшей общей подпоследовательности
 * @param str1Indexes Индексы символов LCS в первой строке
 * @param str2Indexes Индексы символов LCS во второй строке
 */
void printLcsResult(int length, const std::vector<int>& str1Indexes, const std::vector<int>& str2Indexes) {
  std::cout << length << std::endl;

  for (int i = str1Indexes.size() - 1; i >= 0; --i) {
    std::cout << str1Indexes[i] << " ";
  }
  std::cout << std::endl;

  for (int i = str2Indexes.size() - 1; i >= 0; --i) {
    std::cout << str2Indexes[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::string str1, str2;
  std::cin >> str1 >> str2;

  LcsCalculator lcsSolver(str1, str2);

  auto [length, str1Indexes, str2Indexes] = lcsSolver.calculate();
  printLcsResult(length, str1Indexes, str2Indexes);

  return 0;
}
