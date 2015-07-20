#include <string.h>
#include <iostream>
#include "SecondLoop.h"

SecondLoop::SecondLoop()
{
    _allSolutions = std::vector<std::vector<std::pair<u8,u8>>>();
}

SecondLoop::~SecondLoop()
{
}

void SecondLoop::Forward(u8 input[32], u8 output[32])
{
    u8 tmp_2[32];
    memcpy(tmp_2,input,32);
    
	for(u8 i=0;i<16;i++)
    {
       output[i] = confusion[tmp_2[i*2]]^confusion[tmp_2[i*2+1]+256];
    }
}

void SecondLoop::ReverseOneSolutionOnly(u8 target[32], u8 solution[32])
{
    Common::PrintU8ArrayWithName(target, 16, "ReverseSecondLoop__target");
	
	for(unsigned i = 0 ; i < 16 ; ++i)
	{
		for(unsigned j = 0 ; j < 256 ; ++j)
		{
			for(unsigned k = 256 ; k < 512 ; ++k)
			{
				if((confusion[j]^confusion[k]) == target[i])
				{
					if(Common::IsValueReachableInConfusion(j) && Common::IsValueReachableInConfusion(k-256))
					{
						solution[2*i] = j;
						solution[(2*i)+1] = k-256;
						goto endofinnerloops;
					}
				}
			}
		}
		
		endofinnerloops:
		int a = 1;
        ++a;
	}
	
	Common::PrintU8ArrayWithName(solution, 32, "ReverseSecondLoop__deducted_input");
	
	u8 out_control[32];
	Forward(solution, out_control);
	Common::PrintU8ArrayWithName(out_control, 16, "ReverseSecondLoop__out_control");
}

std::vector<std::vector<std::pair<u8,u8>>>& SecondLoop::ReverseFindAllSolutionsAsPairs(u8 target[32])
{
    _allSolutions = std::vector<std::vector<std::pair<u8,u8>>>(16, std::vector<std::pair<u8,u8>>());
	
	for(unsigned i = 0 ; i < 16 ; ++i)
	{
		for(unsigned j = 0 ; j < 256 ; ++j)
		{
			for(unsigned k = 256 ; k < 512 ; ++k)
			{
  				if((confusion[j]^confusion[k]) == target[i]
                    && Common::IsValueReachableInConfusion(j)
                    && Common::IsValueReachableInConfusion(k-256))
				{
					_allSolutions[i].push_back(std::make_pair(j,k-256));
				}
			}
		}
	}

    return _allSolutions;
}

size_t highestOneBitPosition(unsigned long long a)
{
    size_t ret = 0;
    while(a!=0)
    {
        a=a>>1;
        ++ret;
    }
    return ret;
}

bool multiplication_is_safe(unsigned long long a, size_t b)
{
    size_t a_bits=highestOneBitPosition(a), b_bits=highestOneBitPosition(b);
    return (a_bits+b_bits<=64);
}

unsigned long long SecondLoop::GetNumberOfSolutions()
{
    unsigned long long nb = 1;
    for(unsigned i = 0 ; i < _allSolutions.size() ; ++i)
    {
        size_t param = _allSolutions[i].size();
        if(multiplication_is_safe(nb, param))
        {
            nb *= param;
        }
        else
        {
            std::cout << "Cannot compute number of solutions because of integer overflow" << std::endl;
            return 0;
        }
    }
    return nb;
}

void SecondLoop::InitSolutionIterator(unsigned int size_of_solutions)
{
    _currentSolutionId = std::vector<unsigned int>(16, 0);
    _sizeOfSolutions = size_of_solutions;
}

bool SecondLoop::GetNextSolution(u8 solution[32])
{
    if(_allSolutions.empty())
        return false;
    
    for(unsigned i = 0 ; i < _sizeOfSolutions ; ++i)
    {
        solution[2*i] = _allSolutions[i][_currentSolutionId[i]].first;
        solution[(2*i)+1] = _allSolutions[i][_currentSolutionId[i]].second;
    }
    
    bool has_next = false;

    for(unsigned int i = 0 ; i < _sizeOfSolutions ; ++i)
    {
        if(_currentSolutionId[i] < _allSolutions[i].size())
        {
            ++_currentSolutionId[i];
            has_next = true;
            break;
        }
        else
        {
            if(i + 1 == _sizeOfSolutions)
            {
                break;
            }
            
            _currentSolutionId[i] = 0;
        }
    }

    return has_next;
}