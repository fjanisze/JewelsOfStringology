#include <vector>
#include <string>

extern std::vector<long> compute_prefixes(const std::string& pat);
extern long naive_scan(const std::string& pat,long p,long q);
extern std::vector<long> compute_prefixes_naive(const std::string& pat);
extern std::vector<long> compute_suffixes(const std::string& pat);
extern std::vector<long> compute_suffixes_naive(const std::string& pat);
extern std::vector<long> compute_bad_shift_table(const std::string& pat);
extern std::vector<long> compute_boyer_moore_shifts(const std::string& pat);

extern std::vector<long> knuth_morris_pratt(const std::string& text,const std::string& pat);
namespace CLRS
{
extern std::vector<long> knuth_morris_pratt(const std::string& text,const std::string& pat);
}
extern std::vector<long> boyer_moore(const std::string& text,const std::string& pat);
