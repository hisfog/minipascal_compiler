#include <iostream>
#include <boost/regex.hpp>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "boolExpression.h"
#include "basic.h" 
#include "AST.h"    
using namespace std;
#define maxnTypes 20
vector<string> globalQuaternion;
map<string,string> symbolTable;
string handleWhileExpression(vector<string>& vecx,vector<string> getInfox,int begin,int end);
struct quaternion
{
    string first;
    string second;
    string third;
    string fourth;
};
vector <quaternion> allQuaternions;
string pascalTypes[maxnTypes] ={"integer","real"};
string int2str(int itx)
{
    string res;
    stringstream ss;
    ss<<itx;
    ss>>res;
    return res;
}
bool ispascalType(string strx)//判断是否为内置类型
{
	bool res=0;
	for(int i=0;i<maxnTypes;i++)
	{
		if(strx==pascalTypes[i])res=1;
	}
	return res;
}
void printArray(vector<string> vecx,string flag)//for debug
{
	for(int i=0;i<vecx.size();i++)
	{
		if(vecx[i]!="")cout<<flag<<vecx[i];
	}
	cout<<endl;
}
string generateQuaternion(vector<string> post,vector<string>& quaternion)//生成四元式，算术表达式和赋值语句
{
    if(post.size()==1)
    {
        string get="(=,"+post[0]+"--,T1)";//中间变量
        quaternion.push_back(get);
        return "T1";
    }
    stack<string> stack;
    int tmpCnt=0;
    string tail="T";
    for(int i=0;i<post.size();i++)//遍历后序序列
    {
        if(post[i]=="+" || post[i]=="-" || post[i]=="*" || post[i]=="/")//归约
        {
            tmpCnt++;
            tail+=int2str(tmpCnt);
            string getPop1=stack.top();//操作数出栈
            stack.pop();
            string getPop2=stack.top();
            stack.pop();
            string get="("+post[i]+","+getPop1+","+getPop2+","+tail+")";//生成四元式序列
            //cout<<get<<endl;
            quaternion.push_back(get);
            stack.push(tail);//中间值压栈
            tail="T";
        }
        else
        {
            stack.push(post[i]);//操作数压栈
        }

    }
    return tail+int2str(tmpCnt);
}

vector<int> generateQuaternionForBEX(vector<string>& vecx,vector<string>& quaternion,map<string,string>& name2index,map<string,string>& jmpTable0,map<string,string>& jmpTable1)
{//生成布尔表达式的四元式序列
    //printArray(vecx,"!!!!");
    vector<int> waitIndexes;
    int Getsize=0;
    int indexWait=0;
    int jmpNum=quaternion.size();
    for(int i=0;i<vecx.size();i++)
    {
        if(notOpera(vecx[i]))
            Getsize++;
        else
        	vecx.erase(vecx.begin()+i);
    }

    //cout<<"@@@@@@@@@@@@@@@@@@############"<<Getsize<<endl;
    for(int i=0;i<vecx.size();i++)
    {
        string BEX=vecx[i];
        boost::smatch matRes;
        boost::regex pattern("([a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*(==|!=|>=|<=|>|<)[\\s]*((?:[-|+]?[0-9]+[.[0-9]+]?)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*))");
        if(boost::regex_search(BEX,matRes,pattern))
        {
            string getVar1=matRes[1];//第一个操作数
            string getVar2=matRes[3];//第二个操作数
            string operatorStr=matRes[2];//操作符
            string getIndex=name2index[BEX];//获得名称对应的跳转表索引
            
            string tureJmp=jmpTable1[getIndex];
            string falseJmp=jmpTable0[getIndex];
            if(tureJmp!="True" && tureJmp!="False")//下一跳不是出口
            {
                int index=tureJmp.find(':');
                string getRealBEX=tureJmp.substr(index+1);
                vector<string>::iterator itGet = find(vecx.begin(), vecx.end(),getRealBEX); 
                int indexNumber=std::distance(std::begin(vecx), itGet);
                indexNumber=jmpNum+(indexNumber)*2;
                string get="(j"+operatorStr+","+getVar1+","+getVar2+","+int2str(indexNumber)+")";
                quaternion.push_back(get);
                //cout<<"@@@@@@@@I get "<<get<<endl;
            }
            else if(tureJmp=="True")
            {
                //cout<<
                int indexNumber=jmpNum+(vecx.size())*2;
                string get="(j"+operatorStr+","+getVar1+","+getVar2+","+int2str(indexNumber)+")";
                quaternion.push_back(get);
            }
            else
            {
                string get="(j"+operatorStr+","+getVar1+","+getVar2+","+tureJmp+")";
                quaternion.push_back(get);
                indexWait=quaternion.size()-1;
                waitIndexes.push_back(indexWait);
                //cout<<"@@@@@@@@I get "<<get<<endl;
            }
            if(falseJmp!="True" && falseJmp!="False")
            {
                int index=falseJmp.find(':');
                string getRealBEX=falseJmp.substr(index+1);
                //cout<<"~~~~~~~~~~BEX~~~~"<<BEX<<endl;
                //cout<<"~~~~~~~~~~~~~"<<getRealBEX<<endl;
                //printArray(vecx,"!!!!");
                vector<string>::iterator itGet = find(vecx.begin(), vecx.end(),getRealBEX); 
                int indexNumber=std::distance(std::begin(vecx), itGet);
                indexNumber=jmpNum+(indexNumber)*2;
                //cout<<"~~~~jmpNum= "<<jmpNum<<" indexNumber= "<<indexNumber<<endl;
                string get="(j,--,--,"+int2str(indexNumber)+")";
                quaternion.push_back(get);
                //cout<<"@@@@@@@@I get "<<get<<endl;
            }
            else if(falseJmp=="True")
            {
                int indexNumber=jmpNum+(Getsize)*2;
                string get="(j,--,--,"+int2str(indexNumber)+")";
                quaternion.push_back(get);
            }
            else
            {
                string get="(j,--,--,"+falseJmp+")";
                quaternion.push_back(get);
                indexWait=quaternion.size()-1;
                waitIndexes.push_back(indexWait);
                //cout<<"@@@@@@@@I get "<<get<<endl;
            }
            
            
            // for(int i=0;i<matRes.size();i++)
            // {
            //     if(matRes[i]!="")
            //     {
            //         cout<<"@@@@@generateQuaternionForBEX"<<matRes[i]<<endl;
            //         //cnt++;  
            //     }
                
            // }
            //return "";
        } 
    }
    
    return waitIndexes;
}
void generateGraph()//写入.dot文件，便于后续生成语法树的图
{
    streambuf* coutBuf = cout.rdbuf();
    ofstream of("out.dot");
    // 获取文件out.txt流缓冲区指针
    streambuf* fileBuf = of.rdbuf();
    
    // 设置cout流缓冲区指针为out.txt的流缓冲区指针
    cout.rdbuf(fileBuf);

    cout<<"digraph G\n{"<<endl;;
    
    map<string,AST>::iterator mapIter;
    for(mapIter=globalMap.begin();mapIter!=globalMap.end();mapIter++)
    {
        //if((mapIter->second).ptrArray.size()>0)
            //cout<<(mapIter->first)<<" has child node:"<<endl;
        for(int i=0;i<(mapIter->second).ptrArray.size();i++)
        {
            if((mapIter->second).ptrArray[i]!="NULL")
            {
                cout<<"\""<<(mapIter->first)<<"\""<<" -> "<<"\""<<(mapIter->second).ptrArray[i]<<"\""<<endl;
            }
            //cout<<(mapIter->second).ptrArray[i]<<endl;
        }
    }

    cout<<"}"<<endl;
    of.flush();
    of.close();
    cout.rdbuf(coutBuf);
}

