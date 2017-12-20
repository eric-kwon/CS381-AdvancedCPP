/*
Name: ERIC KWON
Project #1
Goal: Write a templatized 2 dimensional safe array class. Test it by using the class for matrix multiplication.
*/

#include <iostream>
#include <cassert>
using namespace std;

template<class T>			// Forward declaration
class SA;

template<class T>			// Forward declaration
ostream& operator <<(ostream& os, SA<T> s);

template<class T>			// Forward declaration
class SM;

template<class T>			// Forward declaration
ostream& operator <<(ostream& os, SM<T> s);

// Safe Array Class
template<class T>
class SA
{
	private:
		int low, high;		// Lower and upper bound
		T* p;				// Pointer to the actual array
	
	public:
		// Default constructor
		SA()											
		{
			low = 0;
			high = -1;
			p = NULL;
		}
		
		// 2 parameter constructor - Allows for initializations such as SA x(10,20);
		SA(int l, int h)									
		{
			if((h-l+1)<=0)						// Boundary check - Make sure high > low & difference is non-negative						
			{
				cerr << "Error: Incorrect bound definition" << endl;
				exit(1);
			}
			low = l;
			high = h;
			p = new T[h-l+1];
		}
		
		// 1 parameter constructor - Allows for initializations such as SA x(10);
		SA(int size)										
		{
			if(size<=0)  						// Boundary check - Make sure non-negative size is entered
			{
				cerr << "Error: Incorrect bound definition" << endl;
				exit(1);
			}
			low = 0;
			high = size - 1;
			p = new T[size];
		}
		
		// Copy constructor - Copies the SA passed as parameter and creates new SA
		SA(const SA& s)								
		{
			int size = s.high - s.low + 1;
			p = new T[size];
			for(int i=0; i<size; i++)
			{
				p[i] = s.p[i];
			}
			low = s.low;
			high = s.high;
		}
		
		// Destructor
		~SA()											
		{
			delete[] p;
		}
		
		// [] Overload - Allows for assignments such as x[15] = 100;
		T& operator [](int i)							
		{
			if(i<low || i>high)					// Boundary check - make sure index is in bound
			{
				cerr << "Error: Index out of bound (SA)" << low << high << i << endl;
				exit(1);
			}
			return p[i-low];
		}
		
		// = Overload - Allows for direct assignment from SA to SA
		SA& operator =(const SA s)
		{
			if(this == &s)
				return *this;
			delete[] p;
			int size = s.high - s.low + 1;
			p = new T[size];
			for(int i=0; i<size; i++)
			{
				p[i] =s.p[i];
			}
			low = s.low;
			high = s.high;
			return *this;
		}
		
		// Friendship to allow << overload - alternative: inline definition inside class
		friend ostream& operator << <T>(ostream& os, SA<T> s);
};

// << Overload - Allows for direct printing of array elements via cout
template<class T>
ostream& operator <<(ostream& os, SA<T> s)
{
	int size = s.high - s.low + 1;
	for(int i=0; i<size; i++)
		os << s.p[i] << " "; 
	os << endl;
	return os;
}

// Safe Matrix Class
template<class T>
class SM
{
	private:
		int y_low, y_high, x_low, x_high;		// Bounds for x(horizontal) and y(vertical) direction - Regularly a[y][x]
		SA<SA<T>> p;							// "Make Believe" pointer for SM class
		
	public:
		// Default constructor
		SM()
		{

		}
		
		// 2 parameter constructor - Allows for initializations such that SM x(10,20) = SM[10][20]
		SM(int y_size, int x_size)
		{
			if(y_size<=0 || x_size<=0)			// Boundary check - make sure non-negative size is entered
			{
				cerr << "Error: Incorrect bound definition" << endl;
				exit(1);
			}
			else if(y_size == x_size)			// Case when x,y directional length are of same value
			{
				int size = y_size;
				y_low = x_low = 0;
				y_high = x_high = size - 1;
				p = SA<SA<T>> (size);			// Y direction safe array - calling 1 parameter safe array constructor
				for(int i=0; i<size; i++)
					p[i] = SA<T> (size);		// X direction safe array per every safe array in Y direction
			}
			else								// Case when x,y directional length are of different value
			{
				y_low = x_low = 0;
				y_high = y_size - 1;
				x_high = x_size - 1;
				p = SA<SA<T>> (y_size);			// Y direction safe array - calling 1 parameter safe array constructor
				for(int i=0; i<y_size; i++)
					p[i] = SA<T> (x_size);		// X direction safe array per every safe array in Y direction
			}
		}
		
