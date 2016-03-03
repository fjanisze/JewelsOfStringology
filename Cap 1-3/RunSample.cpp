#include <iostream>
#include <tuple>
#include <functional>
#include "Functions.hpp"
#include "../sampletext.hpp"
using namespace std;

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
    const int num_of_algorithms{3};
    std::tuple<std::string,std::function<std::vector<long>(const string&,const string&)>> algorithm[num_of_algorithms] = {
        std::make_tuple("Jewels KMP",std::bind(&knuth_morris_pratt,std::placeholders::_1,std::placeholders::_2)),
        std::make_tuple("Jewels BM",std::bind(&boyer_moore,std::placeholders::_1,std::placeholders::_2)),
        std::make_tuple("CLRS",std::bind(&CLRS::knuth_morris_pratt,std::placeholders::_1,std::placeholders::_2))
    };
    while(1)
    {
        cout<<"Input the pattern: ";
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
            long i = text.find(pat);
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

