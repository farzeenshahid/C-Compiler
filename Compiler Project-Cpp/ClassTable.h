#pragma once
#include <string>
#include<iostream>
#include "MemberTable.h"

#include<stack>

using namespace std;
struct ClassInfo {
    string name;
    string type;
    string access_modifier;
    string category;
    string parent;
    MemberTable* member_link;
    SymbolTable* symbol_link;
};

class CNode
{
public:
	string name;
	string type;
	string access_modifier;
	string category;
	string parent;
	MemberTable* member_link;
	SymbolTable* symbol_link;
	CNode* next;

public:
	CNode(string name, string type, string access_modifier, string category, string parent)
	{
		member_link = new MemberTable();
		this->name = name;
		this->type = type;
		this->access_modifier = access_modifier;
		this->category = category;
		this->parent = parent;
		this->next = NULL;
	}
};
class ClassTable
{
public:
	CNode* head;
	ClassTable()
	{
		head = NULL;
	}

	bool insertCT(string name, string type, string access_modifier, string category,string parent, MemberTable *member_link, SymbolTable *symbol_link)
	{
		CNode* node = new CNode(name, type, access_modifier, category, parent);
		if (head == NULL)
		{
			node->member_link = member_link;
			node->symbol_link = symbol_link;
			head = node;
		}
		else
		{
			CNode* thead = head;
			while (thead->next != NULL)
			{
				if (thead->name == name)
				{
					return false;
				}
				thead = thead->next;
			}
			if (thead->name == name)
			{
				return false;
			}
			node->member_link = member_link;
			node->symbol_link = symbol_link;
			thead->next = node;
		}
		//printMainTable();
		return true;
	}
	ClassInfo lookupCT(string name)
    {
        ClassInfo result;  // Create an instance of ClassInfo to hold the result

        CNode* thead = head;
        while (thead != NULL)
        {
            if (thead->name == name)
            {
                // Set the values in the result struct
                result.name = thead->name;
                result.type = thead->type;
                result.access_modifier = thead->access_modifier;
                result.category = thead->category;
                result.parent = thead->parent;
                result.member_link = thead->member_link;
                result.symbol_link = thead->symbol_link;

                // Return the result
                return result;
            }
            thead = thead->next;
        }

        // If not found, set the type to "NULL" and return
        result.type = "NULL";
        return result;
    }

	MemberTable* findMemberTable(string name)
	{
		CNode* thead = head;
		while (thead != NULL)
		{
			if (thead->name == name)
			{
				break;
			}
			thead = thead->next;
		}
		return thead->member_link;
	}

	SymbolTable* findSymbolTable(string name)
	{
		CNode* thead = head;
		while (thead != NULL)
		{
			if (thead->name == name)
			{
				break;
			}
			thead = thead->next;
		}
		return thead->symbol_link;
	}

	

	void printClassTable(string className)
	{
		CNode* troot = head;
		while (troot != NULL)
		{
			cout << "-------printing Class Table of Class: " << className << "--------" << endl;
			cout << "[ " << troot->name << ", " << troot->type << ", " << troot->access_modifier << ", " << troot->category << ", " << troot->parent << ", " << &troot->member_link <<  " ]" << endl;
			troot = troot->next;
		}
		troot = NULL;
		delete troot;
	}	
};
