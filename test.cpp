#include <iostream>
#include <string> // strlen
#include <math.h>   // rand
#include <time.h>   // clock

#include "count_min_sketch.h"

#define AMOUNT 10000


int visualTest();
int massiveTest();

int main() {
    visualTest();
    //massiveTest();

    return 0;
}

int visualTest() {
    std::cout << "visualTest" << std::endl;
    CountMinSketch *a = new CountMinSketch(20, 5);
    CountMinSketch *b = new CountMinSketch(20, 5);

    a->IncrBy("C", 1);
    a->PrintDebug();
    a->IncrBy("C", 15);
    a->PrintDebug();
    b->IncrBy("C", 100);
    b->PrintDebug();
    b->IncrBy("M", 35);
    b->PrintDebug();
    b->IncrBy("S", 87);
    b->PrintDebug();
    b->IncrBy("CMS", 12);
    b->PrintDebug();

    std::cout << "C count at a is: " << a->Query("C") << std::endl;
    std::cout << "C count at b is: " << b->Query("C") << std::endl;
    std::cout << "M count at b is: " << b->Query("M") << std::endl;
    std::cout << "S count at b is: " << b->Query("S") << std::endl;
    std::cout << "CMS count at b is: " << b->Query("CMS") << std::endl;

    CountMinSketch *c = new CountMinSketch(20, 5);
    std::vector<CountMinSketch*> list{a, b};
    std::vector<long long> weight = {3, 2};
    CMS_Merge(c, list, weight);
    std::cout << "q " << a->Query("C") << std::endl;
    std::cout << "q " << b->Query("C") << std::endl;
    std::cout << "q " << c->Query("C") << std::endl;
    c->PrintDebug();
    return 0;
}

int massiveTest() {
    CountMinSketch *cms = new CountMinSketch(AMOUNT * 2.7, 5);
    std::vector<int> original(AMOUNT, 0);
    int totalErr = 0;

    srand(0);
    std::string str;
    for (int i = 0; i < AMOUNT; ++i) {
        original[i] = rand() % 1000;
    }

    clock_t begin = clock();
    for (int i = 0; i < AMOUNT; ++i) {
        str.append(std::to_string(i));
        for (int j = 0; j < original[i]; ++j) {
            cms->IncrBy(str, 1);
        }
    }
    clock_t end = clock();

    str.clear();

    for (int i = 0; i < AMOUNT; ++i) {
        str.append(std::to_string(i));
        uint32_t res = cms->Query(str);
        if (res > original[i]) {
            ++totalErr;
        }
    }

    std::cout << "counter of err is " << totalErr << std::endl;
    std::cout << "execution time " << (double)(end - begin) / CLOCKS_PER_SEC << std::endl;

    return 0;
}