bool expressionSeparate(string strx,vector<string>& vecx)
{//把算术表达式分割为标识符数字运算符
     //strx="cnt+1*a1/a2;";
    boost::smatch matRes;
    //boost::regex pattern("[0-9]+");
    boost::regex pattern("([0-9]+(?:.[0-9]+)?|[a-zA-Z_]+[0-9a-zA-Z_]*)");
    if(boost::regex_search(strx,matRes,pattern))
    {
        for(int i=0;i<matRes.size();i++)
        {
            if(matRes[i]!="")
            {
                //cout<<"matRes[i]"<<matRes[i]<<endl;
            }
        }
        string get=matRes[0];
        //cout<<"geteeegetee "<<get<<endl;
        int pos=strx.find(get);
        for(int i=0;i<pos;i++)
        {
            char c=strx[i];
            
            if(notOpera(c)==0)
            {
                //cout<<"***"<<c<<endl;
                string getCharacter;
                getCharacter=strx[i];
                vecx.push_back(getCharacter);
            }
        }
        vecx.push_back(get);
        string nextStrx=string(strx,pos+get.size());
        //cout<<nextStrx<<endl;
        expressionSeparate(nextStrx,vecx);
        //cout<<nextStrx<<endl;
       // cout<<pos<<endl;
        return 1;
    }
    else return 0;
}

