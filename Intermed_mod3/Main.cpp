// EdxCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Person.h"
#include "Student.h"
#include <string>


int main()
{
	using namespace std;

	cerr << "Hello from cerr!\n" << endl;

	Person* student1 = new Student("Stiv", "Vai", static_cast<unsigned short>(33), static_cast<unsigned int>(5931477));
	student1->OutputAge();
	delete student1;
    return 0;
}

