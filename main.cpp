#include <string.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <utility>
#include <bitset>
#include <set>
#include <unordered_set>
#include <ctime>
#include "Common.h"
#include "SystemSolver.h"
#include "SecondLoop.h"
#include "FirstLoop.h"

typedef std::unordered_map<u32,u32> hash_map;

auto inv_confusion = std::unordered_map<u8,std::set<u32>>();
bool is_inv_confusion_set = false;
std::unordered_set<u8> *confusion_values = nullptr;


u8 input[32]={
//change only this :
0x66,0xd5,0x4e,0x28,0x5f,0xff,0x6b,0x53,0xac,0x3b,0x34,0x14,0xb5,0x3c,0xb2,0xc6,
0xa4,0x85,0x1e,0x0d,0x86,0xc7,0x4f,0xba,0x75,0x5e,0xcb,0xc3,0x6e,0x48,0x79,0x8f
//99,0,81,1,17,0,80,0,228,1,24,1,35,3,188,0,19,1,85,0,241,2,95,0,59,0,105,0,23,0,91,0 // NOK
//
};

void Forward(u8 input[32],u8 out[32],u8 conf[512],u32 diff[32])
{
	for(u32 i=0;i<256;i++)
	{
		for(u8 j=0;j<32;j++)
		{
			out[j]=conf[input[j]];
			input[j]=0;
		}

		for(u8 j=0;j<32;j++)
 			for(u8 k=0;k<32;k++)
				input[j]^=out[k]*((diff[j]>>k)&1);
	}
	for(u8 i=0;i<16;i++)
		out[i]=conf[input[i*2]]^conf[input[i*2+1]+256];
}

bool IsValueReachableInConfusion(u8 val)
{
	if(confusion_values == nullptr)
	{
		confusion_values = new std::unordered_set<u8>();
		for(u32 i = 0 ; i < 512 ; ++i)
		{
			if(confusion_values->find(confusion[i]) == confusion_values->cend())
			{
				confusion_values->insert(confusion[i]);
			}
		}
	}
	
	return confusion_values->find(val) != confusion_values->cend();
}

void PrintU8ArrayWithName(u8 array[], unsigned int size, const char* name, bool with_endline = false)
{
	if(with_endline)
	{
		for(unsigned int i = 0 ; i < size ; ++i)
		{
			std::cout << name << "[" << i << "]=" << (unsigned int)array[i] << std::endl;
		}	
	}
	else
	{
		std::cout << name << std::endl;
		for(unsigned int i = 0 ; i < size ; ++i)
		{
			std::cout << (unsigned int)array[i] << ",";
		}
		std::cout << std::endl;
	}
}

u8 ComputeTmp2Index0LastIteration(u8 tmp_1[32],u8 conf[512],u32 diff[32]);

u8 ForwardUnitary2(u8 input[32],u8 conf[512],u32 diff[32], u8 index)
{
    u8 tmp_1[32];
    u8 tmp_2[32];
    memcpy(tmp_2,input,32);
    
    u8 tmp_22 = input[index];
        
	for(u32 i=0;i<256;i++)
	{
		for(u8 j=0;j<32;j++)
		{
            //tmp_1[j]=conf[tmp_2[j]];
            tmp_1[j]=conf[tmp_22];
            tmp_2[j]=0;
            
            tmp_22 = 0;
		}

        for(u8 j=0;j<32;j++)
        {
            for(u8 k=0;k<32;k++)
            {
                tmp_2[j]^=tmp_1[k]*((diff[j]>>k)&1);
                
                if(j==index)
                    tmp_22^=tmp_1[k]*((diff[index]>>k)&1);
            }
        }
	}

    /*u8 ret = tmp_2[index];
    u8 test = tmp_22;*/

    return tmp_22;
}

