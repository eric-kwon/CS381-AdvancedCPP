#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

class SA // Safe Array Class
{
	private:
		int low, high;
		int* p;
		
	public:
		SA()  // Default Constructor
		{
			low = 0;
			high = -1;
			p = NULL;
		}
		
		SA(int l, int h) // 2 Parameter Constructor
		{
			if ((h-1+1)<=0)
			{
				cout << "Constructor error in bounds definition" << endl;
				exit(1);
			}
			low = l;
			high = h;
			p = new int[h-l+1];
		}
		
		SA(int i) // Single Parameter Constructor
		{
			low = 0;
			high = i-1;
			p = new int[i];
		}
		
		SA(const SA &s) // Copy Constructor
		{
			int size = s.high - s.low + 1;
			p = new int[size];
			for (int i=0; i<size ; i++)
				p[i] = s.p[i];
			low = s.low;
			high = s.high;
		}
		
		~SA() // Destructor
		{
			delete[] p;
		}
		
		int& operator [](int i) // Overload []
		{
			if (i<low || i>high)
			{
				cout << "Index " << i << " out of range" << endl;
				exit(1);
			}
			return p[i-low];
		}
		
		SA& operator =(const SA& s) // Overload =
		{
			if (this == &s)
				return *this;
			delete[] p;
			int size = s.high - s.low + 1;
			p = new int[size];
			for (int i=0 ; i<size ; i++)
				p[i] = s.p[i];
			low = s.low;
			high = s.high;
			return *this;
		}
		
		int getlow() // Getter for Low
		{
			return low;
		}
		
		int gethigh() // Getter for high
		{
			return high;
		}
		
		friend ostream& operator <<(ostream& os, SA& s);
};

ostream& operator <<(ostream& os, SA& s) // Overload << for SA class
{
	int size = s.high - s.low + 1;
	for (int i=0 ; i<size ; i++)
		os << "[" << s.p[i] << "]";
	cout << endl;
	return os;
}

void sort(SA &s)
{
	int low = s.getlow();
	int high = s.gethigh();
	for (int j=low ; j<high ; j++)
	{
		for (int i=low ; i<high ; i++)
		{
			if (s[i] > s[i+1])
			{
				int temp;
				temp = s[i];
				s[i] = s[i+1];
				s[i+1] = temp;
			}	
		}
	}
}

int main()
{
	SA a(10), b(3,5);
	b[3]=5; b[4]=4; b[5]=3;
	for (int i=0 ; i<10 ; i++)
		a[i] = 10-i;
		
	cout << "Printing a" << endl;
	cout << a << endl;
	
	cout << "Printing b" << endl;
	cout << b << endl;
	
	cout << "Printing using a[]" << endl;
	for (int i=0 ; i<10 ; i++)
		cout << "[" << a[i] << "]";
	cout << "\n" <<endl;
	
	cout << "Printing using b[]" << endl;
	for (int i=3 ; i<=5 ; i++)
		cout << "[" << b[i] << "]";
	cout << "\n" << endl;
	
	cout << "Sorting a" << endl;
	sort(a);
	cout << a << endl;
	
	cout << "Sorting b" << endl;
	sort(b);
	cout << b << endl;
}
