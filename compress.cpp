#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<cstdio>
using namespace std;

vector<int> freq;
vector<char> ch;

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

treeNode *root = NULL;
unordered_map<char,vector<int> > table;

void createSubtree(treeNode *&firstNode, treeNode *&secondNode, treeNode *&tempRoot)
{
    tempRoot->freq = firstNode->freq + secondNode->freq;
    tempRoot->left = firstNode;
    tempRoot->right = secondNode;
    return;
}

void printArray(treeNode* node, int ints[], int len)
{
	int i;
	vector<int> code;
	for (i=1; i<len; i++)
	{
		code.push_back(ints[i]);
	}
	table.emplace(node->Ch,code);
}

void printPathsRecur( treeNode* node, int path[], int pathLen, bool code)
{
	if (node==NULL) return;

	path[pathLen] = code;
	pathLen++;

	if (node->left==NULL && node->right==NULL)
	{
		printArray(node,path, pathLen);
	}
	else
	{
		printPathsRecur(node->left, path, pathLen,0);
		printPathsRecur(node->right, path, pathLen,1);
	}
}

void dispPath(treeNode *node, int len)
{
	int path[len];
	printPathsRecur(node, path, 0, 0);
}

void sort(int len)
{
	int swapint,swapchar;
	for( int i=0; i< len; i++ )
	{
		for( int j=i+1; j<len; j++ )
		{
			if( freq[i] > freq[j] )
			{
				swapint = freq[j];
				swapchar = ch[j];
				freq[j] = freq[i];
				ch[j] = ch[i];
				freq[i] = swapint;
				ch[i] = swapchar;
			}
		}
	}
}

void makeTree(int len)
{
	treeNode *nodes[len],*nNode[len-1];
	sort(len);
	for(int i=0; i<len ; i++)
    	{
        	nodes[i] = new treeNode(freq[i],ch[i]);
    	}
    	 	
    	treeNode *emptyNode = new treeNode;
    	
    	for(int i=0; i<len-1 ; i++)
    	{
        	nNode[i] = new treeNode(0,'\r');
        	createSubtree(nodes[i], nodes[i+1], nNode[i]);
        	nodes[i] = nNode[i];
        	nodes[i+1] = emptyNode;
        	
        	treeNode *swapNode = NULL;
    		swapNode = new treeNode;
		for( int i=0; i< len; i++ )
		{
			for( int j=i+1; j<len; j++ )
			{
				if( nodes[i]->freq > nodes[j]->freq )
				{
					swapNode = nodes[j];
					nodes[j] = nodes[i];
					nodes[i] = swapNode;
				}
			}
		}
    	}
    	
    	root = nodes[len-1];
    	dispPath(root,len);
}

int printTable(string fileName)
{
	ifstream inMyStream (fileName); //open my file stream
	ofstream myfile;
	if (inMyStream.is_open())
	{
		myfile.open("compressed.hfc");
		for(unordered_map<char,vector<int> > ::const_iterator iter = table.begin(); iter != table.end(); ++iter)
		{
			myfile<<"|"<<iter->first;
			for(vector<int> :: const_iterator iter1 = iter->second.begin(); iter1 != iter->second.end(); ++iter1)
			{
				myfile<<*iter1;
			}
		}
		myfile<<"~";
		bool bval;
		char c, inp=0b00000000;
		int anCounter=0;
		string buff;
		while (!inMyStream.eof() )
		{
			getline (inMyStream, buff);
			for( int n=0; n < buff.length(); ++n )
			{
				c = char( buff[n] ); //get a letter
				for(vector<int> :: const_iterator iter1 = table[c].begin(); iter1 != table[c].end(); ++iter1)
				{	
			  		if(*iter1)
			  		{
			  			inp=(0b00000001<<anCounter)|inp;
			  		}
			  		else
			  		{
			  			inp=(~(0b00000001<<anCounter))&inp;
			  		}
			  		anCounter++;
			  		if(anCounter==8)
			  		{
			  			myfile<<inp;
			  			anCounter=0;
						inp=0b00000000;
			  		}			
			  	}
			  }
	  	}
	  	if(anCounter!=0)
		{
		 	myfile<<inp;
		  	anCounter=0;
		  	inp=0b00000000;
		}
	  	inMyStream.close(); //close the file stream
		myfile.close();
	return 0;
	}
	else
		return 1;
}

int main (int argc, char *argv[]) 
{
	int len;
	string buff,filepath;
	
	cout<<"Enter valid file path of text file to be compressed:";
	cin>>filepath;

	ifstream inMyStream (filepath); //open my file stream

	if (inMyStream.is_open())
	{
		//create an array to hold the letter counts
		int upperCaseCount[26] = {0};
		int lowerCaseCount[26] = {0};
		int startCharCount[65] = {0};
   		int middleCharCount[6] = {0};
   		int endCharCount[4] = {0};
		//read the text file
		while (!inMyStream.eof() )
		{
			//get a line of text
			getline (inMyStream, buff);

			//read through each letter in the buff
			char oneLetter;
			for( int n=0; n < buff.length(); ++n )
			{
				oneLetter = char( buff[n] ); //get a letter
				if (oneLetter >= 'A' && oneLetter <='Z') 
				{ 
					//decide if it is a capital letter
					upperCaseCount[int(oneLetter)- 65]++; //make the index match the count array
				}
				else if (oneLetter >= 'a' && oneLetter <='z') 
				{ 
					//decide if it is a small letter
					lowerCaseCount[int(oneLetter)- 97]++; //make the index match the count array
				}
				else if (oneLetter >= '\0' && oneLetter <='@') 
				{ 
					//decide if it is a capital letter
					startCharCount[int(oneLetter)- 0]++; //make the index match the count array
				}
				else if (oneLetter >= '[' && oneLetter <='`') 
				{ 
					//decide if it is a capital letter
					middleCharCount[int(oneLetter)- 91]++; //make the index match the count array
				}
				else if (oneLetter >= '{' && oneLetter <='~') 
				{ 
					//decide if it is a capital letter
					endCharCount[int(oneLetter)- 123]++; //make the index match the count array
				}
			}
		}

		inMyStream.close(); //close the file stream

		//display only those with some count value
		for (int i= 0; i < 26; i++)
		{
			if(upperCaseCount[i] != 0)
			{	
				ch.push_back(char(i+65));
				freq.push_back(upperCaseCount[i]);
			}
		}
		for (int i= 0; i < 26; i++)
		{
			if(lowerCaseCount[i] != 0)
			{
				ch.push_back(char(i+97));
				freq.push_back(lowerCaseCount[i]);
			}
		}
		for (int i= 0; i < 65; i++)
		{
			if(startCharCount[i] != 0)
			{
				ch.push_back(char(i+0));
				freq.push_back(startCharCount[i]);
			}
		}
		for (int i= 0; i < 6; i++)
		{
			if(middleCharCount[i] != 0)
			{
				ch.push_back(char(i+91));
				freq.push_back(middleCharCount[i]);
			}
		}
		for (int i= 0; i < 4; i++)
		{
			if(endCharCount[i] != 0)
			{
				ch.push_back(char(i+123));
				freq.push_back(endCharCount[i]);
			}
		}
	
		len = ch.size();
		makeTree(len);
		if(printTable(filepath)==1)
			cout<<"error opening file";
		
	}
	else
	{
		cout << "File Error: Open Failed";
	}
	return 0;
}
