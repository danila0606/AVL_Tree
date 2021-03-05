#include "tree.h"

#include <exception>
#include <gtest/gtest.h>
#include <chrono>
#include <random>

#include <set>

#include <sstream>
#include <fstream>

#define REAL_TEST

void TestGenerator(int d1, int d2, int count, int req_count, const std::string& name, const std::string& answer_name) {

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);
    std::uniform_int_distribution<int> distr(d1, d2);

    std::ofstream ostr(name);
    if (!ostr.is_open()) {
        std::cerr << "Cant open input file!" << std::endl;
        exit(0);
    }

    std::ofstream ostr_ans(answer_name);
    if (!ostr.is_open()) {
        std::cerr << "Cant open answer file!" << std::endl;
        exit(0);
    }

    std::set<int> set_;

    ostr<<count<<std::endl;
    for (int i = 0; i < count; ++i) {
        int key = distr(eng);
        ostr << key << " ";
        set_.insert(key);
    }


    ostr<<"\n"<<req_count<<std::endl;
    for (int i = 0; i < req_count; ++i) {

        int from = distr(eng);
        int to = distr(eng);
        ostr << std::min(from, to) <<" "<<std::max(from, to)<<"  ";

        auto l_iter = set_.lower_bound(std::min(from, to));
        auto r_iter = set_.upper_bound(std::max(from, to));

        size_t ans = 0;
        for (;l_iter != r_iter; ++ans, ++l_iter);
        ostr_ans<< ans << " ";
    }

    ostr.close();
    ostr_ans.close();
}

int main(int argc, char* argv[]) {

#ifdef GTESTS
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif

#ifdef COMPARE_TEST
    try {

        std::ifstream istr("../tests/test_gen1.txt");

        if (!istr.is_open()) {
            std::cerr << "Cant open file!" << std::endl;
            exit(0);
        }

        int N, M, key;
        istr >> N;

        std::set<int> set_;
        tr::Tree<int> tree;

        for (std::size_t i = 0; i < N; ++i) {

            istr >> key;
            if (!istr.good())
                throw std::runtime_error("Problems with std::cin");

            tree.Insert(key);
            set_.insert(key);
        }

        istr >> M;
        std::vector<std::pair<int, int>> requests;
        for (int i = 0; i < M; ++i) {
            int from, to;
            istr >> from >> to;
            requests.emplace_back(from, to);
            if (!istr.good())
                throw std::runtime_error("Problems with std::cin");
        }

        std::ostringstream SETans, AVLans;

        //....................SET...................................................

        auto time_start_avl = std::chrono::system_clock::now();

        for (const auto &req : requests) {
            auto l_iter = set_.lower_bound(req.first);
            auto r_iter = set_.upper_bound(req.second);

            size_t ans = 0;
            for (;l_iter != r_iter; ++ans, ++l_iter);
            SETans<< ans << " ";
        }

        auto time_finish_avl = std::chrono::system_clock::now();

        //............................................................................

        //....................AVL...................................................

        auto time_start_set = std::chrono::system_clock::now();

        for (const auto &req : requests) {
            auto l_iter = tree.LowerBound(req.first);
            auto r_iter = tree.UpperBound(req.second);

            size_t ans = 0;
            for (;l_iter != r_iter; ++ans, ++l_iter);
            AVLans<< ans << " ";
        }

        auto time_finish_set = std::chrono::system_clock::now();


        if (SETans.str() == AVLans.str())
            std::cout <<"The test was successful\n";
        else {
            std::cerr <<"Test Failed!\n";
            exit(0);
        }

        std::cout<<"AVL_TREE: "<<(time_finish_avl - time_start_avl).count()<<std::endl;

        std::cout<<"SET: "<<(time_finish_set - time_start_set).count()<<std::endl;

        //............................................................................
    }
    catch (std::exception& e) {
        std::cerr<<e.what()<<std::endl;
        exit(0);
    }


#endif

#ifdef REAL_TEST

    try {
        int N, M, key;
        std::cin >> N;

        tr::Tree<int> tree;
        for (std::size_t i = 0; i < N; ++i) {

            std::cin >> key;
            if (!std::cin.good())
                throw std::runtime_error("Problems with std::cin");

            tree.Insert(key);
        }

        std::cin >> M;
        std::vector<std::pair<int, int>> requests;
        for (int i = 0; i < M; ++i) {
            int from, to;
            std::cin >> from >> to;
            requests.emplace_back(from, to);
            if (!std::cin.good())
                throw std::runtime_error("Problems with std::cin");
        }

        for (const auto &req : requests) {
            auto l_iter = tree.LowerBound(req.first);
            auto r_iter = tree.UpperBound(req.second);

            size_t ans = 0;
            for (;l_iter != r_iter; ++ans, ++l_iter);
            std::cout << ans << " ";
        }
    }
    catch (std::exception& e) {
        std::cerr<<e.what()<<std::endl;
        exit(0);
    }
#endif

    //TestGenerator(0, 100000,10000, 10, "../tests/test3.txt", "../tests/test_gen3_ans.txt");

    return 0;
}
