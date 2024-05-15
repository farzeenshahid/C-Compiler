#pragma once
#include "LinkedList.h"
#include "SymbolTable.h"
#include "ClassTable.h"
#include <stack>
#include "MemberTable.h"
class SyntaxAnalyzer
{
    stack<string> my_stack;
	string class_name = "empty";
	string class_type;
	string class_access_modifier = "empty";
    string type_modifier;
	string class_parent;
	string class_category;
	ClassTable* classtable = new ClassTable();
    
    MemberInfo RMT;
    ClassInfo RCT;
	string global_type;

	int current_scope = -1;
	MemberTable* membertable = new MemberTable();
	SymbolTable* symboltable = new SymbolTable();
	string name;
	string TYPE;
    string T_E;
    string para_list;
    string ret_type;
	string CR;
    string CCR;
    string member_name;
	LinkedList* t;
public:
	SyntaxAnalyzer(LinkedList* tokens)
	{
		this->t = tokens;
		checkSyntactically();
	}
	void checkSyntactically() {
		if (S()) {
			if(t->getClassPart() == "$")
				cout << "All correct, No syntax error" << endl;
            // else
            //     cout << "Syntax Error at Line No: " << t->getLineno() << "   " << t->getClassPart() << endl;
		}
		else
		{
			cout << "Syntax Error at Line No: " << t->getLineno() << "   " << t->getClassPart() <<"  " << t->getValuePart() << endl;
		}
        //classtable->printClassTable(class_name);
        symboltable->printSymbolTable();
        membertable->printMemberTable(member_name);
	}
    bool S(){
		if(t->getClassPart() == "class" ||  t->getClassPart() == "void" || t->getClassPart() == "enum" || t->getClassPart() == "DT"){
			if (def()) {
                {
					if (t->getClassPart() == "main") {
                        
						t->next(); // Move past 'main'
						if (t->getClassPart() == "(") {
                            current_scope = symboltable->createScope();
							t->next(); // Move past '('
                            if(PL()){
                                if (t->getClassPart() == ")") {
                                    t->next(); // Move past ')'
                                    if (body()) {
                                        if (def()) {
                                            return true;
                                        }
                                    }
							    }
                            }
							
						}
					}
				}
			} else if(t->getClassPart() == "main"){
            t->next(); // Move past 'main'
            if (t->getClassPart() == "(") {
                current_scope = symboltable->createScope();
                t->next(); // Move past '('
                if(PL()){
                    if (t->getClassPart() == ")") {
                        t->next(); // Move past ')'
                        if (body()) {
                            if (def()) {
                                return true;
                            }
                        }
                    }
                }
                
            }
        }

		}  
        
		return false;
	}
    bool def(){
        if(t->getClassPart() == "DT"){
            if(Dec()){
                if(def()){
                    return true;
                }
                
            }
        } else if(t->getClassPart() == "class"){
            if(class_st()){
                if(def()){
                    return true;
                }
            }
        } else if(t->getClassPart() == "enum"){
            if(enum_()){
                if(def()){
                    return true;
                }
            }
        } else if(t->getClassPart() == "void"){
            T_E = "void";
            t->next();
            if(Mfunc()){
                T_E += "->" + para_list; 
                if(def()){
                    return true;
                }
            }
        } else if(t->getValuePart() == "int" || t->getClassPart() == "$"){
            return true;
        }
        return false;
        

    }
    bool Dec(){
        if(t->getClassPart() == "DT"){
            TYPE = t->getValuePart();
            t->next();
            if(t->getClassPart() == "ID"){
                name = t->getValuePart();
                if(symboltable->insertST(name,TYPE, current_scope))
                {

                }
                else
                {
                    cout << "Variable Redeclration Error at Line no: " << t->getLineno() << endl;
                }
                t->next();
                if(Dec_choice()){
                    return true;
                }
            }
        } 
        return false;
        
    }
    bool Dec_choice(){
        if(t->getClassPart() == "assignment_op" || t->getClassPart() == ";" || t->getClassPart() == ","){
            if(init()){
                if(list()){
                    return true;
                }
            }
        } else if(t->getClassPart() == "["){
            if(arr1()){
                return true;
            }
        } else if(t->getClassPart() == "("){
            if(fn_dec()){
                return true;
            }
        }
        return false;
    }
    bool init(){
		if (t->getClassPart() == "assignment_op") {
            if(!symboltable->insertST(name,TYPE,current_scope))
            {
                cout<<"Redeclaration error at line no.: "<<t->getLineno()<<endl;
            }
			t->next();
            string T4 = "";
			if (OE(T4)) {
                string TR = compatibilityCheck(TYPE, T4 , "=");
				if (TR == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
				return true;
			}
		}
		else if (t->getClassPart() == ";" || t->getClassPart() == ",") {
			return true;
		}
		return false;
	}
    bool list()
	{
		if (t->getClassPart() == ";")
		{
			t->next();
			return true;
		}
		else if (t->getClassPart() == ",")
		{
			t->next();
			if (t->getClassPart() == "ID")
			{
                name = t->getValuePart();
                if(symboltable->insertST(name,TYPE, current_scope))
                {

                }
                else
                {
                    cout << "Variable Redeclration Error at Line no: " << t->getLineno() << endl;
                }
				t->next();
				if (init())
				{
					if (list())
					{
						return true;
					}
				}
			}
		}
		return false;
	}
    bool PL() {
		if (t->getClassPart() == "DT") {
            TYPE = t->getValuePart();
            para_list += TYPE;
			t->next(); // Move past 'DT'
			if (t->getClassPart() == "ID") {
                name = t->getValuePart();
                if(symboltable->insertST(name,TYPE,current_scope))
                {

                }
                else 
                {
                    cout << "Variable Redeclration Error at Line no: " << t->getLineno() << endl;
                }
				t->next(); // Move past 'ID'
				if (PL1()) {
					return true;
				}
			}
		}

		// Check if the current token is in the Follow set of <param_list>
		else if (t->getClassPart() == ")") {
            para_list = "void";
			return true; // Empty production (ε)
		}

		return false; // If the production is not matched and not in the Follow set of <param_list>
	}
    bool PL1() {
		if (t->getClassPart() == "assignment_op") {
			t->next(); // Move past '='
            string T4 = "";
			if (OE(T4)) {
                string TR = compatibilityCheck(T4,TYPE,"=");
                if (TR == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
				if (PL2()) {
					return true;
				}
			}
		}

		// If the '=' production is not matched or is not present, try <P2>
		else if (t->getClassPart() == ",") {
			if(PL2()){
				return true;
			}
			
		} else if (t->getClassPart() == ")"){
            TYPE = "-";
            return true;
        }

		return false; // If neither production is matched
	}
    bool PL2() {
		if (t->getClassPart() == ",") {
			t->next(); // Move past ','
			if (t->getClassPart() == "DT") {
                TYPE = t->getValuePart();
                para_list += "," + TYPE;
				t->next(); // Move past 'DT'
				if (t->getClassPart() == "ID") {
                    name = t->getValuePart();
                    if(symboltable->insertST(name, TYPE,current_scope))
                    {

                    }
                    else
                    {
                        cout << "Variable Redeclration Error at Line no: " << t->getLineno() << endl;
                    }
					t->next(); // Move past 'ID'
					if (PL1()) {
						return true;
					}
				}
			}
		}

		// Check if the current token is in the Follow set of <P2>
		if (t->getClassPart() == ")") {
			return true; // Empty production (ε)
		}

		return false; // If the current token is not in the Follow set of <P2> or if the production is not matched
	}
    bool MST()
	{
		if (t->getClassPart() == "if" || t->getClassPart() == "for" ||  t->getClassPart() == "return" || t->getClassPart() == "DT" ||
			t->getClassPart() == "ID" || t->getClassPart() == "continue" || t->getClassPart() == "break"|| t->getClassPart() == "enum")
		{
			if (SST())
			{
				if (MST())
				{
					return true;
				}
			}
		}
		else if (t->getClassPart() == "}")
		{
			return true;
		}
		return false;
	}
    bool SST(){
        name = "";
        TYPE = "";
        if(t->getClassPart() == "if"){
            if(if_else()){
                return true;
            }
        } else if (t->getClassPart() == "for"){
            if(for_st()){
                return true;
            }
        } else if (t->getClassPart() == "return"){
            if(return_st()){
                return true;
            }
        } else if (t->getClassPart() == "continue"){
            t->next();
            if(t->getClassPart() == ";"){
                t->next();
                return true;
            }
        } else if (t->getClassPart() == "break"){
            t->next();
            if(t->getClassPart() == ";"){
                t->next();
                return true;
            }
            
        } else if (t->getClassPart() == "this" || t->getClassPart() == "ID"){
            if(this_st()){
                if(t->getClassPart() == "ID"){
                     name = t->getValuePart();
                     CR = "NULL";
					 //membertable = classtable->findMemberTable(my_stack.top());
					 //stack<int> scope = symboltable->stack_scope;
					 TYPE = symboltable->lookUpST(name, current_scope);
					 if (TYPE == "NULL")
					 {
					 	membertable = classtable->findMemberTable(my_stack.top());
					 	RMT = membertable->lookUpMT(name,CCR);
					 	if (RMT.type == "NULL")
					 	{
					 		cout << "Variable named: " << name << " is Undeclared ! at line number: " << t->getLineno() << endl;
					 	}
					 }
					
                    t->next();
                    if(SST1()){
                        return true;
                    }
                }
            }
            
        } else if (t->getClassPart() == "DT"){
            TYPE = t->getValuePart();
            t->next();
            if(t->getClassPart() == "ID"){
                name = t->getValuePart();
                t->next();
                if(Dec_choice()){
                    return true;
                }
            }
        }  else if(t->getClassPart() == "enum"){
            if(enum_()){
                return true;
            }
        
            
        } 
        return false;
    }
    bool SST1(){
        if(t->getClassPart() == "(" ||t->getClassPart() == "assignment_op" ||t->getClassPart() == "inc_dec" ||t->getClassPart() == "." ||t->getClassPart() == "[" || t->getClassPart() == "=" || t->getClassPart() == ";" || t->getClassPart() == "assignment_op"){
            if(arr_()){
                if(L1()){
                    if(L2()){
                        return true;
                    }
                }
            }
        } else if(t->getClassPart() == "ID"){
             name = t->getValuePart();
			 TYPE = symboltable->lookUpST(name, current_scope);
			 if (TYPE == "NULL")
			 {
			 	membertable = classtable->findMemberTable(my_stack.top());
			 	RMT = membertable->lookUpMT(name,CCR);
                if (RMT.type == "NULL")
                {
                    cout << "Variable named: " << name << " is Undeclared ! at line number: " << t->getLineno() << endl;
                }
			 }
            t->next();
            if(obj_dec()){
                return true;
            }
        }
    }
    bool L1(){
        if(CR == "NULL")
        {
            TYPE = symboltable->lookUpST(name,current_scope);
            if(TYPE == "NULL")
            {
                RMT = membertable->lookUpMT(name,CCR);
                if(RMT.type == "NULL")
                {
                    cout<< "Undeclared variable at line no.: "<< t->getLineno()<<endl;
                    
                }
                TYPE = RMT.type;
                if (RMT.access_modifier == "private")
                {
                    cout<< "This variable can not be accessed outside of class. "<< t->getLineno() <<endl;
                }
                
            }
        }
        else if(CR == CCR)
        {
            RMT = membertable->lookUpMT(name,CR);
            if(RMT.type == "NULL")
            {
                cout<< "Undeclared variable at line no.: "<< t->getLineno()<<endl;
                
            }
            TYPE = RMT.type;
        }
        if(t->getClassPart() == "."){
            if(TYPE == "ID")
            {
                RCT = classtable->lookupCT(TYPE);
                if(RCT.type == "NULL")
                {
                    cout<< "Class is not defined at line no.: "<<t->getLineno() <<endl;
                }   
            }
            string opr = t->getValuePart();
            t->next();
            string T4 = "";
            if(t->getClassPart() == "ID"){
                T4 = t->getClassPart();
                string TR = compatibilityCheck(T4, TYPE , opr);
				if (TR == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
                 name = t->getValuePart();
				// 	scopetable = membertable->findScopeTable(my_stack.top());
				// 	stack<int> scope = membertable->stack_scope;
				 	TYPE = symboltable->lookUpST(name, current_scope);
				 	if (TYPE == "NULL")
				 	{
				 		membertable = classtable->findMemberTable(my_stack.top());
				 		RMT = membertable->lookUpMT(name,CCR);
				 		if (TYPE == "NULL")
				 		{
				 			cout << "Variable named: " << name << " is Undeclared ! at line number: " << t->getLineno() << endl;
				 		}
                        TYPE = RMT.type;
				 	}
                t->next();
                if(L1()){
                    return true;
                }
            }
        } else if(t->getClassPart() == "inc_dec" || t->getClassPart() == "(" || t->getClassPart() == "assignment_op"){
            return true;
        }
        return false;
    }
    bool L2(){
        if(t->getClassPart() == "inc_dec"){
            string TR = compatibilityCheck(TYPE,"inc_dec");
            if(TR == "Incompatible")
            {
                cout<< "Type mismatched at line no. : "<<t->getLineno()<<endl;
            }
            t->next();
            if(t->getClassPart() == ";"){
                t->next();
                return true;
            }
        } else if(t->getClassPart() == "assignment_op"){
            if(Ass_st()){
                return true;
            }
        } else if(t->getClassPart() == "("){
            if(fn_call()){
                return true;
            }
        } 
        return false;
    }
    
    bool fn_call(){
        if(fn_call1()){
            
            if(t->getClassPart() == ";"){
                t->next();
                return true;
            }
        }
    }
    bool fn_call1(){
        if(t->getClassPart() == "("){
            
            t->next();
            if(arg()){
                if(t->getClassPart() == ")"){
                    if(CR == "NULL")
                    {
                        RMT = membertable->Lookup_Fn_MT(name,para_list,CR);
                        stack<int> scope = symboltable->stack_scope;
                        TYPE = symboltable->lookUp_Fn_ST(name,current_scope,scope,para_list);
                        if(RMT.type == "NULL")
                        {
                            cout << "Undeclared at line no.: " << t->getLineno() <<endl;
                        }
                        
                        else if(TYPE == "NULL")
                        {
                            cout << "Undeclared at line no.: " << t->getLineno() <<endl;
                        }
                        TYPE = RMT.type;
                    }
                    else if (CR == CCR)
                    {
                        RMT = membertable->Lookup_Fn_MT(name,para_list,CR);
                        if(RMT.type == "NULL")
                        {
                            cout << "Undeclared at line no.: " << t->getLineno() <<endl;
                        }
                        TYPE = RMT.type;
                    }
                    t->next();
                    return true;
                }

            }
        }
        return false;
    }
    bool arg(){
        if(t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "int_const" || 
        t->getClassPart() == "string_const" || t->getClassPart() == "float_const" || 
        t->getClassPart() == "char_const" ||t->getClassPart() == "!" || t->getClassPart() == "(" ){
            string T4 = "";
            if(OE(T4)){
                para_list = TYPE;
                if(arg1()){
                    return true;
                }
            }
        } else if(t->getClassPart() == ")" || t->getClassPart() == "}"){
            para_list = "void";
            
            return true;
        }
        return false;
    }
    bool arg1(){
        if(t->getClassPart() == ","){
            para_list += ",";
            t->next();
            string T4 = "";
            if(OE(T4)){
                para_list += TYPE;
                if(arg1()){
                    return true;
                }
            }
        }else if (t->getClassPart() == ")" || t->getClassPart() == "}"){
            return true;
        }
        return false;
    }
    bool obj_dec(){
        name = t->getValuePart();
        if(t->getClassPart() == "["){
            if(arr()){
                if(new_init()){
                    if(list2()){
                        return true;
                    }
                }
            }
        } else if(t->getClassPart() == ";"){
            t->next();
            return true;
        } else if(t->getClassPart() == "("){
           if(fn_call()){
            return true;
           }
        }
        return false;
    }
    bool new_init(){
        if(t->getClassPart() == "assignment_op"){
            t->next();
            if(t->getClassPart() == "ID"){
                t->next();
                if(new_init()){
                    if(list2()){
                        return true;
                    }
                    
                }
            }
        } else if(t->getClassPart() == ","){
            return true;
        }
        return false;
    }
    bool list2() {
        if (t->getClassPart() == ",") {
            t->next(); // Move past ','
            if (t->getClassPart() == "ID") {
                t->next(); // Move past 'ID'
                if (arr()) {
                    if (new_init()) {
                        if (list2()) {
                            return true;
                        }
                    }
                }
            }
        } else if (t->getClassPart() == ";") {
            t->next(); // Move past ';'
            return true;
        }

        return false; // If the production is not matched
    }

    bool this_st() {
        if (t->getClassPart() == "this") {
            t->next();
            if (t->getClassPart() == ".") {
                CR=CCR;
                t->next();
                return true;
            }
        }
        else if(t->getClassPart() == "ID"){
            return true;
        }
        return false;
    }

	bool OE(string &T) {
		if (t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const"
			||t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "inc_dec") {
			string T1 = "";
            if (AE(T1)) {
				if (OE_(T1,T)) {
                    if (T == "") T = T1;
					return true;
				}
			}
		}
		return false;
	}
	bool OE_(string T1, string &T) {
		if (t->getClassPart() == "||") {
            string opr = t->getClassPart();
			t->next();
            string T2 = "";
			if (AE(T2)) {
                string T3 = compatibilityCheck(T1, T2, opr);
				if (T3 == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
				if (OE_(T3,T)) {
					return true;
				}
			}
		}
		else if (t->getClassPart() == ")" || t->getClassPart() == "]" || t->getClassPart() == ";" || t->getClassPart() == ",") {
			T = T1;
            return true;
		}
		return false;
	}

	bool AE(string &T) {
		if (t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const"
			|| t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "inc_dec") {
			string T1 = "";
            if (RE(T1)) {
				if (AE_(T1,T)) {
                    if (T == "") T = T1;
					return true;
				}
			}
		}
		return false;
	}
	bool AE_(string T1, string& T) {
		if (t->getClassPart() == "&&") {
            string opr = t->getClassPart();
			t->next();
            string T2 = "";
			if (RE(T2)) {
                string T3 = compatibilityCheck(T1, T2, opr);
				if (T3 == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
				if (AE_(T3,T)) {
                    if (T == "") T = T1;
					return true;
				}
			}
		}
		else if ( t->getClassPart() == "||" ||
			t->getClassPart() == ")" || t->getClassPart() == "]" || t->getClassPart() == ";" || t->getClassPart() == "," ) {
			T=T1;
            return true;
		}
		return false;
	}

	bool RE(string &T) {
		if (t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const"
			||t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "inc_dec") {
			string T1 = "";
            if (E(T1)) {
				if (RE_(T1,T)) {
					return true;
				}
			}
		}
		return false;
	}
	
	bool RE_(string T1, string& T) {
		if (t->getClassPart() == "rop") {
            string opr = t->getValuePart();
			t->next();
            string T2 = "";
			if (E(T2)) {
                string T3 = compatibilityCheck(T1, T2, opr);
				if (T3 == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
				if (RE_(T3,T)) {
                    if (T == "") T = T1;
					return true;
				}
			}
		}
		else if ( t->getClassPart() == "&&" || t->getClassPart() == "||" ||
			t->getClassPart() == ")" || t->getClassPart() == "]" || t->getClassPart() == ";" || t->getClassPart() == "," ) {
			T=T1;
            return true;
		}
		return false;
	}

	bool E(string &R) {
		if (t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const"
			|| t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "inc_dec") {
			string T1 = "";
            if (T(T1)) {
				if (E_(T1,R)) {
					return true;
				}
			}
		}
		return false;
	}
	
	bool E_(string T1, string& Q) {
		if (t->getClassPart() == "pm") {
            string opr = t->getValuePart();
			t->next();
            string T2 = "";
			if (T(T2)) {
                string T3 = compatibilityCheck(T1, T2, opr);
				if (T3 == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
				if (E_(T3,Q)) {
					return true;
				}
			}
		}
		else if (t->getClassPart() == "rop" || t->getClassPart() == "&&" || t->getClassPart() == "||" ||
			t->getClassPart() == ")" || t->getClassPart() == "]" || t->getClassPart() == ";" || t->getClassPart() == ",") {
			Q=T1;
            return true;
		}
		return false;
	}

	bool T(string &T) {
		if (t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const"
			|| t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "inc_dec") {
			string T1 = "";
            if (F(T1)) {
				if (T_(T1,T)) {
					return true;
				}
			}
		}
		return false;
	}
	
	bool T_(string T1, string& T){
		if (t->getClassPart() == "mdm") {
			string opr = t->getValuePart();
			t->next();
			string T2 = "";
			if (F(T2)) {
                string T3 = compatibilityCheck(T1, T2, opr);
				if (T3 == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
				if (T_(T3,T)) {
                    if (T == "") T = T1;
					return true;
				}
			}
		}
		else if (t->getClassPart() == "pm" || t->getClassPart() == "rop" || t->getClassPart() == "&&" || t->getClassPart() == "||" ||
			t->getClassPart() == ")" || t->getClassPart() == "]" || t->getClassPart() == ";" || t->getClassPart() == "," ) {
			T=T1;
            return true;
		}
		return false;
	}

	bool F(string &T) {
		if (t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const") {
			string T1 = "";
            if (constant(T1)) {
                T = T1;
				return true;
			}
		}
		else if (t->getClassPart() == "(") {
			t->next();
            string T4 = "";
			if (OE(T4)) {
                
				if (t->getClassPart() == ")") {
					t->next();
					return true;
				}
			}
		}
		else if (t->getClassPart() == "!") {
            string Operator = t->getClassPart();
			t->next();
            string T4 = "";
			if (F(T4)) {
				T = compatibilityCheck(T4, "!");
				if (T == "Incompatible")
				{
					cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
				}
				return true;
			}
		}
		else if (t->getClassPart() == "ID") {
            name = t->getValuePart();
            TYPE = t->getValuePart();
            CR = "NULL";
            // scopetable = membertable->findScopeTable(my_stack.top());
	 		//stack<int> scope = symboltable->stack_scope;
			T = symboltable->lookUpST(name, current_scope);
			 		if (T == "NULL")
			// 		{
			// 			deftable = membertable->findDefTable(my_stack.top());
			// 			T = deftable->lookUpDT(name);
			// 			if (T == "NULL")
			// 			{
						cout << "Variable named: "<< name <<  " is Undeclared ! at line number: " << t->getLineno() << endl;
			// 			}
			// 		}
			// global_type = T;
            t->next();
            if (XOE1()) {
                return true;
            }
			
		
		}
		return false;
	}
	bool XOE1(){
        if(t->getClassPart() == "("){
            if(fn_call()){
                return true;
            }
        } else if (t->getClassPart() == "." || t->getClassPart() == "[" || t->getClassPart() == "assignment_op" || t->getClassPart() == "inc_dec"){
            if(nt2()){
                return true;
            }
        }
        else if(t->getClassPart() == "mdm" ||t->getClassPart() == "pm" || t->getClassPart() == "rop" || t->getClassPart() == "&&" || t->getClassPart() == "||" ||
			t->getClassPart() == ")" || t->getClassPart() == "]" || t->getClassPart() == ";" || t->getClassPart() == "," ){
                //stack<int> scope = symboltable->stack_scope;
                string T1 = symboltable->lookUpST(name,current_scope);
                if(T1 == "NULL")
                {
                    cout << "Variable named: "<< name <<  " is Undeclared ! at line number: " << t->getLineno() << endl;
                }
                return true;
        }
        return false;
    }
	
	bool nt2() {
        string T;
        if(CR == "NULL")
        {
            //stack<int> scope = symboltable->stack_scope;
            T = symboltable->lookUpST(name,current_scope);
            if(T == "NULL")
            {
                RMT = membertable->lookUpMT(name,class_name);
                if(RMT.type == "NULL")
                {
                    cout << "Variable named: "<< name <<  " is Undeclared ! at line number: " << t->getLineno() << endl;
                }
                T = RMT.type;
                if(RMT.access_modifier == "private")
                {
                    cout<< "Variable named: "<<name << " can not be accessed outside of the class at line number: " <<t->getLineno() <<endl;
                }
            }
        } else if (CR == CCR)
        {
            RMT = membertable->lookUpMT(name,class_name);
            if(RMT.type == "NULL")
            {
                cout << "Variable named: "<< name <<  " is Undeclared ! at line number: " << t->getLineno() << endl;
            }
            T = RMT.type;
        }
		if (t->getClassPart() == ".") {
            if(T == "ID") 
            {
                RCT = classtable->lookupCT(T);
                if(RCT.type == "NULL")
                {
                    cout << "Class is undefined at line number: " <<t->getLineno() << endl;
                } 
                else
                {
                    cout << ". Is not valid" << endl;
                }
            }
			t->next();
			if (t->getClassPart() == "ID") {
                name = t->getValuePart();
				t->next();
				if (XOE1()) {
					return true;
				}
			}
		}
		else if (t->getClassPart() == "inc_dec" || t->getClassPart() == "assignment_op") {
			if(A0()){
                return true;
            }
			
		}else if(t->getClassPart() == "["){
            t->next();
            string T4 = "";
            if(OE(T4)){
                if(T4 != "int")
                {
                    cout << "Invalid index at line number:  " <<t->getLineno() <<endl;
                }
                //stack<int> scope = symboltable->stack_scope;
                T = symboltable->lookUpST(name,current_scope);
                if (T == "NULL")
                {
                    RMT = membertable->lookUpMT(name, class_name);
                    {
                        if(RMT.type == "NULL")
                        {
                            cout << "Variable named: "<< name <<  " is Undeclared ! at line number: " << t->getLineno() << endl;
                        }
                    }
                    T = RMT.type;
                    if(RMT.access_modifier == "private")
                    {
                        cout << "This variable can not be accessed outside of class at line no.: " << t->getLineno() <<endl;
                    }
                } else if (CR == CCR) {
                    RMT = membertable->lookUpMT(name, class_name);
                    if(RMT.type == "NULL")
                        {
                            cout << "Variable named: "<< name <<  " is Undeclared ! at line number: " << t->getLineno() << endl;
                        }
                    
                    T = RMT.type;
                }
                if(t->getClassPart() == "."){
                    if(T == "ID") 
                    {
                        RCT = classtable->lookupCT(T);
                        if(RCT.type == "NULL")
                        {
                            cout << "Class is undefined at line number: " <<t->getLineno() << endl;
                        } 
                        else
                        {
                            cout << ". Is not valid" << endl;
                        }
                    }
                    t->next();
                    if(t->getClassPart() == "ID"){
                        name = t->getValuePart();
                        t->next();
                        return true;
                    }
                }
            }
        }
		else if (t->getClassPart() == "mdm" || t->getClassPart() == "pm" || t->getClassPart() == "rop" || t->getClassPart() == "&&" || t->getClassPart() == "||" ||
			t->getClassPart() == ")" || t->getClassPart() == "]" || t->getClassPart() == ";" || t->getClassPart() == ",") {
			if(CR == "NULL")
            {
            //stack<int> scope = symboltable->stack_scope;
                T = symboltable->lookUpST(name,current_scope);
                if (T == "NULL")
                {
                    RMT = membertable->lookUpMT(name,class_name);
                    if(RMT.type == "NULL")
                        {
                            cout << "Variable named: "<< name <<  " is Undeclared ! at line number: " << t->getLineno() << endl;
                        }
                    
                    T = RMT.type;
                    if(RMT.access_modifier == "private")
                    {
                        cout << "This variable can not be accessed outside of class at line no.: " << t->getLineno() <<endl;
                    }
                } else if (CR == CCR) {
                    RMT = membertable->lookUpMT(name, class_name);
                    if(RMT.type == "NULL")
                        {
                            cout << "Variable named: "<< name <<  " is Undeclared ! at line number: " << t->getLineno() << endl;
                        }
                    
                    T = RMT.type;
                }
            }
            return true;
		}
		return false;
	}
	bool A0(){
        if(t->getClassPart() == "assignment_op"){
            t->next();
            string T4 = "";
            if(OE(T4)){
                return true;
            }
        } else if(t->getClassPart() == "inc_dec") {
            t->next();
            return true;

        } 
        return false;
    }
	bool constant(string& T) {
		if (t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const") {
			T = t->getClassPart();
            t->next();
			return true;
		}
		return false;
	}
    bool arr() {
        if (t->getClassPart() == "[") {
            t->next(); // Move past '['
            if (OE__()) {
                if (t->getClassPart() == "]") {
                    t->next(); // Move past ']'
                    if (arr_()) {
                        return true;
                    }
                }
            }
        }
        return false; // If the production is not matched
    }
    bool OE__(){
        if(t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const"
            ||t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "inc_dec"){
                string T4 = "";
                if(OE(T4)){
                    return true;
                }
        } else if (t->getClassPart() == "]" || t->getClassPart() == ")" || t->getClassPart() == ";" || t->getClassPart() == ","){
            return true;
        }
        else return false;
    }
    bool arr1(){
        if(t->getClassPart() == "["){
            if(arr()){
                if(init_arr()){
                    if(t->getClassPart() == ";"){
                        t->next();
                        return true;
                    }
                }
            }
        } 
        return false;
    }
    bool init_arr() {
        if (t->getClassPart() == "assignment_op") {
            t->next(); // Move past '='
            if (t->getClassPart() == "{") {
                t->next(); // Move past '{'
                if (arg()) {
                    if (t->getClassPart() == "}") {
                        t->next(); // Move past '}'
                        return true;
                    }
                }
            }
        } else if(t->getClassPart() == ";"){
            return true;    
        }
        return false;
    }
    bool arr2() {
        if (t->getClassPart() == ",") {
            t->next(); // Move past ','
            string T4 = "";
            if (OE(T4)) {
                if (arr2()) {
                    return true;
                }
            }
        } else if(t->getClassPart() == "}"){
            return true;
        }
        return false; 
    }
    bool nL(){
        if(t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const"
            ||t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "inc_dec"){
                string T4 = "";
                if(OE(T4)){
                    if(arr2()){
                        return true;
                    }
                }
        } else if(t->getClassPart() == "}"){
            return true;
        }
        return false;
    }
    bool if_else() {
        if (t->getClassPart() == "if") {
            t->next(); // Move past 'if'
            if (t->getClassPart() == "(") {
                t->next(); // Move past '('
                string T4 = "";
                if (OE(T4)) {
                    if(T4 != "bool")
                    {
                        cout << "Error at line no. " <<t->getLineno() <<endl;
                    }
                    if (t->getClassPart() == ")") {
                        current_scope = symboltable->createScope();
                        t->next(); // Move past ')'
                        if (body()) {
                            if(else_st()){
                                return true;
                            }
                                
                        }
                        }
                    }
                }
            
        }
        return false; // If the production is not matched
    }
    bool else_st() {
        if (t->getClassPart() == "else") {
            t->next(); // Move past 'else'
            if (body()) {
                return true;
            }
        } else if(t->getClassPart() == "ID" || t->getClassPart() == "int_const" || t->getClassPart() == "string_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" ||
        t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == ";" ||t->getClassPart() == "{" ){
            return true;
        }
        return false;
    }
    bool body() {
        if (t->getClassPart() == ";") {
            t->next(); // Move past ';'
            return true;
        } else if (t->getClassPart() == "{") {
            current_scope = symboltable->createScope();
            t->next(); // Move past '{'
            if (MST()) {
                if (t->getClassPart() == "}") {
                current_scope = symboltable->deleteScope();
                    t->next(); // Move past '}'
                    return true;
                }
            }
        }
        return false; // If the production is not matched
    }

    bool body1(){
        if(t->getClassPart() == "ID" || t->getClassPart() == "int_const" || t->getClassPart() == "string_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" ||
        t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == ";" ||t->getClassPart() == "{" ){
            if(SST()){
                return true;
            }
        } else if(t->getClassPart() == ";" || t->getClassPart() == "{"){
            if(body()){
                 return true;
            }
           
        }
        return false;
    }
        
    bool arr_(){
        if(t->getClassPart() == "["){
            if(arr()){
                return true;
            }
        } else if (t->getClassPart() == "assignment_op" || t->getClassPart() == ";" || t->getClassPart() == "=" || t->getClassPart() == "." || t->getClassPart() == "inc_dec" || t->getClassPart() == "(" ){
            return true;
        }
        return false;

    }

    bool Des() {
		if (t->getClassPart() == "~") {
			t->next(); // Move past '~'
			if (t->getClassPart() == "ID") {
                name = t->getValuePart();
                TYPE = t->getValuePart();
				t->next(); // Move past 'ID'
				if (t->getClassPart() == "(") {
                    current_scope = symboltable->createScope();
					t->next(); // Move past '('
					if (PL()) {
						if (t->getClassPart() == ")") {
							t->next(); // Move past ')'
                            T_E = para_list + "->" + TYPE;
                            if(!symboltable->insertST(name,T_E,current_scope))
                            {
                               cout<< "This variable " << name<<" is redeclared at line number: "<< t->getLineno() << endl;
                            }
							if (body()) {
								
									return true;
								
							}
						}
					}
				}
			}
		}

		return false; // If the production is not matched
	}
    bool enum_member_tail(){
		if(t->getClassPart() == "assignment_op"){
            t->next();
			if(constant(TYPE)){
				return true;
			}
            string TR = compatibilityCheck(TYPE,"assignment_op");
            //if()
		}
		else if (t->getClassPart() == "," || t->getClassPart() == "}"){
			return true;
		}
		return false;
	}
	bool enum_member() {
		if (t->getClassPart() == "ID") {
            name = t->getValuePart();
            if(!symboltable->insertST(name,TYPE,current_scope))
            {
                cout<< "Redeclaration error at line no. : "<<t->getLineno() <<endl;
            }
			t->next(); // Move past 'ID'
			if (enum_member_tail()) {
				return true;
			}
		}

		return false; // If the production is not matched
	}
	bool enum_list_tail() {
		if (t->getClassPart() == ",") {
			t->next(); // Move past ','
			if (enum_member()) {
				if (enum_list_tail()) {
					return true;
				}
			}
		}
		else if(t->getClassPart() == "}"){
			return true; // The ε (empty production) case, always returns true
		}
		return false;
	}
	bool enum_list(){
		if(t->getClassPart() == "ID"){
			if (enum_member()){
				if (enum_list_tail()){
					return true;
				}
			}
		}
		return false;
	}
	bool enum_() {
		if (t->getClassPart() == "enum") {
            TYPE = t->getValuePart();
			t->next(); // Move past 'enum'
			if (t->getClassPart() == "ID") {
                name = t->getValuePart();
                if(!symboltable->insertST(name,TYPE,current_scope))
                {
                    cout<< "Enum Redeclaration error at line no.: "<< t->getLineno() <<endl;
                }
				t->next(); // Move past 'ID'
				if (t->getClassPart() == "{") {
                    current_scope = symboltable->createScope();
					t->next(); // Move past '{'
					if (enum_list()) {
						if (t->getClassPart() == "}") {
                            current_scope = symboltable->deleteScope();
							t->next(); // Move past '}'
							if (t->getClassPart() == ";") {
								t->next(); // Move past ';'
								return true;
							}
						}
					}
				}
			}
		}

		return false; // If the production is not matched
	}
    bool for_st() {
        if(t->getClassPart() == "for"){
            t->next();
            if(t->getClassPart() == "("){
                current_scope = symboltable->createScope();
                t->next();
                if(F1()){
                    if(F2()){
                        if(t->getClassPart() == ";"){
                            t->next();
                            if(F3()){
                                if(t->getClassPart() == ")"){
                                    t->next();
                                    if(body()){
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
    bool F1(){
        if (t->getClassPart() == ";"){
            t->next();
            return true;
        } else if(t->getClassPart() == "DT"){
            if(Dec()){
                return true;
            }
        } else if (t->getClassPart() == "assingment_op"){
            if(Ass_st()){
                return true;

            }
        }
        return false;
    }
    bool F2(){
        if(t->getClassPart() == "int_const" || t->getClassPart() == "float_const" || t->getClassPart() == "char_const" || t->getClassPart() == "string_const"
            ||t->getClassPart() == "(" || t->getClassPart() == "!" || t->getClassPart() == "this" || t->getClassPart() == "ID" || t->getClassPart() == "inc_dec"){
                string T4 = "";
                if(OE(T4)){
                    if(T4 == "int_const" || T4 == "string_const" || T4 == "char_const" || T4 == "float_const" )
                    {

                    }
                    else
                    {
                        cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
                    }
                    return true;
                }
        } else if (t->getClassPart() == "]" || t->getClassPart() == ")" || t->getClassPart() == ";" || t->getClassPart() == ","){
            return true;
        }
        else return false;
    }
    bool F3(){
        if(t->getClassPart() == "ID"){
            name = t->getValuePart();
            //symboltable = classtable->findMemberTable(my_stack.top());
            //stack<int> scope = symboltable->stack_scope;
            TYPE = symboltable->lookUpST(name, current_scope);
            if(TYPE == "NULL")
            {
                cout<< "variable undeclared at line no.: " <<t->getLineno() <<endl;
            }
            t->next();
            if(X11()){
                return true;
            }
        } else if(t->getClassPart() == "inc_dec"){
            if(inc_dec_st()){
                return true;
            }
        } else if(t->getClassPart() == ")"){
            return true;
        }
        return false;
    }
    bool inc_dec_st(){
        CR = CCR;
        if(t->getClassPart() == "inc_dec"){
            string TR = compatibilityCheck(TYPE,"inc_dec");
            if(TR == "Incompatible")
            {
                cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
            }
            t->next();
            if(this_st()){
                if(t->getClassPart() == "ID"){
                    name = t->getValuePart();
                    t->next();
                    if(A0()){
                        return true;
                    }
                }
            }
        }
        else if(t->getClassPart() == ")"){
            return true;
        }
        return false;
    }
    bool X11(){
        if(t->getClassPart() == "assignment_op"){
        if(Ass_st1()){
            return true;
        }
        } else if(t->getClassPart() == "inc_dec"){
            if(inc_dec_st())
            {
                string TR = compatibilityCheck(TYPE,"inc_dec");
                if (TR == "Incompatible")
                    {
                        cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
                    }
                t->next();
                return true;
            }
            
        } 
        return false;

    }
    bool Ass_st(){
        if(t->getClassPart() == "assignment_op"){
            if(Ass_st1()){
                if(X_()){
                    if(t->getClassPart() == ";")
                    t->next();
                    return true;
                }
            }
        }
        return false;
    }
    bool Ass_st1(){
        if(t->getClassPart() == "assignment_op"){
            t->next();
            string T4 = "";
            if(OE(T4)){
                string TR = compatibilityCheck(T4,global_type,"=");
                if (TR == "Incompatible")
                    {
                        cout << "Type is incompatible at Line No: " << t->getLineno() << endl;
                    }
                t->next();
                return true;
            }
        }
        return false;
    }
    bool X_(){
        if(t->getClassPart() == "assignment_op"){
            if(Ass_st1()){
                if(X_()){
                    return true;
                }
            }
        } else if(t->getClassPart() == ";"){
            return true;
        }
        return false;
    }
    bool Mfunc() {
        if (t->getClassPart() == "ID") {
            name = t->getValuePart();
            t->next(); // Move past the ID
            if (t->getClassPart() == "(") {
                current_scope = symboltable->createScope();
                t->next(); // Move past the opening parenthesis
                if (PL()) {
                    if (t->getClassPart() == ")") {
                        t->next(); // Move past the closing parenthesis
                        //T_E = para_list + "->" + TYPE;
                        if(!symboltable->insertST(name,T_E,current_scope))
                        {
                            cout << "Redeclaration error at line no.: " << t->getLineno() << endl;
                        }
                        if (body()) {
                            return true;
                        }
                    }
                }
            }
        }
        return false; // If the production is not matched
    }
    bool MOD() {
        if (t->getClassPart() == "static" || t->getClassPart() == "virtual") {
            type_modifier = t->getClassPart();
            t->next(); // Move past 'static' or 'virtual'
            return true;
        }
        return false; // If the production is not matched
    }
    bool type() {
        if (t->getClassPart() == "ID") {
            TYPE = t->getValuePart();
            t->next(); // Move past the first "ID"
            if (t->getClassPart() == "ID" ){
                name = t->getValuePart();
                t->next();
                return true;
                }
        } else if (t->getClassPart() == "DT") {
            TYPE = t->getClassPart();
            t->next(); // Move past the first "ID"
            if (t->getClassPart() == "ID" ){
                name = t->getValuePart();
                t->next();
                return true;
                }
        } else if (t->getClassPart() == "void") {
            TYPE = "void";
            t->next(); // Move past the first "ID"
            if (t->getClassPart() == "ID" ){
                name = t->getValuePart();
                t->next();
                return true;
                }
        } 
        
        return false; // If the production is not matched
    }
    bool inher() {
        class_parent = "NULL";
        if (t->getClassPart() == ":") {
            t->next(); // Move past ':'
            if (t->getClassPart() == "AM") {
                class_access_modifier = t->getValuePart();
                t->next();
                if (t->getClassPart() == "ID") {
                    ClassInfo type;
                    type = classtable->lookupCT(t->getValuePart());
                    if (type.type == "NULL")
                    {
                        cout << "Undeclared Identifier at Line no: " << t->getLineno() << endl;
                    }
                    else if (type.access_modifier == "private")
                    {
                        cout<< "Private class, can not inherit at Line no: " << t->getLineno() << endl;
                    }
                    t->next(); // Move past the "ID"
                    return true;
                }
            }
        } else if(t->getClassPart() == "{"){
            
            return true;
        }
        return false; // If the production is not matched
    }
    bool AM(){
        if(t->getClassPart() == "AM"){
            t->next();
            if(t->getClassPart() == ":"){
                t->next();
                return true;
            }
        }
        return false;
    }
    bool fn_dec() {
        if (t->getClassPart() == "(") {
            current_scope = symboltable->createScope();
            t->next(); // Move past '('
            if (PL()) {
                if (t->getClassPart() == ")") {
                    t->next(); // Move past ')'
                    T_E = para_list + "->" + TYPE;
                    if(!symboltable->insertST(name,T_E,current_scope))
                    {
                        cout << "Function Redeclaration error at line no: " << t->getLineno()<<endl;
                    } 
                    else
                    {
                        membertable = classtable->findMemberTable(my_stack.top());
                        if(!membertable->insertMT(name,T_E,class_access_modifier,type_modifier))
                        {
                            cout << "Function Redeclaration error at line no: " << t->getLineno()<<endl;
                        }
                    }
                    if (t->getClassPart() == "{") {
                        t->next(); // Move past '{'
                        if (MST()) {
                            if (t->getClassPart() == "}") {
                                current_scope = symboltable->deleteScope();
                                t->next(); // Move past '}'
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false; // If the production is not matched
    }
    bool class_st() {
    class_access_modifier = "public";
    if (t->getClassPart() == "class") {
        class_type = "class";
        t->next(); // Move past 'class'
        if (t->getClassPart() == "ID") {
            class_name = t->getValuePart();
            my_stack.push(class_name);
            class_access_modifier = "public";
            t->next(); // Move past the class identifier (ID)
            if (inher()) {
                if (t->getClassPart() == "{") {
                    current_scope = symboltable->createScope();
                    if(!classtable->insertCT(class_name,class_type, class_access_modifier, class_category, class_parent, new MemberTable(), new SymbolTable()))
                    {
                        cout << "Redeclaration Error of Class at Line Number: " << t->getLineno() << endl;
                    }
                    
                    t->next(); // Move past the opening curly brace '{'
                    if (cbody()) {
                        if (t->getClassPart() == "}") {
                            // symboltable = classtable->findSymbolTable(my_stack.top());
                            // symboltable->printSymbolTable();
                            membertable = classtable->findMemberTable(my_stack.top());
                            classtable->printClassTable(my_stack.top());
                            current_scope = symboltable->deleteScope();
                            class_name = my_stack.top();
                            my_stack.pop();
                            t->next(); // Move past the closing curly brace '}'
                            if (t->getClassPart() == ";") {
                                t->next(); // Move past the semicolon ';'
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false; // If the production is not matched
}
    bool X1(){
        if(t->getClassPart() == "ID"){
            name = t->getValuePart();
            t->next();
            if(X3()){
                return true;
            }
        } else if(t->getClassPart() == "("){
            if(fn_dec()){
                return true;
            }
        }
        return false;
    }
    
    
    bool X2(){
        if(t->getClassPart() == "("){
            if(fn_dec()){
                return true;

            }
        } else if(t->getClassPart() == "assignment_op"|| t->getClassPart() == ";"|| t->getClassPart() == ","||t->getClassPart() == "["){
            if(Dec_choice_()){
                return true;
            }
        }
        return false;
    }
    bool Dec_choice_(){
        if(t->getClassPart() == "assignment_op" || t->getClassPart() == ";" || t->getClassPart() == ","){
            if(init()){
                if(list()){
                    return true;
                }
            }
        } else if(t->getClassPart() == "["){
            if(arr1()){
                return true;
            }
        } 
        return false;
    }
    bool X3(){
        if(t->getClassPart() == "("){
            if(fn_dec()){
                return true;

            }
        } else if(t->getClassPart() == "["){
            if(obj_dec()){
                return true;
            }
        }
        return false;
    }
    
    bool cbody(){
        //class_category = "General";
		//class_access_modifier = "default";
        if(t->getClassPart() == "ID"){
            para_list = t->getValuePart();

            t->next();
            if(X1()){
                if(cbody()){
                    return true;
                }
            }
        } else if(t->getClassPart() == "DT"){
            para_list = t->getValuePart();
			TYPE = t->getValuePart();
            t->next();
            if(t->getClassPart() == "ID"){
                name = t->getValuePart();
                t->next();
                if(X2()){
                    if(cbody()){
                        return true;
                    }
                }
            }
        } else if(t->getClassPart() == "AM" ){
            class_access_modifier = t->getValuePart();
            t->next();
            if(t->getClassPart() == ":"){
                t->next();
                if(cbody()){
                return true;
                }
            }
            
            
        } else if(MOD()){
            if(t->getClassPart() == "static" || t->getClassPart() == "virtual"){
                if(type()){
                    if(X2()){
                        if(cbody()){
                            return true;
                        }
                        
                    }
                }
            }
            
            
        }else if(t->getClassPart() == "~"){
            if(Des()){
                if(cbody()){
                    return true;
                }
            }
        }
        else if(t->getClassPart() == "}"){
            return true;
        }
        return false;
    }

    bool return_st() {
    if (t->getClassPart() == "return") {
        t->next(); // Move past 'return'
        string T4 = "";
        if (OE(T4)) {
            string t1 = compatibilityCheck(para_list, T4, "assignment_op");
				if (t1 == "Incompatible")
				{
					cout << "Method Return type: " << para_list << " is incompatible at Line No: " << t->getLineno() << " of return Expression type: " << T4 << endl;
				}
            if (t->getClassPart() == ";") {
                t->next(); // Move past the semicolon ';'
                return true;
            }
        }
    }
    return false; // If the production is not matched
    }
    string compatibilityCheck(string Mtype, string Operator)
	{
		if ((Mtype == "int"  || Mtype == "float" || Mtype == "char") && (Operator == "inc_dec"))
		{
			return Mtype;
		}
		else if ((Mtype == "int" || Mtype == "bool" || Mtype == "float" || Mtype == "char") && (Operator == "!"))
		{
			return "bool";
		}
		return "Incompatible";
	}

	string compatibilityCheck(string leftType, string rightType, string Operator)
	{
		if (leftType == "int" && rightType == "int")
		{
			if (Operator == "+" || Operator == "-" || Operator == "*" || Operator == "/" || Operator == "%" || Operator == "=")
				return "int";
			if (Operator == "<" || Operator == ">" || Operator == "<=" || Operator == ">=" || Operator == "!=" || Operator == "==" || Operator == "=")
				return "bool";
		}
		else if ((leftType == "int" && rightType == "float") || (leftType == "float" && rightType == "int") || (leftType == "float" && rightType == "float"))
		{
			if(Operator == "+" || Operator == "-" || Operator == "*" || Operator == "/" || Operator == "%" ||  Operator == "=" )
				return "float";
			if (Operator == "<" || Operator == ">" || Operator == "<=" || Operator == ">=" || Operator == "!=" || Operator == "==")
				return "bool";
		}
		else if ((leftType == "string" && rightType == "string"))
		{
			if(Operator == "+")
				return "string";
			if (Operator == "=" || Operator == "<" || Operator == ">" || Operator == "<=" || Operator == ">=" || Operator == "!=" || Operator == "==")
				return "bool";
		}
		else if (leftType == "bool" && rightType == "bool" && 
			(Operator == "=" || Operator == "<" || Operator == ">" || Operator == "<=" || Operator == ">=" || Operator == "!=" || Operator == "==" || Operator == "&&" || Operator == "||"))
		{
			return "bool";
		}
		else if (leftType == "char" && rightType == "char" && 
			(Operator == "=" || Operator == "<" || Operator == ">" || Operator == "<=" || Operator == ">=" || Operator == "!=" || Operator == "==" || Operator == "&&" || Operator == "||"))
		{
			return "bool";
		}
		return "Incompatible";
	}
    string ConstType(string Mtype)
	{
		if (Mtype == "int_const")
		{
			return "int";
		}

		if (Mtype == "float_const")
		{
			return "float";
		}

		if (Mtype == "string_const")
		{
			return "string";
		}

		if (Mtype == "char_const")
		{
			return "char";
		}

		if (Mtype == "true" || Mtype =="false")
		{
			return "bool";
		}
		return "none";
	}


    

};