#include "expressionHandle.h"
#include <iostream>
#include <boost/regex.hpp>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;
vector<string> globalQuaternion;
map<string,string> symbolTable;
vector <quaternion> allQuaternions;
string pascalTypes[maxnTypes] ={"integer","real"};