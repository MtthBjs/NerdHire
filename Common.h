#ifndef COMMON_H
#define COMMON_H

#include <unordered_set>
#include <vector>

typedef unsigned char u8;
typedef unsigned int u32;

extern u8 confusion[512];
extern u32 diffusion[32];

class Common
{
public:
    Common();
    ~Common();
    static void PrintU8ArrayWithName(u8 array[], unsigned int size, const char* name, bool with_endline = false);
    static void PrintU8ArrayWithName(std::vector<u8> vec, unsigned int size, const char* name, bool with_endline = false);
    static bool IsValueReachableInConfusion(u8 val);
};

#endif // COMMON_H
