/* NAME     - DEEPESH KUMAR SINGH
* CSULB-ID  - 016149963 
* SUBJECT   - CECS 524 Adv. Programming Language
* Homework 4 - C++ 11 Code for SIL Homework 
***************************************************************************************************************************************************************************************
*/

#include <cmath>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <regex>
#include <iterator>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

#define BUFFER_SIZE 1024
#define TOKEN_SIZE 32

typedef struct symbol
{
    string identifierName;
    char* datatype;
    int value;
}symbol;

std::map<string,symbol*> Symbols_Map;
std::map<string,int> line_Map;
std::stack<int> mystack;

bool isIdentifier(const char* token)
{
    if(isalpha(token[0]) || token[0]=='_')
    {
        for(int i=1;token[i]!=(char)NULL;i++)
        {
            if(!isalnum(token[i]))
                return false;
        }
        return true;
    }
    return false;
}

bool contains_operator(const std::string &c)
{
    if (c.find('+') != std::string::npos ||
        c.find('-') != std::string::npos ||
        c.find('*') != std::string::npos ||
        c.find('/') != std::string::npos) 
    {
        return true;
    }
    return false;
}

bool contains_number(const std::string &c)
{
    if (c.find('1') != std::string::npos ||
        c.find('2') != std::string::npos ||
        c.find('3') != std::string::npos ||
        c.find('4') != std::string::npos ||
        c.find('5') != std::string::npos ||
        c.find('6') != std::string::npos ||
        c.find('7') != std::string::npos ||
        c.find('8') != std::string::npos ||
        c.find('9') != std::string::npos ||
        c.find('0') != std::string::npos )
    {
        return true;
    }

    return false;
}

bool contains_space(const std::string &c)
{
    if (c.find(' ') != std::string::npos) 
    {
        return true;
    }
    return false;
}

bool contains_comma(const std::string &c)
{
    if (c.find(',') != std::string::npos) 
    {
        return true;
    }
    return false;
}

bool is_number(const string& s){
   for(int i = 0; i < s.length(); i++)//for each char in string,
      if(! (s[i] >= '0' && s[i] <= '9' || s[i] == ' ') ) return false;

   return true;
}

bool is_digits(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit); 
}

void insert(string identifier,const char* type)
{
	symbol* newnode = (symbol*) malloc(sizeof(symbol));
    newnode->datatype = (char*) malloc((strlen(type)+1)*sizeof(char));
    strcpy(newnode->datatype,type);
	Symbols_Map.insert ( std::pair<string,symbol*>(identifier,newnode) );
}

int getPrecedence(char c)
{
	if(c=='+'||c=='-')
	{
		return 1;
	}
	else if(c=='*' || c=='/')
	{
		return 2;
	}
	else if(c=='^')
	{
		return 3;
	}
 
	return -1;
}

int applyOperator(char oprtr,int op1,int op2)
{
	switch(oprtr)
	{
		case '+':
			return op1+op2;
		case '-':
			return op2-op1;
		case '*':
			return op1*op2;
		case '/':
			return op2/op1;
		case '^':
			return pow(op2,op1);
	}
}
 
bool isOperator(char c)
{
	if(c=='+' || c=='-' || c=='*' || c=='/' || c=='^')
	{
		return true;
	}
	return false;
}

bool isOperator2(char c)
{
	if(c=='<' || c=='=' || c=='>')
	{
		return true;
	}
	return false;
}
int charToInt(char a)
{
	switch(a)
	{
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case '0':
			return 0;

	} 
}

int convertToInt(char str[])
{
	int num=0;
	int l=strlen(str);
	for(int i=0;i<l;i++)
	{
		num=num*10+charToInt(str[i]);
	}
	return num;
 
}

