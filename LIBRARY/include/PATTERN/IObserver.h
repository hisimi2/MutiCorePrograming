#pragma once

#include <memory>
#include <iostream>
using namespace std;

class IObserver 
{
public:
	virtual ~IObserver() {}
	virtual void update(string notification) = 0;
};
