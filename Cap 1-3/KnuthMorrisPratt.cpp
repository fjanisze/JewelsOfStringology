//Code from 2.1 and 3.1, Jewels of Stringology
#include <string>
#include <vector>
using namespace std;

//pg 35
std::vector<long> compute_strong_board(const std::string& pat)
{
    size_t len{pat.size()};
    long t{-1};
    std::vector<long> board(len+1,-1);
    for(long i{0};i<=len;i++)
    {
        if(i==0) board[i] = -1;
        else if(pat[t]!=pat[i])
            board[i] = t;
        else board[i] = board[t];
        while(t>=0 && pat[t]!=pat[i])
            t = board[t];
        ++t;
    }
    return board;
}

//pg 24 (This will search for all the match of pat in text)
std::vector<long> knuth_morris_pratt(const std::string& text,
        const std::string& pat)
{
    auto board = compute_strong_board(pat);
    size_t n{text.size()},m{pat.size()};
    std::vector<long> res;
    for(long i{0},j{0};i<=n-m;)
    {
        while(j<m && pat[j]==text[i+j])
            ++j;
        if(j==m){
            res.push_back(i);
            i += (j - board[j]);
            j = 0;
        }
        else{
            i += (j - board[j]);
            j = board[j]>0?board[j]:0;
        }
    }
    return res;
}

//Original version which search for the first match only
long knuth_morris_pratt_single(const std::string& text,
        const std::string& pat)
{
    auto board = compute_strong_board(pat);
    size_t n{text.size()},m{pat.size()};
    for(long i{0},j{0};i<=n-m;)
    {
        while(j<m && pat[j]==text[i+j])
            ++j;
        if(j==m)
            return i;
        i += (j - board[j]);
        j = board[j]>0?board[j]:0;
    }
    return -1; //Not found
}

//This is the algorithm from CLRS, for comparison purpose
namespace CLRS
{
    std::vector<long> prefix_function(const string& str)
    {
        vector<long> pi(str.size());
        pi[0] = -1;
        for(int q{1},k{-1};q<str.size();q++)
        {
            while(k>=0 && str[k+1] != str[q])
                k = pi[k];
            if(str[k+1]==str[q])
                ++k;
            pi[q] = k;
        }
        return pi;
    }

    std::vector<long> knuth_morris_pratt(const string& text, const string& pattern)
    {
        auto pi = prefix_function(pattern);
        std::vector<long> res;
        for(int q{0},k{-1};q<text.size();q++)
        {
            //1)
            while(k>=0 && pattern[k+1]!=text[q])
                k = pi[k];
            //2)
            if(pattern[k+1] == text[q])
                ++k;
            if(k==pattern.size()-1)
            {
                res.push_back(q-k);
                k = pi[k];
            }
        }
        return res;
    }

}