int evaluateInfix(vector<char *> toks,stack<int>* vs,stack<char>* os)
{
	for(auto i: toks)
	{
		if(i[0]>='0' && i[0]<='9')
		{
			vs->push(convertToInt(i));
		}
		else if(isOperator(i[0]))
		{
			while(!os->empty() && getPrecedence(i[0])<=getPrecedence(os->top()))
			{
				int op1=vs->top();
				vs->pop();
				int op2=vs->top();
				vs->pop();
				int res=applyOperator(os->top(),op1,op2);
				vs->push(res);
				os->pop();
			}
			os->push(i[0]);
		}
 
		else if(i[0]=='(')
		{
			os->push(i[0]);
		}
 
		else if(i[0]==')')
		{
			while(os->top()!='(')
			{
				int op1=vs->top();
				vs->pop();
				int op2=vs->top();
				vs->pop();
				char oprtr=os->top();
				os->pop();
				vs->push(applyOperator(oprtr,op1,op2));
			}
 
			os->pop();
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{   
    symbol* Newnode=0;
    string input_string;
	std::regex regexPattern("[a-zA-Z0-9]{1,32}|[^\\s\\w\"]|\"(\\.|[^\"])+\"");
	ifstream inputFile( "test.txt" );
	ifstream ifile("test.txt");
	int lineNumber = 0;
	int lineNo = 0;
	bool gosubBool=false;
	if (ifile)
	{
		int lineNum = 0;
		SomeLine:int a;
		while (getline( ifile, input_string ))
		{
			lineNum=lineNum+1;
			auto begin_words = std::sregex_iterator(input_string.begin(), input_string.end(), regexPattern);
			auto end_words = std::sregex_iterator();
			for (std::sregex_iterator i = begin_words; i != end_words; ++i)
			{
				std::smatch match_found = *i;
				std::string matchString = match_found.str();
				const char * c = matchString.c_str();
				if(is_digits(matchString))
				{
					line_Map.insert ( std::pair<string,int>(matchString,lineNum) );
				}
				else
				{
					cout<<"Error in the grammar. Exiting ...."<<endl;
					return 0;
				}
				goto SomeLine;
			}
		}
		ifile.close();
	}
	if (inputFile)
	{
		while (getline( inputFile, input_string ))
		{
			lineNumber = lineNumber+1;
			if(lineNumber<lineNo) continue;
			stack<int> vs;
			stack<char> os;
			vector<char *> toks;
 
			char ip[1000];
			int lineCount=0;
			bool found = false;
			int valInt=0;
			string line;
			int intCount = 0;
			int commaCount = 0;
			int count=0;
			int opCount=0;
			bool endTracker = true;
			int bracketTrack = 0;
			bool commaTracker = true;
			int compare1 = -1;
			int compare2 = -1;
			bool boolExp=false;
			int opVal=0;
			bool ifTracker=false;
			bool thenCount=false;
			bool stmtAftThen=false;
			bool gotoTrack=false;
			bool gosubTrack=false;
			bool opCheck=false;
			int nestedloopChk=0;
			auto begin_words = std::sregex_iterator(input_string.begin(), input_string.end(), regexPattern);
			auto end_words = std::sregex_iterator();
			for (std::sregex_iterator i = begin_words; i != end_words; ++i)
			{
				std::smatch match_found = *i;
				std::string matchString = match_found.str();
				const char * c = matchString.c_str();
				if(lineCount==0)
				{
					lineCount=1;
					std::map<string,int>::iterator it;
					it = line_Map.find(matchString);
					if (it != line_Map.end())
					{
						lineNo = it->second;
						continue;		
					}
				}
				if(lineCount==2)
				{
					found = true;
					bool test = isIdentifier(c);
					if(test)
					{			
						std::map<string,symbol*>::iterator it;
						it = Symbols_Map.find(matchString);
						if (it != Symbols_Map.end())
						{
							cout<<endl;
							cout<<"Variable Already defined::"<<matchString<<"::Exiting..."<<endl;
							return 1;
						}						
						insert(matchString,"INTEGER");
						intCount=intCount+1;
					}
					else if(matchString.at(0)==',')
					{
						commaCount=commaCount+1;
						//Do nothin
					}
					else
					{
						cout<<endl;
						cout<<"Invalid Integer expression with invalid identifiers. Exiting ...."<<matchString<<endl;
						return 0;
					}
				}
				if(lineCount==3)
				{
					bool test = isIdentifier(c);
					if(test && !found)
					{
						std::map<string,symbol*>::iterator it;
						it = Symbols_Map.find(matchString);
						if (it != Symbols_Map.end())
						{
							Newnode = it->second;
							found = true;
							continue;		
						}
						else
						{
							cout<<"Error in Expression . Variable not found :"<<matchString<<".Exiting..."<<endl;
							return 0;
						}						
					}
					if(matchString.at(0)=='=')
					{
						//DO NOTHING
					}
					else
					{
						if(Newnode && isIdentifier(c))
						{
							intCount=intCount+1;
							commaTracker=true;
							std::map<string,symbol*>::iterator it;
							it = Symbols_Map.find(matchString);
							if (it != Symbols_Map.end())
							{
								line = line + std::to_string(it->second->value)+' ';
							}
							else
							{
								cout<<"Error in Expression . Variable not found :"<<matchString<<".Exiting..."<<endl;
								return 0;
							}
						}
						else if(is_digits(matchString))
						{
							intCount=intCount+1;
							commaTracker=true;
							line = line+matchString+' ';
						}
						else if(contains_operator(matchString))
						{
							opCount=opCount+1;
							commaTracker=false;
							line = line+matchString+' ';
						}
						else if(matchString.at(0)=='(')
						{
							bracketTrack=bracketTrack+1;
						}
						else if(matchString.at(0)==')')
						{
							bracketTrack=bracketTrack-1;
						}
						else
						{
							cout<<"Invalid Let Expression . "<<matchString<<".Exiting ...";
							return 0;
						}
					}	
				}
				if(lineCount==4)
				{
					found = true;
					if(matchString.at(0)=='"')
					{
						char ch = matchString.back();
						if(ch!='"')
						{
							cout<<"Error in Println Expression . Exiting .....";
							return 0;
						}
						std::replace( matchString.begin(), matchString.end(), ',', '^');
						line=line+matchString;
						commaTracker=true;
					}
					else if(matchString.at(0)==',')
					{
						//Do nothing
						commaTracker=false;
						line = line+',';
					}
					else if(isIdentifier(c))
					{
						commaTracker=true;
						std::map<string,symbol*>::iterator it;
						it = Symbols_Map.find(matchString);
						if (it != Symbols_Map.end())
						{
							line = line + std::to_string(it->second->value)+' ';
							found=true;
						}
						else
						{
							cout<<"\n";
							cout<<"Error in expression . Variable not defined :"<<matchString<<".Exiting..."<<endl;
							return 0;
						}
					}
					else if(is_digits(matchString))
					{
						line = line+matchString+' ';
					}
					else if(contains_operator(matchString))
					{
						line=line+matchString+' ';
					}
					else if(matchString.at(0)=='(')
					{
						bracketTrack=bracketTrack+1;
					}
					else if(matchString.at(0)==')')
					{
						bracketTrack=bracketTrack-1;
					}
					else
					{
						cout<<"Invalid Print Expression. Exiting ...";
						return 0;
					}
				}
				if(lineCount==5)
				{
					found = true;
					if(matchString.at(0)=='"')
					{
						char ch = matchString.back();
						if(ch!='"')
						{
							cout<<"Error in Println Expression . Exiting .....";
							return 0;
						}
						std::replace( matchString.begin(), matchString.end(), ',', '^');
						line=line+matchString;
						commaTracker=true;
					}
					else if(matchString.at(0)==',')
					{
						//Do nothing
						commaTracker=false;
						line = line+',';
					}
					else if(isIdentifier(c))
					{
						commaTracker=true;
						std::map<string,symbol*>::iterator it;
						it = Symbols_Map.find(matchString);
						if (it != Symbols_Map.end())
						{
							line = line + std::to_string(it->second->value)+' ';
							found=true;
						}
						else
						{
							cout<<"Error in Print expression . Variable not defined :"<<matchString<<".Exiting..."<<endl;
							return 0;
						}
					}
					else if(is_digits(matchString))
					{
						line = line+matchString+' ';
					}
					else if(contains_operator(matchString))
					{
						line=line+matchString+' ';
					}
					else if(matchString.at(0)=='(')
					{
						bracketTrack=bracketTrack+1;
					}
					else if(matchString.at(0)==')')
					{
						bracketTrack=bracketTrack-1;
					}
					else
					{
						cout<<"Invalid Println Expression. Exiting ...";
						return 0;
					}					
				}
				if(lineCount==6)
				{
					cout<<"Invaild End Statement . Exiting ..."<<endl;
					return 0;
				}
				if(lineCount==7)
				{
					symbol* Newnode=0;
					bool test = isIdentifier(c);
					if(test)
					{
						line=line+matchString+' ';
						found = true;
						count=count+1;
						intCount=intCount+1;
					}
					else if(matchString.at(0)==',')
					{
						commaCount=commaCount+1;
						//Do nothing
					}
					else
					{
						cout<<endl;
						cout<<"Invalid keyword in Input Expression . "<<matchString<<" .EXITING ...."<<endl;
						return 0;
					}
				}
				if(lineCount==9)
				{
					ifTracker=true;
					if(thenCount)
					{
						stmtAftThen=true;
					}
					if(!strcmp(c,"then"))
					{
						nestedloopChk=nestedloopChk+1;
						thenCount=true;
						continue;
					}
					else if(!strcmp(c,"println"))
					{
						if(boolExp)
						{
							lineCount=5;
							continue;
						}
						else
						{
							break;
						}
					}
					else if(!strcmp(c,"print"))
					{
						if(boolExp)
						{
							lineCount=4;
							continue;
						}
						else
						{
							break;
						}
					}
					else if(!strcmp(c,"let"))
					{
						if(boolExp)
						{
							lineCount=3;
							continue;
						}
						else
						{
							break;
						}
					}
					else if(!strcmp(c,"goto"))
					{
						if(boolExp)
						{
							found=true;
							lineCount=8;
							gotoTrack=true;
							continue;
						}
						else
						{
							break;
						}
					}
					
					if(isOperator2(matchString.at(0)))
					{
						opCheck=true;
						if(matchString.at(0)=='>')
						{
							opVal = 1;
						}			
						else if(matchString.at(0)=='=')	
						{
							opVal = 2;				
						}			
						else if(matchString.at(0)=='<')
						{
							opVal = 3;
						}
					}
					else
					{
						bool num_chk = false;
						int int_token = 0;
						if(is_number(matchString))
						{
							num_chk = true;
							int_token = std::stoi (matchString);
							if(compare1==-1)
							{
								compare1 = int_token;
							}
							else if(compare2==-1)
							{
								compare2 = int_token;
								if(opVal == 1)
								{
									if(compare1 > compare2)
									{
										boolExp = true;
									}
									else
									{
										boolExp = false;
									}
								}
								if(opVal == 2)
								{
									if(compare1 == compare2)
									{
										boolExp = true;
									}
									else
									{
										boolExp = false;
									}
								}
								if(opVal == 3)
								{
									if(compare1 < compare2)
									{
										boolExp = true;
									}
									else
									{
										boolExp = false;
									}
								}
							}
						}
						else
						{
							symbol* ScanNode=0;
							int valInt=0;
							std::map<string,symbol*>::iterator it;
							it = Symbols_Map.find(matchString);
							if (it != Symbols_Map.end())
							{
								ScanNode = it->second;		
							}
							if(ScanNode)
							{
								if(compare1==-1)
								{
									compare1 = ScanNode->value;
								}
								else if(compare2==-1)
								{
									compare2 = ScanNode->value;
									if(opVal == 1)
									{
										if(compare1 > compare2)
										{
											boolExp = true;
										}
										else
										{
											boolExp = false;
										}
									}
									if(opVal == 2)
									{
										if(compare1 == compare2)
										{
											boolExp = true;
										}
										else
										{
											boolExp = false;
										}
									}
									if(opVal == 3)
									{
										if(compare1 < compare2)
										{
											boolExp = true;
										}
										else
										{
											boolExp = false;
										}
									}
								}
							}
							else
							{
								cout<<endl;
								cout<<"Error in Expression . Unrecognized symbol in IF expression :"<<matchString <<" Exiting ....";
								return 0;
							}
						}
					}	
				}
				if(lineCount==8)
				{
					found = true;
					gotoTrack=false;
					std::map<string,int>::iterator it;
					it = line_Map.find(matchString);
					if (it != line_Map.end())
					{
						lineNo = it->second;
						lineNumber=0;
						inputFile.seekg(0, ios::beg);
					}
					else
					{
						cout<<"Error in  GOTO expression . Label "<<matchString<<" does not exist .Exiting..."<<endl;
						return 0;
					}
				}
				if(lineCount==10)
				{
					found = true;
					gosubTrack=false;
					lineNumber=lineNumber+1;
					mystack.push(lineNumber);
					std::map<string,int>::iterator it;
					it = line_Map.find(matchString);
					
					if (it != line_Map.end())
					{
						lineNo = it->second;
						lineNumber=0;
						inputFile.seekg(0, ios::beg);
					}
					else
					{
						cout<<"Error in GOSUB expression . Label "<<matchString<<" does not exist .Exiting..."<<endl;
						return 0;
					}
				}
				if(lineCount==11)
				{
					found = true;
					lineNo=mystack.top();
					mystack.pop();
					lineNumber=0;
					inputFile.seekg(0, ios::beg);
				}
				if(lineCount==1)
				{
					if(!strcmp(c,"integer"))
					{
						lineCount=2;			
					}
					else if(!strcmp(c,"let"))
					{
						lineCount=3;				
					}
					else if(!strcmp(c,"print"))
					{
						lineCount=4;
					}
					else if(!strcmp(c,"println"))
					{
						lineCount=5;
					}
					else if(!strcmp(c,"end"))
					{
						if(gosubBool)
						{
							cout<<"Reached the end of the program without finding a RET after GOSUB . Exiting ..."<<endl;
							return 0;
						}
						lineCount=6;
						endTracker=false;
						return 1;
					}
					else if(!strcmp(c,"input"))
					{
						lineCount=7;
					}
					else if(!strcmp(c,"/"))
					{
						break;
					}
					else if(!strcmp(c,"if"))
					{
						lineCount=9;
					}
					else if(!strcmp(c,"goto"))
					{
						found=true;
						gotoTrack=true;
						lineCount=8;
					}
					else if(!strcmp(c,"gosub"))
					{
						found=true;
						gosubTrack=true;
						gosubBool=true;
						lineCount=10;
					}
					else if(!strcmp(c,"ret"))
					{
						gosubBool=false;
						found = true;
						if(mystack.empty()) 
						{
							cout<<"Incorrect number of ret statement . Exiting ..."<<endl;
							return 0;
						}
						lineNo=mystack.top();
						mystack.pop();
						lineNumber=0;
						inputFile.seekg(0, ios::beg);
					}
					else
					{
						cout<<"UNRECONGNIZED Command : "<<matchString<<". Exiting ...."<<endl;
						return 0;
					}
				}
			}
			if(ifTracker)
			{
				if(thenCount)
				{
					if(!stmtAftThen)
					{
						cout<<"Error in Expression . No Statement after then . Exiting ..."<<endl;
						return 0;
					}
				}
				else
				{
					cout<<"Error in Expression. No Then statement in IF . Exiting ..."<<endl;
					return 0;
				}
				if(!opCheck) 
				{
					cout<<"Error in Expression . No Operator in IF statement . Exiting ..."<<endl;
					return 0;
				}
				if(nestedloopChk!=1)
				{
					cout<<"Error in Expression . Nested If then not allowed . Exiting ..."<<endl;
					return 0;
				}
				if((compare1==-1) || (compare2==-1))
				{
					cout<<"Error in Expression . Exiting ..."<<endl;
					return 0;
				}
			}
			if(found)
			{
				if(lineCount==2)
				{
					if(commaCount!=(intCount-1))
					{
						cout<<endl;
						cout<<"Invalid integer expression . Exiting ...";
						return 0;
					}
				}
				if(lineCount==3)
				{
					if(bracketTrack!=0)
					{
						cout<<endl;
						cout<<"Invalid Let expression with wrong number of brackets. Exiting ...";
						return 0;
					}
					if(commaTracker==false)
					{
						cout<<endl;
						cout<<"Invalid Let expression . Exiting ....";
						return 0;
					}
					if(opCount!=(intCount-1))
					{
						cout<<endl;
						cout<<"Invalid Let expression . Exiting ...";
						return 0;
					}
					try
					{
					strcpy(ip, line.c_str());
					char * pch;
					pch = strtok (ip," ,.-");
					while (pch != NULL)
					{
	    				toks.push_back(pch);
	    				pch = strtok (NULL, " ");
	  				}
					evaluateInfix(toks,&vs,&os);
					while(!os.empty())
	  				{
	  					int op1=vs.top();
						vs.pop();
						int op2=vs.top();
						vs.pop();
						char oprtr=os.top();
						os.pop();
						vs.push(applyOperator(oprtr,op1,op2));	
	  				}
 
	  				while(!vs.empty())
	  				{
	  					Newnode->value=vs.top();
	  					vs.pop();
	  				}
	  				}
	  				catch(...)
	  				{
	  					cout<<"Invalid Let Expression. Exiting ..."<<endl;	
	  					return 0;
					}
				}
				else if(lineCount==4)
				{
					if(commaTracker==false||(bracketTrack!=0))
					{
						cout<<"Invalid Print Expression . Exiting ...."<<endl;
						return 0;
					}
					std::istringstream ss(line);
					std::string token;

					while(std::getline(ss, token, ',')) 
					{
						if(token[0]=='"')
						{
							char ch = token.back();
							if(ch!='"')
							{
								cout<<"Error in Print Expression . Exiting .....";
								return 0;
							}
							std::replace( token.begin(), token.end(), '^', ',');
							token.erase(token.begin()+0);
							size_t abc = token.length();
							token.erase(token.begin()+(abc-1));
							cout<<token;
						}
						else
						{
							if(contains_operator(token))
							{
								strcpy(ip, token.c_str());
								char * pch;
								pch = strtok (ip," ,.-");
								while (pch != NULL)
								{
	    							toks.push_back(pch);
	    							pch = strtok (NULL, " ");
	  							}
								evaluateInfix(toks,&vs,&os);
								while(!os.empty())
	  							{
	  								int op1=vs.top();
									vs.pop();
									int op2=vs.top();
									vs.pop();
									char oprtr=os.top();
									os.pop();
									vs.push(applyOperator(oprtr,op1,op2));	
	  							}
 
	  							while(!vs.empty())
	  							{
	  								cout<<vs.top();
	 			 					vs.pop();
	  							}
							}
							else if(is_number(token))
							{	
								cout<<token;
								//Nothing
							}
							else
							{
								symbol* ScanNode=0;
								int valInt=0;
								std::map<string,symbol*>::iterator it;
								it = Symbols_Map.find(token);
								if (it != Symbols_Map.end())
								{
									ScanNode = it->second;		
								}
								if(ScanNode)
								{
									cout<<ScanNode->value;
								}
								else
								{
									cout<<endl;
									cout<<"Unrecognized symbol in Print expression :"<<token <<" Exiting ....";
									return 0;
								}
							}
						}	
					}
				}
				else if(lineCount==5)
				{
					if(commaTracker==false||(bracketTrack!=0))
					{
						cout<<"Invalid Println Expression . Exiting ...."<<endl;
						return 0;
					}
					std::istringstream ss(line);
					std::string token;

					while(std::getline(ss, token, ',')) 
					{
						if(token[0]=='"')
						{
							char ch = token.back();
							if(ch!='"')
							{
								cout<<"Error in Println Expression . Exiting .....";
								return 0;
							}
							std::replace( token.begin(), token.end(), '^', ',');
							token.erase(token.begin()+0);
							size_t abc = token.length();
							token.erase(token.begin()+(abc-1));
							cout<<token;
						}
						else
						{
							if(contains_operator(token))
							{
								strcpy(ip, token.c_str());
								char * pch;
								pch = strtok (ip," ,.-");
								while (pch != NULL)
								{
	    							toks.push_back(pch);
	    							pch = strtok (NULL, " ");
	  							}
								evaluateInfix(toks,&vs,&os);
								while(!os.empty())
	  							{
	  								int op1=vs.top();
									vs.pop();
									int op2=vs.top();
									vs.pop();
									char oprtr=os.top();
									os.pop();
									vs.push(applyOperator(oprtr,op1,op2));	
	  							}
	  							while(!vs.empty())
	  							{
	  								cout<<vs.top();
	 			 					vs.pop();
	  							}
							}
							else if(is_number(token))
							{	
								cout<<token;
								//Nothing
							}
							else
							{
								symbol* ScanNode=0;
								int valInt=0;
								std::map<string,symbol*>::iterator it;
								it = Symbols_Map.find(token);
								if (it != Symbols_Map.end())
								{
									ScanNode = it->second;		
								}
								if(ScanNode)
								{
									cout<<ScanNode->value;
								}
								else
								{
									cout<<endl;
									cout<<"Unrecognized symbol in println :"<<token <<". Exiting ....";
									return 0;
								}
							}
						}	
					}
					cout<<endl;
				}
				else if(lineCount==7)
				{
					if(commaCount!=(intCount-1))
					{
						cout<<endl;
						cout<<"Invalid Input expression . Exiting ...";
						return 0;
					}
					strcpy(ip, line.c_str());
					char * pch;
					pch = strtok (ip," ,.-");
					while (pch != NULL)
					{
						while(count>0)
						{
							symbol* scanNode=0;
							int valInt=0;
							std::string Str = std::string(pch);
							std::map<string,symbol*>::iterator it;
							it = Symbols_Map.find(Str);
							if (it != Symbols_Map.end())
							{
								scanNode = it->second;		
							}
							if(scanNode)
							{
								std::cin >> valInt;
   								if(std::cin.fail())
   								{
   									cout<<"Please enter an integer value . Exiting ..."<<endl;
   									return 0;
								}
   								else
   								{
   									scanNode->value = valInt;
								}
							}				
							else
							{
								cout<<"Input Variable not defined yet : "<<Str<<endl;
								return 0;
							}
							pch = strtok (NULL, " ");
							count=count-1;
						}
	  				}
				}
				else if(lineCount==8)
				{
					if(gotoTrack)
					{
						cout<<"Error in Expression . No Statement after goto . Exiting ..."<<endl;
						return 0;
					}
				}
				else if(lineCount==10)
				{
					if(gosubTrack)
					{
						cout<<"Error in Expression . No Statement after gosub . Exiting ..."<<endl;
						return 0;
					}
				}
			}
		}
		inputFile.close();
	}
	else
	{
		cout << "Unable to open file \n";
	}
	return 1;
}


/* ***********************************************************************************************************************************************

Input file 1-
===========

10  println "Testing if then"
20  integer a, b
25  print "Enter a:"
30  input a
35  print "Enter b:"
40  input b
50  if a < b then println "a < b"
60  if a = b then println "a = b"
70  if a > b then println "a > b"
80  end

Output 1- 
========
Testing if then
Enter a:67
Enter b:32
a > b

Input File 2-
=============

10  println "If block test"
20  integer a, b
30  print "Enter a b:"
40  input a,b
50  if a < b then goto 100
60  println "in if block"
70  println "doing a second statement"
80  println "and a third"
100  println "out of the block"
200  end

Output 2 -
========

If block test
Enter a b:23 76
out of the block

--------------------------------
 
If block test
Enter a b:76 23
in if block
doing a second statement
and a third
out of the block


Input file 3-
===========

10  goto 100
20  println "line 20"
30  goto 120
40  println "line 40"
50  goto 90
60  println "line 60"
70  println "line 70"
80  goto 20
90  goto 70
100  println "line 100"
110  goto 40
120  end

Output 3- 
========
line 100
line 40
line 70
line 20

Input file 4-
===========

10  println "line 10"
20  gosub 200
30  println "line 30"
40  end
200  println "in the sub"
210  gosub 300
220  println "back from 300"
230  ret
300  println "start of 300 sub"
310  ret
320  end

Output 4- 
========
line 10
in the sub
start of 300 sub
back from 300
line 30


Input file 5-
===========

10  integer c, d
20  let c = 1
25  let d = 1
30  if c > 10 then goto 70
40  println c, "*", d, "=", c*d
50  let d = d + 1
55  if d > 10 then let c = c + 1
57  if d > 10 then let d = 1
60  goto 30
70  end

Output 5- 
========
1 *1 =1
1 *2 =2
1 *3 =3
1 *4 =4
1 *5 =5
1 *6 =6
1 *7 =7
1 *8 =8
1 *9 =9
1 *10 =10
2 *1 =2
2 *2 =4
2 *3 =6
2 *4 =8
2 *5 =10
2 *6 =12
2 *7 =14
2 *8 =16
2 *9 =18
2 *10 =20
3 *1 =3
3 *2 =6
3 *3 =9
3 *4 =12
3 *5 =15
3 *6 =18
3 *7 =21
3 *8 =24
3 *9 =27
3 *10 =30
4 *1 =4
4 *2 =8
4 *3 =12
4 *4 =16
4 *5 =20
4 *6 =24
4 *7 =28
4 *8 =32
4 *9 =36
4 *10 =40
5 *1 =5
5 *2 =10
5 *3 =15
5 *4 =20
5 *5 =25
5 *6 =30
5 *7 =35
5 *8 =40
5 *9 =45
5 *10 =50
6 *1 =6
6 *2 =12
6 *3 =18
6 *4 =24
6 *5 =30
6 *6 =36
6 *7 =42
6 *8 =48
6 *9 =54
6 *10 =60
7 *1 =7
7 *2 =14
7 *3 =21
7 *4 =28
7 *5 =35
7 *6 =42
7 *7 =49
7 *8 =56
7 *9 =63
7 *10 =70
8 *1 =8
8 *2 =16
8 *3 =24
8 *4 =32
8 *5 =40
8 *6 =48
8 *7 =56
8 *8 =64
8 *9 =72
8 *10 =80
9 *1 =9
9 *2 =18
9 *3 =27
9 *4 =36
9 *5 =45
9 *6 =54
9 *7 =63
9 *8 =72
9 *9 =81
9 *10 =90
10 *1 =10
10 *2 =20
10 *3 =30
10 *4 =40
10 *5 =50
10 *6 =60
10 *7 =70
10 *8 =80
10 *9 =90
10 *10 =100



*/
