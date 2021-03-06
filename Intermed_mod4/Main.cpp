// Intermed_mod4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include "ConsoleKit.h"

// Specifies which index is set to the end of stream
enum IndexAttribute { None, First, Second };

std::string getFileExt(const std::string& filename);


bool parseShow(std::stringstream& ss, unsigned int& index1, unsigned int& index2, IndexAttribute& ia);

template <typename T>
bool parseDonate(std::stringstream& ss, std::string& name, T& value);


// If IndexAttribute is set to None then the index value is used to define read position,
// otherwise, these methods read the last item of the stream
template <typename T>
T readItem(std::istream& stream, unsigned int& index, IndexAttribute ia = None);

template <typename T>
T readItemBin(std::istream& stream, unsigned int& index, IndexAttribute ia = None);

template <typename T>
T readNext(std::istream& stream, unsigned int& index);

template <typename T>
T readNextBin(std::istream& stream, unsigned int& index);

template <typename T>
bool writeItem(std::ostream& stream, T value);

template <typename T>
bool writeItemBin(std::ostream& stream, T value);


void outCommsPrompt();
std::string trim(const std::string& str, const std::string& whitespace = " \t");


int main()
{
	using namespace std;
	using valueType = int;
	using ckit = ConsoleKit;

	string comm{ "" };

	// --- The main loop --------------------------------------------------
	while (comm != "x")
	{
		string filename{ "" };
		fstream fs;

		bool isBin{ false };

		// ---open file dialog ---
		while (!fs.is_open())
		{
			if (filename != "")
				cerr << "Unable to open " << filename << "\n\n";

			cout << "Open file: ";
			getline(cin, filename);

			string ext{ getFileExt(filename) };

			if (ext == "txt")
				isBin = false;
			else if (ext != "")
				isBin = true;
			else
				continue;

			// try to open the file for read
			fs.open(filename, ios_base::in);

			if (fs.is_open())
			{
				fs.close();

				// try to open the file for read/write
				if (isBin)
					fs.open(filename, ios_base::binary | ios_base::app | ios_base::in | ios_base::out);
				else
					fs.open(filename, ios_base::app | ios_base::in | ios_base::out);
			}
		}

		outCommsPrompt();

		// --- define read and write function pointers corresponding of the flag isBin ---
		valueType(*read)(istream&, unsigned int&, IndexAttribute) { isBin ? readItemBin<valueType> : readItem<valueType> };
		valueType(*readnext)(istream&, unsigned int&) { isBin ? readNextBin<valueType> : readNext<valueType> };
		bool(*write)(ostream&, valueType) { isBin ? writeItemBin<valueType> : writeItem<valueType> };

		// for commands parsing
		stringstream sstr;

		// --- commands dialog ---
		while (true)
		{
			cout << "\n>>: ";
			getline(cin, comm);

			comm = trim(comm);

			if (comm == "x" || comm == "X" || comm == "exit")
				break;

			sstr.clear();
			sstr.str(comm);
			sstr.seekg(0);

			string choice;
			sstr >> choice;

			if (choice == "show")
			{
				unsigned int ind1{ 0 };
				unsigned int ind2{ 0 };
				IndexAttribute inda{ None };

				if (!parseShow(sstr, ind1, ind2, inda))
				{
					cout << "Invalid input\n";
					continue;
				}

				cout << endl;

				switch (inda)
				{
					valueType val;

					case None:
						val = read(fs, ind1, None);

						while (fs.good() && ind1 <= ind2)
						{
							ckit::outKeyValue(ind1, val);

							val = readnext(fs, ind1);
						}

						if (!fs.fail() && ind1 <= ind2)
							ckit::outKeyValue(ind1, val);

						break;


					case First:
						ckit::outKeyValue(ind1, read(fs, ind1, First));
						break;


					case Second:
						val = read(fs, ind1, None);

						while (fs.good())
						{
							ckit::outKeyValue(ind1, val);
							
							val = readnext(fs, ind1);
						}

						if (!fs.fail())
							ckit::outKeyValue(ind1, val);

						break;
				}
			}
			else if (choice == "donate")
			{
				string name{ "" };
				valueType amount{ 0 };

				if (!parseDonate(sstr, name, amount))
				{
					cout << "Invalid input\n";
					continue;
				}

				if (amount > 0)
				{
					unsigned int ind{ 0 };
					valueType value{ 0 };

					value = read(fs, ind, First);

					if (fs.fail())
					{
						cout << "Failed to get access to the donation records\n";
						continue;
					}

					if (write(fs, value + amount))
						cout << "Thank you, " << name << "!\n";
					else
						cout << "Failed to create donation record\n";
				}
				else
				{
					cout << "Sorry, only positive amount can be accepted\n";
				}
			}
			else
			{
				cout << "Invalid input\n";
			}
		}

		fs.close();

		cout << "\nEnter 'x' to exit: ";
		getline(cin, comm);
		comm = trim(comm);

		if (comm == "x" || comm == "X" || comm == "exit")
			break;
		else
			cout << "-----------------------\n\n\n";
	}

    return 0;
}


