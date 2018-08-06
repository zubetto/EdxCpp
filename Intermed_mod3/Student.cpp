#include "stdafx.h"
#include <iostream>
#include "Student.h"


bool Student::validate()
{
	return _phone > 0;
}

Student::Student()
{
}

Student::Student(const std::string & firstName, const std::string & lastName, unsigned short age, unsigned int phone) :
	Person(firstName, lastName, age, phone)
{
}


Student::~Student()
{
	std::cout << "Goodbye from ~Student" << std::endl;
}

void Student::OutputAge()
{
	Student::OutputIdentity();
	Person::OutputAge();
}

void Student::OutputIdentity()
{
	std::cout << "I am a student " << _firstName << " " << _lastName << std::endl;
	std::cout << "My phone: " << _phone << std::endl;
}
