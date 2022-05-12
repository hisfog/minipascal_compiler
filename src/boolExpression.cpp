#include <iostream>
#include <boost/regex.hpp>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stack>   
using namespace std;


void PushOperation(char *opera, char *ss, int *op, int *s)//入操作数栈
{
    opera[*op] = ss[*s];
    (*op)++;
    (*s)++;
}
void PushOperation(vector<string>& opera,vector<string>& ss,int& op,int& s)//入操作数栈
{
    //opera.push_back(ss[s]);
   	opera[op]=ss[s];
    //cout<<ss[s]<<"21"<<endl;
    s++;
    op++;
}

bool notOpera(string strx)//判断是否为运算符，括号
{
    //cout<<"Yes"<<endl;
    if(strx=="+" || strx=="-" || strx=="*" || strx=="/" || strx=="(" || strx==")" || strx=="&" || strx=="|" || strx=="and" || strx=="or")
    	{
    		//cout<<"I juge 0"<<strx<<endl;
    		return 0;
    	}
    else return 1;
}
bool notOpera(char ch)//判断是否为运算符，括号
{
    
    if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='(' || ch==')' || ch=='&' || ch=='|')return 0;
    else return 1;
}
void printArr(vector<string> vecx,string flag)//for debug
{
	for(int i=0;i<vecx.size();i++)
	{
		if(vecx[i]!="")cout<<flag<<vecx[i];
	}
	cout<<endl;
}

bool matchTest(string strx,vector<string>& vecx)//分隔布尔表达式
{
    //strx=" and x13==13 or x15==15.6";
    boost::smatch matRes;
    boost::regex pattern("^(?:[\\s]+(and|or))[\\s]+((?:[a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*(?:==|!=|>=|<=|>|<)[\\s]*(?:(?:[-|+]?[0-9]+(?:.[0-9]+)?)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*)))((?:[\\s]+(?:and|or)[\\s]+(?:(?:[a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*(?:==|!=|>=|<=|>|<)[\\s]*(?:(?:[-|+]?[0-9]+(?:.[0-9]+)?)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*))))*)");
   
    int cnt=0;
    if(boost::regex_search(strx,matRes,pattern))
    {
       for(int i=0;i<matRes.size();i++)
        {
            if(matRes[i]!="")
            {
                //cout<<matRes[i]<<endl;
                cnt++;  
            }
            
        }
        if(cnt==4)
        {
            string getOperator=matRes[1];
            string getBoolExpression=matRes[2];
            vecx.push_back(getOperator);
            vecx.push_back(getBoolExpression);
            string nextStrx=matRes[3];
            matchTest(nextStrx,vecx);
        }
        else if(cnt==3)
        {
            string getOperator=matRes[1];
            string getBoolExpression=matRes[2];
            vecx.push_back(getOperator);
            vecx.push_back(getBoolExpression); 
        }
        return 1;  
    }
    else return 0;
}
string int2strForBEX(int itx)
{
    string res;
    stringstream ss;
    ss<<itx;
    ss>>res;
    return res;
}


