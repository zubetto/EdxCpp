#pragma once
#include "Person.h"
#include <iostream>

class Student : public Person
{
private:
	bool validate();

public:
	Student();
	Student(const std::string& firstName, const std::string& lastName, unsigned short age, unsigned int phone);
	~Student();

	void OutputAge();
	void OutputIdentity();
};