u8 ForwardUnitary(u8 input[32],u8 conf[512],u32 diff[32], u8 index)
{
    u8 tmp_1[32];
    u8 tmp_2[32];
    memcpy(tmp_2,input,32);
    
    u8 tmp_22 = input[index];
        
	for(u32 i=0;i<256;i++)
	{
		for(u8 j=0;j<32;j++)
		{
            tmp_1[j]=conf[tmp_2[j]];
            tmp_2[j]=0;
            
            tmp_22 = 0;
		}

        for(u8 j=0;j<32;j++)
        {
            for(u8 k=0;k<32;k++)
            {
                tmp_2[j]^=tmp_1[k]*((diff[j]>>k)&1);
                
                if(j==index)
                    tmp_22^=tmp_1[k]*((diff[index]>>k)&1);
            }
        }
	}

    /*u8 ret = tmp_2[index];
    u8 test = tmp_22;*/

    return tmp_2[index];
}

void Forward2(u8 input[32],u8 out[32],u8 conf[512],u32 diff[32])
{
    u8 tmp_1[32];
    u8 tmp_2[32];
    memcpy(tmp_2,input,32);
    
	for(u32 i=0;i<256;i++)
	{
		for(u8 j=0;j<32;j++)
		{
            tmp_1[j]=conf[tmp_2[j]];
            tmp_2[j]=0;
		}

        for(u8 j=0;j<32;j++)
        {
            for(u8 k=0;k<32;k++)
            {
                tmp_2[j]^=tmp_1[k]*((diff[j]>>k)&1);
            }
        }
	}

	for(u8 i=0;i<16;i++)
    {
       out[i] = conf[tmp_2[i*2]]^conf[tmp_2[i*2+1]+256];
    }
}


u8 ComputeTmp2Index0LastIteration(u8 tmp_1[32],u8 conf[512],u32 diff[32])
{
    return tmp_1[0]^tmp_1[7]^tmp_1[10]^tmp_1[12]^
    tmp_1[13]^tmp_1[15]^tmp_1[18]^tmp_1[19]^
    tmp_1[21]^tmp_1[22]^tmp_1[25]^tmp_1[28]^
    tmp_1[29]^tmp_1[30]^tmp_1[31];
}

// Given a target character (input param: target), find the list of 3-tuples (i1,i2,i3) of indices of confusion
// (thus each index is in [0..511]), such that for each 3-tuple (i1,i2,i3):
// * confusion[i1]^confusion[i2]^confusion[i3] == target
std::vector<std::tuple<u32,u32,u32>> FindIndicesOfConfusionForTarget_General3(u8 target)
{
    auto result = std::vector<std::tuple<u32,u32,u32>>();
    
    for(unsigned int i = 0 ; i < 512 ; ++i)
    {
        for(unsigned int j = 0 ; j < 512 ; ++j)
        {
            for(unsigned int k = 0 ; k < 512 ; ++k)
            {
                u8 val = confusion[i]^confusion[j]^confusion[k];
                if(val == target)
                {
                    auto tup = std::make_tuple(i,j,k);
                    result.push_back(tup);
                }            
            }
        }
    }
    
    return result;
}


// Given a target character (input param: target), find the list of pairs (i1,i2) of indices of confusion
// (thus each index is in [0..511]), such that for each pair (i1,i2):
// * confusion[i1]^confusion[i2] == target
std::vector<std::pair<u32,u32>> FindIndicesOfConfusionForTarget_General(u8 target)
{
    auto result = std::vector<std::pair<u32,u32>>();
    
    for(unsigned int i = 0 ; i < 512 ; ++i)
    {
        for(unsigned int j = 0 ; j < 512 ; ++j)
        {
            u8 val = confusion[i]^confusion[j];
            if(val == target)
            {
                auto pair = std::make_pair(i,j);
                result.push_back(pair);
            }
        }
    }
    
    return result;
}


// Given a target character (input param: target), find the list of pairs (i1,i2) of indices of confusion
// (thus each index is in [0..511]), such that for each pair (i1,i2):
// * i1 is in [0..255]
// * i2 in in [256..511]
// * confusion[i1]^confusion[i2] == target
std::vector<std::pair<u32,u32>> FindIndicesOfConfusionForTarget(u8 target)
{
    auto result = std::vector<std::pair<u32,u32>>();
    
    for(unsigned int i = 0 ; i < 256 ; ++i)
    {
        for(unsigned int j = 256 ; j < 512 ; ++j)
        {
            u8 val = confusion[i]^confusion[j];
            if(val == target)
            {
                auto pair = std::make_pair(i,j);
                result.push_back(pair);
            }
        }
    }
    
    return result;
}

