#include <string.h>
#include <iostream>
#include "FirstLoop.h"

FirstLoop::FirstLoop()
{
}

FirstLoop::~FirstLoop()
{
}

void FirstLoop::ForwardSingle(u8 input[32], u8 output[32])
{
    ForwardLoop(input, output, 1);
}

void FirstLoop::ForwardLoop(u8 input[32], u8 output[32])
{
    ForwardLoop(input, output, 256);
}

void FirstLoop::ForwardLoop(u8 input[32], u8 output[32], unsigned int loops)
{
    u8 tmp_1[32];
    u8 tmp_2[32];
    memcpy(tmp_2,input,32);
    
	for(u32 i=0;i<loops;i++)
	{
		for(u8 j=0;j<32;j++)
		{
            tmp_1[j]=confusion[tmp_2[j]];
            tmp_2[j]=0;
		}

        for(u8 j=0;j<32;j++)
        {
            for(u8 k=0;k<32;k++)
            {
                tmp_2[j]^=tmp_1[k]*((diffusion[j]>>k)&1);
            }
        }
	}
    
    memcpy(output, tmp_2, 32);
}

// Important choice: consider only the *first* index of confusion which reaches the target
// Important note: this only considers the first 16 bits of target
void FirstLoop::ReverseSingleOneSolutionOnly(u8 target[32], u8 solution[32])
{
    auto deducted_intermediate_vec = std::vector<u8>(32, 0);
	SolveSystemForU8s(target, deducted_intermediate_vec);
	
	u8 deducted_intermediate[32]; for(unsigned int i = 0 ; i < 32 ; ++i) deducted_intermediate[i] = deducted_intermediate_vec[i];
	
	for(unsigned i = 0 ; i < 16; ++i)
	{
		bool found = false;
		
		for(unsigned j = 0 ; j < 256 ; ++j)
		{
			if(confusion[j] == deducted_intermediate[i])
			{
				solution[i] = j;
				found = true;
				break;
			}
		}
		
		if(!found)
		{
			std::cout << "Could not match target value " << (unsigned int)deducted_intermediate[i] << " in confusion at iteration #" << i << std::endl;
		}
	}
}


// TODO: this does not work at all. Need to change it like this:
// - the return type for "solutions" is correct
// - but solutions is NOT a list of strings!
// - instead, solutions uses the same technique as SecondLoop:
// - solutions[0] = list of possible u8's for character at position 0 in a solution to the reverse problem
// - ...
// - solutions[i] = list of possible u8's for character at position i in a solution to the reverse problem
// - then, we need, again similarly to SecondLoop, a method to init an iterator over solutions + a method to get next solution
bool FirstLoop::ReverseSingleAllSolutions(u8 target[32], std::vector<std::vector<u8>>& solutions)
{
    solutions.clear();
    
    auto deducted_intermediate_vec = std::vector<u8>(32, 0);
	
	if(!SolveSystemForU8s(target, deducted_intermediate_vec))
	{
		return false;
	}
	
	u8 deducted_intermediate[32]; for(unsigned int i = 0 ; i < 32 ; ++i) deducted_intermediate[i] = deducted_intermediate_vec[i];
	
	for(unsigned i = 0 ; i < 32; ++i)
	{
		solutions.push_back(std::vector<u8>());
		
		bool found = false;
		
        // Although there are 512 values in confusion, we only look at the ones whose index can fit on a u8 because this is the size of any solution input
		for(unsigned int j = 0 ; j < 256 ; ++j)
		{
			if(confusion[j] == deducted_intermediate[i])
			{
				found = true;
				solutions[i].push_back((u8)j);
			}
		}
		
		if(!found)
		{
			return false;
		}
	}
}

bool FirstLoop::SolveSystemForU8s(u8 target[32], std::vector<u8>& solution)
{
    auto solutions = std::vector<vb>(8, vb(32, 0));
    
    for(unsigned i = 0 ; i < 8 ; ++i)
    {
        if(!SolveSystemForOneBit(target, i, solutions[i]))
        {
            // TMP
            std::cout << "No solution for the system for bit number " << i << std::endl;
            
            return false;
        }
    }
    
    solution = std::vector<u8>(32, 0);
    
    for(unsigned i = 0 ; i < 8 ; ++i)
    {
        for(unsigned j = 0 ; j < 32 ; ++j)
        {
            solution[j] += (solutions[i][j] << i);
        }
    }
    
    //TMP
    /*std::cout << "Solution of system:" << std::endl;
    for(unsigned i = 0; i < 32 ; ++i)
    {
        std::cout << (unsigned int)solution[i] << ",";
    }
    std::cout << std::endl;*/
    
    return true;
}

bool FirstLoop::SolveSystemForOneBit(u8 target[32], unsigned int bitNumber, vb& solution)
{
    if(bitNumber >= 8)
    {
        return false;
    }

    // TMP
    //std::cout << "Let's solve the system" << std::endl;

    // build the vector for target
    auto targetVec = vb(32, 0);
    for(unsigned i = 0 ; i < 32 ; ++i)
    {
        targetVec[i] = ((target[i]>>bitNumber)&1);
    }
    
    // build the matrix, including target vector
    auto system = mb();
    for(unsigned i = 0 ; i < 32 ; ++i)
    {
        system.push_back(vb(33,0));
        
        for(unsigned j = 0 ; j < 32 ; ++j)
        {
            system[i][j] = ((diffusion[i]>>j)&1);
        }
        
        system[i][32] = targetVec[i];
    }
    
    // Solve the system
    auto syso = SystemSolver();
    syso.set(system);
    
    // TMP
    //syso.PrintMatrix();
    
    if(syso.solve())
    {
        solution = syso.get_solution();
        
        //TMP
        //syso.PrintSolution();
        
        return true;
    }
    
    // TMP
    //std::cout << "No solution..." << std::endl;
    
    return false;
}