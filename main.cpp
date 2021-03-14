#include "CompareTest.h"

#include <exception>
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {

#ifdef GTESTS
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif

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

    return 0;
}
