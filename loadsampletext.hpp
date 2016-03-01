#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <memory>

const std::string sample_file1{"textsample.txt"};
const std::string sample_file2{"textsample2.txt"};

class text_sample
{
    std::vector<std::string> samples;
public:
    text_sample(std::vector<std::string> filenames)
    {
        for(auto filename:filenames)
        {
            std::ifstream in_file(filename);
            if(!in_file.is_open()) 
            {
                //lookup in the main directory
                in_file.open("../"+filename);
                if(!in_file.is_open())
                    throw std::runtime_error("Unable to open the sample text file");
            }
            std::string line,text;
            while(getline(in_file,line))
            {
                text += line;
            }
            samples.push_back(text);
        }
    }
    static const std::string& get_text(unsigned sample_num = 0)
    {
        static text_sample instance({sample_file1,
                sample_file2});
        if(sample_num>instance.samples.size())
            throw std::range_error("sample_num bigger than the amount of samples");
        return instance.samples[sample_num];
    }
};


struct freq_item
{
    long idx,
         freq;
    freq_item(long i,long f):
        idx{i},freq{f}{}
    bool operator<(const freq_item& other) const
    {
        return freq<other.freq;
    }
};

class text_sample_generator
{
    std::map<std::string,long> word_freq;
    std::priority_queue<freq_item> frequencies;
    std::map<std::string,long> word_idx;
    std::string sample_text;
    std::unique_ptr<std::unique_ptr<long[]>[]> word_graph;
    static constexpr bool skip_character(const char ch)
    {
        return (ch=='.'||ch==','||ch==':'||ch==';'||
                ch=='"'||ch=='!'||ch=='?'||ch=='('||
                ch==')'||ch==' '||ch=='\n'||ch=='\t'||
                ch=='['||ch==']'||ch=='-'||ch=='|'||
                ch=='\r');
    }
public:
    text_sample_generator()
    {
        const std::string& origin_text=text_sample::get_text();
        sample_text.reserve(origin_text.size());
        long prev{0};
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
        //Generate unique idx for the adjacency matrix
        //and map with the proper frequencies.
        //limit the amount of elements to 1000
        long size{0};
        for(auto entry:word_freq)
        {
            frequencies.push(freq_item(size,entry.second));
            word_idx[entry.first]=size++;
            if(size>=30) break;
        }
        //Generate the ajacency matrix
        //Let's the exceptions flow away.
        word_graph = std::make_unique<std::unique_ptr<long[]>[]>(size);
        for(long i{0};i<size;i++)
            word_graph[i] = std::make_unique<long[]>(size);
        //Scan again the text and fill the matrix
        std::string prev_word;
        current_word.clear();
        for(long i{0};i<origin_text.size();i++)
        {
            char cur_char=origin_text[i];
            if(!skip_character(cur_char)){
                current_word.push_back(cur_char);
            }
            else if(!current_word.empty()){
                if(!prev_word.empty()){
                    ++word_graph[word_idx[prev_word]][word_idx[current_word]];
                }
                prev_word = current_word;
                current_word.clear();
            }
        }
    }
};
