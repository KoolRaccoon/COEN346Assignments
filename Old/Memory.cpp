#include "Memory.hpp"



Memory::Memory()
{
    ageInt = 0;
    contentValue = 0;
}

Memory::Memory(int id, int aging, int value) {
    varID = id;
    ageInt = aging;
    contentValue = value;
}

Memory::~Memory()
{
}

void Memory::setVarID(int id)
{
    varID = id;
}

void Memory::setAgeint(int aging)
{
    ageInt = aging;
    
}

void Memory::setValue(int value)
{
    contentValue = value;
}

int Memory::getVarID()
{
    return varID;
}

int Memory::getAge()
{
    return ageInt;
}

int Memory::getValue()
{
    return contentValue;
}
