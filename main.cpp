#include "CompareTest.h"

#include <exception>
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {

#ifdef GTESTS
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif

    try {
        Tester::DefaultTest();


        //Tester::CompareTest("tests/test_gen1.txt");
    }
    catch (std::exception& e) {
        std::cerr<<e.what()<<std::endl;
        exit(0);
    }

    return 0;
}
