// ДЛЯ РЕВЬЮ

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

class BigInt {
 private:
  static const int kBase = 1000000000;
  static const int kBasis = 9;
  static const int kBlock = 10;

 public:
  //=====================================================================================================//
  BigInt() : BigInt(0) {}
  std::vector<int64_t> arr;
  bool positive_flag = true;

  BigInt(int64_t k_value) {
    if (k_value == std::numeric_limits<int64_t>::min() ||
        k_value == std::numeric_limits<int64_t>::max()) {
      *this = BigInt(std::to_string(k_value));
      return;
    }
    positive_flag = (k_value >= 0);
    if (k_value == std::numeric_limits<int64_t>::min()) {
      k_value = std::numeric_limits<int64_t>::max() - 1;
    }

    if (k_value < 0) {
      k_value *= -1;
    }

    int cnt = 0;
    while (k_value > 0) {
      arr.push_back(k_value % kBase);

      k_value /= kBase;
      cnt++;
    }
    RemovingZeros();
  }

  explicit BigInt(const std::string& str) {
    positive_flag = true;
    int64_t size = (str.size() + kBasis - 1) / kBasis;
    arr.resize(size, 0);
    int64_t presf = 0;
    if (!str.empty() && str[0] == '-') {
      positive_flag = false;
      if (str[1] == '0') {
        positive_flag = true;
      }
      presf++;
    }
    int64_t cur = 0;
    for (int64_t i = str.size() - 1; i >= presf; i -= kBasis) {
      int block = 0;
      if (i - kBasis + 1 >= presf) {
        for (int64_t j = i - kBasis + 1; j < i + 1; j++) {
          block *= kBlock;
          block += str[j] - '0';
        }
      } else {
        for (int64_t j = presf; j < i + 1; j++) {
          block *= kBlock;
          block += str[j] - '0';
        }
      }
      arr[cur++] = block;
    }
    RemovingZeros();
  }

  explicit BigInt(int new_num) : BigInt(static_cast<int64_t>(new_num)) {}
  explicit BigInt(double value) : BigInt(static_cast<int64_t>(value)) {}

  explicit BigInt(unsigned long long new_num) : positive_flag(true) {
    while (new_num > 0) {
      arr.push_back(new_num % kBase);
      new_num /= kBase;
    }
    RemovingZeros();
  }
  //=====================================================================================================//

  void RemovingZeros();
  bool IsZero() const;

  std::string GetString() const;
  bool AbsMeaning(const BigInt& other) const;
  static BigInt Subtract(const BigInt& num1, const BigInt& num2);

  const std::vector<int64_t>& GetArr() const;

  std::strong_ordering operator<=>(const BigInt& other) const;
  bool operator==(const BigInt& other) const;

  BigInt& operator*=(const BigInt& num);
  BigInt& operator+=(const BigInt& num);
  BigInt& operator-=(const BigInt& num);
  BigInt& operator/=(const BigInt& num);
  BigInt& operator%=(const BigInt& num2);

  BigInt operator*(const BigInt& other) const;
  BigInt operator/(const BigInt& other) const;
  BigInt operator%(const BigInt& other) const;

  void shiftLeft();

  void Plus(const BigInt& num);
  static BigInt SubtractWithAbs(const BigInt& a, const BigInt& b);

  friend std::ostream& operator<<(std::ostream& out, const BigInt& num);
  friend std::istream& operator>>(std::istream& in, BigInt& num);

  friend BigInt operator<<(const BigInt& a, int shift);

  void DivideCore(const BigInt& b, BigInt& result, BigInt& current);
  static int BinarySearchQuotient(const BigInt& b, const BigInt& current);
  bool PrepareForDivision(const BigInt& num, BigInt& result,
                          BigInt& current) const;

  void ShiftRight();

  friend BigInt operator+(const BigInt& a, const BigInt& b);
  friend BigInt operator-(const BigInt& a, const BigInt& b);
};

// //=================================================================//
// //=================================Операции_сравнения==============//
// //=================================================================//

std::strong_ordering BigInt::operator<=>(const BigInt& other) const {
  if (IsZero() && other.IsZero()) {
    return std::strong_ordering::equal;
  }
  size_t s1 = arr.size();
  size_t s2 = other.arr.size();
  bool posf = other.positive_flag;
  if (positive_flag != posf) {
    return positive_flag ? std::strong_ordering::greater
                         : std::strong_ordering::less;
  }
  bool to_reverse_sign = false;
  if (static_cast<int>(positive_flag) == 0 && static_cast<int>(posf) == 0) {
    to_reverse_sign = true;
  }
  bool truth = static_cast<bool>(1 ^ static_cast<int>(to_reverse_sign));
  bool lie = static_cast<bool>(0 ^ static_cast<int>(to_reverse_sign));
  std::strong_ordering result = s1 <=> s2;
  if (s1 != s2) {
    return !positive_flag ? 0 <=> result : result;
  }
  for (int i = static_cast<int>(s1) - 1; i >= 0; i--) {
    if (arr[i] < other.arr[i]) {
      return truth ? std::strong_ordering::less : std::strong_ordering::greater;
    }
    if (arr[i] > other.arr[i]) {
      return lie ? std::strong_ordering::less : std::strong_ordering::greater;
    }
  }
  return std::strong_ordering::equal;
}