int judgeMatchExpression(string strx,vector<string>& vecx)//判断语句类型
{
    if(matchAssignmentExpression(strx,vecx)||matchAssignmentExpressionForArray(strx,vecx))
        return 1;
    else if(matchIfExpression(strx,vecx))
        return 2;
    else if(matchWhileExpression(strx,vecx))
        return 3;
    else if(matchBegin(strx))
        return 4;
    else if(matchEnd(strx))
        return 5;
    else
        return 0;
}
string handleAssignmentExpression(vector<string> getInfo,int& iter)//处理赋值语句
{
    string leftValueNode_str;
    //cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<getInfo[0]<<endl;
    if(getInfo[0]=="ARRAY_AE!")
    {
    	leftValueNode_str=getInfo[1]+"-"+int2str(iter);
    	//cout<<"~!#~#~$~$~@#~$@@$~"<<endl;
    }
    else
    {
    	leftValueNode_str=getInfo[0]+"-"+int2str(iter);
    }
    AST assignmentExpressionNode("assign-"+int2str(iter));
    AST leftValueNode(leftValueNode_str);
    AST equalSignNode(":=-"+int2str(iter));
    // AST assignmentExpressionNode("assign-"+int2str(iter));
    // AST leftValueNode(getInfo[0]+"-"+int2str(iter));
    // AST equalSignNode(":=-"+int2str(iter));
    vector<string> rightExpression_inOrder;
    vector<string> rightExpression_postOrder;
    //##ARRAY_AE!##acc[1][2]##acc##vcc+bss##1##2

    if(getInfo[0]=="ARRAY_AE!")
    	expressionSeparate(getInfo[3],rightExpression_inOrder);
    else 
    	expressionSeparate(getInfo[1],rightExpression_inOrder);
    //cout<<"ok"<<endl;
    //printArray(rightExpression_inOrder," ~~~ ");
    for(int i=0;i<rightExpression_inOrder.size();i++)
    {
    	if(isVarName(rightExpression_inOrder[i]))
    	{
    		auto p=symbolTable.find(rightExpression_inOrder[i]);
    		if(p == symbolTable.end())
    		{
    			cout<<"error:can not find var defined in symbolTable at line "<<iter+1<<endl;
    			return "ERROR_IN_VAR_FIND";
    		}
    	}
    }
    auto p=symbolTable.find("acc");
    inorder2postorder(rightExpression_inOrder,rightExpression_postOrder);
    //cout<<"!!!ok"<<endl;
    //@ARRAY_AE@arrray@dc36@11@b1@c2
    string getAssignTmp=generateQuaternion(rightExpression_postOrder,globalQuaternion);
    //printArray(getInfo,"##");
    if(getInfo[0]=="ARRAY_AE!")
    {
    	string getQuaternionArr="(=,"+getInfo[4]+",--,T)";
    	globalQuaternion.push_back(getQuaternionArr);
    	for(int k=5;k<getInfo.size();k++)
    	{
    		getQuaternionArr="(*,T,"+int2str(k-2)+",T)";
    		globalQuaternion.push_back(getQuaternionArr);
    		getQuaternionArr="(+,"+getInfo[k]+",T,T)";
    		globalQuaternion.push_back(getQuaternionArr);

    	}
    	string assignQuaternion="([]=,"+getAssignTmp+",--,"+getInfo[2]+"[T])";
    	globalQuaternion.push_back(assignQuaternion);
    	 string ASTname=buildASTtree(rightExpression_inOrder,rightExpression_postOrder,"AE-"+int2str(iter)+"-");
    	//buildTree(vec,postorder,0, vec.size() - 1, postorder.size() - 1, 0,"E");
    	assignmentExpressionNode.append(equalSignNode.data);
    	assignmentExpressionNode.append(leftValueNode.data);
    	assignmentExpressionNode.append(ASTname);
    	//compoundStatementNode.append(assignmentExpressionNode.data);
    	globalMap.insert(pair<string,AST>(assignmentExpressionNode.data,assignmentExpressionNode));
    	globalMap.insert(pair<string,AST>(leftValueNode.data,leftValueNode));
   	 	globalMap.insert(pair<string,AST>(equalSignNode.data,equalSignNode));
   		return assignmentExpressionNode.data;
    }
    string assignQuaternion="(=,"+getAssignTmp+",--,"+getInfo[0]+")";
    globalQuaternion.push_back(assignQuaternion);
    // for(int i=0;i<rightExpression_postOrder.size();i++)
    //     cout<<"###rightExpression_postOrder "<<rightExpression_postOrder[i]<<endl;
     //for(int i=0;i<rightExpression_inOrder.size();i++)
         //cout<<rightExpression_inOrder[i]<<endl;
                    //for(int i=0;i<rightExpression_postOrder.size();i++)
                        //cout<<rightExpression_postOrder[i]<<endl;
    string ASTname=buildASTtree(rightExpression_inOrder,rightExpression_postOrder,"AE-"+int2str(iter)+"-");
    //buildTree(vec,postorder,0, vec.size() - 1, postorder.size() - 1, 0,"E");
    assignmentExpressionNode.append(equalSignNode.data);
    assignmentExpressionNode.append(leftValueNode.data);
    assignmentExpressionNode.append(ASTname);
    //compoundStatementNode.append(assignmentExpressionNode.data);
    globalMap.insert(pair<string,AST>(assignmentExpressionNode.data,assignmentExpressionNode));
    globalMap.insert(pair<string,AST>(leftValueNode.data,leftValueNode));
    globalMap.insert(pair<string,AST>(equalSignNode.data,equalSignNode));
    return assignmentExpressionNode.data;

}
string handleIfExpression(vector<string>& vecx,vector<string> getInfox,int begin,int end,int elseBegin,int elseEnd)//处理if语句
{//begin+1到end是复合语句
    //cout<<"handleing "<<vecx[begin]<<endl;
   // cout<<"~~~~~~~~~~~~~~~~~~~~elseBegin= "<<elseBegin<<" elseEnd= "<<elseEnd<<endl;
    //printArray(getInfox,"{{{{{{{{{{{{");
    map<string,string> jmpTable1;
    map<string,string> jmpTable0;
    AST ifExpressionNode("if-"+int2str(begin));//生成if条件语法树和jmp表
    vector<string> boolExpression_postOrder;
     //for(int i=0;i<getInfox.size();i++)
        //cout<<getInfox[i]<<endl;
    inorder2postorder(getInfox,boolExpression_postOrder);
     //for(int i=0;i<boolExpression_postOrder.size();i++)
         //cout<<boolExpression_postOrder[i]<<endl;
    string firstBEX=getInfox[0];
    map<string,string> name2index;
    string ASTname=buildASTtreeForQuaternion(getInfox,boolExpression_postOrder,"BE-"+int2str(begin)+"-",firstBEX,jmpTable0,jmpTable1,name2index);
    map<string,string>::iterator nameiter;
    // for(nameiter=name2index.begin();nameiter!=name2index.end();nameiter++)
    // {
    //     cout<<"&&&\""<<(nameiter->first)<<"\""<<" -> "<<"\""<<(nameiter->second)<<"\""<<endl;
    // }
    ifExpressionNode.append(ASTname);
    father.insert(pair<string,string>(ASTname,"boolEXroot"));
    //value.insert(pair<string,int>(ASTname,-1));
    jmpTable1.insert(pair<string,string>(ASTname,"-1"));
    jmpTable0.insert(pair<string,string>(ASTname,"-1"));
    //compoundStatementNode.append(ifExpressionNode.data);
    
    //cout<<getInfox[0]<<endl;

    //cout<<"DFS ing"<<endl;
    DFS(value,firstBEX,ASTname,jmpTable0,jmpTable1);//短路求值，生成跳转表
    //cout<<"DFS"<<endl;
    //value.clear();
    // cout<<"####jmap1:"<<endl;
    // map<string,string>::iterator jmap1;
    // for(jmap1=jmpTable1.begin();jmap1!=jmpTable1.end();jmap1++)
    // {
    //     cout<<"\""<<(jmap1->first)<<"\""<<" -> "<<"\""<<(jmap1->second)<<"\""<<endl;
    // }
    // cout<<"####jmap0:"<<endl;
    // map<string,string>::iterator jmap0;
    // for(jmap0=jmpTable0.begin();jmap0!=jmpTable0.end();jmap0++)
    // {
    //     cout<<"\""<<(jmap0->first)<<"\""<<" -> "<<"\""<<(jmap0->second)<<"\""<<endl;
    // }
    //printArray(getInfox,"-------------");
    vector<int> waitFalseOutIndex=generateQuaternionForBEX(getInfox,globalQuaternion,name2index,jmpTable0,jmpTable1);//false出口
    //ifAction翻译完毕后回填    
    map<string,int>::iterator mapIter;
    for(mapIter=value.begin();mapIter!=value.end();mapIter++)
    {
        mapIter->second=4;
    }
    int iter=begin+1;
    AST ifActionNode("ifAction-"+int2str(iter)+"to"+int2str(end));
    while(iter<end)
    {
        vector<string> getInfo;
        int getType=judgeMatchExpression(vecx[iter],getInfo);
        cout<<vecx[iter]<<" has type "<<getType<<" at line"<<iter+1<<endl;
        switch(getType)
        {
            case 1://赋值语句
            {
                string resData=handleAssignmentExpression(getInfo,iter);
                ifActionNode.append(resData);
                cout<<resData<<" is ok"<<endl;
                break;
            }
            case 2://if语句
            {
                    int getIter=iter;
                    getIter++;
                    int stackCnt=0;
                    if(matchBegin(vecx[getIter]))
                    {
                        stackCnt++;
                        getIter++;
                        while(getIter<end && stackCnt!=0)
                        {
                            if(matchBegin(vecx[getIter]))stackCnt++;
                            else if(matchEnd(vecx[getIter]))stackCnt--;
                            getIter++;
                        }
                        if(stackCnt!=0)
                        {
                            cout<<"error match end of if expression at line "<<getIter+1<<endl;
                            return "NULL";
                        }
                        int ifEnd=getIter;
                        int elseBegin=-1;
                        int elseEnd=-1;
                        int elseStackCnt=0;
                        if(getIter<end)
                        {
                            //cout<<"~~~~~~~~~~~~$$$$$$$$$$is else? "<<vecx[getIter]<<" getiter= "<<getIter<<endl;
                            if(matchElse(vecx[getIter]))
                            {
                                getIter++;
                                if(getIter<end && matchBegin(vecx[getIter]))
                                {
                                    elseStackCnt++;
                                    getIter++;
                                    elseBegin=getIter;//else的第一条语句
                                    while(getIter<end && elseStackCnt!=0)
                                    {
                                        if(matchBegin(vecx[getIter]))elseStackCnt++;    
                                        else if(matchEnd(vecx[getIter]))elseStackCnt--;
                                        getIter++;

                                    }
                                    
                                    if(elseStackCnt!=0)
                                    {
                                            cout<<"error1 match end of else expression at line "<<getIter+1<<endl;
                                            return 0;
                                    }
                                    elseEnd=getIter;//end语句
                                }
                                else
                                {
                                        cout<<"error match end of else expressio at line "<<getIter+1<<endl;
                                }
                            }
                        }
                        
                        string resData=handleIfExpression(vecx,getInfo,iter,ifEnd,elseBegin,elseEnd);
                        iter=getIter-1;
                        //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                        ifActionNode.append(resData);
                    }
                    else//不是复合语句
                    {
                        int elseBegin=-1;
                        int elseEnd=-1;
                        int elseStackCnt=0;
                        if(getIter<end)
                        {
                            //cout<<"~~~~~~~~~~~~$$$$$$$$$$is else? "<<vecx[getIter]<<" getiter= "<<getIter<<endl;
                            if(matchElse(vecx[getIter]))
                            {
                                getIter++;
                                if(getIter<end && matchBegin(vecx[getIter]))
                                {
                                    elseStackCnt++;
                                    getIter++;
                                    elseBegin=getIter;//else的第一条语句
                                    while(getIter<end && elseStackCnt!=0)
                                    {
                                        if(matchBegin(vecx[getIter]))elseStackCnt++;    
                                        else if(matchEnd(vecx[getIter]))elseStackCnt--;
                                        getIter++;

                                    }
                                    
                                    if(elseStackCnt!=0)
                                    {
                                            cout<<"error1 match end of else expression at line "<<getIter+1<<endl;
                                            return 0;
                                    }
                                    elseEnd=getIter;//end语句
                                }
                                else
                                {
                                        cout<<"error match end of else expression at line "<<getIter+1<<endl;
                                }
                            }
                        }
                        
                        string resData=handleIfExpression(vecx,getInfo,iter,iter+1,elseBegin,elseEnd);
                        iter=getIter-1;
                        //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                        ifActionNode.append(resData);
                    }
                    break;
                //handleIfExpression(getInfo,iter);
                
            }
            case 3://while
            {
                int getIter=iter;
                getIter++;
                int stackCnt=0;
                if(matchBegin(vecx[getIter]))
                {
                    stackCnt++;
                    getIter++;
                    while(getIter<end && stackCnt!=0)
                    {
                        if(matchBegin(vecx[getIter]))stackCnt++;
                        else if(matchEnd(vecx[getIter]))stackCnt--;
                        getIter++;
                    }
                    if(stackCnt!=0)
                    {
                        cout<<"error match end of while expression at line "<<getIter+1<<endl;
                        return "NULL";
                    }
                    string resData=handleWhileExpression(vecx,getInfo,iter,getIter);
                    iter=getIter-1;
                    //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                    ifActionNode.append(resData);
                }
                else//不是复合语句
                {
                    string resData=handleWhileExpression(vecx,getInfo,iter,getIter+1);
                    iter=getIter;
                    //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                    ifActionNode.append(resData);
                }
                break;
            }
            case 4://begin
            {
                //cout<<"get "<<vecx[iter]<<endl;
                break;
            }
            case 5://end
            {
                break;
            }
            default :
            {
                cout<<"error match expression type at line "<<iter+1<<endl;
                return "NULL";
            }
            
        }
        iter++;
        
    }//ifAction翻译完毕
    string getQuaternion="(j,--,--,AllOut)";//全出口，在elseAction翻译完毕后回填
    globalQuaternion.push_back(getQuaternion);
    getQuaternion="(j,--,--,"+int2str(globalQuaternion.size())+")";
    for(int i=0;i<waitFalseOutIndex.size();i++)
    	globalQuaternion[ waitFalseOutIndex[i] ]=getQuaternion;//回填false出口
    int waitAllOutIndex=globalQuaternion.size()-1;
    iter=elseBegin;
    AST elseActionNode("elseAction-"+int2str(elseBegin)+"to"+int2str(elseEnd));
    while(iter<elseEnd)
    {
        vector<string> getInfo;
        int getType=judgeMatchExpression(vecx[iter],getInfo);
        cout<<vecx[iter]<<" has type "<<getType<<" at line"<<iter+1<<endl;
        switch(getType)
        {
            case 1://赋值语句
            {
                string resData=handleAssignmentExpression(getInfo,iter);
                elseActionNode.append(resData);
                cout<<resData<<" is ok"<<endl;
                break;
            }
            case 2://if语句
            {
                    int getIter=iter;
                    getIter++;
                    int stackCnt=0;
                    if(matchBegin(vecx[getIter]))
                    {
                        stackCnt++;
                        getIter++;
                        while(getIter<end && stackCnt!=0)
                        {
                            if(matchBegin(vecx[getIter]))stackCnt++;
                            else if(matchEnd(vecx[getIter]))stackCnt--;
                            getIter++;
                        }
                        if(stackCnt!=0)
                        {
                            cout<<"error match end of if expression at line "<<getIter+1<<endl;
                            return "NULL";
                        }
                        int ifEnd=getIter;
                        int elseBegin=-1;
                        int elseEnd=-1;
                        int elseStackCnt=0;
                        if(getIter<end)
                        {
                           // cout<<"~~~~~~~~~~~~$$$$$$$$$$is else? "<<vecx[getIter]<<" getiter= "<<getIter<<endl;
                            if(matchElse(vecx[getIter]))
                            {
                                getIter++;
                                if(getIter<end && matchBegin(vecx[getIter]))
                                {
                                    elseStackCnt++;
                                    getIter++;
                                    elseBegin=getIter;//else的第一条语句
                                    while(getIter<end && elseStackCnt!=0)
                                    {
                                        if(matchBegin(vecx[getIter]))elseStackCnt++;    
                                        else if(matchEnd(vecx[getIter]))elseStackCnt--;
                                        getIter++;

                                    }
                                    
                                    if(elseStackCnt!=0)
                                    {
                                            cout<<"error1 match end of else expression at line "<<getIter+1<<endl;
                                            return 0;
                                    }
                                    elseEnd=getIter;//end语句
                                }
                                else
                                {
                                        cout<<"error match end of else expressio at line "<<getIter+1<<endl;
                                }
                            }
                        }
                        string resData=handleIfExpression(vecx,getInfo,iter,ifEnd,elseBegin,elseEnd);
                        iter=getIter-1;
                        //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                        elseActionNode.append(resData);
                    }
                    else//不是复合语句
                    {
                        int elseBegin=-1;
                        int elseEnd=-1;
                        int elseStackCnt=0;
                        if(getIter<end)
                        {
                            //cout<<"~~~~~~~~~~~~$$$$$$$$$$is else? "<<vecx[getIter]<<" getiter= "<<getIter<<endl;
                            if(matchElse(vecx[getIter]))
                            {
                                getIter++;
                                if(getIter<end && matchBegin(vecx[getIter]))
                                {
                                    elseStackCnt++;
                                    getIter++;
                                    elseBegin=getIter;//else的第一条语句
                                    while(getIter<end && elseStackCnt!=0)
                                    {
                                        if(matchBegin(vecx[getIter]))elseStackCnt++;    
                                        else if(matchEnd(vecx[getIter]))elseStackCnt--;
                                        getIter++;

                                    }
                                    
                                    if(elseStackCnt!=0)
                                    {
                                            cout<<"error1 match end of else expression at line "<<getIter+1<<endl;
                                            return 0;
                                    }
                                    elseEnd=getIter;//end语句
                                }
                                else
                                {
                                        cout<<"error match end of else expression at line "<<getIter+1<<endl;
                                }
                            }
                        }
                    }
                    break;
                //handleIfExpression(getInfo,iter);
                
            }
            case 3://while
            {
                int getIter=iter;
                getIter++;
                int stackCnt=0;
                if(matchBegin(vecx[getIter]))
                {
                    stackCnt++;
                    getIter++;
                    while(getIter<end && stackCnt!=0)
                    {
                        if(matchBegin(vecx[getIter]))stackCnt++;
                        else if(matchEnd(vecx[getIter]))stackCnt--;
                        getIter++;
                    }
                    if(stackCnt!=0)
                    {
                        cout<<"error match end of while expression at line "<<getIter+1<<endl;
                        return "NULL";
                    }
                    string resData=handleWhileExpression(vecx,getInfo,iter,getIter);
                    iter=getIter-1;
                    //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                    elseActionNode.append(resData);
                }
                else//不是复合语句
                {
                    string resData=handleWhileExpression(vecx,getInfo,iter,getIter+1);
                    iter=getIter;
                    //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                    ifActionNode.append(resData);
                }
                break;
            }
            case 4://begin
            {
                //cout<<"get "<<vecx[iter]<<endl;
                break;
            }
            case 5://end
            {
                break;
            }
            default :
            {
                cout<<"error match expression type at line "<<iter+1<<endl;
                return "NULL";
            }
            
        }
        iter++;
    }
    string getAllOutQua="(j,--,--,"+int2str(globalQuaternion.size())+")";
    globalQuaternion[waitAllOutIndex]=getAllOutQua;//回填全出口
    globalMap.insert(pair<string,AST>(ifActionNode.data,ifActionNode));
    globalMap.insert(pair<string,AST>(elseActionNode.data,elseActionNode));

    ifExpressionNode.append(ifActionNode.data);
    ifExpressionNode.append(elseActionNode.data);
    globalMap.insert(pair<string,AST>(ifExpressionNode.data,ifExpressionNode));

    return ifExpressionNode.data;

}
string handleWhileExpression(vector<string>& vecx,vector<string> getInfox,int begin,int end)//处理while语句
{
    //cout<<"handleing "<<vecx[begin]<<endl;
    map<string,string> jmpTable1;
    map<string,string> jmpTable0;
    AST whileExpressionNode("while-"+int2str(begin));//生成if条件语法树和jmp表
    vector<string> boolExpression_postOrder;
    inorder2postorder(getInfox,boolExpression_postOrder);

    string firstBEX=getInfox[0];
    map<string,string> name2index;
    string ASTname=buildASTtreeForQuaternion(getInfox,boolExpression_postOrder,"BE-"+int2str(begin)+"-",firstBEX,jmpTable0,jmpTable1,name2index);
    map<string,string>::iterator nameiter;
    // for(nameiter=name2index.begin();nameiter!=name2index.end();nameiter++)
    // {
    //     cout<<"&&&\""<<(nameiter->first)<<"\""<<" -> "<<"\""<<(nameiter->second)<<"\""<<endl;
    // }
    whileExpressionNode.append(ASTname);
    father.insert(pair<string,string>(ASTname,"boolEXroot"));
    //value.insert(pair<string,int>(ASTname,-1));
    jmpTable1.insert(pair<string,string>(ASTname,"-1"));
    jmpTable0.insert(pair<string,string>(ASTname,"-1"));
    
    //cout<<getInfox[0]<<endl;
    //cout<<"DFS ing"<<endl;
    DFS(value,firstBEX,ASTname,jmpTable0,jmpTable1);//短路求值，生成跳转表
    //value.clear();
    //cout<<"DFS"<<endl;
    // cout<<"####jmap1:"<<endl;
    // map<string,string>::iterator jmap1;
    // for(jmap1=jmpTable1.begin();jmap1!=jmpTable1.end();jmap1++)
    // {
    //     cout<<"\""<<(jmap1->first)<<"\""<<" -> "<<"\""<<(jmap1->second)<<"\""<<endl;
    // }
    // cout<<"####jmap0:"<<endl;
    // map<string,string>::iterator jmap0;
    // for(jmap0=jmpTable0.begin();jmap0!=jmpTable0.end();jmap0++)
    // {
    //     cout<<"\""<<(jmap0->first)<<"\""<<" -> "<<"\""<<(jmap0->second)<<"\""<<endl;
    // }
    int endBackIndex=globalQuaternion.size();//while循环回跳的地址
    vector<int> waitFalseOutIndex=generateQuaternionForBEX(getInfox,globalQuaternion,name2index,jmpTable0,jmpTable1);//false出口
    map<string,int>::iterator mapIter;
    for(mapIter=value.begin();mapIter!=value.end();mapIter++)
    {
        mapIter->second=4;
    }
    int iter=begin+1;
    AST whileActionNode("whileAction-"+int2str(iter)+"to"+int2str(end));//生成节点
    //cout<<"*&*&*&*&*&*"<<vecx[iter]<<endl;
    while(iter<end)
    {
        vector<string> getInfo;
        int getType=judgeMatchExpression(vecx[iter],getInfo);
        cout<<vecx[iter]<<" has type "<<getType<<" at line"<<iter+1<<endl;
        switch(getType)
        {
            case 1://赋值语句
            {
                string resData=handleAssignmentExpression(getInfo,iter);
                whileActionNode.append(resData);
                //cout<<resData<<" **************is ok"<<endl;
                //for(int k=0;k<getInfo.size();k++)
                   // cout<<"ASS "<<getInfo[k]<<endl;
                break;
            }
            case 2://if语句
            {
                int getIter=iter;
                getIter++;
                int stackCnt=0;
                if(matchBegin(vecx[getIter]))
                {
                    stackCnt++;
                    getIter++;
                    while(getIter<end && stackCnt!=0)
                    {
                        if(matchBegin(vecx[getIter]))stackCnt++;
                        else if(matchEnd(vecx[getIter]))stackCnt--;
                        getIter++;
                    }
                    if(stackCnt!=0)
                    {
                        cout<<"error match end of if expression at line "<<getIter+1<<endl;
                        return "NULL";
                    }
                    int ifEnd=getIter;
                    int elseBegin=-1;
                    int elseEnd=-1;
                    int elseStackCnt=0;
                    if(getIter<end)
                    {
                        //cout<<"~~~~~~~~~~~~$$$$$$$$$$is else? "<<vecx[getIter]<<" getiter= "<<getIter<<endl;
                        if(matchElse(vecx[getIter]))
                        {
                            getIter++;
                            if(getIter<end && matchBegin(vecx[getIter]))//寻找所有复合语句
                            {
                                elseStackCnt++;
                                getIter++;
                                elseBegin=getIter;//else的第一条语句
                                while(getIter<end && elseStackCnt!=0)
                                {
                                    if(matchBegin(vecx[getIter]))elseStackCnt++;    
                                    else if(matchEnd(vecx[getIter]))elseStackCnt--;
                                    getIter++;

                                }
                                
                                if(elseStackCnt!=0)
                                {
                                        cout<<"error1 match end of else expression at line "<<getIter+1<<endl;
                                        return 0;
                                }
                                elseEnd=getIter;//end语句
                            }
                            else
                            {
                                    cout<<"error match end of else expressio at line "<<getIter+1<<endl;
                            }
                        }
                    }
                    string resData=handleIfExpression(vecx,getInfo,iter,ifEnd,elseBegin,elseEnd);
                    iter=getIter-1;
                    //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                    whileActionNode.append(resData);
                }
                else//不是复合语句
                {
                    int elseBegin=-1;
                        int elseEnd=-1;
                        int elseStackCnt=0;
                        if(getIter<end)
                        {
                            //cout<<"~~~~~~~~~~~~$$$$$$$$$$is else? "<<vecx[getIter]<<" getiter= "<<getIter<<endl;
                            if(matchElse(vecx[getIter]))
                            {
                                getIter++;
                                if(getIter<end && matchBegin(vecx[getIter]))
                                {
                                    elseStackCnt++;
                                    getIter++;
                                    elseBegin=getIter;//else的第一条语句
                                    while(getIter<end && elseStackCnt!=0)
                                    {
                                        if(matchBegin(vecx[getIter]))elseStackCnt++;    
                                        else if(matchEnd(vecx[getIter]))elseStackCnt--;
                                        getIter++;

                                    }
                                    
                                    if(elseStackCnt!=0)
                                    {
                                            cout<<"error1 match end of else expression at line "<<getIter+1<<endl;
                                            return 0;
                                    }
                                    elseEnd=getIter;//end语句
                                }
                                else
                                {
                                        cout<<"error match end of else expression at line "<<getIter+1<<endl;
                                }
                            }
                        }
                }
                break;
                //handleIfExpression(getInfo,iter);
                
            }
            case 3://while
            {
                int getIter=iter;
                getIter++;
                int stackCnt=0;
                if(matchBegin(vecx[getIter]))//寻找所有复合语句
                {
                    stackCnt++;
                    getIter++;
                    while(getIter<end && stackCnt!=0)
                    {
                        if(matchBegin(vecx[getIter]))stackCnt++;
                        else if(matchEnd(vecx[getIter]))stackCnt--;
                        getIter++;
                    }
                    if(stackCnt!=0)
                    {
                        cout<<"error match end of while expression at line "<<getIter+1<<endl;
                        return "NULL";
                    }
                    string resData=handleWhileExpression(vecx,getInfo,iter,getIter);
                    iter=getIter-1;
                    //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                    whileActionNode.append(resData);
                }
                else//不是复合语句
                {
                    string resData=handleWhileExpression(vecx,getInfo,iter,iter+1);
                    iter=getIter;
                    //cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                    whileActionNode.append(resData);
                }
                break;
            }
            case 4://begin
            {
                //cout<<"get "<<vecx[iter]<<endl;
                break;
            }
            case 5://end
            {
                break;
            }
            default :
            {
                cout<<"error match expression type at line "<<iter+1<<endl;
                return "NULL";
            }
            
        }
        iter++;
    }
    //while循环体翻译完毕，执行回跳

    string endBackQuaternion="(jwb,--,--,"+int2str(endBackIndex)+")";
    globalQuaternion.push_back(endBackQuaternion);//回跳到条件判断语句
    string getQuaternion="(jss,--,--,"+int2str(globalQuaternion.size())+")";
    for(int i=0;i<waitFalseOutIndex.size();i++)
    	globalQuaternion[ waitFalseOutIndex[i] ]=getQuaternion;//回填false出口
    globalMap.insert(pair<string,AST>(whileActionNode.data,whileActionNode));

    whileExpressionNode.append(whileActionNode.data);
    globalMap.insert(pair<string,AST>(whileExpressionNode.data,whileExpressionNode));//生成最终节点

    return whileExpressionNode.data;
}

