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
	}

	double data = operandStack.top();
	operandStack.pop();
	return data;
	
}

void GMemory::auto_resize(int n) {

}

void GMemory::set(int address, double data) {

}

void GMemory::add(int address, double data) {

}

double GMemory::get(int address) {
    return 0.01;  // TODO: 임시 리턴값
}

int GMemory::size() {
	return this->memory.size();
}

void GMemory::push_back (double d) {
	this->memory.push_back(d);
}

void GMemory::resize(int size) {
	this->memory.resize(size);
}