void StudyConfusion()
{
    auto mapB = std::unordered_map<u8, std::vector<u32>>(); // map backward: value -> list of indexes in confusion such that confusion[index] == value

    auto mappedNumbers = std::unordered_map<u32, u32>(); // map a number i with the number of different values in confusion which are mapped i times
    
    for(u32 i = 0 ; i < /*512*/256 ; ++i)
    {        
        auto key = confusion[i];
        auto value = i;
        if(mapB.find(key) == mapB.cend())
        {
            mapB.insert(std::make_pair(key, std::vector<u32>()));
        }
        
        (mapB[key]).push_back(value);
    }

    for(unsigned int i = 0 ; i < 256 ; ++i)
    {
        if(mapB.find(i) == mapB.cend())
        {
            std::cout << "Value not mapped by confusion: " << i << "\t";
            for(int j = 7 ; j >= 0 ; --j) {std::cout << ((i>>j)&1) << " ";} std::cout << std::endl;
            (mappedNumbers[0])++;
        }
        else
        {
            auto vectorSize = (mapB[i]).size();
            (mappedNumbers[vectorSize])++;
            if(vectorSize == 2)
            {
                std::cout << "Value mapped 2 times in confuz: " << i << "\t";
                for(int j = 7 ; j >= 0 ; --j) {std::cout << ((i>>j)&1) << " ";} std::cout << std::endl;
            }
        }
    }
    
    for(unsigned int i = 0 ; i < mappedNumbers.size(); ++i)
    {
        std::cout << "Number of values mapped " << i << " times: " << mappedNumbers[i] << std::endl;
    }
}

void StudyDiffusion()
{
    for(unsigned int i = 0 ; i < 32 ; ++i)
    {
        unsigned int nbOfOnes = 0;
        //auto bs = std::bitset<32>(diffusion[i]);
        for(unsigned int j = 0 ; j < 32 ; ++j)
        {
            if((((diffusion[i])>>j)&1)==1)
                ++nbOfOnes;
        }
        
        /*std::cout << "diffusion[" << i << "]=" << diffusion[i]
            << "\twith " << nbOfOnes << " 1's in binary" << std::endl;*/
        /*std::cout 
			<< "dif[" << i << "]\t" 
			<< bs << std::endl;*/
		/*std::cout << "\"[" << i  << "]\",";
		for(auto j = 0 ; j < 32 ; ++j)
		{
			std::cout << (bs[j] ? std::string("1") : std::string("0")) << ",";
		}
		std::cout << std::endl;*/
    }
}


void TestSystemSolver()
{
	/*auto syso = SystemSolver();
	const auto SZ = SystemSolver::SIZE;
	
	unsigned mat[SZ][SZ+1] =
	{
		{0, 1, 1, 1, 1},
        {1, 1, 1, 1, 0},
		{1, 0, 1, 0, 0},
		{1, 0, 1, 1, 0}
	};
	
	syso.set(mat);
	if(syso.solve())
	{
		auto solution = syso.get_solution();
		std::cout << "solution:" << std::endl;
		syso.PrintSolution();
	}
	else
	{
		std::cout << "no solution..." << std::endl;
	}*/
}

bool SolveSystemForOneBit(u8 target[32], unsigned int bitNumber, vb& solution)
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

bool SolveSystemForU8s(u8 target[32], std::vector<u8>& solution)
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

void ForwardOneRound(u8 input[32], u8 output[32])
{
    u8 tmp_1[32];
    u8 tmp_2[32];
    memcpy(tmp_2,input,32);
    
	//for(u32 i=0;i<256;i++)
	//{
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
	//}
    
    memcpy(output, tmp_2, 32);
}

