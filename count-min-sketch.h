#include <vector>
#include <string>
#include <stdint.h> 
// define some constant
# define LONG_PRIME 4294967311l

//https://www.sohu.com/a/149108288_211120
//需要n个hash函数，和n个数组，对于任何一个数，hash到n个数组中，统计时返回n个数组中对应hash位置的最小值。

class CountMinSketch
{
public:
    CountMinSketch(uint32_t width, uint32_t depth);
    ~CountMinSketch();

    /*  Recommends width & depth for expected n different items,
    with probability of an error  - prob and over estimation
    error - overEst (use 1 for max accuracy) */
    void DimFromProb(double overEst, double prob, uint32_t *width, uint32_t *depth);    

    /*  Increases item count in value.
    Value must be a non negative number */
    uint32_t IncrBy(const std::string& item, uint32_t value);

    /* Returns an estimate counter for item */
    uint32_t Query(const std::string& item);

    /*Reset the counter for item*/
    void Reset(const std::string& item);

    /*  Merges multiple CMSketches into a single one.
        All sketches must have identical width and depth.
        dest must be already initialized.
    */
    friend void CMS_Merge(CountMinSketch* dest, std::vector<CountMinSketch*> src, std::vector<long long> weights);

    void PrintDebug();

private:
    uint32_t width_;
    uint32_t depth_;
    std::vector<uint32_t> array_;
    uint32_t counter_;
};
