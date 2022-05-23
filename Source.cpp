#include<iostream>
#include<string>
#include <iomanip>
#include <vector>
#include <stack>       
#include <sstream>
using namespace std;
int wire = 0;
int cmosnum = 0;
string PNODE = "";
string NNODE = "";
string PNODES = "";
string NNODES = "";
string NOT_NODE = "";
string lastprocedure = "";
string tempgate;
string tempgate2;
bool before_after = false, before_after2 = false;
vector<int> orindices, andindices, notindices;
class CMOS
{
public:
	string name;
	string drain;
	string source;
	string gate;
	string body;
	string type;
	CMOS()
	{
		body = source;
	}
};
string add_wire()
{
	wire++;
	return "W" + to_string(wire);
}
string add_mosfet()
{
	cmosnum++;
	return "M" + to_string(cmosnum);
}
int detectcharacter(char c)
{

	if (c == '~' || c == '&' || c == '|')
	{
		return 1;
	}
	else
		return 0;
}
vector<int> indicesofoperators(string input, char oper)
{
	vector<int> tempp;
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == oper)
		{
			tempp.push_back(i);
		}
	}

	return tempp;
}

void display(vector<CMOS>& c)
{
	cout << "In this netlist, we have a VDD source of 5 volts and 0 is the GND " << endl << "The net list is as follows: " << endl;
	cout << "Mname drain gate source body type" << endl;
	for (int i = 0; i < c.size(); i++)
	{
		cout << c[i].name << " " << c[i].drain << " " << c[i].gate << " " << c[i].source << " " << c[i].body << " " << c[i].type << endl;
	}
	cout << "END" << endl;
}
void notgate(string& expression, vector<CMOS>& mosfets, string& input, bool last, bool flag)
{
	CMOS temp1, temp2;
	temp1.name = add_mosfet();
	temp1.source = "VDD";
	temp1.type = "PMOS";
	temp1.body = temp1.source;
	temp1.drain = ((last) ? "Y" : add_wire());
	
	if (flag)
	{
		temp1.gate = NOT_NODE;

	}
	else
	{
		temp1.gate = (input[(input.find('~') - 1)]);
	}
	temp2.name = add_mosfet();
	temp2.drain = temp1.drain;
	temp2.type = "NMOS";
	temp2.gate = temp1.gate;
	temp2.source = "0";
	temp2.body = temp2.source;
	mosfets.push_back(temp1);
	mosfets.push_back(temp2);
	NOT_NODE = temp2.drain;
	
}
void orgate(string& expression, vector<CMOS>& mosfets, string& input, bool last, bool firsttime)
{
	if (firsttime)
	{
		CMOS temp1, temp2, temp3, temp4;
		temp1.name = add_mosfet();
		temp1.source = "VDD";
		temp1.type = "PMOS";
		temp1.body = temp1.source;
		temp1.drain = add_wire();

		temp1.gate = input[input.find('|') - 1];

		temp2.name = add_mosfet();
		temp2.source = temp1.drain;
		temp2.type = "PMOS";
		temp2.body = temp2.source;
		temp2.drain = ((last) ? "Y" : add_wire());
		temp2.gate = input[input.find('|') + 1];

		temp3.name = add_mosfet();
		temp3.drain = ((last) ? "Y" : add_wire());

		temp3.source = "0";
		temp3.type = "NMOS";
		temp3.body = temp3.source;

		temp3.gate = temp1.gate;

		temp4.name = add_mosfet();
		temp4.source = temp3.source;
		temp4.type = "NMOS";
		temp4.body = temp4.source;
		temp4.drain = temp3.drain;
		temp4.gate = temp2.gate;

		mosfets.push_back(temp1);
		mosfets.push_back(temp2);
		mosfets.push_back(temp3);
		mosfets.push_back(temp4);
		PNODE = temp2.drain;
		NNODE = temp3.drain;
		NNODES = temp3.source;
		PNODES = temp2.drain;

	}
	else
	{

		CMOS temp1, temp2;
		temp1.name = add_mosfet();
		temp1.source = (lastprocedure == "AND") ? PNODE : PNODES;
		temp1.type = "PMOS";
		temp1.body = temp1.source;
		temp1.drain = (last) ? "Y" : add_wire();

		temp1.gate = (input[input.find('|') - 1] == '@') ? input[input.find('|') + 1] : input[input.find('|') - 1];
		if ((temp1.gate == "@"))
			temp1.gate = tempgate2;
		if ((temp1.gate == "@"))
			temp1.gate = tempgate2;
		temp2.name = add_mosfet();
		temp2.source = (last) ? "0" : NNODES;

		temp2.type = "NMOS";
		temp2.body = temp2.source;
		temp2.drain = NNODE;
		temp2.gate = (input[input.find('|') - 1] == '@') ? input[input.find('|') + 1] : input[input.find('|') - 1];
		if ((temp2.gate == "@"))
			temp2.gate = tempgate;
		if ((temp2.gate == "@"))
			temp2.gate = tempgate2;
		mosfets.push_back(temp1);
		mosfets.push_back(temp2);
		PNODE = temp1.drain;
		NNODE = temp2.drain;
		PNODES = temp1.drain;
		NNODES = temp2.source;

	}

}
void andgate(string& expression, vector<CMOS>& mosfets, string& input, bool last, bool firstone)
{
	if ((input[input.find('&') - 2] == '|') && (input[input.find('&') - 3] == '@'))
	{
		CMOS temp1, temp2, temp3, temp4;
		temp1.name = add_mosfet();
		temp1.source = PNODE;
		temp1.type = "PMOS";
		temp1.body = temp1.source;
		temp1.drain = ((last) ? "Y" : add_wire());
		temp1.gate = input[input.find('&') - 1];

		temp2.name = add_mosfet();
		temp2.source = temp1.source;
		temp2.type = "PMOS";
		temp2.body = temp2.source;
		temp2.drain = temp1.drain;
		temp2.gate = input[input.find('&') + 1];

		temp3.name = add_mosfet();
		
		temp3.source = NNODES;
		temp3.type = "NMOS";
		temp3.body = temp3.source;
		temp3.gate = temp1.gate;

		temp4.name = add_mosfet();
		temp4.source = add_wire();
		temp3.drain = temp4.source;
		temp4.type = "NMOS";
		temp4.body = temp4.source;
		temp4.drain = NNODE;
		temp4.gate = temp2.gate;
		NNODE = temp4.drain;
		PNODE = temp2.drain;
		PNODES = temp2.source;
		NNODES = temp3.source;
		mosfets.push_back(temp1);
		mosfets.push_back(temp2);
		mosfets.push_back(temp3);
		mosfets.push_back(temp4);
		
		orindices.pop_back();
	 
	}
	else
	{
		if (firstone)
		{
			CMOS temp1, temp2, temp3, temp4;
			temp1.name = add_mosfet();
			temp1.source = "VDD";
			temp1.type = "PMOS";
			temp1.body = temp1.source;
			temp1.drain = ((last) ? "Y" : add_wire());
			temp1.gate = input[input.find('&') - 1];

			temp2.name = add_mosfet();
			temp2.source = temp1.source;
			temp2.type = "PMOS";
			temp2.body = temp2.source;
			temp2.drain = temp1.drain;
			temp2.gate = input[input.find('&') + 1];

			temp3.name = add_mosfet();
			temp3.source = "0";
			temp3.type = "NMOS";
			temp3.body = temp3.source;
			temp3.gate = temp1.gate;

			temp4.name = add_mosfet();
			temp4.source = add_wire();
			temp3.drain = temp4.source;
			temp4.type = "NMOS";
			temp4.body = temp4.source;
			temp4.drain = ((last) ? "Y" : add_wire());
			temp4.gate = temp2.gate;
			NNODE = temp4.drain;
			PNODE = temp2.drain;
			PNODES = temp2.source;
			NNODES = temp3.source;
			mosfets.push_back(temp1);
			mosfets.push_back(temp2);
			mosfets.push_back(temp3);
			mosfets.push_back(temp4);
			
			
		}
		else
		{
			CMOS temp1, temp2;
			temp1.name = add_mosfet();
			
			temp1.source = PNODES;
			
			temp1.type = "PMOS";
			temp1.body = temp1.source;
			if (input.find('&') + 1 != -1)
			{
				temp1.gate = input[input.find('&') + 1];
			}
			temp1.drain = PNODE;
		
			if ((temp1.gate == "@"))
				temp1.gate = tempgate;
			if ((temp1.gate == "@"))
				temp1.gate = tempgate2;
			temp2.name = add_mosfet();
			temp2.source = NNODE;
			
			temp2.type = "NMOS";
			temp2.body = temp2.source;
			temp2.drain = last ? "Y" : add_wire();

			
			temp2.gate = input[input.find('&') + 1];
			if ((temp2.gate == "@"))
				temp2.gate = tempgate;
			if ((temp2.gate == "@"))
				temp2.gate = tempgate2;
			mosfets.push_back(temp1);
			mosfets.push_back(temp2);
			PNODE = temp1.drain;
			NNODE = temp2.drain;
			PNODES = temp1.source;
			NNODES = temp2.source;
		}
	}
}