// String getSiYuanShi(String string)
// {
//     String SiYuanShiString="";
//     String[] string_array=string.split(" ");
//     Stack<String> stack=new Stack<>();
//     int result_num=0;
//     String result="T";
//     for(int i=0;i<string_array.length;i++){      //开始扫描字符串数组
//         if(string_array[i].equals("+")||string_array[i].equals("-")||string_array[i].equals("*")||string_array[i].equals("/")){
//             result_num++;
//             result+=result_num;
//             SiYuanShiString+="("+string_array[i]+","+stack.pop()+","+stack.pop()+","+result+")\n";
//             stack.push(result);
//             result="T";
//         }else{
//             stack.push(string_array[i]);
//         }
//     }

//    return SiYuanShiString;
// }


int compiler(vector<string> vecx)//编译器，假设已经去除所有空行
{
    int iter=0;
    string programName;
    if(!matchProgram(vecx[iter],programName))
    {
        cout<<"error match program"<<endl;
        return -1;
    }
    AST rootAST("root");
    
    string strPname="program"+int2str(iter);
    AST programNode(strPname);
    AST programNameNode(programName);
    globalMap.insert(pair<string,AST>(programName,programNameNode));
    programNode.append(programName);
    rootAST.append(strPname);
    
    globalMap.insert(pair<string,AST>(strPname,programNode));
    iter++;
    vector<string> varVec;

    while(iter<vecx.size() && (matchVarExpression(vecx[iter],varVec)||matchVarExpressionForArray(vecx[iter],varVec)))//逐行读取
    {
        //cout<<vecx[iter]<<endl;
        AST varExpressionNode("var"+int2str(iter));
        rootAST.append(varExpressionNode.data);
        vector<string> chache;
        for(int k=0;k<varVec.size();k++)
        {
            string str=varVec[k]+int2str(iter);
            AST varNode(str);
            varExpressionNode.append(str);
            //cout<<varVec[k]<<" ~~~~~~~01010 "<<endl;
            globalMap.insert(pair<string,AST>(str,varNode));
            
            if(ispascalType(varVec[k]))
            {
            	for(int i=0;i<chache.size();i++)
					symbolTable.insert(pair<string,string>(chache[i],varVec[k]));//填写符号表
				chache.clear();
            }
            else
            {
            	chache.push_back(varVec[k]);	
            }

        }
        //cout<<"ok"<<endl;
        globalMap.insert(pair<string,AST>("var"+int2str(iter),varExpressionNode));
        iter++;
        varVec.clear();
        
    }
    
    
    if(iter>=vecx.size())
    {
        cout<<"error:expect begin at line "<<iter+1<<endl;
        return 0;
    }
    if(matchBegin(vecx[iter])==0)
    {
        cout<<"error match begin expression at line "<<iter+1<<endl;
        return 0;
    }
    else
    {
        AST compoundStatementNode("CompoundStatement");
        while(iter<vecx.size())
        {
            vector<string> getInfo;
            int getType=judgeMatchExpression(vecx[iter],getInfo);
            cout<<vecx[iter]<<" has type "<<getType<<" line= "<<iter+1<<endl;
            //printArray(getInfo," ~~~ ");
            switch(getType)
            {
                case 1://赋值语句
                {//
                    string resData=handleAssignmentExpression(getInfo,iter);
                    compoundStatementNode.append(resData);
                    // AST assignmentExpressionNode("assign-"+int2str(iter));
                    // AST leftValueNode(getInfo[0]+"-"+int2str(iter));
                    // AST equalSignNode(":=-"+int2str(iter));
                    // vector<string> rightExpression_inOrder;
                    // vector<string> rightExpression_postOrder;

                    // expressionSeparate(getInfo[1],rightExpression_inOrder);
                    // inorder2postorder(rightExpression_inOrder,rightExpression_postOrder);
                    // //for(int i=0;i<rightExpression_inOrder.size();i++)
                    //     //cout<<rightExpression_inOrder[i]<<endl;
                    // //for(int i=0;i<rightExpression_postOrder.size();i++)
                    //     //cout<<rightExpression_postOrder[i]<<endl;
                    // string ASTname=buildASTtree(rightExpression_inOrder,rightExpression_postOrder,"AE-"+int2str(iter)+"-");
                    // //buildTree(vec,postorder,0, vec.size() - 1, postorder.size() - 1, 0,"E");
                    // assignmentExpressionNode.append(equalSignNode.data);
                    // assignmentExpressionNode.append(leftValueNode.data);
                    // assignmentExpressionNode.append(ASTname);
                    // compoundStatementNode.append(assignmentExpressionNode.data);
                    // globalMap.insert(pair<string,AST>(assignmentExpressionNode.data,assignmentExpressionNode));
                    // globalMap.insert(pair<string,AST>(leftValueNode.data,leftValueNode));
                    // globalMap.insert(pair<string,AST>(equalSignNode.data,equalSignNode));
                    break;
                }
                case 2://if,getInfo是布尔表达式和逻辑运算符
                {
                    //break;
                    int getIter=iter;
                    getIter++;
                    int stackCnt=0;
                    if(matchBegin(vecx[getIter]))
                    {
                        stackCnt++;
                        getIter++;
                        while(getIter<vecx.size() && stackCnt!=0)
                        {
                            if(matchBegin(vecx[getIter]))stackCnt++;
                            else if(matchEnd(vecx[getIter]))stackCnt--;
                            getIter++;
                        }
                        if(stackCnt!=0)
                        {
                            cout<<"error match end of if expression at line "<<getIter+1<<endl;
                            return 0;
                        }
                        int ifEnd=getIter;
                        int elseBegin=-1;
                        int elseEnd=-1;
                        int elseStackCnt=0;
                        if(getIter<vecx.size())
                        {
                            //cout<<"~~~~~~~~~~~~$$$$$$$$$$is else? "<<vecx[getIter]<<" getiter= "<<getIter<<endl;
                            if(matchElse(vecx[getIter]))
                            {
                                
                                getIter++;
                                if(getIter<vecx.size() && matchBegin(vecx[getIter]))
                                {
                                    elseStackCnt++;
                                    getIter++;
                                    elseBegin=getIter;//else的第一条语句
                                    while(getIter<vecx.size() && elseStackCnt!=0)
                                    {
                                        if(matchBegin(vecx[getIter]))elseStackCnt++;    
                                        else if(matchEnd(vecx[getIter]))elseStackCnt--;

                                        getIter++;

                                    }
                                    
                                    if(elseStackCnt!=0)
                                    {
                                        cout<<"error1 match end of else expression at line "<<getIter+1<<endl;
                                        return 0;
                                    }
                                    elseEnd=getIter;//end语句
                                }
                                else
                                {
                                    cout<<"error match end of else expressio at line "<<getIter+1<<endl;
                                }
                            }
                        }

                        string resData=handleIfExpression(vecx,getInfo,iter,ifEnd,elseBegin,elseEnd);
                        cout<<"end if resData= "<<resData<<endl;
                        iter=getIter-1;//fix bug compoundStatementNode含有if层级的ifnode子节点
                        cout<<"####vecx[getIter] "<<vecx[getIter]<<endl;
                        compoundStatementNode.append(resData);
                    }
                    else//不是复合语句
                    {
                    	int elseBegin=-1;
                        int elseEnd=-1;
                        int elseStackCnt=0;
                        if(getIter<vecx.size())
                        {
                            //cout<<"~~~~~~~~~~~~$$$$$$$$$$is else? "<<vecx[getIter]<<" getiter= "<<getIter<<endl;
                            if(matchElse(vecx[getIter]))
                            {
                                getIter++;
                                if(getIter<vecx.size() && matchBegin(vecx[getIter]))
                                {
                                    elseStackCnt++;
                                    getIter++;
                                    elseBegin=getIter;//else的第一条语句
                                    while(getIter<vecx.size() && elseStackCnt!=0)
                                    {
                                        if(matchBegin(vecx[getIter]))elseStackCnt++;    
                                        else if(matchEnd(vecx[getIter]))elseStackCnt--;
                                        getIter++;

                                    }
                                    
                                    if(elseStackCnt!=0)
                                    {
                                            cout<<"error1 match end of else expression at line "<<getIter+1<<endl;
                                            return 0;
                                    }
                                    elseEnd=getIter;//end语句
                                }
                                else
                                {
                                        cout<<"error match end of else expression at line "<<getIter+1<<endl;
                                }
                            }
                        }
                    	//
                    }
                    
                   

                   //  //cout<<"706 IF mod"<<endl;
                   //  AST ifExpressionNode("if-"+int2str(iter));
                   //  vector<string> boolExpression_postOrder;
                   //   //for(int i=0;i<getInfo.size();i++)
                   //      //cout<<getInfo[i]<<endl;
                   //  inorder2postorder(getInfo,boolExpression_postOrder);
                   // // cout<<"710"<<endl;
                   
                   //  //for(int i=0;i<boolExpression_postOrder.size();i++)
                   //      //cout<<boolExpression_postOrder[i]<<endl;
                   //  string firstBEX=getInfo[0];
                   //  string ASTname=buildASTtreeForQuaternion(getInfo,boolExpression_postOrder,"BE-"+int2str(iter)+"-",firstBEX);
                   //  ifExpressionNode.append(ASTname);
                   //  father.insert(pair<string,string>(ASTname,"boolEXroot"));
                   //  //value.insert(pair<string,int>(ASTname,-1));
                   //  jmpTable1.insert(pair<string,string>(ASTname,"-1"));
                   //  jmpTable0.insert(pair<string,string>(ASTname,"-1"));
                   //  compoundStatementNode.append(ifExpressionNode.data);
                   //  globalMap.insert(pair<string,AST>(ifExpressionNode.data,ifExpressionNode));
                   //  //cout<<getInfo[0]<<endl;
                   //  //cout<<"DFS .."<<endl;
                   //  DFS(value,firstBEX,ASTname);
                    //cout<<"DFS 1221"<<endl;
                    //DFS(map<string,int> vec,string BEX,map<string,string>& jmpTable,map<string,string>& father)

                    break;
                }
                case 3://while
                {
                    int getIter=iter;
                    getIter++;
                    int stackCnt=0;
                    if(matchBegin(vecx[getIter]))
                    {
                        stackCnt++;
                        getIter++;
                        while(getIter<vecx.size() && stackCnt!=0)
                        {
                            if(matchBegin(vecx[getIter]))stackCnt++;
                            else if(matchEnd(vecx[getIter]))stackCnt--;
                            getIter++;
                        }
                        if(stackCnt!=0)
                        {
                            cout<<"error match end of if expression at line "<<getIter+1<<endl;
                            return 0;
                        }
                        cout<<"begin while"<<endl;
                        string resData=handleWhileExpression(vecx,getInfo,iter,getIter);
                        cout<<"end while resData= "<<resData<<endl;
                        iter=getIter-1;//fix bug compoundStatementNode含有if层级的ifnode子节点
                        compoundStatementNode.append(resData);
                    }
                    else//不是复合语句
                    {
                    	string resData=handleWhileExpression(vecx,getInfo,iter,getIter+1);
                        //cout<<"end while resData= "<<resData<<endl;
                        iter=getIter;//fix bug compoundStatementNode含有if层级的ifnode子节点
                        compoundStatementNode.append(resData);
                    }
                    break;
                }
                case 4://begin
                {
                    break;
                }
                case 5://end
                {
                    break;
                }
                default:
                {
                    cout<<"error match expression type at line "<<iter<<endl;
                    return 0;
                }
            }
            iter++;
        }
        globalMap.insert(pair<string,AST>(compoundStatementNode.data,compoundStatementNode));
        rootAST.append(compoundStatementNode.data);
        globalMap.insert(pair<string,AST>("root",rootAST));
    }
    generateGraph();
    // cout<<"globalMapForQ:"<<endl;
    // map<string,AST>::iterator mapIter;
    // for(mapIter=globalMapForQ.begin();mapIter!=globalMapForQ.end();mapIter++)
    // {
    //     for(int i=0;i<(mapIter->second).ptrArray.size();i++)
    //     {
    //         if((mapIter->second).ptrArray[i]!="NULL")
    //         {
    //             cout<<"\""<<(mapIter->first)<<"\""<<" -> "<<"\""<<(mapIter->second).ptrArray[i]<<"\""<<endl;
    //         }
    //     }
    // }
    // cout<<"fmap:"<<endl;
    // map<string,string>::iterator fmap;
    // for(fmap=father.begin();fmap!=father.end();fmap++)
    // {
    //     cout<<"\""<<(fmap->first)<<"\""<<" -> "<<"\""<<(fmap->second)<<"\""<<endl;
    // }
    // cout<<"vmap:"<<endl;
    // map<string,int>::iterator vmap;
    // for(vmap=value.begin();vmap!=value.end();vmap++)
    // {
    //     cout<<"\""<<(vmap->first)<<"\""<<" -> "<<"\""<<(vmap->second)<<"\""<<endl;
    // }
    // cout<<"jmap1:"<<endl;
    // map<string,string>::iterator jmap1;
    // for(jmap1=jmpTable1.begin();jmap1!=jmpTable1.end();jmap1++)
    // {
    //     cout<<"\""<<(jmap1->first)<<"\""<<" -> "<<"\""<<(jmap1->second)<<"\""<<endl;
    // }
    // cout<<"jmap0:"<<endl;
    // map<string,string>::iterator jmap0;
    // for(jmap0=jmpTable0.begin();jmap0!=jmpTable0.end();jmap0++)
    // {
    //     cout<<"\""<<(jmap0->first)<<"\""<<" -> "<<"\""<<(jmap0->second)<<"\""<<endl;
    // }
    cout<<"symbolTable:"<<endl;//输出符号表
    map<string,string>::iterator jmap0;
    for(jmap0=symbolTable.begin();jmap0!=symbolTable.end();jmap0++)
    {
        cout<<"\""<<(jmap0->first)<<"\""<<" -> "<<"\""<<(jmap0->second)<<"\""<<endl;
    }
    cout<<"globalQuaternion:"<<endl;//输出四元式
    for(int k=0;k<globalQuaternion.size();k++)
    {
        cout<<k<<" "<<globalQuaternion[k]<<endl;
    }
    return 1;

}
int main(char argc,char * argv[])//dot -Tpng out.dot -o out.png
{       
   
	// string lstr="arrray[11][b1][c2]:=dc36;";
	// vector<string> vecs;
	// matchAssignmentExpressionForArray(lstr,vecs);
	// printArray(vecs,"@");

	vector<string> vecs;
	string filePath = "pascal.pas";
	if (argc > 1)
		filePath = string(argv[1]);
	
  	ifstream file;
  	file.open(filePath,ios::in);
  	if(!file.is_open())
        return 0;   
    std::string strLine;
    while(getline(file,strLine))
    {
        if(strLine.empty())
           continue;
 		vecs.push_back(strLine);          
    }
    compiler(vecs);
    

    //string str="    begin";
    //bool bx=matchBegin(str);
    //cout<<bx<<endl;
    
    /*
    string strx="cnt:=cnt+a1/a2;";
    vector<string> vec;
    matchAssignmentExpression(strx,vec);
    for(int i=0;i<vec.size();i++)
    {
        cout<<vec[i]<<endl;
    }
    */
    
    /*string strxxx="cnt+a1/a2;";
    vector<string> vecxxx;
    TEST(strxxx,vecxxx);
    for(int i=0;i<vecxxx.size();i++)
    {
        cout<<vecxxx[i]<<endl;
    }
    */
    
    /*
    string str=" and x13==13 or x15==15.6 and a==b or xc==15.6";
    str="  while( x12 == a123 and x13==13 or x15==15.6)do";
    str="n:=n+1;";
    str="if(a<=b and x13==13 or x15==15.6)then";
    vector<string> vec;
    //matchAssignmentExpression(str,vec);
    //matchWhileExpression(str,vec);
    matchIfExpression(str,vec);
    //matchTest(str,vec);
    for(int i=0;i<vec.size();i++)
    {
        cout<<vec[i]<<endl;
    }
    vector<string> postorderl;
    //expressionSeparate(getInfo[1],rightExpression_inOrder);
    inorder2postorder(vec,postorderl);
    for(int i=0;i<postorderl.size();i++)
    {
        cout<<postorderl[i]<<endl;
    }
    */

    
    /*
    vector<string> vec;
    vector<string> postorder;
    vec.push_back("cnt");
    vec.push_back("+");
    vec.push_back("a1");
    vec.push_back("/");
    vec.push_back("a2");
    inorder2postorder(vec,postorder);
    buildTree(vec,postorder,0, vec.size() - 1, postorder.size() - 1, 0,"E");
    map<string,AST>::iterator mapIter;
    for(mapIter=globalMap.begin();mapIter!=globalMap.end();mapIter++)
    {
        if((mapIter->second).ptrArray.size()>0)
            cout<<(mapIter->first)<<endl;
        for(int i=0;i<(mapIter->second).ptrArray.size();i++)
        {
            cout<<(mapIter->second).ptrArray[i]<<endl;
        }
    }
    */
    

    // char *ss;
    // string res;
    // PexpretoSexpre(ss,res);
    /*
    vector<string> vec;
    vector<string> res;
    vec.push_back("cnt");
    vec.push_back("+");
    vec.push_back("a1");
    vec.push_back("/");
    vec.push_back("a2");
    PexpretoSexpre(vec,res);
    for(int i=0;i<res.size();i++)
    {
        cout<<res[i]<<endl;
    }
    */
	//string get="(a<1 or a>2)and(b<3 or b>4)";
	// string get;
	// vector<string> ress;
	// vector<string> post;
 //    convertBoolExpression(get,ress,post);
 //    printArray(ress,"~~~");
 //    printArray(post,"!!!");
    //buildASTtreeForQuaternion
    // vector<string> bexs;
    // bexs.push_back("(");
    // bexs.push_back("1");
    // bexs.push_back("or");
    // bexs.push_back("2");
    // bexs.push_back(")");
    // bexs.push_back("and");
    // bexs.push_back("(");
    // bexs.push_back("3");
    // bexs.push_back("or");
    // bexs.push_back("4");
    // bexs.push_back(")");
    // vector<string> bexspost;
    // PexpretoSexpreForBEX(bexs,bexspost);
    // for(int i=0;i<bexspost.size();i++)
    // 	cout<<bexspost[i]<<endl;
    // char* s="(1+2)*(3+4)";
    // string testRes;
    // funcPexpretoSexpre(s,testRes);
    // cout<<testRes<<endl;

   

    // vecs.push_back("program pen;");
    // vecs.push_back("var acc,vcc:integer;");//填符号表
    // vecs.push_back("var bss:real;");
    // vecs.push_back("begin");
    // vecs.push_back("cnt:=c1*a1+cnt+c2*a2;");
    // vecs.push_back("if(s <= 100 and s < 101 or s!=102)then");
    // vecs.push_back("begin");
    // vecs.push_back("s:=s+a1;");
    // vecs.push_back("if(s==100 or s==101)then");
    // vecs.push_back("begin");
    // vecs.push_back("cnt:=cnt+a1;");
    // vecs.push_back("end");
    // vecs.push_back("a:=a;");
    // vecs.push_back("while(cnt<10 and cnt>c2)do");
    // vecs.push_back("begin");
    // vecs.push_back("cnt:=cnt+1;");
    // vecs.push_back("end");
    // vecs.push_back("cnt:=cnt*a2;");
    // vecs.push_back("end");
    // vecs.push_back("else");//20
    // vecs.push_back("begin");
    // vecs.push_back("acc:=vcc+bss;");
    // vecs.push_back("end");
    //vecs.push_back("end");
    // compiler(vecs);

    // vector<string> ccc;
    // string sss="a";
    // expressionSeparate(sss,ccc);
    // for(int i=0;i<ccc.size();i++)
    // {
    //     cout<<ccc[i]<<endl;
    // }

    // vector<string> post;
    // post.push_back("c1");
    // post.push_back("a1");
    // post.push_back("*");
    // post.push_back("cnt");
    // post.push_back("c2");
    // post.push_back("a2");
    // post.push_back("*");
    // post.push_back("+");
    // post.push_back("+");
    // vector<string> res;
    // generateQuaternion(post,res);

 /*string str="1.1+cnt+1*a1";
 vector<string> vvv;
 expressionSeparate(str,vvv);*/
    // char* ss="1*2+3";
    // vector<string> instr;
    // vector<string> poststr;
    // instr.push_back("a1");
    // instr.push_back("*");
    // instr.push_back("a2");
    // instr.push_back("+");
    // instr.push_back("a3");
    // string result;
    // PexpretoSexpre(ss,result);
    // PexpretoSexpre(instr,poststr);
    // for(int i=0;i<poststr.size();i++)
    // {
    //     cout<<poststr[i]<<endl;
    // }
    // cout<<result<<endl;
    
    //string str1="NPU123@mail.nwpu.edu.cn";
    //string str2="NWPUisthebest.com";

    /*
    string str;
    vector<string> vec;
    matchVarExpression(str,vec);
    */
    
    /*
    matchProgram(str,str);
    matchEmptyLine(str);
    string str1="+123.233";
    string str2="s:=0.0;";
    int addr=judgeInitAssignment(str2);
    cout<<addr<<endl;
    initAssignment(str2,addr);
    if(isRealNum(str1))
        cout<<str1<<" is an email"<<endl;
    else
        cout<<str1<<" is not an email"<<endl;

    if(isRealNum(str2))
        cout<<str2<<" is an email"<<endl;
    else
        cout<<str2<<" is not an email"<<endl;
        */

    return 0;
}






