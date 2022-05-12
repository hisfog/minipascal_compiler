struct nameAndType
{
    string varName;
    string varType;
}
vector<nameAndType> symbols;
void Judge(string strx)
{
    string first3Str;
    for(int i=0;i<3;i++)
    {
        first3Str+=strx[i];
    }
    if(first3Str=="var"
       && strx[3]==' ')
    {
        int i=3;
        while(strx[i]==' ')i++;
        vector<nameAndType> symbolsGet;
        
        while(strx[i]!=':' && i<strx.size())
        {
            nameAndType nameGet;
            while(strx[i]!=','&& strx[i]!=':')
            {
                nameGet.varName+=strx[i];
                //cout<<strx[i]<<endl;
                 i++;

            }
            if(strx[i]==',')i++;//非正常结束的while循环没有更新i
            symbolsGet.push_back(nameGet); //需要变量名合法性检查
        }
        string typeGet;
        while(strx[i]!=';')
        {
            typeGet+=strx[i];
            i++;
        }
        for(int i=0;i<symbolsGet.size();i++)//是否为类型保留字
        {
            symbolsGet[i].varType=typeGet;
            symbols.push_back(symbolsGet[i]);
        }


    }
    else
    {
        cout<<"error"<<endl;
    }
}

//派生var
bool isUper(char chx)
{
    return chx>='A' && chx<='Z';
}
bool isLower(char chx)
{
    return chx>='a' && chx<='z';
}
bool isUnderline(char chx)
{
    return chx=='_';
}
bool isNumber(char chx)
{
    return chx>='0' && chx<='9';
}
//派生var
void nextVarJudge(string strx)
{
    int i=0;
    if(isNumber(strx[0]))//有问题
    {
        cout<<"error"<<endl;
    }
    while(strx[i]==' ')i++;
    vector<nameAndType> symbolsGet;
    while(strx[i]!=':' && i<strx.size())
    {
        if(!(isUper(strx[i]) || isLower(strx[i]) || isNumber(strx[i])))
            cout<<"error"<<endl;
        else
        {
            nameAndType nameGet;
             while(strx[i]!=','&& strx[i]!=':')
            {
                nameGet.varName+=strx[i];
                //cout<<strx[i]<<endl;
                 i++;

            }
            if(strx[i]==',')i++;//非正常结束的while循环没有更新i
            symbolsGet.push_back(nameGet); //需要变量名合法性检查
        }
    }
    string typeGet;
    while(strx[i]!=';')
    {
        typeGet+=strx[i];
        i++;
    }
    for(int i=0;i<symbolsGet.size();i++)//是否为类型保留字
    {
        symbolsGet[i].varType=typeGet;
        symbols.push_back(symbolsGet[i]);
    }
}



void int2str(int itx,string& strx)
{
    stringstream ss;
    ss<<itx;
    ss>>strx;
}
class AST
{
    public:
    vector<string> ptrArray;//子节点
    AST* ptrFather;//父节点
    string data;
    AST(string strx);
    ~AST();
    void append(AST* ptrx);


}
AST::AST(string strx)
{
    data=strx;
}
AST::~AST()
{

}
AST::append(string strx)
{
    ptrArray.push_back(strx);
}
map<string,AST> globalMap;
int compiler(vector<string> vecx)//假设已经去除所有空行
{
    int i=0;
    if(!matchProgram(strx[i]))
    {
        cout<<"error match program"<<endl;
        return -1;
    }
    AST rootAST("root");
    map.insert(pair<string,AST>("root",rootAST));
    AST programNode("program"+int2str(i));
    rootAST.append("program"+int2str(i));
    map.insert(pair<string,AST>("program"+int2str(i),programNode));
    i++;
    while(matchVarExpression(strx[i]))
    {
        AST varNode("var"+int2str(i));
        //
        
        map.insert(pair<string,AST>("var"+int2str(i),varNode));
    }

}




void finalPexpretoSexpre(vector<string>& ss ,vector<string>& result)
{
    int i=0,j=0;
    int op=0;
    vector<string> num;
    num.push_back("0");
    vector<string> opera;
    opera.push_back("0");
    while (i<ss.size())
    {
    	if (notOpera(ss[i]))    /* 如果是数字 */
        {
            num[j] = ss[i];    /* 数字直接入后缀表达式栈 */
            j++;
            i++;
        }
        else 
        {
        	if(ss[i]=="or")
        	{
        		if (op == 0)    /* 如果是空栈 */
                {
                    PushOperation(opera, ss, &op, &i);    /* 入运算符栈 */
                }
                if (!notOpera(opera[op-1]))
                {
                	if(opera[op-1]=="or")
                	{
                		PushOperation(opera, ss, op, i);
                	}
                	else if(opera[op-1]=="and")
                	{
                		num[j] = opera[op-1];    /* 将操作数出栈 */
                        opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
                        j++;
                        i++;
                	}
                	else if(opera[op-1]=="(")
                	{
                		PushOperation(opera, ss, op, i);
                	}
                }
        	}
        	else if(ss[i]=="and")
        	{
        		if (op == 0)    /* 如果是空栈 */
                {
                    PushOperation(opera, ss, &op, &i);    /* 入运算符栈 */
                }
                if (!notOpera(opera[op-1]))
                {
                	if(opera[op-1]=="or")
                	{
                		PushOperation(opera, ss, op, i);
                	}
                	else if(opera[op-1]=="and")
                	{
                		PushOperation(opera, ss, op, i);
                	}
                	else if(opera[op-1]=="(")
                	{
                		PushOperation(opera, ss, op, i);
                	}
                }
        	}
        	else if(ss[i]=="(")
            {
                PushOperation(opera, ss, op, i);
            }
            else if(ss[i]==")")
            {
                while (opera[op-1] != '(')
                {
                    num[j] = opera[op-1];    /* 将运算符栈中的元素依次入栈到后缀表达式栈中，直到遇到左括号为止 */
                    j++;
                    op--;
                }
                op--;
                i++;
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
}
void funcPexpretoSexpre(char *ss,string& result)
{
    //ss="1+2*3";
    char num[100] = "0";    /* 存储后缀表达式 */
    char opera[100] = "0";    /* 存储运算符 */
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
                    
                        case '*':
                            {    /* 加法优先级低于乘法 */
                                num[j] = opera[op-1];    /* 将操作数出栈 */
                                opera[op-1] = ss[i];        /* 将新的操作数压入栈中 */
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
                        
                        case '*':
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