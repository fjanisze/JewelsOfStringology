#include <gmock/gmock.h>
using namespace std;

TEST(fuck,fuck1)
{
    EXPECT_EQ(2,2);
}

int main(int argc,char** argv)
{
    ::testing::InitGoogleMock(&argc,argv);
    return RUN_ALL_TESTS();
}
