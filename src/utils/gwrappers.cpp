#include "gwrappers.hpp"


void Gstack::push(double n) {
	operandStack.push(n);
}

int Gstack::size() {
	return operandStack.size();
}

double Gstack::pop() {
	if (operandStack.empty()) {
		error_exit("Operandstack underflow!");

	double data = operandStack.top();
	operandStack.pop();
	return data;
	}
}