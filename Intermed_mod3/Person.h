#pragma once

#include <string>

class Person
{
private:
	unsigned short _age;

protected:
	unsigned int _phone;

public:
	Person();
	Person(const std::string& firstName, const std::string& lastName, unsigned short age, unsigned int phone);
	virtual ~Person();

	std::string _firstName;
	std::string _lastName;

	virtual void OutputAge();
	virtual void OutputIdentity() = 0;
};