void TestSystemSolverVersusForwardOneRound(u8 input[32])
{
    u8 confInput[32];
    for(unsigned i = 0 ; i < 32 ; ++i)
    {
        confInput[i] = confusion[input[i]];
    }
    
    u8 outputForwardOneRound[32];
    ForwardOneRound(input, outputForwardOneRound);
    
    std::vector<u8> solution;
    SolveSystemForU8s(outputForwardOneRound, solution);
    
    for(unsigned i = 0 ; i < 32 ; ++i)
    {
        std::cout << "solution[" << i << "]=" << (unsigned)solution[i]
            << " and confInput[" << i << "]=" << (unsigned)confInput[i] << std::endl;
        
        if(solution[i] != confInput[i])
        {
            std::cout << "MISMATCH: solution[" << i << "]=" << (unsigned)solution[i]
                << " and confInput[" << i << "]=" << (unsigned)confInput[i] << std::endl;
            return;
        }
    }
    
    std::cout << "Ol Korrect!" << std::endl;
}

void InvConfusionInit()
{
	for(unsigned i = 0 ; i < 255 ; ++i)
	{
		inv_confusion[i] = std::set<u32>();
	}
	
	for(unsigned i = 0 ; i < 512 ; ++i)
	{
		(inv_confusion[confusion[i]]).insert(i);
	}
	
	is_inv_confusion_set = true;
}

std::set<u32> InvConfusion(u8 val)
{
	if(!is_inv_confusion_set)
	{
		std::cout << "Need to init the inverse of confusion" << std::endl;
		throw 1;
	}
	
	return inv_confusion[val];
}

void ForwardSecondLoop(u8 input[32],u8 out[32],u8 conf[512])
{
    u8 tmp_2[32];
    memcpy(tmp_2,input,32);
    
	for(u8 i=0;i<16;i++)
    {
       out[i] = conf[tmp_2[i*2]]^conf[tmp_2[i*2+1]+256];
    }
}

void ReverseSecondLoop_Analysis(u8 input[32])
{
	u8 output[32];
	ForwardSecondLoop(input, output, confusion);
	
	auto possible_inputs = std::vector<std::set<std::pair<u8,u8>>>(16, std::set<std::pair<u8,u8>>());
	
	for(unsigned i = 0 ; i < 16 ; ++i)
	{
		for(unsigned j = 0 ; j < 256 ; ++j)
		{
			for(unsigned k = 256 ; k < 512 ; ++k)
			{
				if((confusion[j]^confusion[k]) == output[i])
				{
					possible_inputs[i].insert(std::make_pair(j,k-256));
				}
			}
		}
	}
	
    // test for one index: OK!
	/*unsigned test_index = 15;
	std::cout << "Possible inputs at test_index=" << test_index << ":" << std::endl;
	for(auto it = possible_inputs[test_index].cbegin() ; it != possible_inputs[test_index].cend() ; ++it)
	{
		std::cout << "input[" << 2*test_index << "]=" << (unsigned int)(it->first) << "\t"
			<< "input[" << (2*test_index)+1 << "]=" << (unsigned int)(it->second) << std::endl;
	}
	std::cout << "Actual input at test_index=" << test_index << ":" << std::endl;
	std::cout << "input[" << 2*test_index << "]=" << (unsigned int)input[2*test_index] << "\t"
			<< "input[" << (2*test_index)+1 << "]=" << (unsigned int)input[(2*test_index)+1] << std::endl;*/

    auto deduced_inputs = std::vector<std::vector<std::vector<u8>>>(
        16, std::vector<std::vector<u8>>());
        
    for(auto it = possible_inputs[0].cbegin() ; it != possible_inputs[0].cend() ; ++it)
	{
        auto cur = std::vector<u8>(2, 0);
        cur[0] = it->first;
        cur[1] = it->second;
        deduced_inputs[0].push_back(cur);
	}
    
	//TMP	
	/*for(unsigned i = 0 ; i < deduced_inputs[0].size() ; ++i)
	{
		std::cout << "deduced_inputs[0][" << i << "]="
			<< (unsigned int)deduced_inputs[0][i][0] << ","
			<< (unsigned int)deduced_inputs[0][i][1] << std::endl;
	}*/
	
	std::cout << "Started deducing inputs by cartesian product" << std::endl;
    for(unsigned i = 1 ; i < /*16*/ 3 ; ++i)
    {
        auto cur_vec = std::vector<std::vector<u8>>();
        
        for(auto prev_vec : (deduced_inputs[i-1]))
        {
            //for(auto prev_str : prev_vec)
            //{
                for(auto it = possible_inputs[i].cbegin() ; it != possible_inputs[i].cend() ; ++it)
                {
                    auto cur_str = std::vector<u8>(prev_vec.begin(), prev_vec.end());
                    cur_str.push_back(it->first);
                    cur_str.push_back(it->second);
                    cur_vec.push_back(cur_str);
						//break;
                }
            //}
			
			if(cur_vec.size()%10000 == 0)
			{
				std::cout << "In iteration #" << i << " (0-based), there are currently " << cur_vec.size() << " solutions" << std::endl;
			}
        }
        
        deduced_inputs[i] = cur_vec;
        
        std::cout << "Number of deduced inputs at step " << i << " = " << cur_vec.size() << std::endl;
    }
    std::cout << "Finished deducing inputs by cartesian product" << std::endl;
	
    // TMP
	unsigned int max_size = 2;
	std::cout << "Started searching" << std::endl;
    for(unsigned i = 0 ; i < (deduced_inputs[max_size]).size() ; ++i)
    {
        std::vector<u8> str = deduced_inputs[max_size][i];
        
		if(i < 1)
		{
			std::cout << "str[" << i << "]=";
			for(unsigned int j = 0 ; j < 2*(1+max_size); ++j)
			{
				std::cout << (unsigned int)str[j] << ",";
			}
			std::cout << std::endl;
		}
       
		bool found = true;
		for(unsigned int j = 0 ; j < 2*(1+max_size); ++j)
		{
			if(str[j] != input[j])
			{
				found = false;
				break;
			}
		}
       
		if(found)
		{
			std::cout << "Found!" << std::endl;
		}
    }
    std::cout << "Finished searching" << std::endl;
	std::cout << "input=";
	for(unsigned int j = 0 ; j < 2*(1+max_size); ++j)
	{
		std::cout << (unsigned int)input[j] << ",";
	}
	std::cout << std::endl;
}

