//#include <iostream>
bool matchProgram(std::string strx,std::string& programName);//匹配program语句
bool matchEmptyLine(std::string strx);//匹配空行

bool matchVarExpression(std::string strx,std::vector<std::string>& vecx);//匹配var变量申明语句
bool matchVarExpressionForArray(std::string strx,std::vector<std::string>& vecx);//匹配var变量申明语句

bool matchBegin(std::string strx);//匹配begin
bool matchEnd(std::string strx);//匹配end
bool matchElse(std::string strx);//匹配else

bool matchAssignmentExpression(std::string strx,std::vector<std::string>& vecx);//匹配赋值语句
bool matchAssignmentExpressionForArray(std::string strx,std::vector<std::string>& vecx);//匹配赋值语句

bool isVarName(std::string strx);