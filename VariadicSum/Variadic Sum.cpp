/*
Name: Eric Kwon
Assignment 0
sum(...) function with unlimited arguments
*/

#include <iostream>
#include <cstdarg>

using namespace std;

int sum(int numArg,  ...)
{
	va_list arguments;
	int sumTotal = 0;
	
	va_start(arguments, numArg);
	for (int i=0 ; i < numArg ; i++)
	{
		sumTotal += va_arg(arguments,int);
	}
	va_end(arguments);
	
	return sumTotal;
	
}


int main()
{
	// sum(# of arguments, 1st arg, ... , nth arg);
	cout << sum(5,2,5,5,4,3);
	// Will return the sum of 2,5,5,4,3
}

