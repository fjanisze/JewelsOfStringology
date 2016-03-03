#include <fstream>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <sstream>

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

class sample_generator
{
public:
    virtual std::string get_text() = 0;
    virtual std::string get_generator_name() = 0;
    virtual ~sample_generator(){};
};

class text_sample_generator : public sample_generator
{
    long word_count;
    std::map<std::string,long> word_freq;
    std::vector<freq_item> frequencies;
    std::map<long,std::string> word_idx;
    std::string sample_text;
    std::vector<long> generated_text;
    const std::string& origin_text;
    std::stringstream output;
    static constexpr bool skip_character(const char ch)
    {
        return (ch=='.'||ch==','||ch==':'||ch==';'||
                ch=='"'||ch=='!'||ch=='?'||ch=='('||
                ch==')'||ch==' '||ch=='\n'||ch=='\t'||
                ch=='['||ch==']'||ch=='-'||ch=='|'||
                ch=='\r'||ch=='&'||ch=='\''||ch=='_');
    }
    long parse_sample_text();
    long generate_random_text();
public:
    text_sample_generator(long number_of_word):
        origin_text{text_sample::get_text()},
        word_count{number_of_word}
    {
        generate_random_text();
    }
    std::string get_generator_name(){
        return "Random Text Generator";
    }
    std::string get_text();
    ~text_sample_generator(){}
};

class fibonacci_text_generator : public sample_generator
{
    std::string text;
public:
    fibonacci_text_generator(long length)
    {
        std::string a("b"),
            b("a");
        for(long i{0};i<length;i++)
        {
            std::string n=b+a;
            a=b;
            b=n;
        }
        text=a+b;
    }
    std::string get_generator_name(){
        return "Fibonacci Text Generator";
    }
    std::string get_text(){
        return text;
    }
    ~fibonacci_text_generator(){}
};

const uint64_t m1  = 0x5555555555555555; //binary: 0101...
const uint64_t m2  = 0x3333333333333333; //binary: 00110011..
const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
const uint64_t m8  = 0x00ff00ff00ff00ff; //binary:  8 zeros,  8 ones ...
const uint64_t m16 = 0x0000ffff0000ffff; //binary: 16 zeros, 16 ones ...
const uint64_t m32 = 0x00000000ffffffff; //binary: 32 zeros, 32 ones
const uint64_t hff = 0xffffffffffffffff; //binary: all ones
const uint64_t h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,2,3...

class thue_morse_text_generator : public sample_generator
{
    std::string text;
    int cout_bit_set(uint64_t x) {
        x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
        x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits 
        x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits 
        x += x >>  8;  //put count of each 16 bits into their lowest 8 bits
        x += x >> 16;  //put count of each 32 bits into their lowest 8 bits
        x += x >> 32;  //put count of each 64 bits into their lowest 8 bits
        return x & 0x7f;
    }
public:
    thue_morse_text_generator(long length)
    {
        text.reserve(length);
        for(long i{0};i<length;i++)
            text+=(cout_bit_set(i)%2==0)?'a':'b';
    }
    std::string get_generator_name(){
        return "Thue-Morse Text Generator";
    }
    std::string get_text(){
        return text;
    }
    ~thue_morse_text_generator(){}
};
