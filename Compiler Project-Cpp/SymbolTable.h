#pragma once
#include <string>
#include<iostream>
#include<stack>
#include <vector>

using namespace std;
static int stackSize = 0;
class SNode
{
public:
	string name;
	string type;
	int scope;
    string para_list;
    
	SNode(string name, string type, int scope)
	{
		this->name = name;
		this->type = type;
		this->scope = scope;
	}
    SNode(string name, string type, int scope, string para_list)
	{
		this->name = name;
		this->type = type;
		this->scope = scope;
        this->para_list = para_list;
	}
};

class SymbolTable
{
	vector<SNode> symboltable;

public:
    stack<int> stack_scope;
	bool insertST(string name, string type, int scope)
	{
		SNode* node = new SNode(name, type, scope);
		if (symboltable.empty())
		{
			symboltable.push_back(*node);
			return true;
		}
		for (auto i = symboltable.begin(); i != symboltable.end(); ++i)
		{
			if (i->name == name && i->scope == scope)
			{
				return false;
			}
		}
		symboltable.push_back(*node);
		return true;
	}

	string lookUpST(string name, int scope)
	{
		while (!stack_scope.empty())
		{
			for (auto i = symboltable.begin(); i != symboltable.end(); ++i)
			{
				if (i->name == name && i->scope == scope)
				{
					return i->type;
				}
			}
			scope = stack_scope.top();
			stack_scope.pop();
		}
		return "NULL";
	}
    string lookUp_Fn_ST(string name, int scope, stack<int> stack_scope, string para_list)
	{
		while (!stack_scope.empty())
		{
			for (auto i = symboltable.begin(); i != symboltable.end(); ++i)
			{
				if (i->name == name && i->scope == scope && i->para_list == para_list)
				{
					return i->type;
				}
			}
			scope = stack_scope.top();
			stack_scope.pop();
		}
		return "NULL";
	}
    int createScope()
    {
        stack_scope.push(stackSize);
        stackSize++;
        return stackSize-1;
    }
    int deleteScope(){
        stack_scope.pop();
        if(!stack_scope.empty())
        {
            return (stack_scope.top());
        }
        else
        {
            return -1;
        }
    }
	void printSymbolTable()
	{
		cout << "-------printing Symbol Table: " << endl;
		if (symboltable.empty())
		{
			cout << "Symbol Table is empty." << endl;
		}
		else
		{
			cout << "Name\tType\tScope" << endl;
			for (auto i = symboltable.begin(); i != symboltable.end(); ++i)
			{
				cout << i->name << "\t" << i->type << "\t" << i->scope << endl;
			}
		}
	}
};
