#include "SystemSolver.h"
#include <iostream>
//#include <stdlib.h>

const unsigned int SZ = SystemSolver::SIZE;

void SystemSolver::set(const unsigned array[SZ][SZ+1])
{
    set(array);
}

void SystemSolver::set(const unsigned **array)
{
	_matrix = mb();
	
	for(unsigned i = 0 ; i < SZ ; ++i)
	{
        auto line = vb(SZ+1, 0);
        _matrix.push_back(line);
        
		for(unsigned j = 0 ; j < SZ+1 ; ++j)
		{
			_matrix[i][j] = (array[i][j] == 0 ? 0 : 1);
		}
	}
	
	_is_set = true;
}

void SystemSolver::set(const mb& matrix)
{
    _matrix = matrix;
    _is_set = true;
}

bool SystemSolver::solve()
{
	if(!_is_set)
	{
        std::cout << "Need to call set to solve" << std::endl;
		return false;
	}
	
	for(unsigned k = 0 ; k < SZ-1 ; ++k)
	{
		if(_matrix[k][k] == 0)
		{
            bool found_proper_line = false;
            
            for(unsigned i = k + 1 ; i < SZ ; ++i)
            {
                if(_matrix[i][k] != 0)
                {
                    found_proper_line = true;
                    swap_lines(k,i);
                    break;
                }
            }
            
            if(!found_proper_line)
            {
                return false;
            }
		}
		
		for(unsigned i = k + 1 ; i < SZ ; ++i)
		{
			if(_matrix[i][k] != 0)
			{
				for(unsigned j = k ; j < SZ+1 ; ++j)
				{
					_matrix[i][j] = _matrix[i][j] ^ _matrix[k][j];
				}
			}
		}
	}
	
	_solution = vb(SZ, 0);

	for(int m = SZ-1 ; m >= 0 ; --m)
	{
		_solution[m] = _matrix[m][SZ];
		for(unsigned n  = m + 1 ; n < SZ ; ++n)
		{
			if(_solution[n] != 0)
			{
				_solution[m] = _solution[m] ^ _matrix[m][n];
			}
		}
	}
	
    _is_solved = true;
	return true;
}

vb SystemSolver::get_solution()
{
	return _solution;
}

void SystemSolver::PrintMatrix()
{
	if(! _is_set)
	{
		std::cout << "Need to call set() to be able to print matrix" << std::endl;
		return;
	}
	
	for(unsigned i = 0 ; i < SZ ; ++i)
	{
		for(unsigned j = 0 ; j < SZ+1 ; ++j)
		{
			std::cout << _matrix[i][j];
		
			if(j < SZ)
			{
				std::cout << ",";
			}
		}
		
		std::cout<< std::endl;
	}
}

void SystemSolver::PrintSolution()
{
	if(!_is_solved)
	{
		std::cout << "Need to call solve() to be able to print solution" << std::endl;
		return;
	}
	
	for(unsigned i = 0 ; i < SZ ; ++i)
	{
		std::cout << _solution[i];
		
		if(i < SZ - 1)
		{
			std::cout << ",";
		}
	}
	
	std::cout << std::endl;
}

void SystemSolver::swap_lines(unsigned int l1, unsigned int l2)
{
    if(l1 >= SZ || l2 >= SZ || !_is_set)
    {
        return;
    }
    
    auto tmp = vb(_matrix[l1]);
    _matrix[l1] = _matrix[l2];
    _matrix[l2] = tmp;
}

SystemSolver::SystemSolver() : _is_set(false), _is_solved(false)
{	
}

SystemSolver::~SystemSolver()
{
}

