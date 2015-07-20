#ifndef SYSTEMSOLVER_H
#define SYSTEMSOLVER_H

#include <vector>

typedef std::vector<unsigned int> vb; // vector of bits
typedef std::vector<std::vector<unsigned int>> mb; // matrix of bits

class SystemSolver
{	
	
public:
	const static unsigned int SIZE = 32;
	SystemSolver();
	~SystemSolver();
	void set(const unsigned array[SIZE][SIZE+1]);
    void set(const unsigned **array);
    void set(const mb& matrix);
	bool solve();
	vb get_solution();
	void PrintMatrix();
	void PrintSolution();
private:
	bool _is_set;
    bool _is_solved;
	mb _matrix;
	vb _solution;
    void swap_lines(unsigned int l1, unsigned int l2);

};

#endif // SYSTEMSOLVER_H