void PexpretoSexpreForBEX(vector<string>& ss ,vector<string>& result)//对于布尔表达式的转后缀
{
    int i=0,j=0;
    int op=0;
    vector<string> num;
    num.resize(100);
    //num[10]="ioe";
    //num.push_back("0");
    vector<string> opera;
    opera.resize(100);
    //opera.push_back("0");
    while (i<ss.size())
    {
    	//cout<<i<<endl;
    	if (notOpera(ss[i]))    /* 如果是数字 */
        {
            //cout<<ss[i]<<endl;
            num[j]=ss[i];    /* 数字直接入后缀表达式栈 */
            j++;
            i++;
            //printArr(num,"$$$");
        }
        else 
        {
        	if(ss[i]=="or")
        	{
        		//cout<<"or"<<endl;
        		if (op == 0)    /* 如果是空栈 */
                {
                    PushOperation(opera, ss, op, i);    /* 入运算符栈 */
                    //printArr(opera,"!!!");
                    continue;
                }
                if (!notOpera(opera[op-1]))
                {
                	if(opera[op-1]=="or")
                	{
                		PushOperation(opera, ss, op, i);
                		//printArr(opera,"!!!");
                	}
                	else if(opera[op-1]=="and")
                	{
                		num[j]=opera[op-1];    /* 将操作数出栈 */
                        opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                        j++;
                        i++;
                       // printArr(num,"$$$");
                	}
                	else if(opera[op-1]=="(")
                	{
                		PushOperation(opera, ss, op, i);
                		//printArr(opera,"!!!");
                	}
                }
        	}
        	else if(ss[i]=="and")
        	{
        		//cout<<"and"<<endl;
        		if (op == 0)    /* 如果是空栈 */
                {
                    PushOperation(opera, ss, op, i);    /* 入运算符栈 */
                    //printArr(opera,"!!!");
                    continue;
                }
                if (!notOpera(opera[op-1]))
                {
                	if(opera[op-1]=="or")
                	{
                		PushOperation(opera, ss, op, i);
                		//printArr(opera,"!!!");
                	}
                	else if(opera[op-1]=="and")
                	{
                		PushOperation(opera, ss, op, i);
                		//printArr(opera,"!!!");
                	}
                	else if(opera[op-1]=="(")
                	{
                		PushOperation(opera, ss, op, i);
                		//printArr(opera,"!!!");
                	}
                }
        	}
        	else if(ss[i]=="(")
            {
                PushOperation(opera, ss, op, i);
                //printArr(opera,"!!!");
            }
            else if(ss[i]==")")
            {
                //cout<<")"<<endl;
                while (opera[op-1] != "(")
                {
                    num[j]=opera[op-1];    /* 将运算符栈中的元素依次入栈到后缀表达式栈中，直到遇到左括号为止 */
                    j++;
                    op--;
                    //printArr(num,"$$$");
                }
                op--;
                i++;
            }
            else
            {
            	cout<<"error"<<endl;
            }
        }
    }
    while (op != 0)
    {
        num[j]=opera[op-1];    /* 将运算符栈中的元素依次入栈到后缀表达式栈中 */
        j++;
        op--;
    }
    for(int i=0;i<num.size();i++)
    {
    	if(num[i]!="")result.push_back(num[i]);
    }
    //result=num;
}

bool convertBoolExpression(string strx,vector<string>& inVecx,vector<string>& postVecx)//把布尔表达式转化为简单形式，若转化为后缀式失败则有语法错误
{//若转化成功则返回inorder和postorder，支持带括号的布尔表达式
	//strx="(a==1 or a==2)and(b==1 or b==2 and b!=3)";
	boost::smatch matRes;
    boost::regex pattern("((?:[a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*(?:==|!=|>=|<=|>|<)[\\s]*(?:(?:[-|+]?[0-9]+(?:.[0-9]+)?)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*))|(?:and|or|\\(|\\)))");
    int cnt=0;
    vector<string> seperateVec;
    while(boost::regex_search(strx,matRes,pattern))
    {
    	string firstResult=matRes[0];
    	//cout<<matRes[0]<<endl;
    	int position=strx.find(firstResult);
    	seperateVec.push_back(firstResult);
    	if(firstResult!="(" && firstResult!=")")
    		inVecx.push_back(firstResult);
    	//int replacePosition=strRes.find()
    	strx.replace(position,firstResult.size(),int2strForBEX(cnt));
    	//strx=string(strx,position+firstResult.size());
    	cnt++;
    	//cout<<strx<<endl;
    }
    PexpretoSexpreForBEX(seperateVec,postVecx);
    //resStr=strx;
    return 1;
}
bool matchIfExpression(string strx,vector<string>& vecx)//匹配if语句，不含括号的可以
{
    //strx="if( x12 == a123)";
    //strx="  if( x12 == a123 and x13==13 or x15==15.6)";
    boost::smatch matRes;
    boost::regex pattern("^(?:[\\s]*if\\([\\s]*)((?:[a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*(?:==|!=|>=|<=|>|<)[\\s]*(?:(?:[-|+]?[0-9]+(?:.[0-9]+)?)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*)))((?:[\\s]+(?:and|or)[\\s]+(?:(?:[a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*(?:==|!=|>=|<=|>|<)[\\s]*(?:(?:[-|+]?[0-9]+(?:.[0-9]+)?)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*))))*)\\)then");
    if(boost::regex_search(strx,matRes,pattern))
    {
       int cnt=0;
       for(int i=0;i<matRes.size();i++)
        {
            if(matRes[i]!="")
            {
                //cout<<matRes[i]<<endl;
                cnt++;
            }
            
        }
        string getFirstExpression=matRes[1];
        vecx.push_back(getFirstExpression);
        if(cnt==3)
        {
            string nextStrx=matRes[2];
            matchTest(nextStrx,vecx);
        }        
        
        return 1; 
    }

    else return 0;

}

