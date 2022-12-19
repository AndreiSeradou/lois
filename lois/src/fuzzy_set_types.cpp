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

#include "fuzzy_set_types.h"

SetElem::SetElem(const std::string &str) {
  if (str.front() < 'a' || str.front() > 'z') {
    throw std::invalid_argument("");
  }

  ch = str.front();

  std::string numStr = str.substr(1);
  if (numStr.empty()) {
    num = -1;
  } else {
    num = std::stoul(numStr);
  }
}

std::string SetElem::toString() const {
  if (num != -1) {
    return ch + std::to_string(num);
  }
  return ch + std::string();
}

bool SetElem::operator<(const SetElem &el) const {
  if (ch != el.ch) {
    return ch < el.ch;
  }
  return num < el.num;
}

FuzzySetElem::FuzzySetElem(const SetElem &inVar, double inVal) : var(inVar), val(inVal) {
}

FuzzySetElem::FuzzySetElem(std::string str) {
  if (str.front() != '(' || str.back() != ')') {
    throw std::invalid_argument("");
  }

  str = str.substr(1);
  str.pop_back();

  size_t i = str.find(',');
  if (i == std::string::npos) {
    throw std::invalid_argument("");
  }

  var = SetElem(str.substr(0, i));
  val = std::stod(str.substr(i + 1));

  if (val < 0 || val > 1) {
    throw std::invalid_argument("");
  }
}

std::string FuzzySetElem::toString() const {
  return "(" + var.toString() + "," + std::to_string(val) + ")";
}

bool FuzzySetElem::operator<(const FuzzySetElem &el) const {
  return var < el.var;
}

FuzzySet::FuzzySet(std::string str) {
  if (str.front() != '{' || str.back() != '}') {
    throw std::invalid_argument("");
  }

  str = str.substr(1);
  str.pop_back();

  for (;;) {
    size_t i = str.find(')');
    if (i == std::string::npos) {
      throw std::invalid_argument("");
    }

    i++;
    insert(FuzzySetElem(str.substr(0, i)));

    if (i >= str.length()) {
      break;
    }

    str = str.substr(i + 1);
  }
}

std::string FuzzySet::toString() const {
  constexpr double epsilon = 0.00001;

  std::string str = "{";

  for (const auto &el : *this) {
    if (el.val > epsilon) {
      str += el.toString() + ",";
    }
  }

  str.pop_back();
  return str + "}";
}

void FuzzySet::fillFrom(const FuzzySet &set) {
  for (const auto &el : set) {
    if (set.contains(el)) {
      insert(FuzzySetElem(el.var, 0.0));
    }
  }
}