		// 4 parameter constructor - Allows for initializations such as SM x(10,20,30,40)
		// Such initialization results in x[11][11] with 10-20 bound for y and 30-40 bound for x
		SM(int y_l, int y_h, int x_l, int x_h)
		{
			if(((y_h-y_l+1)<=0) || ((x_h-x_l+1)<=0))	// Boundary check - make sure high > low & difference is non-negative
			{
				cerr << "Error: Incorrect bound definition" << endl;
				exit(1);
			}
			y_low = y_l;
			y_high = y_h;
			x_low = x_l;
			x_high = x_h;
			p = SA<SA<T>> (y_low,y_high);		// Y direction safe array - calling 2 parameter safe array constructor
			for(int i=y_low; i<=y_high; i++)
				p[i] = SA<T> (x_low,x_high);	// X direction safe array per every safe array in Y direction WITH set bounds
		}
		
		// [] Overload - Allows for direct operation such taht SM[y][x] will return SA[x] of SA[y]
		SA<T>& operator [](int i)
		{
			if(i<y_low || i>y_high)				// Boundary check - make sure index is in bound
			{
				cerr << "Error: Index out of bound (SM)" << y_low << y_high << i << endl;
				exit(1);
			}
			return p[i];
		}
		
		// * Overload - Allows for matrix multiplication
		SM<T> operator *(SM& s)
		{
			if(((x_high)-(x_low)+1) != (((s.y_high)-(s.y_low)))+1)	// Dimension check for matrix multiplication
			{
				cerr << "Error: Inappropriate dimensions for multiplication" << endl;
				exit(1);	
			}
			int this_y = y_high - y_low + 1;	// Will be the y direction size of matrix
			int this_x = x_high - x_low + 1;
			int s_y = s.y_high - s.y_low + 1;
			int s_x = s.x_high - s.x_low + 1;	// Will be the x direction size of matrix
			SM<int> pm(this_y,s_x);
			for(int y=0; y<this_y; y++)			// Matrix initialization to 0
			{
				for(int x=0; x<s_x; x++)
				{
					pm[y][x] = 0;
				}
			}
			for(int y=0; y<this_y; y++)			// Run-thru for matrix multiplication
			{
				for(int x=0; x<s_x; x++)
				{
					for(int run=0; run<s_y; run++)
					{
						pm[y][x] += (*this)[y+y_low][run+x_low] * s[run+s.y_low][x+s.x_low];
					}
				}
			}
			return pm;
		}
		
		// + Overload - Allows for direct addition of safe matrix elements
		SM<T> operator +(SM& s)
		{
			int this_y = y_high - y_low + 1;
			int this_x = x_high - x_low + 1;
			int s_y = s.y_high - s.y_low + 1;
			int s_x = s.x_high - s.x_low + 1;
			if(this_y!=s_y || this_x!=s_x)	// Dimension check for matrix addition
			{
				cerr << "Error: Inappropriate dimensions for addition" << endl;
				exit(1);	
			}
			SM<int> pm(this_y,this_x);
			for(int y=0; y<this_y; y++)			// Matrix initialization to 0
			{
				for(int x=0; x<this_x; x++)
				{
					pm[y][x] = 0;
				}
			}
			for(int y=0; y<this_y; y++)			// Run-thru for matrix addition
			{
				for(int x=0; x<this_x; x++)
				{
					pm[y][x] = (*this)[y][x] + s[y][x];
				}
			}
			return pm;
		}
		
				// + Overload - Allows for direct subtraction of safe matrix elements
		SM<T> operator -(SM& s)
		{
			int this_y = y_high - y_low + 1;
			int this_x = x_high - x_low + 1;
			int s_y = s.y_high - s.y_low + 1;
			int s_x = s.x_high - s.x_low + 1;
			if(this_y!=s_y || this_x!=s_x)	// Dimension check for matrix addition
			{
				cerr << "Error: Inappropriate dimensions for subtraction" << endl;
				exit(1);	
			}
			SM<int> pm(this_y,this_x);
			for(int y=0; y<this_y; y++)			// Matrix initialization to 0
			{
				for(int x=0; x<this_x; x++)
				{
					pm[y][x] = 0;
				}
			}
			for(int y=0; y<this_y; y++)			// Run-thru for matrix subtraction
			{
				for(int x=0; x<this_x; x++)
				{
					pm[y][x] = (*this)[y][x] - s[y][x];
				}
			}
			return pm;
		}
		