bool matchWhileExpression(string strx,vector<string>& vecx)//匹配while语句
{
    //strx="if( x12 == a123)";
    //strx="  if( x12 == a123 and x13==13 or x15==15.6)";
    boost::smatch matRes;
    boost::regex pattern("^(?:[\\s]*while\\([\\s]*)((?:[a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*(?:==|!=|>=|<=|>|<)[\\s]*(?:(?:[-|+]?[0-9]+(?:.[0-9]+)?)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*)))((?:[\\s]+(?:and|or)[\\s]+(?:(?:[a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*(?:==|!=|>=|<=|>|<)[\\s]*(?:(?:[-|+]?[0-9]+(?:.[0-9]+)?)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*))))*)\\)do");
    if(boost::regex_search(strx,matRes,pattern))
    {
       int cnt=0;
       for(int i=0;i<matRes.size();i++)
        {
            if(matRes[i]!="")
            {
                //cout<<matRes[i]<<endl;
                cnt++;
            }
            
        }
        string getFirstExpression=matRes[1];
        vecx.push_back(getFirstExpression);
        if(cnt==3)
        {
            string nextStrx=matRes[2];
            matchTest(nextStrx,vecx);
        }        
        
        return 1; 
    }

    else return 0;

}



// void testPexpretoSexpre(vector<string>& ss ,vector<string>& result)
// {
// 	int i=0,j=0;
//     int op=0;
//     vector<string> num;
//     vector<string> opera;
//     while (i<ss.size())
//     {
//     	if (notOpera(ss[i]))    /* 如果是数字 */
//         {
//             num.push_back(ss[i]);
//             //cout<<ss[i]<<"47"<<endl;
//             i++;
//             j++;
//             //continue;
//             cout<<"数字"<<endl;
//         }
//         else
//         {
//         	if(ss[i]=="or")
//             {
//                 cout<<"OR"<<endl;
//                 if (op == 0)
//                 {//cout<<"797979"<<endl;
//                     PushOperation(opera, ss, op, i);
//                     continue;
//                 }
//                 if(!notOpera(opera[op-1]))
//                 {
//                     if(opera[op-1]=="or")
//                     {
//                        PushOperation(opera, ss, op, i); 
//                        continue;
//                     }
//                     else if(opera[op-1]=="and")
//                     {
//                         num.push_back(opera[op-1]);    /* 将操作数出栈 */
//                         opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
//                         j++;
//                         i++;
//                         continue;
//                     }
//                     else if(opera[op-1]=="(")
//                     {
//                         PushOperation(opera, ss, op, i);
//                         continue;
//                     }
//                 }

//             }
//         	else if(ss[i]=="and")//*
//             {
//                 cout<<"AND"<<endl;
//                 if(op == 0)
//                 {
//                     PushOperation(opera, ss, op, i);
//                     continue;
//                 }
//                 if(!notOpera(opera[op-1]))
//                 {
//                     if(opera[op-1]=="or")
//                     {
//                         PushOperation(opera, ss, op, i);
//                         continue;
//                     }
//                     else if(opera[op-1]=="and")
//                     {
//                         PushOperation(opera, ss, op, i); 
//                         continue; 
//                     }
                    
//                     else if(opera[op-1]=="(")
//                     {
//                         PushOperation(opera, ss, op, i);
//                         continue;
//                     }
//                 }
//                 //continue;           
//             }
//             else if(ss[i]=="(")
//             {
//                 PushOperation(opera, ss, op, i);
//                 continue;
//             }
//             else if(ss[i]==")")
//             {
//                 while (opera[op-1] != "(")
//                 {
//                     num.push_back(opera[op-1]);    /* 将运算符栈中的元素依次入栈到后缀表达式栈中，直到遇到左括号为止 */
//                     j++;
//                     op--;
//                 }
//                 op--;
//                 i++;
//                 continue;
//             }
//         }
//     }
//     int opMem=op;
//     while (op != 0)
//     {
//         num.push_back(opera[op-1]);    /* 将运算符栈中的元素依次入栈到后缀表达式栈中 */
//         //cout<<num[j]<<endl;
//         j++;
//         op--;

