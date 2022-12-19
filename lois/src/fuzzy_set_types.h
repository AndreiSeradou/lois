//////////////////////////////////////////////////////////////////////////////////////
// Лабораторная работа по дисциплине ЛОИС
// Выполнена студентами группы 921704 БГУИР
// Середов А. С.
// Вариант 23 - Реализовать прямой нечеткий логический вывод, используея импликацию Лукасевича
// Код повзаимствован у Валюкевич В.И.
// 10.12.2022
// Использованные материалы:
// "Нечеткий логический вывод в системе принятия решений" А. А. Ахрем, М. Р. Ашинянц. С. А. Петров
// https://habr.com/ru/post/111187/ - Прямой нечеткий логический вывод

#include <set>
#include <stdexcept>
#include <vector>

struct SetElem {
  char ch{};
  size_t num{};

  SetElem() = default;

  explicit SetElem(const std::string &str);

  std::string toString() const;

  bool operator<(const SetElem &el) const;
};

struct FuzzySetElem {
  SetElem var;
  double val{};

  FuzzySetElem() = default;

  explicit FuzzySetElem(const SetElem &inVar, double inVal);

  explicit FuzzySetElem(std::string str);

  std::string toString() const;

  bool operator<(const FuzzySetElem &el) const;
};

class FuzzySet : public std::set<FuzzySetElem> {
public:
  FuzzySet() = default;

  explicit FuzzySet(std::string str);

  std::string toString() const;

  void fillFrom(const FuzzySet &set);
};
