#include "stdafx.h"
#include <iostream>
#include "Person.h"


Person::Person()
{
}

Person::Person(const std::string & firstName, const std::string & lastName, unsigned short age, unsigned int phone) :
	_firstName{firstName}, _lastName{lastName}, _age{age}, _phone{phone}
{
}


Person::~Person()
{
	std::cout << "Goodbye from ~Person" << std::endl;
}

void Person::OutputAge()
{
	std::cout << "I am " << _age << " years old" << std::endl;
}
