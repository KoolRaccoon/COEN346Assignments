#include "MMU.hpp"



MMU::MMU()
{
}


MMU::~MMU()
{
}

MMU::MMU(string act, int id, int value)
{
    action = act;
    varID = id;
    toValue = value;
}

void MMU::setAction(string act)
{
    action = act;
}

void MMU::setActionID(int aID)
{
    actionID = aID;
}

void MMU::setVarID(int id)
{
    varID = id;
}

void MMU::setToValue(int value)
{
    toValue = value;
}

string MMU::getAction()
{
    return action;
}

int MMU::getVarID()
{
    return varID;
}

int MMU::getActionID()
{
    return actionID;
}

int MMU::getToValue()
{
    return toValue;
}
