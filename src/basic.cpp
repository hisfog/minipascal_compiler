#include <iostream>
#include <boost/regex.hpp>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
using namespace std;
bool isVarName(string strx)
{
	boost::smatch matRes;
    boost::regex pattern("[a-zA-Z_]+[0-9a-zA-Z_]*");
    if(boost::regex_search(strx,matRes,pattern))
    	return 1;
    else 
    	return 0;

}
bool matchProgram(string strx,string& programName)//匹配program语句
{
    //strx="program  _pen_1  ;";
    boost::smatch matRes; // 匹配结果
    //boost::regex reg("[^program][a-zA-Z_]+[0-9a-zA-Z_]*"); //截取program名
    const boost::regex pattern("^program[\\s]+((?:[a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*);");//program开头后接>0个空白，标识符，空白;
    //const boost::regex pattern("^program[\\s]*[a-zA-Z]*[\\s];");
    if(boost::regex_match(strx.c_str(),pattern))
    {
       
        boost::regex_search(strx,matRes,pattern);
        programName=matRes[1];
        //cout<<programName<<endl;
        return 1;
    }
    else
        return 0;
        //cout<<"error"<<endl;
    
    
}


bool matchEmptyLine(string strx)//匹配空行
{
    //strx="      \n";
    const boost::regex pattern("[\\s]*\\n");
    if(regex_match(strx.c_str(), pattern))
    {
        cout<<"Emptyline"<<endl;
        return 1;
    }
    else
    {
        cout<<"not empty"<<endl;
        return 0;
    }
}

bool matchVarExpression(string strx,vector<string>& vecx)//匹配var变量申明语句
{
    //strx="  var    sa,c,e,faa,dr:integer;";
    boost::smatch matRes;
    //boost::regex pattern("([a-z]+,\\>)");
    boost::regex pattern("^(?:[\\s]*var[\\s]+)((?:[a-zA-Z_]+[0-9a-zA-Z_]*,)*)([a-zA-Z_]+[0-9a-zA-Z_]*:(?:[a-zA-Z_])+;)");
    if(boost::regex_search(strx,matRes,pattern))
    {
        //cout<<"matchVarExpression"<<endl;
        string tmp;
        string get=matRes[1];
        //cout<<"1"<<get<<endl;
        for(int i=0;i<get.size();i++)
        {
            if(get[i]!=',')
                tmp+=get[i];
            else
            {
                vecx.push_back(tmp);
                //cout<<"tmp"<<get<<endl;
                tmp="";
            }
        }
        get=matRes[2];
        //cout<<"2"<<get<<endl;
        tmp="";
        for(int i=0;i<get.size();i++)
        {
            if(get[i]!=':' && get[i]!=';')
                tmp+=get[i];
            else
            {
                vecx.push_back(tmp);
                //cout<<"tmp2"<<get<<endl;
                tmp="";
            }
        }
        return 1;
        /*
        for(int i=0;i<vecx.size();i++)
        {
            cout<<"#"<<vecx[i]<<endl;
        }
        */
        /*
        for(int i=0;i<matRes.size();i++)
        {
            cout<<matRes[i]<<endl;
        }*/
        
        
    }
    else
        return 0;
    
}

bool matchBegin(string strx)//匹配begin
{
    boost::regex pattern("^[\\s]*begin[\\s]*");
    if(boost::regex_match(strx,pattern))
    {
        //cout<<"begin iis ok"<<endl;
        return 1;
    }
    else return 0;
}
bool matchEnd(string strx)//匹配end
{
    boost::regex pattern("^[\\s]*end[\\s]*");
    if(boost::regex_match(strx,pattern))
    {
        return 1;
    }
    else return 0;
}

bool matchElse(string strx)//匹配else
{
    boost::regex pattern("^[\\s]*else[\\s]*");
    if(boost::regex_match(strx,pattern))
    {
        return 1;
    }
    else return 0;
}

