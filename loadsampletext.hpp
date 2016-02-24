#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

class text_sample
{
    std::vector<std::string> samples;
public:
    text_sample(std::vector<std::string> filenames)
    {
        for(auto filename:filenames)
        {
            std::ifstream in_file(filename);
            if(!in_file.is_open()) throw std::runtime_error("Unable to open the sample text file");
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
        static text_sample instance({"../textsample.txt",
                "../textsample2.txt"});
        if(sample_num>instance.samples.size())
            throw std::range_error("sample_num bigger than the amount of samples");
        return instance.samples[sample_num];
    }
};

