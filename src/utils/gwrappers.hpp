#include <stack>
#include <vector>
#include "errors.hpp"

class Gstack{
	private:
		std::stack<double> operandStack;
	
	public:
		void push(double n);
		int size();
		double pop();
};

class GMemory{
	private:
		std::vector<double>memory;
	
	public:
		void auto_resize(int n);
		void set(int address, double data);
		void add(int address, double data);
		double get(int address);
		int size();
		void push_back(double d);
};