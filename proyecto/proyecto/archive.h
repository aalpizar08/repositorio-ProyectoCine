#pragma once
#include <iostream>
#include <string>
using namespace std;
class Archive
{
private:
	string info;
public:
	Archive();
	string getInfo() const;
};
