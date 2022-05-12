#include "AST.h"

map<string,AST> globalMap;
map<string,AST> globalMapForQ;
map<string,int> value;//真值
map<string,string> jmpTable1;
map<string,string> jmpTable0;
map<string,string> father;
AST::AST()
{
    data="";
    expressionData="";
}
AST::AST(string strx)
{
    data=strx;
    expressionData="";
}
AST::~AST()
{

}
void AST::append(string strx)
{
    ptrArray.push_back(strx);
}


string buildTree(vector<string>& inorder, vector<string>& postorder, int inStart, int inEnd, int postEnd, int postStart,string name)
{//建立算术表达式的语法树
    if (inStart > inEnd || postEnd < postStart) return "NULL";//return AST("NULL");
        AST expressionNode(name+":"+postorder[postEnd]);//TreeNode *res;
        //res = new TreeNode(preorder[preStart]);
        int flag;
        for (int i = inStart; i <= inEnd; i++) {
            if (inorder[i] == postorder[postEnd]) {
                flag = i;
                break;
            }
        }
        AST leftNode=buildTree(inorder, postorder, inStart, flag - 1, postStart - inStart + flag - 1, postStart,name+"0");//left
        expressionNode.append(leftNode.data);
        globalMap.insert(pair<string,AST>(leftNode.data,leftNode));
        AST rightNode=buildTree(inorder, postorder, flag + 1, inEnd, postEnd - 1, postEnd - inEnd + flag,name+"1");//right
        expressionNode.append(rightNode.data);
        globalMap.insert(pair<string,AST>(rightNode.data,rightNode));
        globalMap.insert(pair<string,AST>(expressionNode.data,expressionNode));
        return expressionNode.data;
}
AST buildTreeForQuaternion(vector<string>& inorder, vector<string>& postorder, int inStart, int inEnd, int postEnd, int postStart,string name,string& firstBEX,map<string,string>& jmpTable0,map<string,string>& jmpTable1,map<string,string>& name2index)
{//建立布尔表达式的语法树，并填写跳转表
    if (inStart > inEnd || postEnd < postStart) return AST("NULL");//return "NULL";//
        AST expressionNode(name+":"+postorder[postEnd]);//TreeNode *res;
        name2index.insert(pair<string,string>(postorder[postEnd],expressionNode.data));
        if(postorder[postEnd]=="and" ||postorder[postEnd]=="or")
        {
            expressionNode.expressionData=postorder[postEnd];

            //cout<<expressionNode.expressionData<<endl;
        }
        if(postorder[postEnd]==firstBEX)
        {
            firstBEX=name+":"+postorder[postEnd];
        }
        //res = new TreeNode(preorder[preStart]);
        int flag;
        for (int i = inStart; i <= inEnd; i++) {
            if (inorder[i] == postorder[postEnd]) {
                flag = i;
                break;
            }
        }
        //建立左边子树
        AST leftNode=buildTreeForQuaternion(inorder, postorder, inStart, flag - 1, postStart - inStart + flag - 1, postStart,name+"0",firstBEX,jmpTable0,jmpTable1,name2index);//left
        expressionNode.append(leftNode.data);
        globalMap.insert(pair<string,AST>(leftNode.data,leftNode));
        globalMapForQ.insert(pair<string,AST>(leftNode.data,leftNode));
        if(leftNode.expressionData=="")value.insert(pair<string,int>(leftNode.data,-1));
        father.insert(pair<string,string>(leftNode.data,expressionNode.data));
        jmpTable1.insert(pair<string,string>(leftNode.data,"-1"));
        jmpTable0.insert(pair<string,string>(leftNode.data,"-1"));
        //建立右子树
        AST rightNode=buildTreeForQuaternion(inorder, postorder, flag + 1, inEnd, postEnd - 1, postEnd - inEnd + flag,name+"1",firstBEX,jmpTable0,jmpTable1,name2index);//right
        expressionNode.append(rightNode.data);
        globalMap.insert(pair<string,AST>(rightNode.data,rightNode));
        globalMapForQ.insert(pair<string,AST>(rightNode.data,rightNode));
        if(rightNode.expressionData=="")value.insert(pair<string,int>(rightNode.data,-1));
        father.insert(pair<string,string>(rightNode.data,expressionNode.data));
        jmpTable1.insert(pair<string,string>(rightNode.data,"-1"));
        jmpTable0.insert(pair<string,string>(rightNode.data,"-1"));

        globalMap.insert(pair<string,AST>(expressionNode.data,expressionNode));
        globalMapForQ.insert(pair<string,AST>(expressionNode.data,expressionNode));
        if(expressionNode.expressionData=="")value.insert(pair<string,int>(expressionNode.data,-1));
        return expressionNode;//.data;
}
string buildASTtree(vector<string>& inorder, vector<string>& postorder,string name)
{
    return buildTree(inorder,postorder,0, inorder.size()-1, postorder.size()-1, 0,name);
}
string buildASTtreeForQuaternion(vector<string>& inorder, vector<string>& postorder,string name,string& firstBEX,map<string,string>& jmpTable0,map<string,string>& jmpTable1,map<string,string>& name2index)
{
    return buildTreeForQuaternion(inorder,postorder,0, inorder.size()-1, postorder.size()-1, 0,name,firstBEX,jmpTable0,jmpTable1,name2index).data;
}


