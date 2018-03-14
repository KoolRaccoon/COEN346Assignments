#pragma once
#include <string>

using namespace std;

class MMU
{
public:
    MMU();
    virtual ~MMU();
    MMU(string, int, int);
    
    void setAction(string);
    void setActionID(int);
    void setVarID(int);
    void setToValue(int);
    
    string getAction();
    int getVarID();
    int getActionID();
    int getToValue();
    
private:
    string action;
    int actionID;
    int varID;
    int toValue;
};