void ReverseSecondLoop(u8 input[32])
{
	u8 output[32];
	ForwardSecondLoop(input, output, confusion);
	
	//auto possible_inputs = std::vector<std::pair<u8,u8>>(16, std::pair<u8,u8>());
	u8 deduced_input_single[32];
    deduced_input_single[0] = deduced_input_single[0];
	
	for(unsigned i = 0 ; i < 16 ; ++i)
	{
		for(unsigned j = 0 ; j < 256 ; ++j)
		{
			for(unsigned k = 256 ; k < 512 ; ++k)
			{
				if((confusion[j]^confusion[k]) == output[i])
				{
					//possible_inputs[i] = std::make_pair(j,k-256);
					deduced_input_single[2*i] = j;
					deduced_input_single[(2*i)+1] = k-256;
					goto endofinnerloops;
				}
			}
		}
		
		endofinnerloops:
		int a = 1;
        ++a;
	}
	
	// Looks good
	/*std::cout << "deduced_input_single=";
	for(unsigned i = 0 ; i < 32 ; ++i)
	{
		std::cout << (unsigned int)deduced_input_single[i] << ",";
	}
	std::cout << std::endl;*/
	
	// Looks good. Remember to only compare the first 16 bits, not all 32 bits!
	/*u8 output_for_deducted_input_single[32];
	ForwardSecondLoop(deduced_input_single, output_for_deducted_input_single, confusion);
	
	std::cout << "Checking for inconsistencies..." << std::endl;
	for(unsigned i = 0 ; i < 16 ; ++i)
	{
		std::cout << "output[" << i << "]=" << (unsigned int)output[i] << ", deducted[" << i << "]=" << (unsigned int)output_for_deducted_input_single[i] << std::endl;
		if(output[i] != output_for_deducted_input_single[i])
		{
			std::cout << "Inconsitency! i=" << i << std::endl;
		}
	}
	std::cout << "Finished checking for inconsistencies..." << std::endl;*/
}

