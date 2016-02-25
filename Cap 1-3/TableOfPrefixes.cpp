//Code from 3.2, Jewels of Stringology
#include <iostream>
#include <string>
#include <vector>
#include <gmock/gmock.h>
using namespace std;

//Find the longest k for which pat[p...p+k-1p]=pat[q...q+k-1] with k>=1
long naive_scan(const string& pat,
        long p,long q)
{
    long k{0},len{pat.size()};
    while(p<len && q<len){
        if(pat[p++]!=pat[q++]) break;
        ++k;
    }
    return k;
}

//Brute force code
vector<long> compute_prefixes_naive(const string& pat)
{
    vector<long> table(pat.size(),0);
    for(int i{1};i<pat.size();i++)
    {
        table[i]=naive_scan(pat,0,i);
    }
    return table;
}

//pg 38
vector<long> compute_prefixes(const string& pat)
{
    vector<long> table(pat.size(),0);
    long s{0};
    for(int i{1};i<pat.size();i++)
    {
        long k{i-s};
        long r{s+table[s]-1};
        if(r<i){
            table[i]=naive_scan(pat,i,0);
            if(table[i]>0)
                s = i;
        }
        else if(table[k]+k<table[s]){
            table[i]=table[k];
        }else{
            table[i]=r-i+1+naive_scan(pat,r+1,r-i+1);
            s=i;
        }
    }
    return table;
}

TEST(compute_prefix_test,test1)
{
    const string pat("abaababaabaababaababaabac");
    const long expected_result[] = {0,0,1,3,0,6,0,1,11,0,1,3,0,11,0,1,3,0,6,0,1,3,0,1,0};
    vector<long> ans = compute_prefixes(pat);
    for(int i{0};i<sizeof(expected_result)/sizeof(long);i++)
        EXPECT_EQ(ans[i],expected_result[i]);
}

TEST(compute_prefix_test,test2)
{
    const string pat("ababababbabnabnabnabnanbabnabnbnabanbbanbabababbbbaaanannananbanbanbabanbabab");
    const long expected_result[] = {0,0,6,0,4,0,2,0,0,2,0,0,2,0,0,2,0,0,
        2,0,0,1,0,0,2,0,0,2,0,0,0,0,3,0,1,
        0,0,0,1,0,0,6,0,4,0,2,0,0,0,0,1,1,
        1,0,1,0,0,1,0,1,0,0,1,0,0,1,0,0,3,
        0,1,0,0,4,0,2,0};
    vector<long> ans = compute_prefixes(pat);
    for(int i{0};i<sizeof(expected_result)/sizeof(long);i++)
        EXPECT_EQ(ans[i],expected_result[i]);
}

TEST(compute_prefix_test,test3)
{
    const string pat("lla la alla la al al alalla la  alla la alla allallal allalalal lalalal alllallall");
    auto naive = compute_prefixes_naive(pat);
    auto smart = compute_prefixes(pat);
    ASSERT_EQ(naive.size(),smart.size());
    for(int i{0};i<naive.size();i++)
        EXPECT_EQ(naive[i],smart[i]);
}

TEST(naive_scan_test,test1)
{
    const string pat("ababa");
    EXPECT_EQ(naive_scan(pat,0,2),3);
    EXPECT_EQ(naive_scan(pat,0,1),0);
    EXPECT_EQ(naive_scan(pat,2,2),3);
}

TEST(naive_scan_test,test2)
{
    const string pat("abaababaa");
    EXPECT_EQ(naive_scan(pat,0,3),3);
    EXPECT_EQ(naive_scan(pat,3,5),3);
    EXPECT_EQ(naive_scan(pat,6,7),0);
    EXPECT_EQ(naive_scan(pat,1,2),0);
    EXPECT_EQ(naive_scan(pat,0,4),0);

}

TEST(naive_scan_test,test3)
{
    const string pat("aaaaaaa");
    EXPECT_EQ(naive_scan(pat,0,3),4);
    EXPECT_EQ(naive_scan(pat,1,2),5);
}


int main(int argc,char** argv)
{
    ::testing::InitGoogleMock(&argc,argv);
    return RUN_ALL_TESTS();
}

