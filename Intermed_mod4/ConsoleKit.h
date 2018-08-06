#pragma once
#include <ios>
#include <iostream>

class ConsoleKit
{
private:
	static std::ios_base::fmtflags _notation; // is used to output values
	static unsigned short _keyWidth;
	static unsigned short _valueWidth;
	static unsigned short _precision; // is used to output values
	static std::string _delim;

public:
	ConsoleKit();
	~ConsoleKit();

	static void setNotation(std::ios_base::fmtflags notation);
	static std::ios_base::fmtflags getNotation();

	static void setKeyWidth(unsigned short w);
	static void setValueWidth(unsigned short w);
	static void setPrecision(unsigned short n);
	static void setDelimeter(const std::string& delim);

	static void outKeyValue(int key, int value);
	static void outKeyValue(int key, double value);
};

