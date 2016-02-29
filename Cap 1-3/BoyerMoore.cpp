#include <iostream>
#include "Functions.hpp"
using namespace std;

/*
 * In this implementation I'm using both the shift euristic,
 * as from JoS pg 29, bad shift table in case of mismatch, 
 * and good shift table in case of match
*/

//Rightmost occurrence of a character
std::vector<long> compute_bad_shift_table(const std::string& pat)
{
    long pat_len{pat.size()};
    //The bad shift table is calculated for all
    //the alphabet. Assuming 255 characters.
    std::vector<long> table(1<<8);
    for(long i{0};i<1<<8;i++)
        table[i]=pat_len;
    for(long i{0};i<pat_len-1;i++)
        table[pat[i]]=pat_len-i-1;
    return table;
}

//This generate the table for the good suffix rule
std::vector<long> compute_boyer_moore_shifts(const std::string& pat)
{
    std::vector<long> suffix_table = compute_suffixes(pat);
    long pat_len{pat.size()};
    std::vector<long> shifts(pat.size());
    for(long i{0};i<pat_len;i++)
        shifts[i]=pat_len;
    long j{0};
    for(long i{pat_len-1};i>=0;i--)
    {
        if(suffix_table[i]==i+1)
        {
            for(;j<pat_len-1-i;j++)
            {
                if(shifts[j]==pat_len)
                {
                    shifts[j]=pat_len-1-i;
                }
            }
        }
    }
    for(long i{0};i<pat_len-1;i++)
        shifts[pat_len-1-suffix_table[i]] = pat_len-1-i;
    return shifts;
}

//Algorithm fomr pg 28 implementing both the bad shift table
//and good shift table. You may find the description of both
//the heuristic at page 29
long boyer_moore_single(const std::string& text,
        const std::string& pat)
{
    long m{pat.size()},
         n{text.size()};
    std::vector<long> good_shift_table = compute_boyer_moore_shifts(pat),
        bad_shift_table = compute_bad_shift_table(pat);
    for(long i{0};i<=n-m;i++)
    {
        long j=m-1;
        for(;j>=0&&pat[j]==text[i+j];--j);
        if(j<0) return i;
        //Note that the index calculation for the bad
        //shift may be negative, whereas good_shift_table[i] >= 0
        //for all i.
        i+=std::max(good_shift_table[j],
                bad_shift_table[text[i+j]]-m+1+j);
    }
}

//Note that this is mostly identical to the 'single' implementation
std::vector<long> boyer_moore(const std::string& text,
        const std::string& pat)
{
    long m{pat.size()},
         n{text.size()};
    std::vector<long> good_shift_table = compute_boyer_moore_shifts(pat),
        bad_shift_table = compute_bad_shift_table(pat),
        results;
    for(long i{0};i<=n-m;)
    {
        long j=m-1;
        for(;j>=0&&pat[j]==text[i+j];--j);
        if(j<0){
            results.push_back(i);
            i+=good_shift_table[0];
        }
        else{
            i+=std::max(good_shift_table[j],
                bad_shift_table[text[i+j]]-m+1+j);
        }
    }
    return results;
}


