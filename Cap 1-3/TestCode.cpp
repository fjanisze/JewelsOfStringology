#include <iostream>
#include <vector>
#include <string>
#include <gmock/gmock.h>
#include "Functions.hpp"
using namespace std;

TEST(compute_prefix_test,test1)
{
    const string pat("abaababaabaababaababaabac");
    const long expected_result[] = {25,0,1,3,0,6,0,1,11,0,1,3,0,11,0,1,3,0,6,0,1,3,0,1,0};
    vector<long> ans = compute_prefixes(pat);
    ASSERT_EQ(ans.size(),25);
    for(int i{0};i<25;i++)
        ASSERT_EQ(ans[i],expected_result[i]);
}

TEST(compute_prefix_test,test2)
{
    const string pat("ababababbabnabnabnabnanbabnabnbnabanbbanbabababbbbaaanannananbanbanbabanbabab");
    const long expected_result[] = {77,0,6,0,4,0,2,0,0,2,0,0,2,0,0,2,0,0,
        2,0,0,1,0,0,2,0,0,2,0,0,0,0,3,0,1,
        0,0,0,1,0,0,6,0,4,0,2,0,0,0,0,1,1,
        1,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,3,
        0,1,0,0,4,0,2,0};
    vector<long> ans = compute_prefixes(pat);
    ASSERT_EQ(ans.size(),77);
    for(int i{0};i<77;i++)
        ASSERT_EQ(ans[i],expected_result[i]);
}

TEST(compute_prefix_test,test3)
{
    const string pat("lla la alla la al al alalla la  alla la alla allallal allalalal lalalal alllallall");
    //Clearly the naive version is bug free :)
    auto naive = compute_prefixes_naive(pat);
    auto smart = compute_prefixes(pat);
    ASSERT_EQ(naive.size(),smart.size());
    for(int i{0};i<naive.size();i++)
        ASSERT_EQ(naive[i],smart[i]);
}

TEST(naive_scan_test,test1)
{
    const string pat("ababa");
    ASSERT_EQ(naive_scan(pat,0,2),3);
    ASSERT_EQ(naive_scan(pat,0,1),0);
    ASSERT_EQ(naive_scan(pat,2,2),3);
}

TEST(naive_scan_test,test2)
{
    const string pat("abaababaa");
    ASSERT_EQ(naive_scan(pat,0,3),3);
    ASSERT_EQ(naive_scan(pat,3,5),3);
    ASSERT_EQ(naive_scan(pat,6,7),0);
    ASSERT_EQ(naive_scan(pat,1,2),0);
    ASSERT_EQ(naive_scan(pat,0,4),0);

}

TEST(naive_scan_test,test3)
{
    const string pat("aaaaaaa");
    ASSERT_EQ(naive_scan(pat,0,3),4);
    ASSERT_EQ(naive_scan(pat,1,2),5);
}

TEST(compute_suffixes_test,test1)
{
    const string pat("abaababaabaababaababaaba");
    //I'm silently assuming that the naive versione is bug free :)
    auto naive = compute_suffixes_naive(pat);
    auto smart = compute_suffixes(pat);
    ASSERT_EQ(naive.size(),smart.size());
    for(int i{0};i<naive.size();i++)
        ASSERT_EQ(naive[i],smart[i]);
}


TEST(compute_suffixes_test,test2)
{
    const string pat("blublubbublublublubulblbuulbublbublublbu");
    auto naive = compute_suffixes_naive(pat);
    auto smart = compute_suffixes(pat);
    ASSERT_EQ(naive.size(),smart.size());
    for(int i{0};i<naive.size();i++)
        ASSERT_EQ(naive[i],smart[i]);
}

TEST(compute_suffixes_test,test3)
{
    const string pat("gcagagag");
    const long expected_result[]={1,0,0,2,0,4,0,8};
    auto ans = compute_suffixes(pat);
    ASSERT_EQ(ans.size(),8);
    for(int i{0};i<8;i++)
        ASSERT_EQ(ans[i],expected_result[i]);
}

