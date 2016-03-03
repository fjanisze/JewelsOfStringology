//Code from 3.2, Jewels of Stringology
#include <string>
#include <vector>
using namespace std;

//Find the longest k for which pat[p...p+k-1p]=pat[q...q+k-1] with k>=1
long naive_scan(const string& pat,
        long p,long q)
{
    size_t k{0},len{pat.size()};
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
    for(long i{1};i<pat.size();i++)
    {
        table[i]=naive_scan(pat,0,i);
    }
    table[0]=pat.size();
    return table;
}

//pg 38
vector<long> compute_prefixes(const string& pat)
{
    vector<long> table(pat.size(),0);
    long s{0};
    for(long i{1};i<pat.size();i++)
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
    table[0]=pat.size();
    return table;
}

