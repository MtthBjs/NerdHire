#ifndef SECONDLOOP_H
#define SECONDLOOP_H

#include <vector>
#include "Common.h"

class SecondLoop
{
public:
    SecondLoop();
    ~SecondLoop();
    void Forward(u8 input[32], u8 output[32]);
    void ReverseOneSolutionOnly(u8 target[32], u8 solution[32]); // /!\ one solution only
    std::vector<std::vector<std::pair<u8,u8>>>& ReverseFindAllSolutionsAsPairs(u8 target[32]);
    void InitSolutionIterator(unsigned int size_of_solutions = 16);
    bool GetNextSolution(u8 solution[32]);
    unsigned long long GetNumberOfSolutions(); // Returns 0 if overflow

private:
    std::vector<std::vector<std::pair<u8,u8>>> _allSolutions;
    std::vector<unsigned int> _currentSolutionId;
    unsigned int _sizeOfSolutions;

};

#endif // SECONDLOOP_H
