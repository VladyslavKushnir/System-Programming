#include <cstdlib>
#include <ostream>
#include <iostream>
#include <algorithm>


int main()
{
	char c = 'Vlad';
	int i = 10;
	long f = 54;
	short d = 124;
	signed char sh = -96;
	unsigned char uc = 96;
	unsigned int ui = 2565231;
	unsigned long ul = 5653545645;
	unsigned long long ull = 63545455455;
	std::cout << "Char " << c << " Size " << sizeof(c) << "  Min " << CHAR_MIN << " Max " << CHAR_MAX << std::endl;
	std::cout << "Int " << i << " Size " << sizeof(i) << "  Min " << INT_MIN << " Max " << INT_MAX << std::endl;
	std::cout << "Long " << f << " Size " << sizeof(f) << "  Min " << LONG_MIN << " Max " << LONG_MAX << std::endl;
	std::cout << "Short " << d << " Size " << sizeof(d) << "  Min " << SHRT_MIN << " Max " << SHRT_MAX << std::endl;
	std::cout << "Signed Char " << sh << " Size " << sizeof(sh) << "  Min " << SCHAR_MIN << " Max " << SCHAR_MAX << std::endl;
	std::cout << "Unsigned Char " << uc << " Size " << sizeof(uc) << "  Min " << "0" << " Max " << UCHAR_MAX << std::endl;
	std::cout << "Unsigned Int " << ui << " Size " << sizeof(ui) << "  Min " << "0" << " Max " << UINT_MAX << std::endl;
	std::cout << "Unsigned Long " << ul << " Size " << sizeof(ul) << "  Min " << "0" << " Max " << ULONG_MAX << std::endl;
	std::cout << "Unsigned Long Long " << ull << " Size " << sizeof(ull) << "  Min " << "0" << " Max " << ULLONG_MAX << std::endl;
	system("pause");
	return 0;
}

