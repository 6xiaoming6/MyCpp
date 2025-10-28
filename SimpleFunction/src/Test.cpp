#include <iostream>
#include "Function.h"
void Print(int a)
{
	std::cout << "value:" << a << std::endl;
}

int main()
{
	Function<void(int)> f = Print;
	f(1);
}