void ReverseSecondLoop(u8 target[32], u8 deducted_input[32])
{
	PrintU8ArrayWithName(target, 16, "ReverseSecondLoop__target");
	
	for(unsigned i = 0 ; i < 16 ; ++i)
	{
		for(unsigned j = 0 ; j < 256 ; ++j)
		{
			for(unsigned k = 256 ; k < 512 ; ++k)
			{
				if((confusion[j]^confusion[k]) == target[i])
				{
					if(IsValueReachableInConfusion(j) && IsValueReachableInConfusion(k-256))
					{
						deducted_input[2*i] = j;
						deducted_input[(2*i)+1] = k-256;
						goto endofinnerloops;
					}
				}
			}
		}
		
		endofinnerloops:
		int a = 1;
        ++a;
	}
	
	PrintU8ArrayWithName(deducted_input, 32, "ReverseSecondLoop__deducted_input");
	
	u8 out_control[32];
	ForwardSecondLoop(deducted_input, out_control, confusion);
	PrintU8ArrayWithName(out_control, 16, "ReverseSecondLoop__out_control");
}

void ForwardOneRoundPlusSecondLoop(u8 input[32],u8 out[32],u8 conf[512],u32 diff[32])
{
    u8 tmp_1[32];
    u8 tmp_2[32];
    memcpy(tmp_2,input,32);
    
	for(u32 i=0;i<1;i++)
	{
		for(u8 j=0;j<32;j++)
		{
            tmp_1[j]=conf[tmp_2[j]];
            tmp_2[j]=0;
		}

        for(u8 j=0;j<32;j++)
        {
            for(u8 k=0;k<32;k++)
            {
                tmp_2[j]^=tmp_1[k]*((diff[j]>>k)&1);
            }
        }
	}

	PrintU8ArrayWithName(tmp_1, 32, "tmp_1");
	PrintU8ArrayWithName(tmp_2, 32, "tmp_2");

	for(u8 i=0;i<16;i++)
    {
       out[i] = conf[tmp_2[i*2]]^conf[tmp_2[i*2+1]+256];
    }
	
	PrintU8ArrayWithName(out, 32, "out");
}