int shortCircuitBoolCalculate(int x,int y,string op)//短路求值
{
    if(x==-1)
    {
        if(y==1 && op=="or")return 1;
        else if(y==0 && op=="and")return 0;
    }
    else
    {
        if(op=="or")
        {
            if(x==0 && y==0)return 0;
            else return 1;
        }
        else if(op=="and")
        {
            if(x==1 && y==1)return 1;
            else return 0;
        }
    }
}
string getAnotherChildData(string one,string fatherData)//获取另一个子节点（不同于one的）
{
    //cout<<one<<" one "<<fatherData<<" fatherData"<<endl;
    int twoOfChild=((globalMapForQ[fatherData]).ptrArray).size();
    for(int i=0;i<twoOfChild;i++)
    {
        string get=(globalMapForQ[fatherData]).ptrArray[i];
        if(get!=one)
        {
            //cout<<get<<" get"<<endl;
            return get;
        }
    }
    
}
string calculateNextBoolExpression(map<string,int> vec,string BE,string rootBEX)//计算下一个要考虑的布尔表达式
//string calculateNextBoolExpression(map<string,int> vec,string BE,map<string,string>& father)//father应该提供data到data的映射
{//vec应该提供data到int的映射
    //cout<<"calculateNextBoolExpression "<<BE<<endl;
    string fatherData=father[BE];
    string anotherChildData=getAnotherChildData(BE,fatherData);
    //cout<<"anotherChildData= "<<anotherChildData<<endl;
    int anotherBoolValue=vec[anotherChildData];//
    //cout<<"while begin"<<endl;
    //string anotherBoolValue=(fatherPtr->anotherChild(BE)).realData;//布尔值
    string fatherOPdata=(globalMapForQ[fatherData]).expressionData;//父节点的操作符
    //cout<<"while begin"<<endl;
    while(anotherBoolValue!=-1 && anotherBoolValue!=3 ||(vec[BE]==1&&fatherOPdata=="or") || (vec[BE]==0&&fatherOPdata=="and"))
    {
        //if(anotherBoolValue!=-1 || (anotherBoolValue==-1 && vec[BE]==1 && fatherOPdata=="or") || (anotherBoolValue==-1 && vec[BE]==0 && fatherOPdata=="and"))
        vec[fatherData]=shortCircuitBoolCalculate(anotherBoolValue,vec[BE],fatherOPdata);
        if((vec[BE]==1&&fatherOPdata=="or") || (vec[BE]==0&&fatherOPdata=="and"))
            vec[anotherChildData]=3;//表示另一个节点为0或1不影响父节点的取值，此时nextBEX应该略过该节点
        //cout<<"update vec["<<fatherData<<"]="<<vec[fatherData]<<endl;
        //cout<<vec[fatherData]<<" vec[fatherData] "<<fatherData<<" fatherData"<<endl;
        if(fatherData==rootBEX)return vec[fatherData]==1?"True":"False";
        BE=fatherData;
        fatherData=father[BE];
        anotherChildData=getAnotherChildData(BE,fatherData);
        anotherBoolValue=vec[anotherChildData];
        fatherOPdata=(globalMapForQ[fatherData]).expressionData;
        //cout<<vec[BE]<<"&&&"<<fatherOPdata<<endl;
    }
    //cout<<"while ok"<<endl;
    map<string,int>::iterator mapIter;
    for(mapIter=vec.begin();mapIter!=vec.end();mapIter++)
    {
        if(mapIter->second==-1)
        {
            //cout<<"return "<<mapIter->first<<endl;
            return mapIter->first;
        }
    }
}
void DFS(map<string,int> vec,string BEX,string rootBEX,map<string,string>& jmpTable0,map<string,string>& jmpTable1)//深搜，用于填写跳转表
//void DFS(map<string,int> vec,string BEX,map<string,string>& jmpTable,map<string,string>& father)
{
    map<string,int>::iterator jmap1;
    int cntNot4=0;
    for(jmap1=vec.begin();jmap1!=vec.end();jmap1++)
    {
        if(jmap1->second!=4)cntNot4++;
    }
    if(cntNot4==1)
    {
    	jmpTable1[BEX]="True";
    	jmpTable0[BEX]="False";
    	return ;
    }
    if(BEX=="True" || BEX=="False")
    {
        return ;
    }
    //cout<<"i get"<<BEX<<endl;
    vec[BEX]=1;
    //cout<<"ok"<<endl;
    string NBE=calculateNextBoolExpression(vec,BEX,rootBEX);
    //cout<<"BEX "<<BEX<<" NBE "<<NBE<<endl;
    if(jmpTable1[BEX]=="-1")//若之前已经填过该项则不可再次修改
    {
        jmpTable1[BEX]=NBE;
        //cout<<"jmpTable1["<<BEX<<"]="<<NBE<<endl;
    }  
    DFS(vec,NBE,rootBEX,jmpTable0,jmpTable1);

    vec[BEX]=0;
    NBE=calculateNextBoolExpression(vec,BEX,rootBEX);
    if(jmpTable0[BEX]=="-1")
    {
        jmpTable0[BEX]=NBE;
        //cout<<"jmpTable0["<<BEX<<"]="<<NBE<<endl;
    }   
    DFS(vec,NBE,rootBEX,jmpTable0,jmpTable1);//递归搜索
    return ;
}