// bool isRealNum( string strx)
// {
//     //const boost::regex pattern("\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*");
//     //const boost::regex pattern("^[-|+]?\\d+[.\\d+]?$");
//     //const boost::regex pattern("^[-?\\d+][\\.\\d+]?$");
//     const boost::regex pattern("^[-|+]?[0-9]+[.[0-9]+]?$");
//     return boost::regex_match(strx.c_str(),pattern);
// }
// bool isUper(char chx)
// {
//     return chx>='A' && chx<='Z';
// }
// bool isLower(char chx)
// {
//     return chx>='a' && chx<='z';
// }

// bool isUnderline(char chx)
// {
//     return chx=='_';
// }
// bool isNumber(char chx)
// {
//     return chx>='0' && chx<='9';
// }


// int judgeInitAssignment(string strx)
// {
//     int res=strx.find(":=");
//     return res;
// }
// void initAssignment(string strx,int assignIndex)
// {
//     int i=0;
//     while(strx[i]==' ')i++;
//     if(isNumber(strx[i]))//有问题
//     {
//         cout<<"error"<<endl;
//         return;
//     }
//     quaternion get;
//     get.first=":=";
//     get.third="_";
//     for(;i<assignIndex;i++)
//     {
//         if(!(isUper(strx[i]) || isLower(strx[i]) || isNumber(strx[i])))
//         {
//             cout<<"error"<<endl;
//             break;
//         }
//         else
//         {
//             get.fourth+=strx[i];
//         }
//     }
   
//     string numStr;
//     i=i+2;
//     while(strx[i]!=';' && i<strx.size())
//     {
//         numStr+=strx[i];
//         i++;
//     }
//      cout<<"777"<<endl;
//     if(isRealNum(numStr))
//     {
//         get.second=numStr;
//     }
//     allQuaternions.push_back(get);
//     cout<<get.first<<endl;
//     cout<<get.second<<endl;
//     cout<<get.third<<endl;
//     cout<<get.fourth<<endl;
// }