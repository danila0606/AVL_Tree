#include "CompareTest.h"

//Example:
//Tester::CompareTest("tests/test_gen1.txt");
//TestGenerator(0, 100000,10000, 10, "../tests/test3.txt", "../tests/test_gen3_ans.txt");

void Tester::TestGenerator(int d1, int d2, int count, int req_count, const std::string& name, const std::string& answer_name) {

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

void Tester::CompareTest(const std::string& filename) { //"../tests/test_gen1.txt"
    try {

        std::ifstream istr(filename);

        if (!istr.is_open()) {
            std::cerr << "Cant open file!" << std::endl;
            exit(0);
        }

        int N, M, key;
        istr >> N;

        std::set<int> set_;
        tr::Tree<int> tree;

        std::vector<int> data(N);
        for (std::size_t i = 0; i < N; ++i) {

            istr >> key;
            if (!istr.good())
                throw std::runtime_error("Problems with std::cin");

            data.push_back(key);
        }

        //insert......................................................

        auto time_start_avl_inserting = std::chrono::system_clock::now();
        for (int & i : data)
            tree.Insert(i);
        auto time_finish_avl_inserting = std::chrono::system_clock::now();

        auto time_start_set_inserting = std::chrono::system_clock::now();
        for (int & i : data)
            set_.insert(i);
        auto time_finish_set_inserting = std::chrono::system_clock::now();

        //.............................................................

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


        if (SETans.str() == AVLans.str()) {
            std::cout << "The test was successful\n";
        }
        else {
            std::cerr <<"Test Failed!\n";
            std::cerr <<"SET answer: "<<SETans.str()<<std::endl;
            std::cerr <<"AVL answer: "<<AVLans.str()<<std::endl;
            exit(0);
        }

        std::cout<<"AVL_TREE (insert time): "<<(time_finish_avl_inserting - time_start_avl_inserting).count()<<std::endl;
        std::cout<<"AVL_TREE (searching time): "<<(time_finish_avl - time_start_avl).count()<<std::endl;

        std::cout<<"SET: (insert time)"<<(time_finish_set_inserting - time_start_set_inserting).count()<<std::endl;
        std::cout<<"SET: (searching time)"<<(time_finish_set - time_start_set).count()<<std::endl;

        //............................................................................
    }
    catch (std::exception& e) {
        std::cerr<<e.what()<<std::endl;
        exit(0);
    }
}