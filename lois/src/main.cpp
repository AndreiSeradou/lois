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

#include <fstream>
#include <iostream>

#include "fuzzy_set_types.h"

using Relation = std::vector<std::vector<double>>;

FuzzySet conclusion(std::vector<FuzzySet> sets);

Relation relation(const FuzzySet &xSet, const FuzzySet &ySet);

double tnorm(double x, double y);

double impl(double x, double y);

void readSets(std::vector<std::string> &setNames, std::vector<FuzzySet> &sets);

void printImplication(const std::vector<std::string> &setsNames);

int main() {
  try {
    std::vector<std::string> setNames;
    std::vector<FuzzySet> sets;
    readSets(setNames, sets);

    FuzzySet conclSet = conclusion(sets);
    printImplication(setNames);
    std::cout << conclSet.toString() << "\n\n";
  } catch (...) {
    std::cout << "Invalid input\n";
  }
}

FuzzySet conclusion(std::vector<FuzzySet> sets) {
  FuzzySet conclSet = *sets.begin();

  for (auto iter = sets.begin() + 1; iter != sets.end(); ++iter) {
    FuzzySet xSet = conclSet;
    FuzzySet &ySet = *iter;
    conclSet.clear();

    xSet.fillFrom(ySet);
    ySet.fillFrom(xSet);

    const Relation rel = relation(xSet, ySet);
    FuzzySet newConclSet;

    size_t i = 0;
    for (const auto &y : ySet) {
      double val = 0;

      size_t j = 0;
      for (const auto &x : xSet) {
        val = std::max(val, tnorm(x.val, rel[j][i]));
        j++;
      }

      conclSet.insert(FuzzySetElem(y.var, val));
      i++;
    }
  }

  return conclSet;
}

Relation relation(const FuzzySet &xSet, const FuzzySet &ySet) {
  Relation relation(xSet.size());

  size_t i = 0;
  for (const auto &x : xSet) {
    relation[i] = std::vector<double>(ySet.size());
    size_t j = 0;

    for (const auto &y : ySet) {
      relation[i][j] = impl(x.val, y.val);
      j++;
    }

    i++;
  }

  return relation;
}

double tnorm(double x, double y) {
  return x * y;
}

double impl(double x, double y) {
  if (x <= y) {
    return 1.0;
  }
  return y / x;
}

void readSets(std::vector<std::string> &setNames, std::vector<FuzzySet> &sets) {
  std::ifstream input(INPUT_FILE_PATH);
  std::string str;

  while (std::getline(input, str)) {
    size_t i = str.find('=');
    if (i == std::string::npos) {
      throw std::invalid_argument("");
    }

    if (str.front() < 'A' || str.front() > 'Z') {
      throw std::invalid_argument("");
    }

    {
      std::string numStr = str.substr(1, i - 1);
      if (!numStr.empty()) {
        std::stoul(numStr);
      }
    }

    std::string name = str.substr(0, i);
    if (std::find(setNames.begin(), setNames.end(), name) != setNames.end()) {
      throw std::invalid_argument("");
    }

    setNames.push_back(str.substr(0, i));
    sets.emplace_back(str.substr(i + 1));
  }
}

void printImplication(const std::vector<std::string> &setsNames) {
  std::string str = "(";

  for (const auto &name : setsNames) {
    str += name + "=>";
  }

  str.pop_back();
  str.pop_back();

  std::cout << str << ")=\n";
}
