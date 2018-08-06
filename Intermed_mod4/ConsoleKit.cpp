#include "stdafx.h"
#include "ConsoleKit.h"
#include <iomanip>
#include <string>

std::ios_base::fmtflags ConsoleKit::_notation{ std::cout.fixed };
unsigned short ConsoleKit::_keyWidth{ 4 };
unsigned short ConsoleKit::_valueWidth{ 9 };
unsigned short ConsoleKit::_precision{ 2 };
std::string ConsoleKit::_delim{ " | " };

ConsoleKit::ConsoleKit()
{
}


ConsoleKit::~ConsoleKit()
{
}

void ConsoleKit::setNotation(std::ios_base::fmtflags notation)
{
	if (notation & std::ios_base::floatfield)
	{
		_notation = notation;
	}
}

std::ios_base::fmtflags ConsoleKit::getNotation()
{
	return _notation;
}

void ConsoleKit::setKeyWidth(unsigned short w)
{
	_keyWidth = w;
}

void ConsoleKit::setValueWidth(unsigned short w)
{
	_valueWidth = w + 1 + _precision;
}

void ConsoleKit::setPrecision(unsigned short n)
{
	_valueWidth -= _precision;
	_valueWidth += n;
	_precision = n;
}

void ConsoleKit::setDelimeter(const std::string & delim)
{
	_delim = delim;
}

void ConsoleKit::outKeyValue(int key, int value)
{
	using namespace std;

	cout << right << setw(_keyWidth) << key;
	cout << _delim;
	cout << setprecision(_precision) << setw(_valueWidth) << value << endl;
}

void ConsoleKit::outKeyValue(int key, double value)
{
	using namespace std;

	cout.unsetf(ios::fixed | ios::scientific);
	cout << right << setw(_keyWidth) << key;
	cout << _delim;
	cout.setf(_notation);
	cout << setprecision(_precision) << setw(_valueWidth) << value << endl;
}