// Important choice: consider only the *first* index of confusion which reaches the target
// Important note: this only considers the first 16 bits of target
void ReverseFirstLoopOneRound(u8 target[32], u8 deducted_input[32])
{
	auto deducted_intermediate_vec = std::vector<u8>(32, 0);
	SolveSystemForU8s(target, deducted_intermediate_vec);
	
	u8 deducted_intermediate[32]; for(unsigned int i = 0 ; i < 32 ; ++i) deducted_intermediate[i] = deducted_intermediate_vec[i];
	
	PrintU8ArrayWithName(deducted_intermediate, 32, "deducted_intermediate");
	
	for(unsigned i = 0 ; i < 16; ++i)
	{
		bool found = false;
		
		for(unsigned j = 0 ; j < 256 ; ++j)
		{
			if(confusion[j] == deducted_intermediate[i])
			{
				deducted_input[i] = j;
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

void ReverseOneRoundPlusSecondLoop_Test(u8 input[32])
{
	PrintU8ArrayWithName(input, 32, "input");
	
	u8 output[32];
	ForwardOneRoundPlusSecondLoop(input, output, confusion, diffusion);
	
	PrintU8ArrayWithName(output, 16, "output");
	
	u8 deducted_input_second_loop[32];
	ReverseSecondLoop(output, deducted_input_second_loop);
	
	
	// KO
	/*auto deducted_input_vec = std::vector<u8>(32, 0);
	SolveSystemForU8s(deducted_input_second_loop, deducted_input_vec);
	
	u8 deducted_input[32]; for(unsigned int i = 0 ; i < 32 ; ++i) deducted_input[i] = deducted_input_vec[i];*/
	u8 deducted_input[32];
	ReverseFirstLoopOneRound(deducted_input_second_loop, deducted_input);
	
	PrintU8ArrayWithName(deducted_input, 32, "deducted_input");
	
	u8 output_control[32];
	ForwardOneRoundPlusSecondLoop(deducted_input, output_control, confusion, diffusion);
	
	PrintU8ArrayWithName(output_control, 16, "output_control");
	
	std::cout << "Checking..." << std::endl;
	for(unsigned int i = 0 ; i < 16 ; ++i)
	{
		if(output_control[i] != output[i])
		{
			std::cout << "Inconsistency at position i=" << i << " !!!" << std::endl;
		}
	}
	std::cout << "Finished checking" << std::endl;
}

void TestSecondLoop()
{
    u8 target[]="Hire me!!!!!!!!";
    
    auto sl = SecondLoop();
    sl.ReverseFindAllSolutionsAsPairs(target);
    
    u8 current_solution[32];
    sl.InitSolutionIterator(2);
    auto nbSolutions = sl.GetNumberOfSolutions();
    std::cout << "Total number of solutions: " << nbSolutions << std::endl;
    unsigned long long solutionNumber = 0;
    unsigned long start = clock();
    
    while(sl.GetNextSolution(current_solution))
    {
        ++solutionNumber;
        if(solutionNumber%10000000 == 0)
        {
            std::cout << "Number of seconds to reach solution #" << solutionNumber << ": " << ((clock()-start)/1000UL) << std::endl;
        }
    }
}

void TestSecondLoopAndFirstLoopOneRound()
{
    u8 target[]="Hire me!!!!!!!!";
    
    auto sl = SecondLoop();
    sl.ReverseFindAllSolutionsAsPairs(target);
    
    u8 current_solution_second_loop[32];
    sl.InitSolutionIterator();
    
    for(unsigned int i = 0 ; i < 300 ; ++i)
    {
        std::cout << "solution (second loop) #" << i << std::endl;
        if(sl.GetNextSolution(current_solution_second_loop))
        {
            Common::PrintU8ArrayWithName(current_solution_second_loop, 32, "current_solution_sl");
            auto solutions_first_loop = std::vector<std::vector<u8>>();
            auto fl = FirstLoop();
            fl.ReverseSingleAllSolutions(current_solution_second_loop, solutions_first_loop);
            for(auto solution : solutions_first_loop)
            {
                Common::PrintU8ArrayWithName(solution, 32, "current_solution_fl");
            }
        }
        else
        {
            std::cout << "No more solutions (second loop)..." << std::endl;
            break;
        }
    }
}

void JustTryThis()
{
	u8 in[32];
	for(unsigned int i = 0 ; i < 32 ; ++i) in[i] = 1;
	
	for(unsigned int i = 0 ; i < 256 ; ++i)
	{
		/*if(i < 32)
			in[i] = 1;
		else
			in[i - 32] = 0;
			 */
		in[2] = i;
		u8 out[32];
		Forward2(in, out, confusion, diffusion);
		for(unsigned int j = 0 ; j < 32 ; ++j) { std::cout << (unsigned int)in[j] << ","; }
		std::cout << "=>";
		for(unsigned int j = 0 ; j < 16 ; ++j) { std::cout << (unsigned int)out[j] << ";"; }
		std::cout << std::endl;
		
	}		
}

void CanYouBelieveIt(u8 targ[32])
{
	u8 in[32];
	for(unsigned int i = 0 ; i < 32 ; ++i) in[i] = 0;
	bool final_match = true;
	
	unsigned int final = /*16*/ 2; // Biggest OK: 1
	for(unsigned int i = 0 ; i < final ; ++i)
	{
		bool matched = false;
		for(unsigned long long j = 0 ; j < 256*256 ; ++j)
		{
			u8 j1 = j % 256;
			u8 j2 = j / 256;
			
				std::cout << "j=" << j << ",j1=" << (unsigned int) j1  << ",j2=" << (unsigned int) j2 << std::endl;
				
			u8 out[32];
			in[2*i] = j1;
			in[(2*i)+1] = j2;
			Forward2(in, out, confusion, diffusion);
			if(out[i] == targ[i])
			{
				bool sub_matches = true;
				for(unsigned int k = 0 ; k < i ; ++k)
				{
					if(out[k] != targ[k])
					{
						sub_matches = false;
						break;
					}
				}
				
				if(sub_matches)
				{
					matched = true;
					break;
				}
			}
		}
		
		if(!matched)
		{
			std::cout << "No match found at i=" << i << std::endl;
			final_match = false;
			break;
		}
	}
	
	if(final_match)
	{
		std::cout << "And the finalist is:" << std::endl;
		for(unsigned int i = 0 ; i < 32 ; ++i)
		{
			std::cout << (unsigned int) in[i] << ",";
		}
		std::cout << std::endl;
	}
	
	
	std::cout << "Finished" << std::endl;
}

u8 ApplyConf(u8 in, unsigned int nbApplications)
{
	for(unsigned int i = 0 ; i < nbApplications ; ++i)
	{
		in = confusion[in];
	}
	
	return in;
}

void SomeOtherTests()
{
	u8 in = 0;
	unsigned int nb = 256;
	std::cout << "ApplyConf(" << (unsigned int)in << "," << nb << ")=" << (unsigned int) ApplyConf(in,nb) << std::endl;
}

void StudyConfusionAgain()
{
    unsigned long maxLen = 0;
    
    for(unsigned int i = 0 ; i < 256 ; ++i)
    {
        u8 cur = i;
        unsigned long len = 0;
        do
        {
            cur = confusion[cur];
            ++len;
        } while(cur != i && len < 256);
        std::cout << "len[" << i << "]=" << len << std::endl;
        if(maxLen == 0 || len < maxLen)
        {
            maxLen = len;
        }
    }
    
    std::cout << "maxLen=" << maxLen << std::endl;
}

int main(int argc, char* argv[])
{
	u8 target[]="Hire me!!!!!!!!";
	u8 output[32];

    //StudyConfusion();
    StudyConfusionAgain();
    
    /*u8 tgt = '!';
    auto res = FindIndicesOfConfusionForTarget(tgt);
    std::cout << "Pairs of indices which map to " << tgt << ": " << std::endl;
    for(size_t i = 0 ; i < res.size() ; ++i)
    {
        std::cout << "(" << (res[i]).first << "," << (res[i]).second << ")" << std::endl;
    }
    std::cout << "Number of pairs of indices which map to " << tgt << ": " << res.size() << std::endl;*/

    /*u8 tgt = 252;
    auto res = FindIndicesOfConfusionForTarget_General3(tgt);
    std::cout << "3-tuples of indices which map to " << (int)tgt << ": " << std::endl;
    std::cout << "Number of 3-tuples of indices which map to " << tgt << ": " << res.size() << std::endl;*/

    //StudyDiffusion();

    // TEST
    //Generate(0,0);
	
	// TEST FindFullIntsWhichSatisfyEquations
	/*u8* targetss = (u8*)malloc(32* sizeof(u8));
	for(auto i = 0; i < 32 ; ++i) targetss[i] = 0;
	auto res = FindFullIntsWhichSatisfyEquations(targetss, diffusion);*/
	
    // Ok (but for SIZE == 4 only)
	//TestSystemSolver();

    // OK!
    /*u8 exampleOfTarget[32] = {
        1, 2, 3, 4, 5, 6, 7, 8,
        9, 0, 1, 2, 3, 4, 5, 6,
        7, 8, 9, 0, 1, 2, 3, 4,
        5, 6, 7, 8, 9, 0, 1, 2
    };
    std::vector<u8> solution;
    SolveSystemForU8s(exampleOfTarget, solution);*/
    
	// OK!
    //TestSystemSolverVersusForwardOneRound(input);

	// OK
	/*InvConfusionInit();
	u8 val = 0; auto res = InvConfusion(val);
	std::cout << "Indices of confusion whose value is " << (unsigned int)val << ":" << std::endl;
	for(auto i : res)	{ std::cout << i << ","; } std::cout << std::endl;*/

	// OK
	//ReverseSecondLoop(input);

    // Ongoing...
	//ReverseOneRoundPlusSecondLoop_Test(input);

    //TestSecondLoop();
    //TestSecondLoopAndFirstLoopOneRound();

	// Let's try to add something here...
	//JustTryThis();
	//CanYouBelieveIt(target);
	//SomeOtherTests();
	/*u8 in[32] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
		};
	Forward2(in,output,confusion,diffusion);*/
	
	//Forward2(input,output,confusion,diffusion);

    int exitCode = memcmp(output,target,16);
    std::cout << "exit code: " << exitCode << std::endl;
    std::cin.get();

    return exitCode;

	//return memcmp(output,target,16); // => contact jobs(at)nerd.nintendo.com
}