const std::vector<int64_t>& BigInt::GetArr() const { return arr; }

bool BigInt::operator==(const BigInt& other) const {
  if (IsZero() && other.IsZero()) {
    return true;
  }
  if (positive_flag != other.positive_flag) {
    return false;
  }

  if (arr.size() != other.arr.size()) {
    return false;
  }

  for (size_t i = 0; i < arr.size(); i++) {
    if (arr[i] != other.arr[i]) {
      return false;
    }
  }

  return true;
}

// bool BigInt::operator!=(const BigInt& other) const { return !(*this == other); }

// //=================================================================//
// //===============================Арифметические_операции===========//
// //=================================================================//

BigInt operator+(const BigInt& a, const BigInt& b) {
  BigInt new_a(a);
  new_a += b;
  return new_a;
}

BigInt operator-(const BigInt& a, const BigInt& b) {
  BigInt new_a(a);
  new_a -= b;
  return new_a;
}

BigInt BigInt::operator*(const BigInt& other) const {
  if (this->IsZero() || other.IsZero()) {
    return BigInt(0);
  }
  BigInt result = *this;
  result *= other;
  return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
  BigInt result = *this;
  BigInt zero(0);

  if (other.IsZero() || this->IsZero()) {
    return zero;
  }
  result /= other;
  return result;
}

BigInt BigInt::operator%(const BigInt& other) const {
  BigInt result = *this;
  result %= other;
  return result;
}

// //=================================================================//
// //===============================Ввод_и_вывод======================//
// //=================================================================//

std::istream& operator>>(std::istream& in, BigInt& num) {
  std::string res;
  in >> res;
  num = BigInt(res);
  return in;
}

std::ostream& operator<<(std::ostream& out, const BigInt& num) {
  out << num.GetString();
  return out;
}

// //=================================================================//
// //======================asSignment_версии_арифметических_операций==//
// //=================================================================//

BigInt& BigInt::operator*=(const BigInt& num) {
  static BigInt zero(0);
  if (this->IsZero() || num.IsZero()) {
    return zero;
  }
  positive_flag = (positive_flag == num.positive_flag);
  std::vector<int64_t> res(arr.size() + num.arr.size(), 0);

  for (size_t i = 0; i < arr.size(); i++) {
    for (size_t j = 0; j < num.arr.size(); j++) {
      res[i + j] += arr[i] * num.arr[j];
      if (res[i + j] >= kBase) {
        res[i + j + 1] += res[i + j] / kBase;
        res[i + j] %= kBase;
      }
    }
  }

  arr = res;
  RemovingZeros();

  return *this;
}

BigInt& BigInt::operator+=(const BigInt& num) {
  if (positive_flag == num.positive_flag) {
    Plus(num);
  } else {
    if (positive_flag) {
      *this = SubtractWithAbs(*this, num);
    } else {
      *this = SubtractWithAbs(num, *this);
    }
  }
  RemovingZeros();
  return *this;
}

BigInt& BigInt::operator-=(const BigInt& num) {
  if (!num.positive_flag) {
    if (positive_flag) {
      Plus(num);
    } else {
      *this = SubtractWithAbs(num, *this);
    }
  } else {
    if (!positive_flag) {
      Plus(num);
    } else {
      *this = SubtractWithAbs(*this, num);
    }
  }
  RemovingZeros();
  return *this;
}

void BigInt::ShiftRight() {
  if (this->arr.empty()) {
    this->arr.push_back(0);
    return;
  }
  this->arr.push_back(this->arr[this->arr.size() - 1]);
  for (size_t i = this->arr.size() - 2; i > 0; --i) {
    this->arr[i] = this->arr[i - 1];
  }
  this->arr[0] = 0;
}

void BigInt::shiftLeft() {
    if (this->arr.empty()) {
        this->arr.push_back(0);
        return;
    }
    this->arr.push_back(0);
}

bool BigInt::PrepareForDivision(const BigInt& num, BigInt& result,
                                BigInt& current) const {
  if (num.IsZero() || this->IsZero()) {
    return false;
  }

  result.arr.resize(this->arr.size());
  current = BigInt(0);
  return true;
}

