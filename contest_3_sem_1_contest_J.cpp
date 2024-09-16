#include <iostream>
#include <string>
#include <vector>

/**
 * @class LCS
 * @brief Класс для решения задачи поиска наибольшей общей подпоследовательности
 */
class LCS {
private:
  std::string str1, str2; // Две строки для сравнения
  std::vector<std::vector<int>> dp; // Таблица дп
  std::vector<int> str1Indexes; // Индексы символов строки 1
  std::vector<int> str2Indexes; // Индексы символов строки 2
  int n, m;                     // Длины строк

public:
  /**
   * @brief Конструктор класса
   * @param s1 Первая строка
   * @param s2 Вторая строка
   */
  LCS(const std::string &s1, const std::string &s2)
      : str1(s1), str2(s2), n(s1.length()), m(s2.length()) {
    dp.resize(n + 1, std::vector<int>(m + 1, 0));
  }

  /**
   * @brief Метод для вычисления длины наибольшей общей подпоследовательности и
   * заполнения таблицы дп
   */
  void computeLCS() {
    for (int i = 1; i <= n; ++i) {
      for (int j = 1; j <= m; ++j) {
        if (str1[i - 1] == str2[j - 1]) {
          dp[i][j] = dp[i - 1][j - 1] + 1;
        } else {
          dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
      }
    }
  }

  /**
   * @brief Метод для восстановления индексов LCS и их вывода
   */
  void reconstructLCS() {
    int i = n, j = m;
    while (i > 0 && j > 0) {
      if (str1[i - 1] == str2[j - 1]) {
        str1Indexes.push_back(i);
        str2Indexes.push_back(j);
        --i;
        --j;
      } else if (dp[i - 1][j] > dp[i][j - 1]) {
        --i;
      } else {
        --j;
      }
    }
  }

  /**
   * @brief Метод для вывода результата
   */
  void printResult() const {
    int length = dp[n][m];
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
};

int main() {
  std::string str1, str2;
  std::cin >> str1 >> str2;

  LCS lcsSolver(str1, str2);
  lcsSolver.computeLCS();
  lcsSolver.reconstructLCS();
  lcsSolver.printResult();

  return 0;
}