vector<string> storevariables(string input)
{
	vector<string> variables;
	string str;
	string temp;
	temp = input.substr(2, input.size() - 2);
	for (int i = 0; i < temp.size(); i++)
	{
		if (!detectcharacter(temp[i]))
		{
			str.push_back(temp[i]);
			variables.push_back(str);
		}
	}
	return variables;
}
bool found_in_vector(vector<CMOS>mosfets, string a)
{
	for (int i = 0; i < mosfets.size(); i++)
	{
		if (mosfets[i].gate == a)
		{
			return true;
		}
	}
	return false;
}
vector<string> find_missing(vector<CMOS> mosfets, vector<string> variables)
{
	vector<string> v;

	for (int i = 0; i < variables.size(); i++)
	{
		if (!found_in_vector(mosfets, variables[i]))
		{
			v.push_back(variables[i]);

		}
	}
	return v;
}
void adjust(vector<CMOS>& c,vector<string> variables)
{
	vector<string> missing = find_missing(c, variables);
	int index = 0;
	for (int i = 0; i < c.size(); i++)
	{
		
		if (c[i].drain == PNODE || c[i].drain == NNODE)
		{
			c[i].drain = "Y";
		}
	}
	
	for (int i = 0; i < c.size(); i++)
	{

		c[i].body = c[i].source;

	}
	for (int i = 0; i < c.size(); i++)
	{

		if (c[i].gate == "@")
		{

			c[i].gate = missing[index];
			c[i + 1].gate = missing[index];
			index++;
		}

	}
}
void get_expression()
{// the input will change so for ex when y=a&b|c we will do a&b then replace gthe input with y=@|c where @ is a flag for a&b
	string Y;
	string input;
	string outputofgates;
	string expression;
	bool lastnot = false, lastand = false, lastor = false;
	bool firstand = true, firstor = true;
	bool last = false;
	vector<CMOS> mosfets;
	bool cexpression=false;
	cout << "Enter the boolean expression " << endl;
	cin >> input;
	vector<string> variables = storevariables(input);
	notindices = indicesofoperators(input, '~');
	orindices = indicesofoperators(input, '|');
	andindices = indicesofoperators(input, '&');
	string temp;
	if (orindices.size() == 0 && andindices.size() == 0 && notindices.size() == 0)
	{
		input = input + "~~";
		notgate(expression, mosfets, input, lastnot, 0);
		input.erase((input.begin() + input.find('~')));
		notgate(expression, mosfets, input, 1, 1);
		input.erase((input.begin() + input.find('~')));

	}
	if (orindices.size() == 0 && andindices.size() == 0)
	{
		lastnot = true;
	}
	else
	{
		cexpression = true;
	}
	for (int i = 0; i < notindices.size(); i++)
	{
		notgate(expression, mosfets, input, lastnot,0);
		input.erase((input.begin() + input.find('~')));
	}
	for (int i = 0; i < andindices.size(); i++)
	{
		if (input.find('|') != -1)
		{
			if (input[input.find('|') - 1] == '@')
			{
				before_after = true; before_after2 = true;
			}
		}
		andgate(expression, mosfets, input, last, firstand);
		lastprocedure = "AND";
		firstand = false;
		int x = input.find('&');
		tempgate = input[x + 1];
		tempgate2 = input[x - 1];
		if (x != -1)
		{
			input.replace((x - 1), 3, "@");
			
		}
		if (input.size() == 5)
		{
			last = true;
			
		}
	}
	for (int i = 0; i < orindices.size(); i++)
	{
		orgate(expression, mosfets, input, last, firstand);
		lastprocedure = "OR";
		firstand = false;
		int x = input.find('|');
		tempgate = input[x + 1];
		tempgate2 = input[x - 1];
		if (x != -1)
		{
			input.replace((x - 1), 3, "@");
			
		}
		if (input.size() == 5)
		{
			last = true;
			
		}
	}

	adjust(mosfets,variables);
	if (cexpression)
	{
		NOT_NODE = "Y";
		notgate(expression, mosfets, input, 1, 1);
		mosfets[mosfets.size() - 2].drain = mosfets[mosfets.size() - 1].drain = "out";
	}
	display(mosfets);
}
int main()
{
	get_expression();
}