std::string getFileExt(const std::string& filename) 
{
	size_t i = filename.rfind('.', filename.length());

	if (i != std::string::npos)
		return(filename.substr(i + 1, filename.length() - i));
	else
		return("");
}

bool parseShow(std::stringstream& ss, unsigned int& index1, unsigned int& index2, IndexAttribute& ia)
{
	using namespace std;

	if (ss.eof())
	{
		ia = First;
		return true;
	}

	string indinfo{ "" };
	ss >> indinfo;

	if (indinfo == "all")
	{
		index1 = 0;
		ia = Second;
		return true;
	}
	else
	{
		ss.seekg(0);

		ss >> indinfo >> index1;

		if (ss.fail())
		{
			return false;
		}
		else if (ss.eof())
		{
			index2 = index1;
			ia = None;
			return true;
		}
		else
		{
			ss >> index2;

			if (ss.fail())
			{
				return false;
			}
			else
			{
				ia = None;
				return true;
			}
		}
	}
}

template <typename T>
bool parseDonate(std::stringstream & ss, std::string & name, T& value)
{
	ss >> name >> value;

	return !ss.fail();
}

template <typename T>
T readItem(std::istream & stream, unsigned int & index, IndexAttribute ia)
{
	stream.clear();
	stream.seekg(0);

	T tmp{ 0 };
	unsigned int i{ 0 };

	if (ia == None)
	{
		while (stream.good() && i <= index)
		{
			stream >> tmp;
			++i;
		}
	}
	else
	{
		while (stream.good())
		{
			stream >> tmp;
			++i;
		}
	}

	index = i - 1;

	return tmp;
}

template <typename T>
T readItemBin(std::istream & stream, unsigned int & index, IndexAttribute ia)
{
	stream.clear();
	T value{ 0 };

	// --- try to read at the specified index ---
	if (ia == None)
	{
		stream.seekg(index * sizeof(T));
		stream.read((char*)&value, sizeof(T));

		if (stream.good())
			return value; // >>> successful reading at the specified index >>>
		else
			stream.clear();
	}

	// --- read the last item ---
	stream.seekg(-static_cast<int>(sizeof(T)), std::ios::end);

	if (stream.good()) // then there is at least one entry
	{
		index = static_cast<unsigned int>(stream.tellg() / sizeof(T));

		stream.read((char*)&value, sizeof(T));
	}
	else // there are no entries
	{
		stream.clear();
		index = 0;
	}

	return value;
}

template<typename T>
inline T readNext(std::istream & stream, unsigned int & index)
{
	++index;
	T value;
	stream >> value;

	return value;
}

template<typename T>
inline T readNextBin(std::istream & stream, unsigned int & index)
{
	++index;
	T value;
	stream.read((char*)&value, sizeof(T));

	return value;
}

template<typename T>
bool writeItem(std::ostream & stream, T value)
{
	stream.clear();
	stream << std::endl << value;

	return !stream.fail();
}

template<typename T>
bool writeItemBin(std::ostream & stream, T value)
{
	stream.clear();
	stream.seekp(0, std::ios::end);
	stream.write((char*)&value, sizeof(T));

	return !stream.fail();
}

void outCommsPrompt()
{
	using namespace std;
	static constexpr int w{ 14 };

	cout << "\n Available commands:\n\n";
	cout << "show [all] [index1 [index2]]\n";
	cout << left << setw(w) << " " << " - to show the last item\n";
	cout << setw(w) << " all" << " - to show all items\n";
	cout << setw(w) << " index" << " - to show item at this index\n";
	cout << setw(w) << " index1 index2" << " - to show items from index1 to index2 inclusive\n\n";

	cout << "donate <name> <donation_amount>\n";
	cout << " for example: donate Tom 25\n\n";

	cout << "exit | x\n";
	cout << " to exit\n---------\n";
}


// --------------------------------------------------------------------------------------------------------------------------------
// Taken from https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string

std::string trim(const std::string& str, const std::string& whitespace)
{
	const auto strBegin = str.find_first_not_of(whitespace);

	if (strBegin == std::string::npos)
		return ""; // no content

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

std::string reduce(const std::string& str, const std::string& fill = " ", const std::string& whitespace = " \t")
{
	// trim first
	auto result = trim(str, whitespace);

	// replace sub ranges
	auto beginSpace = result.find_first_of(whitespace);
	while (beginSpace != std::string::npos)
	{
		const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
		const auto range = endSpace - beginSpace;

		result.replace(beginSpace, range, fill);

		const auto newStart = beginSpace + fill.length();
		beginSpace = result.find_first_of(whitespace, newStart);
	}

	return result;
}