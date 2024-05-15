#pragma once

#include<iostream>
#include<stdio.h>
#include<vector>
#include "ClassTable.h"

using namespace std;
struct MemberInfo {
    string name;
    string type;
    string access_modifier;
    string type_modifier;
};
class MemberNode
{
public:
	string name;
	string type;
	string access_modifier;
	string type_modifier;
    string para_list;
	
public:
	MemberNode(string name, string type, string access_modifier, string type_modifier)
	{
		this->name = name;
		this->type = type;
		this->access_modifier = access_modifier;
		this->type_modifier = type_modifier;
		
	}
	MemberNode(string name, string type, string access_modifier, string type_modifier, string para_list)
	{
		this->name = name;
		this->type = type;
		this->access_modifier = access_modifier;
		this->type_modifier = type_modifier;
		this->para_list = para_list;
	}

};
class MemberTable
{
	vector<MemberNode> member_table;


public:
	bool insertMT(string name, string type, string access_modifier, string type_modifier)
	{
		MemberNode* node = new MemberNode(name, type, access_modifier, type_modifier);
		if (member_table.empty())
		{
			member_table.push_back(*node);
			
			return true;
		}
		for (auto i = member_table.begin(); i != member_table.end(); ++i)
		{
			if (contains(type, "-"))
			{
				if (i->name == node->name && i->type.substr(0, i->type.find("-")) == type.substr(0, type.find("-")))
				{
					return false;
				}
			}
			else
			{
				if (i->name == node->name)
				{
					return false;
				}
			}
		}
		member_table.push_back(*node);
		
		return true;
	}

	MemberInfo lookUpMT(string name, string className)
	{
		MemberInfo result;  // Create an instance of MemberInfo to hold the result

        for (auto i = member_table.begin(); i != member_table.end(); ++i)
        {
            if (i->name == name && i->access_modifier != "private")
            {
                // Set the values in the result struct
                result.name = i->name;
                result.type = i->type;
                result.access_modifier = i->access_modifier;
                result.type_modifier = i->type_modifier;

                // Return the result
                return result;
            }
        }

        // If not found, set the type to "NULL" and return
        result.type = "NULL";
        return result;
	}
    MemberInfo Lookup_Fn_MT(string name, string para_list, string className){
		MemberInfo result;
        for (auto i = member_table.begin(); i != member_table.end(); ++i)
		{
			if (i->name == name && i->access_modifier != "private" && i->para_list == para_list)
			{
				result.name = i->name;
                result.type = i->type;
                result.access_modifier = i->access_modifier;
                result.type_modifier = i->type_modifier;

                // Return the result
                return result;
			}
		}
		result.type = "NULL";
        return result;
    }
	void printMemberTable(string memberName)
	{
		cout << "-------printing Member Table of : " << memberName << "--------" << endl;
		for (auto i = member_table.begin(); i != member_table.end(); ++i)
		{
			cout << i->name << "\t\t\t" << i->type << "\t\t\t" << i->access_modifier << "\t\t\t" << i->type_modifier << endl;
		}
	}

private:
	bool contains(const std::string& str, const std::string substr)
	{
		if (str.size() < substr.size()) return false;

		for (int i = 0; i < str.size(); i++)
		{
			if (str.size() - i < substr.size()) return false;

			bool match = true;
			for (int j = 0; j < substr.size(); j++)
			{
				if (str.at(i + j) != substr.at(j))
				{
					match = false;
					break;
				}
			}
			if (match) return true;
		}
		return false;
	}
};
