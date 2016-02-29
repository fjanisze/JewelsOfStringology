#include <vector>
#include <string>
#include <algorithm>
#include "Functions.hpp"

//pg 40, needed for the Boyer-Moore algorithm
std::vector<long> compute_suffixes(const std::string& pat)
{
    std::string reversed_pat(pat);
    std::reverse(reversed_pat.begin(),reversed_pat.end());
    std::vector<long> table = compute_prefixes(reversed_pat);
    std::reverse(table.begin(),table.end());
    return table;
}

//Brute force algorithm
std::vector<long> compute_suffixes_naive(const std::string& pat)
{
    long pat_len{pat.size()};
    std::vector<long> table(pat_len);
    table[pat_len-1]=pat_len;
    for(long i{pat_len-2};i>=0;i--)
    {
        long j{0};
        for(;(i-j>=0)&&(j<pat_len)&&(pat[pat_len-1-j]==pat[i-j]);j++);
        table[i]=j;
    }
    table.back()=pat_len;
    return table;
}
