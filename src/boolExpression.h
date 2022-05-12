//#include <iostream>
bool matchTest(std::string strx,std::vector<std::string>& vecx);//分隔布尔表达式
bool matchIfExpression(std::string strx,std::vector<std::string>& vecx);//匹配if语句
bool matchWhileExpression(std::string strx,std::vector<std::string>& vecx);//匹配while语句
void PushOperation(char *opera, char *ss, int *op, int *s);
void PexpretoSexpre(char *ss,std::string& result);
void inorder2postorder(std::string strx,std::string& result);
bool notOpera(char ch);

void PushOperation(std::vector<std::string>& opera,std::vector<std::string>& ss,int& op,int& s);
bool notOpera(std::string strx);
void PexpretoSexpre(std::vector<std::string>& ss ,std::vector<std::string>& result);
void inorder2postorder(std::vector<std::string> vecx,std::vector<std::string>& result);

bool convertBoolExpression(std::string strx,std::vector<std::string>& vecx,std::vector<std::string>& postVecx);

//void testPexpretoSexpre(std::vector<std::string>& ss ,std::vector<std::string>& result);

//void funcPexpretoSexpre(char *ss,std::string& result);

void PexpretoSexpreForBEX(std::vector<std::string>& ss ,std::vector<std::string>& result);
