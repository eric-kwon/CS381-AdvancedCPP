#include <iostream>

using namespace std;

int main()
{
	int b[3][2];
	
	cout << b << endl;
	
	cout << sizeof(b) << endl;
	// Since it's 3x2 array there are 6 element space total
	// Since each integer is 4 bits, total is 24
	
	cout << sizeof(b+0) << endl;
	// Since b+0 is now a pointer, the size is 8
	
	cout << sizeof(*(b+0)) << endl;
	// Dereferencing a pointer to b+0 is b[0]
	// Since there are 2 columnn its 8 on first row (4+4)
	
	cout << "The address of b is: " << b << endl;
	// Prints 0x6ffe30
	
	cout << "The address of b+1 is: " << b+1 << endl;
	// Prints 0x66ffe38
	// Since it's a 2d array with 8 bits each row, add 8
	
	cout << "The address of &b is: " << &b << endl;
	// Prints 0x6ffe30
	// Reference to pointer - still same address
	
	cout << "The address of &b+1 is: " << &b+1 << endl;
	// Prints 0x6ffe48
	// References to the next empty space, since 24 bits taken, add 24
	
	return 0;
}
