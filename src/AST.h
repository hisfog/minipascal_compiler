#ifndef AST_H
#define AST_H

#include <vector>
#include <map>
#include <iostream>
#include <boost/regex.hpp>
using namespace std;

//vector<string> pascalTypes;
class AST//语法树
{
    public:
    vector<string> ptrArray;//子节点
    AST* ptrFather;//父节点
    string data;
    string expressionData;
    string root;
    AST(string strx);
    AST();
    ~AST();
    void append(string ptrx);
    //AST* findFather(string booExpression);
    //string anotherChildData(string boolExpression);

};

extern map<string,AST> globalMap;
extern map<string,AST> globalMapForQ;
extern map<string,int> value;//真值
extern map<string,string> jmpTable1;
extern map<string,string> jmpTable0;
extern map<string,string> father;

string buildTree(vector<string>& inorder, vector<string>& postorder, int inStart, int inEnd, int postEnd, int postStart,string name);
//建立算术表达式的语法树
AST buildTreeForQuaternion(vector<string>& inorder, vector<string>& postorder, int inStart, int inEnd, int postEnd, int postStart,string name,string& firstBEX,map<string,string>& jmpTable0,map<string,string>& jmpTable1,map<string,string>& name2index);
//建立布尔表达式的语法树，并填写跳转表
string buildASTtree(vector<string>& inorder, vector<string>& postorder,string name);

string buildASTtreeForQuaternion(vector<string>& inorder, vector<string>& postorder,string name,string& firstBEX,map<string,string>& jmpTable0,map<string,string>& jmpTable1,map<string,string>& name2index);

int shortCircuitBoolCalculate(int x,int y,string op);//短路求值
string getAnotherChildData(string one,string fatherData);//获取另一个子节点（不同于one的）
string calculateNextBoolExpression(map<string,int> vec,string BE,string rootBEX);//计算下一个要考虑的布尔表达式
void DFS(map<string,int> vec,string BEX,string rootBEX,map<string,string>& jmpTable0,map<string,string>& jmpTable1);//搜索，用于填写跳转表


#endif