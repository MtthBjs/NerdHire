#ifndef FIRSTLOOP_H
#define FIRSTLOOP_H

#include <vector>
#include "Common.h"
#include "SystemSolver.h"

class FirstLoop
{
public:
    FirstLoop();
    ~FirstLoop();
    void ForwardSingle(u8 input[32], u8 output[32]);
    void ForwardLoop(u8 input[32], u8 output[32]);
    void ForwardLoop(u8 input[32], u8 output[32], unsigned int loops);
    void ReverseSingleOneSolutionOnly(u8 target[32], u8 solution[32]); // /!\ choice = only 1st solution
    void ReverseSingleAllSolutions(u8 target[32], std::vector<std::vector<u8>>& solutions);

private:
    bool SolveSystemForU8s(u8 target[32], std::vector<u8>& solution);
    bool SolveSystemForOneBit(u8 target[32], unsigned int bitNumber, vb& solution);
};

#endif // FIRSTLOOP_H
