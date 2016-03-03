//Naive implementation of the searching algorithms from JoS
#include <vector>
#include <string>

//pg20
std::vector<long> brute_force1(const std::string& text,
        const std::string& pat)
{
    size_t m{pat.size()},
           n{text.size()};
    std::vector<long> results;
    for(long i{0};i<=n-m;i++)
    {
        long j{0};
        while(j<m&&pat[j]==text[j+i])
            ++j;
        if(j==m){
            results.push_back(i);
        }
    }
    return results;
}

//pg27
std::vector<long> brute_force2(const std::string& text,
        const std::string& pat)
{
    size_t m{pat.size()},
           n{text.size()};
    std::vector<long> results;
    for(long i{0};i<=n-m;i++)
    {
        long j{(long)m-1};
        while(j>=0&&pat[j]==text[j+i])
            --j;
        if(j<0){
            results.push_back(i);
        }
    }
    return results;
}

