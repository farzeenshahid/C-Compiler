#pragma once
#include<iostream>
#include<stdio.h>
#include"LinkedList.h"


using namespace std;

class Token
{
private:
	LinkedList* list = new LinkedList();
	string keyword[25] = { "if","else","for","void","return",
		"break","continue","true","false","class","enum","virtual","namespace"
		"this","static","new","main"};
	
	string DT[5] = { "int","float","char","string","bool" };
	string AM[3] = {"public","private","protected"};
	

	
public:
	void generateToken(string class_part, string value_part, int line_no) 
	{
		if (value_part != "")
			list->append(class_part, value_part, line_no);
	}
	string getprev() {
		return list->tail->class_part;
	}
	void generateToken(string value_part, int line_no)
	{
		regex isalphabet("[_A-Za-z]");
		regex floatrgx("[+-]{0,1}[0-9]*[\\.][0-9]+");
		regex charConst("\\'([\\\\][torbn])\\'");
		regex char1Const("\\'\.?\\'");
		regex digit("[+-]{0,1}[0-9]+");
		regex isID("([_a-zA-Z][_A-Za-z0-9]*[A-Za-z0-9])|[A-Za-z]");
		regex sttring("\"([a-zA-Z0-9]?([\\\\][torbna0\"\'\\\\])?|[^a-zA-Z0-9\\\\])*\"");

		if (value_part != "")
		{
			if (regex_match(string(1, value_part[0]), isalphabet))		//if alphabet or ID
			{
				if (regex_match(value_part, isID))
				{
					string CP = isKeyWord(value_part);
					if (CP == "")
					{
						if (isDT(value_part))
						{
							list->append("DT", value_part, line_no);
						}
						else if (isAM(value_part))  // Check for Access Modifiers
						{
							list->append("AM", value_part, line_no);
						}
						// else if (isMain(value_part))
						// {
						// 	list->append("ID", value_part, line_no);
						// }
						else
						{
							list->append("ID", value_part, line_no);
						}
					}
					else
					{
							list->append(CP, " _ ", line_no);
					}
				}
				else
				{
					list->append("invalid_lexene", value_part, line_no);
				}
			}
			else if (regex_match(value_part, sttring))			//String constant?
			{
					list->append("string_const", value_part.substr(1,value_part.size()-2), line_no);
			}
			else if (regex_match(value_part, charConst) || regex_match(value_part, char1Const))			//char constant
			{
				list->append("char_const", value_part.substr(1,value_part.size()-2), line_no);
			}
			else if (regex_match(value_part, digit))	//if digit
			{
				list->append("int_const", value_part, line_no);
			}
			else if (regex_match(value_part, floatrgx))		//if float
			{
				list->append("float_const", value_part, line_no);
			}
			else if (value_part == ".")					//if dot
			{
				list->append(value_part," _ ", line_no);
			}
			else
			{
				list->append("invalid_lexene", value_part, line_no);
			}	
		}
	}

	string isKeyWord(string word) 
	{
		for (int i = 0; i < sizeof(keyword)/sizeof(keyword[0]); i++)
		{
			if (word == keyword[i]) 
			{
				return keyword[i];
			}
		}
		return "";
	}

	bool isDT(string word)
	{
		for (int i = 0; i < sizeof(DT) / sizeof(DT[0]); i++)
		{
			if (word == DT[i])
			{
				return true;
			}
		}
		return false;
	}
	bool isAM(string word)
	{
		for (int i = 0; i < sizeof(AM) / sizeof(AM[0]); i++)
		{
			if (word == AM[i])
			{
				return true;
			}
		}
		return false;
	}
	// bool isMain(string word) {
	// 	if (word == "main") {
	// 		// Check if it doesn't have any characters after "main"
	// 		// You can use a regular expression to check for this condition
	// 		regex noArgs("main\\s*\\(\\s*\\)");

	// 		if (regex_match(word, noArgs)) {
	// 			return true;
	// 		}
	// 	}

	// 	return false;
	// }



	void printList()
	{
		list->print();
	}

	LinkedList* tokenPointer()
	{
		return list;
	}
};