//     }
//     result=num;
// }
void PexpretoSexpre(vector<string>& ss ,vector<string>& result)
{
   // printArr(ss," 777 ");
    int i=0,j=0;
    int op=0;
    vector<string> num;
    num.resize(100);
    vector<string> opera;
    opera.resize(100);
    while (i<ss.size())
    {
        //cout<<ss[i]<<endl;
        if (notOpera(ss[i]))    /* 如果是数字 */
        {
            num[j]=ss[i];
            //cout<<ss[i]<<"47"<<endl;
            i++;
            j++;
            //continue;
        }
        else
        {
            if(ss[i]=="and")//*
            {
                //cout<<"andndnadnadn"<<endl;
                if (op == 0)
                {
                    //cout<<"5959"<<endl;
                    PushOperation(opera, ss, op, i);
                    continue;
                }
                if (!notOpera(opera[op-1]))
                {
                    if(opera[op-1]=="and")
                    {
                        PushOperation(opera, ss, op, i);  
                    }
                    else if(opera[op-1]=="or")
                    {
                        PushOperation(opera, ss, op, i);
                    }
                    // else if(opera[op-1]=="(")
                    // {
                    //     PushOperation(opera, ss, op, i);
                    // }
                }
                continue;           
            }
            else if(ss[i]=="or")
            {
                //cout<<"aooroarjro"<<endl;
                if (op == 0)
                {//cout<<"797979"<<endl;
                    PushOperation(opera, ss, op, i);
                    continue;
                }
                //cout<<opera[op-1]<<endl;
                if (!notOpera(opera[op-1]))
                {
                    if(opera[op-1]=="or")
                    {
                       //cout<<"82"<<endl;
                       PushOperation(opera, ss, op, i); 
                    }
                    else if(opera[op-1]=="and")
                    {
                        num[j]=(opera[op-1]);    /* 将操作数出栈 */
                        opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                        j++;
                        i++;
                        //cout<<"91"<<endl;
                    }
                    // else if(opera[op-1]=="(")
                    // {
                    //     PushOperation(opera, ss, op, i);
                    //     //break;
                    // }
                    //continue;
                }
                
                continue;
            }

            char c=*(ss[i].c_str());
            //cout<<"???"<<endl;
            switch (c)    /* 如果是操作数 */
            {
            case '+':
                {
                    if (op == 0)    /* 如果是空栈 */
                    {
                        PushOperation(opera, ss, op, i);    /* 入运算符栈 */
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        char ch=*(opera[op-1].c_str());
                        switch (ch)
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '*':
                            {    /* 加法优先级低于乘法 */
                                num[j]=(opera[op-1]);    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                                j++;
                                i++;
                                break;
                            }
                        case '&':
                            {    /* 加法优先级低于乘法 */
                                num[j]=(opera[op-1]);    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                                j++;
                                i++;
                                break;
                            }
                        case '/':
                            {
                                num[j]=(opera[op-1]);
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '|':
                {
                    if (op == 0)    /* 如果是空栈 */
                    {
                        PushOperation(opera, ss, op, i);    /* 入运算符栈 */
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        char ch=*(opera[op-1].c_str());
                        switch (ch)
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '*':
                            {    /* 加法优先级低于乘法 */
                                num[j]=(opera[op-1]);    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                                j++;
                                i++;
                                break;
                            }
                        case '&':
                            {    /* 加法优先级低于乘法 */
                                num[j]=(opera[op-1]);    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                                j++;
                                i++;
                                break;
                            }
                        case '/':
                            {
                                num[j]=(opera[op-1]);
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '-':
                {
                    if (op == 0)
                    {
                        PushOperation(opera, ss, op, i);
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        char ch=*(opera[op-1].c_str());
                        switch (ch)
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '*':
                            {
                                num[j]=(opera[op-1]);
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '&':
                            {
                                num[j]=(opera[op-1]);
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '/':
                            {
                                num[j]=(opera[op-1]);
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '*':
                {
                    if (op == 0)
                    {
                        PushOperation(opera, ss, op, i);
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        char ch=*(opera[op-1].c_str());
                        switch (ch)
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '*':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '&':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '/':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '&':
                {
                    if (op == 0)
                    {
                        PushOperation(opera, ss, op, i);
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        char ch=*(opera[op-1].c_str());
                        switch (ch)
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '*':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '&':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '/':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '/':
                {
                    if (op == 0)
                    {
                        PushOperation(opera, ss, op, i);
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        char ch=*(opera[op-1].c_str());
                        switch (ch)
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '*':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '&':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '/':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, op, i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '(':
                {
                    PushOperation(opera, ss, op, i);
                    break;
                }
            case ')':    /* 如果遇到右括号 */
                {
                    while (opera[op-1] != "(")
                    {
                        num[j]=(opera[op-1]);    /* 将运算符栈中的元素依次入栈到后缀表达式栈中，直到遇到左括号为止 */
                        j++;
                        op--;
                    }
                    op--;
                    i++;
                    break;
                }
            default:
                {
                    printf("传入表达式不符合要求\n");
                    exit(0);
                }

            }
        }
    }
    //cout<<"ok"<<endl;
    int opMem=op;
    while (op != 0)
    {
        num[j]=(opera[op-1]);    /* 将运算符栈中的元素依次入栈到后缀表达式栈中 */
        //cout<<num[j]<<endl;
        j++;
        op--;

    }
    for(int i=0;i<num.size();i++)
    {
    	if(num[i]!="")result.push_back(num[i]);
    }
    //result=num;
}
void PexpretoSexpre(char *ss,string& result)
{
    //ss="1+2*3";
    char num[100] = "0";    /* 存储后缀表达式 */
    char opera[100] = "0";    /* 存储运算符 */
    /*
    num----j
    opera----op
    ss----i
    */
    int i, j, op;

    op = i = j = 0;

    while (ss[i] != '\0')
    {
        if (notOpera(ss[i]))    /* 如果是数字 */
        {
            num[j] = ss[i];    /* 数字直接入后缀表达式栈 */
            j++;
            i++;
        }
        else
        {
            switch (ss[i])    /* 如果是操作数 */
            {
            case '+':
                {
                    if (op == 0)    /* 如果是空栈 */
                    {
                        PushOperation(opera, ss, &op, &i);    /* 入运算符栈 */
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        switch (opera[op-1])
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '*':
                            {    /* 加法优先级低于乘法 */
                                num[j] = opera[op-1];    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                                j++;
                                i++;
                                break;
                            }
                        case '&':
                            {    /* 加法优先级低于乘法 */
                                num[j] = opera[op-1];    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                                j++;
                                i++;
                                break;
                            }
                        case '/':
                            {
                                num[j] = opera[op-1];
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '|':
                {
                    if (op == 0)    /* 如果是空栈 */
                    {
                        PushOperation(opera, ss, &op, &i);    /* 入运算符栈 */
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        switch (opera[op-1])
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '*':
                            {    /* 加法优先级低于乘法 */
                                num[j] = opera[op-1];    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                                j++;
                                i++;
                                break;

                            }
                        case '&':
                            {    /* 加法优先级低于乘法 */
                                num[j] = opera[op-1];    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                                j++;
                                i++;
                                break;
                            }
                        case '/':
                            {
                                num[j] = opera[op-1];
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '-':
                {
                    if (op == 0)
                    {
                        PushOperation(opera, ss, &op, &i);
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        switch (opera[op-1])
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '*':
                            {
                                num[j] = opera[op-1];
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '&':
                            {
                                num[j] = opera[op-1];
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '/':
                            {
                                num[j] = opera[op-1];
                                opera[op-1] = ss[i];
                                j++;
                                i++;
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '*':
                {
                    if (op == 0)
                    {
                        PushOperation(opera, ss, &op, &i);
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        switch (opera[op-1])
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '*':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '&':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '/':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '&':
                {
                    if (op == 0)
                    {
                        PushOperation(opera, ss, &op, &i);
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        switch (opera[op-1])
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '*':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '&':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '/':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '/':
                {
                    if (op == 0)
                    {
                        PushOperation(opera, ss, &op, &i);
                        break;
                    }
                    if (!notOpera(opera[op-1]))
                    {
                        switch (opera[op-1])
                        {
                        case '+':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '|':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '-':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '*':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '&':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '/':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        case '(':
                            {
                                PushOperation(opera, ss, &op, &i);
                                break;
                            }
                        }
                    }
                    break;
                }
            case '(':
                {
                    PushOperation(opera, ss, &op, &i);
                    break;
                }
            case ')':    /* 如果遇到右括号 */
                {
                    while (opera[op-1] != '(')
                    {
                        num[j] = opera[op-1];    /* 将运算符栈中的元素依次入栈到后缀表达式栈中，直到遇到左括号为止 */
                        j++;
                        op--;
                    }
                    op--;
                    i++;
                    break;
                }
            default:
                {
                    printf("传入表达式不符合要求\n");
                    exit(0);
                }

            }
        }
    }
    int opMem=op;
    while (op != 0)
    {
        num[j] = opera[op-1];    /* 将运算符栈中的元素依次入栈到后缀表达式栈中 */
        j++;
        op--;
    }
    result=num;
    //cout<<result<<endl;

}

void inorder2postorder(string strx,string& result)//综合调用
{
    //cout<<strx<<endl;
    PexpretoSexpre((char*)strx.c_str(),result);
    return ;
}
void inorder2postorder(vector<string> vecx,vector<string>& result)
{
    //cout<<strx<<endl;
    PexpretoSexpre(vecx,result);
    return ;
}

// void funcPexpretoSexpre(char *ss,string& result)
// {
//     //ss="1+2*3";
//     char num[100] = "0";    /* 存储后缀表达式 */
//     char opera[100] = "0";    /* 存储运算符 */
//     int i, j, op;

//     op = i = j = 0;

//     while (ss[i] != '\0')
//     {
//         if (notOpera(ss[i]))    /* 如果是数字 */
//         {
//             num[j] = ss[i];    /* 数字直接入后缀表达式栈 */
//             j++;
//             i++;
//             cout<<"~~~~~~~num="<<num<<endl;
//         }
//         else
//         {
//             switch (ss[i])    /* 如果是操作数 */
//             {
//             case '+':
//                 {
//                     if (op == 0)    /* 如果是空栈 */
//                     {
//                         PushOperation(opera, ss, &op, &i);    /* 入运算符栈 */
//                     	cout<<"~~~~~~~opera="<<opera<<endl;
//                         break;
//                     }
//                     if (!notOpera(opera[op-1]))
//                     {
//                         switch (opera[op-1])
//                         {
//                         case '+':
//                             {
//                                 PushOperation(opera, ss, &op, &i);
//                                 cout<<"~~~~~~~opera="<<opera<<endl;
//                                 break;
//                             }
                    
//                         case '*':
//                             {    /* 加法优先级低于乘法 */
//                                 num[j] = opera[op-1];    /* 将操作数出栈 */
//                                 opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
//                                 j++;
//                                 i++;
//                                 cout<<"~~~~~~~num="<<num<<endl;
//                                 break;
//                             }
                    
//                         case '(':
//                             {
//                                 PushOperation(opera, ss, &op, &i);
//                                 cout<<"~~~~~~~opera="<<opera<<endl;
//                                 break;
//                             }
//                         }
//                     }
//                     break;
//                 }
            
//             case '*':
//                 {
//                     if (op == 0)
//                     {
//                         PushOperation(opera, ss, &op, &i);
//                         cout<<"~~~~~~~opera="<<opera<<endl;
//                         break;
//                     }
//                     if (!notOpera(opera[op-1]))
//                     {
//                         switch (opera[op-1])
//                         {
//                         case '+':
//                             {
//                                 PushOperation(opera, ss, &op, &i);
//                                 cout<<"~~~~~~~opera="<<opera<<endl;
//                                 break;
//                             }
                        
//                         case '*':
//                             {
//                                 PushOperation(opera, ss, &op, &i);
//                                 cout<<"~~~~~~~opera="<<opera<<endl;
//                                 break;
//                             }
                        
//                         case '(':
//                             {
//                                 PushOperation(opera, ss, &op, &i);
//                                 cout<<"~~~~~~~opera="<<opera<<endl;
//                                 break;
//                             }
//                         }
//                     }
//                     break;
//                 }
            
//             case '(':
//                 {
//                     PushOperation(opera, ss, &op, &i);
//                     cout<<"~~~~~~~opera="<<opera<<endl;
//                     break;
//                 }
//             case ')':    /* 如果遇到右括号 */
//                 {
//                     while (opera[op-1] != '(')
//                     {
//                         num[j] = opera[op-1];    /* 将运算符栈中的元素依次入栈到后缀表达式栈中，直到遇到左括号为止 */
//                         j++;
//                         op--;
//                         cout<<"~~~~~~~num="<<num<<endl;
//                     }
//                     op--;
//                     i++;
//                     break;
//                 }
//             default:
//                 {
//                     printf("传入表达式不符合要求\n");
//                     exit(0);
//                 }

//             }
//         }
//     }
//     cout<<"~~~~~~~num="<<num<<endl;
//     cout<<"~~~~~~~opera="<<opera<<endl;
//     cout<<"###op= "<<op<<endl;
//     int opMem=op;
//     while (op != 0)
//     {
//         num[j] = opera[op-1];    /* 将运算符栈中的元素依次入栈到后缀表达式栈中 */
//         j++;
//         op--;
//     }
//     result=num;
//     //cout<<result<<endl;

// }

