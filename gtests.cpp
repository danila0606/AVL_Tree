
#include "tree.h"

#include <string>
#include <fstream>
#include <sstream>
#include <exception>

#include <gtest/gtest.h>

std::string MyTest(const std::string& file_name) {

    std::ifstream istr(file_name);

    if (!istr.is_open()) {
        std::cerr << "Cant open file!" << std::endl;
        exit(0);
    }

    int N, M, key;
    istr >> N;

    tr::Tree<int> tree;
    for (std::size_t i = 0; i < N; ++i) {

        istr >> key;
        if (!istr.good())
            throw std::runtime_error("Problems with reading file");

        tree.Insert(key);
    }

    istr >> M;
    std::vector<std::pair<int, int>> requests;
    for (int i = 0; i < M; ++i) {
        int from, to;
        istr >> from >> to;
        requests.emplace_back(from, to);
        if (istr.good())
            throw std::runtime_error("Problems with reading file");
    }
    std::ostringstream answer;

    for (const auto &req : requests) {
        auto l_iter = tree.LowerBound(req.first);
        auto r_iter = tree.UpperBound(req.second);

        size_t ans = 0;
        for (;l_iter != r_iter; ++ans, ++l_iter);
        answer << ans << " ";
    }
    istr.close();

    return answer.str();
}



TEST(Default_test, 1) {

    EXPECT_EQ(MyTest("tests/test1.txt"), "3 0 ");
}

TEST(Default_test, 2) {

    EXPECT_EQ(MyTest("tests/test2.txt"), "6 5 ");
}

TEST(Default_test_10000, 1) {

    EXPECT_EQ(MyTest("tests/test3.txt"), "2586 3430 4724 2140 1347 2636 6612 694 390 7826 ");
}

TEST(Copy_Ctor, 1) {
    tr::Tree<int> tree;

    tree.Insert(10);
    tree.Insert(20);
    tree.Insert(30);
    tree.Insert(40);

    tr::Tree temp(tree);

    auto it1 = tree.begin();
    it1++;it1++;it1++;

    auto it = temp.begin();
    ++it;++it;++it;

    EXPECT_EQ(*it, *it1);
}