bool matchAssignmentExpression(string strx,vector<string>& vecx)//匹配赋值语句
{
    boost::smatch matRes;
    boost::regex pattern("^([a-zA-Z_]+[0-9a-zA-Z_]*)[\\s]*:=(.+);");
    if(boost::regex_search(strx,matRes,pattern))
    {
        for(int i=1;i<matRes.size();i++)
        {
            if(matRes[i]!="")
            {
                vecx.push_back(matRes[i]);
                //cout<<matRes[i]<<endl;
            }
            
        } 
        return 1;
    }
    else return 0;
}
bool matchAssignmentExpressionForArrayVer1(string strx,vector<string>& vecx)//匹配赋值语句
{
    boost::smatch matRes;
    boost::regex pattern("^((?:[a-zA-Z_]+[0-9a-zA-Z_]*)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*(\\[(?:(?:[-|+]?[0-9]+)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*))\\])+))[\\s]*:=(.+);");
    if(boost::regex_search(strx,matRes,pattern))
    {
        for(int i=1;i<matRes.size();i++)
        {
            if(matRes[i]!="")
            {
                vecx.push_back(matRes[i]);
                //cout<<matRes[i]<<endl;
            }
            
        } 
        return 1;
    }
    else return 0;
}
bool matchAssignmentExpressionForArray(string strx,vector<string>& vecx)//匹配赋值语句
{
    boost::smatch matRes;
    boost::regex pattern("^((?:([a-zA-Z_]+[0-9a-zA-Z_]*)(?:\\[(?:(?:[-|+]?[0-9]+)|(?:[a-zA-Z_]+[0-9a-zA-Z_]*))\\])+))[\\s]*:=(.+);");
    if(boost::regex_search(strx,matRes,pattern))
    {
        vecx.push_back("ARRAY_AE!");
        vector<string> get;
        vecx.push_back(matRes[1]);
        for(int i=1;i<matRes.size();i++)
        {
            if(matRes[i]!="")
            {
                get.push_back(matRes[i]);
                //cout<<"$$$$$$$$$$$$$$$$"<<matRes[i]<<endl;
            }
            
        }
        if(matRes.size()>=4)
        {
        	vecx.push_back(matRes[2]);
        	vecx.push_back(matRes[3]);
        }
        
        string allInfo=get[0];
        boost::smatch matIndexRes;
    	boost::regex indexPattern("(?:\\[)([^\\[\\]]+)(?:\\])");//提取数组index信息
    	while(boost::regex_search(allInfo,matIndexRes,indexPattern))
    	{
    		string getIndexRes=matIndexRes[1];
    		//cout<<getIndexRes<<endl;
    		vecx.push_back(getIndexRes);
    		int position=allInfo.find(getIndexRes);
    		allInfo.replace(position,getIndexRes.size(),"");
    		//cout<<"!"<<allInfo<<endl;
    	}


        return 1;
    }
    else return 0;
}

bool matchVarExpressionForArray(string strx,vector<string>& vecx)//匹配var变量申明语句
{
    //strx="  var    sa,c,e,faa,dr:integer;";
    boost::smatch matRes;
    //boost::regex pattern("([a-z]+,\\>)");
    boost::regex pattern("^(?:[\\s]*var[\\s]+)((?:[a-zA-Z_]+[0-9a-zA-Z_]*,)*)([a-zA-Z_]+[0-9a-zA-Z_]*:(?:[a-zA-Z_])+\\[[0-9]+\\.\\.[0-9]+\\];)");
    if(boost::regex_search(strx,matRes,pattern))
    {
        //cout<<"matchVarExpression"<<endl;
        string tmp;
        string get=matRes[1];
        //cout<<"1"<<get<<endl;
        for(int i=0;i<get.size();i++)
        {
            if(get[i]!=',')
                tmp+=get[i];
            else
            {
                vecx.push_back(tmp);
                //cout<<"tmp"<<get<<endl;
                tmp="";
            }
        }
        get=matRes[2];
        //cout<<"2"<<get<<endl;
        tmp="";
        for(int i=0;i<get.size();i++)
        {
            if(get[i]!=':' && get[i]!=';')
                tmp+=get[i];
            else
            {
                vecx.push_back(tmp);
                //cout<<"tmp2"<<get<<endl;
                tmp="";
            }
        }
        return 1;  
    }
    else
        return 0;
    
}