		// Copy constructor - Copies the SM passed as parameter and creates new SM
		SM(const SA<SA<T>>& s)
		{}										// Copy constructor derived from SA class, no need for definition				
		
		// Destructor
		~SM(){}									// Destructor derived from SA class, no need for definition
		
		// Friendship to allow << overload - alternative: inline definition inside class
		friend ostream& operator << <T>(ostream& os, SM<T> s);
		
		int get_ylow()							// Getter for y direction low boundary
		{
			return y_low;
		}
		
		int get_yhigh()							// Getter for y direction high boundary
		{
			return y_high;
		}
		
		int get_xlow()							// Getter for x direction low boundary
		{
			return x_low;
		}
		
		int get_xhigh()							// Getter for x direction high boundary
		{
			return x_high;
		}		
};

// << Overload - Allows for direct printing of matrix elements via cout
template<class T>
ostream& operator <<(ostream& os, SM<T> s)
{
	int y_size = s.y_high - s.y_low + 1;
	for(int y=s.y_low; y<=s.y_high; y++)		// As << already overloaded for second part of SA, only need to define first part
	{
		os << s[y];
	}
	return os;
}

int main()
{
	/* Main provided by TA - venus/~kbian */
	
	// Test function [][]
	SM<int> a(3,2);
	int temp=0;
	for(int i=a.get_ylow(); i<=a.get_yhigh(); i++)
	{
		for(int j=a.get_xlow(); j<=a.get_xhigh(); j++)
		{
			temp++;
			a[i][j] = temp;
		}
	}
	
	cout << "Test function: Print using [][]" << endl;
	for(int i=a.get_ylow(); i<=a.get_yhigh(); i++)
	{
		for(int j=a.get_xlow(); j<=a.get_xhigh(); j++)
		{
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	
	// Test function <<
	cout << "Test function: Print using <<" << endl;
	cout << a << endl;
	
	// Test copy constructor for pass by value
	SM<int> c(a);
	c=a;
	cout << "Test constructor: SM<T> c(a)" << endl;
	cout << c << endl;
	
	// Test upper and lower bounds
	SM<int> d(2,5,8,9);
	d[2][8]=1;
	d[2][9]=2;
	d[3][8]=3;
	d[3][9]=4;
	d[4][8]=5;
	d[4][9]=6;
	d[5][8]=7;
	d[5][9]=8;
	cout << "Test constructor: Upper and lower bound" << endl;
	cout << d << endl;
	
	// Test function *
	SM<int> e(2,2);
	e[0][0]=1;
	e[0][1]=2;
	e[1][0]=3;
	e[1][1]=4;
	SM<int> f(2,3);
	f[0][0]=5;
	f[0][1]=6;
	f[0][2]=7;
	f[1][0]=8;
	f[1][1]=9;
	f[1][2]=10;
	cout << "Test function: multiplication" << endl;
	cout << "Matrix 1" << endl;
	cout << e;
	cout << "Matrix 2" << endl;
	cout << f;
	SM<int> g=e*f;
	cout << "Matrix 1 * Matrix 2" << endl;
	cout << g << endl;
	
	// Test function +
	SM<int> h(2,3);
	h[0][0]=1;
	h[0][1]=2;
	h[0][2]=3;
	h[1][0]=4;
	h[1][1]=5;
	h[1][2]=6;
	SM<int> i(2,3);
	i[0][0]=7;
	i[0][1]=8;
	i[0][2]=9;
	i[1][0]=10;
	i[1][1]=11;
	i[1][2]=12;
	cout << "Test function: addition" << endl;
	cout << "Matrix 3" << endl;
	cout << h;
	cout << "Matrix 4" << endl;
	cout << i;
	SM<int> j=h+i;
	cout << "Matrix 3 + Matrix 4" << endl;
	cout << j << endl;
	
	// Test function -
	SM<int> k(2,3);
	k[0][0]=1;
	k[0][1]=2;
	k[0][2]=3;
	k[1][0]=4;
	k[1][1]=5;
	k[1][2]=6;
	SM<int> l(2,3);
	l[0][0]=7;
	l[0][1]=8;
	l[0][2]=9;
	l[1][0]=10;
	l[1][1]=11;
	l[1][2]=12;
	cout << "Test function: subtraction" << endl;
	cout << "Matrix 5" << endl;
	cout << k;
	cout << "Matrix 6" << endl;
	cout << l;
	SM<int> m=k-l;
	cout << "Matrix 5 - Matrix 6" << endl;
	cout << m;
	
	return 0;	
}

