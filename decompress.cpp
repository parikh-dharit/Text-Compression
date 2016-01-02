#include <unordered_map>
#include <iostream>
#include <string>
#include <fstream>
#include<cstdio>
using namespace std;

struct treeNode
{
    int freq;
    char Ch;
    treeNode *left;
    treeNode *right;
    treeNode()                      //Constructor
    {
        freq=0;
        left=NULL;
        right=NULL;
    }
    treeNode(int frequency, char character)                      //Constructor
    {
        freq=frequency;
        Ch = character;
        left=NULL;
        right=NULL;
    }
};

void dmTree(char c, string s, treeNode *&node)
{
	
	if(node==NULL)
	{
		node = new treeNode;
	}
	if(s[0]=='0')
	{
		dmTree(c,s.substr(1),node->left);
	}
	else if(s[0]=='1')	
	{
		dmTree(c,s.substr(1),node->right);
	}
	else
	{
		node->Ch=c;
		node->freq = 1;
		return;	
	}
	return;
}

treeNode *droot = NULL;
treeNode *dcurr = NULL;

void findValue(bool b)
{
	ofstream myfile;
	myfile.open("decompress.txt", std::ofstream::out | std::ofstream::app);
	if(b)
	{
		dcurr=dcurr->right;	
	}
	else
	{
		dcurr=dcurr->left;
	}
	if(dcurr->freq==1)
	{
		myfile<<dcurr->Ch;
		dcurr=droot;
	}
	return;
}

void decode(string path)
{
	unordered_map<char, string > lTable;
	ifstream inMyStream (path);
	string buff, hCode;
	bool brk = false, started=false, codeStarted, bval;
	int cnt = 0, flag = 0;
	char letter,ch;

	if (inMyStream.is_open())
	{
		while (!inMyStream.eof() )
		{
			getline (inMyStream, buff);
			for( int n=0; n < buff.length(); ++n )
			{
				letter = char(buff[n]);
				if(brk != true)
				{
					if(letter == '|')
					{
						if(started)
						{
							lTable.emplace(ch,hCode);
						}
						ch=char(buff[n+1]);
						n++;
						codeStarted=true;
						if(started==false)
						{
							started=true;
						}
					}
					else if(letter == '~')
					{
						lTable.emplace(ch,hCode);
						brk = true;
						for(auto iter=lTable.begin(); iter!=lTable.end(); ++iter )
						{
							dmTree(iter->first,iter->second,droot);
						}
						dcurr=droot;
					}
					else
					{
						if(codeStarted)
						{
							codeStarted=false;
							hCode=letter;
						}
						else
							hCode = hCode + letter;
					}
				}
				else
				{
					for(int ct1=0; ct1<8; ct1++)
					{
						bval=letter&(00000001<<ct1);
						findValue(bval);
					}
				}
			}
		}
	}
}

int main()
{
	string filepath;
	
	cout<<"Enter valid file path of text file to be decompressed:";
	cin>>filepath;
	
	decode(filepath);
return 0;
}
