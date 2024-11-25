#ifndef _GWRAPPERS_HPP_
#define _GWRAPPERS_HPP_

#include <stack>
#include <vector>
#include "../Gurungi.hpp"

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
		void resize(int size);
};

#endif