int BigInt::BinarySearchQuotient(const BigInt& b, const BigInt& current) {
  int l = 0;
  int r = kBase;
  int x = 0;
  while (l <= r) {
    int m = (l + r) / 2;
    BigInt t = b * m;
    if (t <= current) {
      x = m;
      l = m + 1;
    } else {
      r = m - 1;
    }
  }
  return x;
}

void BigInt::DivideCore(const BigInt& b, BigInt& result, BigInt& current) {
  for (long long i = static_cast<long long>(this->arr.size()) - 1; i >= 0;
       --i) {
    current.ShiftRight();
    current.arr[0] = this->arr[i];
    current.RemovingZeros();

    int x = BinarySearchQuotient(b, current);
    result.arr[i] = x;
    current = current - b * x;
  }
}

BigInt& BigInt::operator/=(const BigInt& num) {
  BigInt result;
  BigInt current;
  if (!PrepareForDivision(num, result, current)) {
    return *this;
  }

  BigInt b = num;
  b.positive_flag = true;

  DivideCore(b, result, current);

  result.positive_flag = this->positive_flag == num.positive_flag;
  result.RemovingZeros();
  *this = result;
  return *this;
}

BigInt& BigInt::operator%=(const BigInt& num2) {
  BigInt res = *this / num2;
  *this -= res * num2;
  RemovingZeros();
  return *this;
}

BigInt operator<<(const BigInt& a, int shift) {
    BigInt result = a;
    for (int i = 0; i < shift; ++i) {
        result.shiftLeft();
    }
    return result;
}

// //=================================================================//
// //===============================ъъъъъъъъъъъ=======================//
// //=================================================================//

void BigInt::RemovingZeros() {
  while (!arr.empty() && arr.back() == 0) {
    arr.pop_back();
  }
  if (arr.empty()) {
    arr.resize(1, 0);
  }
}

bool BigInt::IsZero() const {
  if (arr.empty()) {
    return true;
  }

  return (arr[0] == 0 && arr.size() == 1);
}

std::string BigInt::GetString() const {
  std::string res;
  if (arr.empty()) {
    res = "0";
  } else {
    if (static_cast<int>(positive_flag) == 0 && !IsZero()) {
      res += '-';
    }
    res += std::to_string(arr.back());
    for (int64_t i = static_cast<int>(arr.size()) - 2; i >= 0; i--) {
      std::string cur_block = std::to_string(arr[i]);
      int cnt_of_zeros_to_add_front = 0;
      if (kBasis > cur_block.size()) {
        cnt_of_zeros_to_add_front = kBasis - cur_block.size();
      }
      if (cnt_of_zeros_to_add_front >= 1) {
        std::string to_add(cnt_of_zeros_to_add_front, '0');
        cur_block = to_add + cur_block;
      }
      res += cur_block;
    }
  }
  return res;
}

void BigInt::Plus(const BigInt& num) {
  bool carry = false;
  size_t common_size = std::max(arr.size(), num.arr.size());
  bool to_stop = false;
  for (size_t i = 0; i < common_size || carry; i++) {
    if (arr.size() == i) {
      arr.push_back(0);
    }
    int to_add = 0;
    if (i < num.arr.size()) {
      to_add = num.arr[i];
    } else {
      to_stop = true;
    }
    arr[i] += static_cast<int>(carry) + to_add;
    if (arr[i] >= kBase) {
      arr[i] -= kBase;
      carry = true;
    } else {
      carry = false;
      if (to_stop) {
        break;
      }
    }
  }
}

bool BigInt::AbsMeaning(const BigInt& other) const {
  if (IsZero() && other.IsZero()) {
    return true;
  }

  int64_t size1 = arr.size();
  int64_t size2 = other.arr.size();
  if (size1 < size2) {
    return true;
  }
  if (size1 > size2) {
    return false;
  }

  for (int64_t i = size1 - 1; i >= 0; i--) {
    if (arr[i] > other.arr[i]) {
      return false;
    }
    if (arr[i] < other.arr[i]) {
      return true;
    }
  }
  return true;
}

BigInt BigInt::Subtract(const BigInt& num1, const BigInt& num2) {
  BigInt res(num1);
  size_t size = std::max(num1.arr.size(), num2.arr.size());
  int carry = 0;
  bool flag = false;

  for (size_t i = 0; i < size || (carry == 1); i++) {
    int sub = 0;
    if (i < num2.arr.size()) {
      sub = num2.arr[i];
    } else {
      flag = true;
    }
    res.arr[i] -= sub + carry;
    if (res.arr[i] < 0) {
      res.arr[i] += kBase;
      carry = 1;
    } else {
      carry = 0;
      if (flag) {
        break;
      }
    }
  }
  res.RemovingZeros();
  return res;
}

