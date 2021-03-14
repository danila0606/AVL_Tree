#pragma once

#include <string>
#include <set>
#include <vector>
#include <chrono>
#include <random>

#include <sstream>
#include <fstream>

#include "tree.h"

class Tester final {
public:
    static void TestGenerator(int d1, int d2, int count, int req_count, const std::string &name, const std::string &answer_name);
    static void CompareTest(const std::string &filename);
};