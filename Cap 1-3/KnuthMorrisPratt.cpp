#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <tuple>

#include "../loadsampletext.hpp"
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

//pg 24
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

//This is the algorithm from CLRS
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

int main()
{
    cout.sync_with_stdio(false);
    string pat;
    int sample_index{0};
    cout<<"Which sample text you want to load?";
    cin >> sample_index;
    const string& text = text_sample::get_text(sample_index);
    cout<<"Text lenght: "<<text.size()<<endl;
    //Use both the functions, from Jewels and CLRS
    const int num_of_algorithms{2};
    std::tuple<std::string,std::function<std::vector<long>(const string&,const string&)>> algorithm[num_of_algorithms] = {
        std::make_tuple("Jewels",std::bind(&knuth_morris_pratt,std::placeholders::_1,std::placeholders::_2)),
        std::make_tuple("CLRS",std::bind(&CLRS::knuth_morris_pratt,std::placeholders::_1,std::placeholders::_2))
    };
    while(1)
    {
        cin >> pat;
        if(pat.empty()) break;
        for(int n{0};n<num_of_algorithms;n++)
        {
            cout<<"Running \""<<std::get<0>(algorithm[n])<<"\""<<endl;
            auto match = std::get<1>(algorithm[n])(text,pat);
            cout<<"Matches: "<<match.size()<<", at index(s):"<<endl;
            long display_cnt{1};
            for(auto e:match){
                cout<<e<<",";
                if(++display_cnt==50) break;
            }
            if(match.size()>0){
                if(display_cnt<match.size())
                    cout<<" ..."<<match.size()-display_cnt<<" more...\n";
                else
                    cout<<endl;
            }
            //Check for errors,just in case
            size_t i = text.find(pat);
            int ok_cnt{0};
            while(i!=string::npos)
            {
                if(i==match[ok_cnt])
                    ++ok_cnt;
                else{
                    cout<<"mismatch: "<<match[ok_cnt]<<" != "<<i<<endl;
                    break;
                }
                i = text.find(pat,i+1);
            }
            if(ok_cnt!=match.size())
                cout<<"This crap does not work!\n";
        }
    }
}
