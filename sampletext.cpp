#include "sampletext.hpp"
#include <sstream>
using namespace std;

long text_sample_generator::parse_sample_text()
{
    sample_text.reserve(origin_text.size());
    std::string current_word;
    for(long i{0};i<origin_text.size();i++)
    {
        char cur_char=origin_text[i];
        if(!skip_character(cur_char)){
            current_word.push_back(cur_char);
        }
        else if(!current_word.empty()){
            ++word_freq[current_word];
            current_word.clear();
        }
    }
    long size{0},tot_count{0};
    for(auto entry:word_freq)
    {
        frequencies.emplace_back(freq_item(size,entry.second));
        word_idx[size++]=entry.first;
        tot_count+=entry.second;
    }
    std::sort(frequencies.rbegin(),frequencies.rend());
    return tot_count;
}

long text_sample_generator::generate_random_text()
{
    long tot_count = parse_sample_text();
    double scale = (double)word_count/tot_count;
    tot_count = 0;
    for(auto& entry:frequencies)
    {
        entry.freq = std::max(1.0,entry.freq*scale);
        tot_count += entry.freq;
        if(tot_count>=word_count) break;
    }
    std::vector<long> text_indexes(word_count);
    std::iota(text_indexes.begin(),text_indexes.end(),0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(text_indexes.begin(),text_indexes.end(),gen);
    generated_text.resize(word_count);
    long text_idx{0};
    for(long i{0};i<frequencies.size()&&text_idx<word_count;i++)
    {
        while(text_idx<word_count&&frequencies[i].freq>0)
        {
            generated_text[text_indexes[text_idx++]]=frequencies[i].idx;
            --frequencies[i].freq;
        }
    }
    return text_idx;
}

std::string text_sample_generator::get_text()
{
    std::stringstream output;
    //Output the text, adding some random punctuation
    std::random_device rd;
    std::mt19937 gen(rd());
    const char punctuation[5]={'.',',','?','!',';'};
    for(long i{0};i<generated_text.size();i++)
    {
        if(i%30==0) output<<std::endl;
        else{
            if(std::generate_canonical<double,10>(gen)<.10)
                output<<punctuation[i%5];
            output<<" ";
        }
        output<<word_idx[generated_text[i]];
    }
    return output.str();
}