BigInt BigInt::SubtractWithAbs(const BigInt& a, const BigInt& b) {
  bool num2_less = b.AbsMeaning(a);
  BigInt res;
  if (num2_less) {
    res = Subtract(a, b);
    res.positive_flag = true;
  } else {
    res = Subtract(b, a);
    res.positive_flag = false;
  }
  return res;
}

bool find(int fm, int sm, int len) {
  for (int i = 1; i < len; ++i) {
    int cf = ((fm >> i) & 1);
    int pf = ((fm >> (i - 1)) & 1);
    if (cf == pf) {
      int cs = ((sm >> i) & 1);
      int ps = ((sm >> (i - 1)) & 1);
      if (cf == cs && pf == ps) {
        return true;
      }
    }
  }
  return false;
}

std::vector<std::vector<BigInt>> mul(const std::vector<std::vector<BigInt>>& fm,
                                     const std::vector<std::vector<BigInt>>& sm,
                                     int len, const BigInt& mod) {
  std::vector<std::vector<BigInt>> res(len, std::vector<BigInt>(len, 0));
  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < len; ++j) {
      for (int k = 0; k < len; ++k) {
        res[i][j] = (res[i][j] + fm[i][k] * sm[k][j]) % mod;
      }
    }
  }
  return res;
}

std::vector<std::vector<BigInt>> exp(
    const std::vector<std::vector<BigInt>>& matrix, long long n, int len,
    const BigInt& mod) {
  if (n == 1) {
    return matrix;
  }

  if (n % 2 == 0) {
    std::vector<std::vector<BigInt>> half = exp(matrix, n / 2, len, mod);
    return mul(half, half, len, mod);
  } else {
    return mul(exp(matrix, n - 1, len, mod), matrix, len, mod);
  }
}

class Solver {
 public:
  void solve(const BigInt& n, int m, int mod) {
    BigInt one(1);
    BigInt two(2);

    if (n == one) {
      std::cout << (1LL << m) % mod << std::endl;
    } else {
      long long size = (1LL << m);
      std::vector<std::vector<long long>> matrix(
          size, std::vector<long long>(size, 0));
      for (int mask = 0; mask < size; ++mask) {
        for (int prevMask = 0; prevMask < size; ++prevMask) {
          if (!findSquare(mask, prevMask, m)) {
            matrix[mask][prevMask] = 1;
          }
        }
      }

      auto ansMatrix = exp(matrix, n - one, size, mod);
      long long ans = 0;
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          ans = (ans + ansMatrix[i][j]) % mod;
        }
      }
      std::cout << ans << std::endl;
    }
  }

 private:
  std::vector<std::vector<long long>> exp(
      const std::vector<std::vector<long long>>& matrix, const BigInt& n,
      long long len, int mod) {
    if (n == BigInt(1)) {
      return matrix;
    }
    if (n % BigInt(2) == BigInt(0)) {
      auto sqrt = exp(matrix, n / BigInt(2), len, mod);
      return multiply(sqrt, sqrt, len, mod);
    }
    return multiply(exp(matrix, n - BigInt(1), len, mod), matrix, len, mod);
  }

  std::vector<std::vector<long long>> multiply(
      const std::vector<std::vector<long long>>& firstMatrix,
      const std::vector<std::vector<long long>>& secondMatrix, long long len,
      int mod) {
    std::vector<std::vector<long long>> res(len,
                                            std::vector<long long>(len, 0));
    for (long long i = 0; i < len; i++) {
      for (long long j = 0; j < len; j++) {
        for (long long k = 0; k < len; k++) {
          res[i][j] =
              (res[i][j] + firstMatrix[i][k] * secondMatrix[k][j]) % mod;
        }
      }
    }
    return res;
  }

  bool findSquare(int firstMask, int secondMask, int len) {
    for (int i = 1; i < len; i++) {
      int curFirstBit = (firstMask >> i) & 1;
      int prevFirstBit = (firstMask >> (i - 1)) & 1;
      if (curFirstBit == prevFirstBit) {
        int curSecondBit = (secondMask >> i) & 1;
        int prevSecondBit = (secondMask >> (i - 1)) & 1;
        if (curFirstBit == curSecondBit && prevFirstBit == prevSecondBit) {
          return true;
        }
      }
    }
    return false;
  }
};

void getInput(BigInt& n, int& m, int& mod) {
  std::cin >> n;
  std::cin >> m >> mod;
}

void processInputAndSolve(Solver& solver, const BigInt& n, int m, int mod) {
  solver.solve(n, m, mod);
}

int main() {
  Solver solver;
  BigInt n;
  int m, mod;
  getInput(n, m, mod);
  processInputAndSolve(solver, n, m, mod);
  return 0;
}