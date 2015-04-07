#include <iostream>
#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

int sum(int a, int b) {
    return a + b;
}

TEST(BM, service) {
    ASSERT_EQ(3, sum(1,2));
    ASSERT_EQ(4, sum(1,3));
}
