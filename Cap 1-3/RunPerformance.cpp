#include <iostream>
#include <chrono>
#include <functional>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include "../sampletext.hpp"
#include "Functions.hpp"
using namespace std;
using placeholders::_1;
using placeholders::_2;

using search_function = function<vector<long>(const string&,const string&)>;

template<typename TEXT_GEN>
void bench(long text_size,const vector<string>& search_patterns)
{
    TEXT_GEN generator(text_size);
    cout<<"Using "<<generator.get_generator_name()<<", text size: "<<text_size<<endl;
    string text=generator.get_text();
    tuple<string,search_function> functions[2] = {
        make_tuple("Knuth-Morris-Pratt",bind(&knuth_morris_pratt,_1,_2)),
        make_tuple("Boyer-Moore",bind(&boyer_moore,_1,_2))
    };
    for(int pat_idx{0};pat_idx<search_patterns.size();pat_idx++)
    {
        for(int i{0};i<2;i++)
        {
            cout<<"Running "<<get<0>(functions[i])<<", pat: \""<<
                search_patterns[pat_idx]<<"\", time: ";
            auto time_start=chrono::high_resolution_clock::now();
            long c = get<1>(functions[i])(text,search_patterns[pat_idx]).size();
            auto time_stop=chrono::high_resolution_clock::now();
            cout<<chrono::duration_cast<chrono::milliseconds>(
                    time_stop-time_start).count()<<"ms ("<<c<<")\n";
        }
        cout<<endl;
    }
}

int main(int argc,char** argv)
{
    cout.sync_with_stdio(false);
    bench<text_sample_generator>(50000000,{"kill","house","ban","a","between","little","ill","walhalla"});
    bench<fibonacci_text_generator>(35,{"a","abba","baabaaba","baabaab","ab","aab"});
    bench<thue_morse_text_generator>(50000000,{"abba","abbaabba","a","ab","aabb"});
    return 0;
}