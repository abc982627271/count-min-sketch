#include <stdlib.h>
#include <math.h> 
#include <assert.h>
#include <limits>
#include <algorithm>
#include <iostream>
#include "count_min_sketch.h"
#include "MurmurHash2.h"

#define CMS_HASH(item, itemlen, i) MurmurHash2(item, itemlen, i)

CountMinSketch::CountMinSketch(uint32_t width, uint32_t depth){
    width_ = width;
    depth_ = depth;
    counter_ = 0;
    array_.resize(width*depth);
    for(int i=0; i < array_.size(); i++){
        array_[i] = 0;
    }
}

CountMinSketch::~CountMinSketch() {
    
}

uint32_t CountMinSketch::IncrBy(const std::string& item, uint32_t value) {

    uint32_t minCount = uint32_t(-1); // 4294967295 
    for (int i = 0; i < depth_; ++i) {
        uint32_t hash = CMS_HASH(item.c_str(), item.length(), i);
        array_[(hash % width_) + (i * width_)] += value;
        minCount = std::min(minCount, array_[(hash % width_) + (i * width_)]);
    }
    counter_ += value;
    return minCount;
}

uint32_t CountMinSketch::Query(const std::string& item) {

    uint32_t minCount = uint32_t(-1); // 4294967295
    for (uint32_t i = 0; i < depth_; ++i) {
        uint32_t hash = CMS_HASH(item.c_str(), item.length(), i);
        minCount = std::min(minCount, array_[(hash % width_) + (i * width_)]);
    }
    return minCount;
}

void CountMinSketch::Reset(const std::string& item) {
    for (int i = 0; i < depth_; ++i) {
        uint32_t hash = CMS_HASH(item.c_str(), item.length(), i);
        counter_ -= array_[(hash % width_) + (i * width_)];
        array_[(hash % width_) + (i * width_)] = 0;
    }
}


void CountMinSketch::DimFromProb(double error, double delta, uint32_t *width, uint32_t *depth) {
    assert(error > 0 && error < 1);
    assert(delta > 0 && delta < 1);

    *width = ceil(2 / error);
    *depth = ceil(log10f(delta) / log10f(0.5));
}


void CMS_Merge(CountMinSketch* dest, std::vector<CountMinSketch*> src, std::vector<long long> weights){
    //src 和 weights的size必须一致
    assert(src.size() == weights.size());

    uint32_t itemCount = 0;
    uint32_t cmsCount = 0;
    uint32_t width = dest->width_;
    uint32_t depth = dest->depth_;

    uint32_t quantity = src.size();

    for (uint32_t i = 0; i < depth; ++i) {
        for (uint32_t j = 0; j < width; ++j) {
            itemCount = 0;
            for (uint32_t k = 0; k < quantity; ++k) {
                itemCount += src[k]->array_[(i * width) + j] * weights[k];
            }
            dest->array_[(i * width) + j] = itemCount;
        }
    }

    for (uint32_t i = 0; i < quantity; ++i) {
        cmsCount += src[i]->counter_ * weights[i];
    }
    dest->counter_ = cmsCount;
}


/************ used for debugging *******************/
void CountMinSketch::PrintDebug() {

    for (int i = 0; i < depth_; ++i) {
        for (int j = 0; j < width_; ++j) {
            std::cout << array_[(i * width_) + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Counter is " << counter_ << std::endl;
}