TEST(compute_boyer_moore_shifts_test,test1)
{
    const std::string pat("gcagagag");
    const long ans[]={7,7,7,2,7,4,7,1};
    auto result = compute_boyer_moore_shifts(pat);
    ASSERT_EQ(result.size(),8);
    for(int i{0};i<8;i++)
        ASSERT_EQ(result[i],ans[i]);
}

TEST(compute_boyer_moore_shifts_test,test2)
{
    const std::string pat("anabanana");
    const long ans[]={6,6,6,6,6,2,8,4,1};
    auto result = compute_boyer_moore_shifts(pat);
    ASSERT_EQ(result.size(),9);
    for(int i{0};i<9;i++)
        ASSERT_EQ(result[i],ans[i]);
}


class searching_test : public ::testing::Test
{
protected:
    std::string sample;
public:
    searching_test(){}
    void SetUp()
    {
        sample = "abaabaabaabbabaaaabbbabababababbbcbi\
                  bcbcbacbacbacbaabccaabccaacbbaccacca\
                  abbcbcbcbcaabcbcbbcabcabcabcabbcaaaa\
                  ab abb abab abaab abbaab aabbaab abb\
                  abc abcc abccc abbccc aaabbcc aaabab\
                  aba ab abaa b a baab aabaabb abaacbb";
    }
    void TearDown(){}
};

TEST_F(searching_test,test_abaab)
{
    vector<long> kmp_results = knuth_morris_pratt(sample,"abaab");
    vector<long> bm_results = boyer_moore(sample,"abaab");
    const long expected_results[]={0,3,6,174,292};
    ASSERT_EQ(kmp_results.size(),5);
    ASSERT_EQ(bm_results.size(),5);
    for(int i{0};i<5;i++)
    {
        ASSERT_EQ(kmp_results[i],expected_results[i]);
        ASSERT_EQ(bm_results[i],expected_results[i]);
    }
}

TEST_F(searching_test,test_aab)
{
    vector<long> kmp_results = knuth_morris_pratt(sample,"aab");
    vector<long> bm_results = boyer_moore(sample,"aab");
    const long num_of_results{16};
    const long correct_values[]={2,5,8,16,68,73,118,176,183,187,191,
        239,247,287,291,294};
    ASSERT_EQ(kmp_results.size(),num_of_results);
    ASSERT_EQ(bm_results.size(),num_of_results);
    for(int i{0};i<num_of_results;i++)
    {
        ASSERT_EQ(kmp_results[i],correct_values[i]);
        ASSERT_EQ(bm_results[i],correct_values[i]);
    }
}

TEST_F(searching_test,test_ababab)
{
    vector<long> kmp_results = knuth_morris_pratt(sample,"ababab");
    vector<long> bm_results = boyer_moore(sample,"ababab");
    const long num_of_results{3};
    const long correct_values[]={21,23,25};
    ASSERT_EQ(kmp_results.size(),num_of_results);
    ASSERT_EQ(bm_results.size(),num_of_results);
    for(int i{0};i<num_of_results;i++)
    {
        ASSERT_EQ(kmp_results[i],correct_values[i]);
        ASSERT_EQ(bm_results[i],correct_values[i]);
    }
}

TEST_F(searching_test,test_c)
{
    vector<long> kmp_results = knuth_morris_pratt(sample,"c");
    vector<long> bm_results = boyer_moore(sample,"c");
    const long num_of_results{38};
    const long correct_values[]={33,55,57,60,63,66,71,72,76,77,
        80,84,85,87,88,111,113,115,117,
        121,123,126,129,132,135,139,218,
        222,223,227,228,229,234,235,236,
        243,244,303};
    ASSERT_EQ(kmp_results.size(),num_of_results);
    ASSERT_EQ(bm_results.size(),num_of_results);
    for(int i{0};i<num_of_results;i++)
    {
        ASSERT_EQ(kmp_results[i],correct_values[i]);
        ASSERT_EQ(bm_results[i],correct_values[i]);
    }
}

int main(int argc,char** argv)
{
    ::testing::InitGoogleMock(&argc,argv);
    return RUN_ALL_TESTS();
}

