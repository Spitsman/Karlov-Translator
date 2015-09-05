// Translator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <fstream>
#include <iterator>
#include "Tables.h"
#include "Integer.h"
using namespace std;

Token Transliterator(char c)
{
	Token res;
	if ('0' <= c && c <= '9')
	{
		res._class = DIGIT;
		res.value = c - '0';
	}
	else if ('a' <= c && c <= 'z')
	{
		res._class = LETTER;
		res.value = c - 'a';
	}
	else if (c == '+' || c == '-' || c == '*' || c == '/')
	{
		res._class = OPERATION;
		if (c == '+') res.value = 1;
		else if (c == '-') res.value = 2;
		else if (c == '*') res.value = 3;
		else if (c == '/') res.value = 4;
	}
	else if (c == '<' || c == '>' || c == '=' || c == '!')
	{
		res._class = RATIO;
		if (c == '=') res.value = 1;
		else if (c == '!') res.value = 2;
		else if (c == '<') res.value = 3;
		else if (c == '>') res.value = 4;
	}
	else if (c == ' ')
	{
		res._class = SPACE;
		res.value = 0;
	}
	else if (c == '\n' || c == '\0')
	{
		res._class = CR;
		res.value = 0;
	}
	else if (c == ';')
	{
		res._class = SEMICOLON;
		res.value = 0;
	}
	else
	{
		res._class = ERROR;
		res.value = 0;
	}
	return res;
}

//Регистры
lexemes REG_CLASS;
int* REG_POINTER = 0;
int REG_NUMBER = 0;
int REG_RATIO = 0;
string REG_VAR;
int REG_IDENT = 0;
int REG_VALUE = 0;
int FLAG_CONST;

list<int> table_const;
list<Var> table_variable;
list<lexeme> table_lexemes;


void add_constant()
{
	if (FLAG_CONST == 1) table_const.push_back(REG_NUMBER);
	REG_POINTER = &table_const.back();
}

void add_variable()
{
	for (list <Var>::iterator i = table_variable.begin(); i != table_variable.end(); i++)
	{
		if (REG_VAR == i->name)
		{
			REG_POINTER = &i->value;
			return;
		}
	}
	table_variable.push_back({ REG_VAR, 0 });
	Var* temp = &table_variable.back();
	REG_POINTER = &temp->value;
}

void add_lexeme()
{
	lexeme l;
	l.type = REG_CLASS;
	switch (REG_CLASS)
	{
	case PUSH:
		l.value = REG_POINTER;
		break;
	case POP:
		l.value = REG_POINTER;
		break;
	case OPERAT:
		REG_NUMBER = REG_VALUE;
		add_constant();
		l.value = REG_POINTER;
		break;
	case RAT:
		REG_NUMBER = REG_RATIO;
		add_constant();
		l.value = REG_POINTER;
		break;
	case JMP:
		add_constant();
		l.value = REG_POINTER;
		break;
	case JI:
		add_constant();
		l.value = REG_POINTER;
		break;
	default:
		REG_NUMBER = 0;
		add_constant();
		l.value = REG_POINTER;
	}
	table_lexemes.push_back(l);
}

//Процедуры
states Error(Token t)
{
	REG_CLASS = ERR;
	return J1;
}
states A1a(Token t)
{
	add_lexeme();
	return A1;
}
states A2b(Token t)
{
	add_lexeme();
	return A2;
}
states A2c(Token t)
{
	add_constant();
	add_lexeme();
	return A2;
}
states A2d(Token t)
{
	add_variable();
	add_lexeme();
	return A2;
}
states B1a(Token t)
{
	REG_IDENT = init_vector[t.value] - 1;
	if (REG_IDENT == -1) return Error(t);
	return B1;
}
states B1b(Token t)
{
	REG_IDENT++;
	return B1;
}
states C1a(Token t)
{
	REG_CLASS = OPERAT;
	REG_VALUE = t.value;
	add_lexeme();
	return C1;
}
states C1b(Token t)
{
	REG_CLASS = THEEND;
	add_lexeme();
	return C1;
}
states C1c(Token t)
{
	REG_CLASS = READ;
	add_lexeme();
	return C1;
}
states C1d(Token t)
{
	REG_CLASS = WRITE;
	add_lexeme();
	return C1;
}
states C1e(Token t)
{
	add_constant();
	add_lexeme();
	return C1;
}
states C1f(Token t)
{
	add_variable();
	add_lexeme();
	return C1;
}
states C1g(Token T)
{
	add_lexeme();
	return C1;
}
states C1h(Token t)
{
	if (t.value != 1) return Error(t);
	switch (REG_RATIO)
	{
	case 2:
		REG_RATIO = 2;
		break;
	case 3:
		REG_RATIO = 5;
		break;
	case 4:
		REG_RATIO = 6;
		break;
	default:
		return Error(t);
	}
	if (REG_RATIO == 0) return Error(t);
	add_lexeme();
	return C1;
}
states D1a(Token t)
{
	REG_CLASS = RAT;
	REG_RATIO = t.value;
	return D1;
}
states E1a(Token t)
{
	REG_CLASS = PUSH;
	FLAG_CONST = 1;
	return E1;
}
states E2a(Token t)
{
	REG_CLASS = JI;
	FLAG_CONST = 0;
	return E2;
}
states E2b(Token t)
{
	REG_CLASS = JMP;
	FLAG_CONST = 0;
	return E2;
}
states E3a(Token t)
{
	REG_CLASS = POP;
	return E3;
}
states G1a(Token t)
{
	REG_NUMBER = t.value;
	return G1;
}
states G1b(Token t)
{
	REG_NUMBER = REG_NUMBER * 10 + t.value;
	return G1;
}
states H1a(Token t)
{
	REG_VAR = t.value + 'a';
	return H1;
}
states H1b(Token t)
{
	REG_VAR.insert(REG_VAR.end(), t.value + 'a', 1);
	return H1;
}
states I1a(Token t)
{
	REG_CLASS = COMMENT;
	return H1;
}
states I2a(Token t)
{
	REG_CLASS = COMMENT;
	return I2;
}
states I2b(Token t)
{
	add_constant();
	add_lexeme();
	return I2;
}
states I2c(Token t)
{
	add_variable();
	add_lexeme();
	return I2;
}
states M1(Token t)
{
	if (tabl[REG_IDENT].letter == t.value + 'a') { return tabl[REG_IDENT].func(t); }//перейти к процедуре, на которую указывает регистр обнаружения
	else REG_IDENT = tabl[REG_IDENT].alt;
	if (REG_IDENT == 0) return Error(t);
	return B1;
}
void EXIT1()
{
	REG_CLASS = ENDLINE;
	add_lexeme();
	//stop
}
void EXIT2()
{
	add_lexeme();
	REG_CLASS = ENDLINE;
	add_lexeme();
	//stop
}
void EXIT3()
{
	add_constant();
	add_lexeme();
	REG_CLASS = ENDLINE;
	add_lexeme();
}
void EXIT4()
{
	add_variable();
	add_lexeme();
	REG_CLASS = ENDLINE;
	add_lexeme();
}

void scanner()
{
	states q = A1;
	Token t;
	std::ifstream in("file.txt");
	char c;
	while (in)
	{
		in.get(c);
		t = Transliterator(c);
		switch (q)
		{
		case A1:
			switch (t._class)
			{
			case LETTER:
				q = B1a(t);
				break;
			case OPERATION:
				q = C1a(t);
				break;
			case RATIO:
				q = D1a(t);
				break;
			case SPACE:
				q = A1;
				break;
			case CR:
				q = A1;
				break;
			case SEMICOLON:
				q = I1a(t);
				break;
			default:
				q = Error(t);
			}
			break;
		case A2:
			switch (t._class)
			{
			case LETTER:
				q = B1a(t);
				break;
			case OPERATION:
				q = C1a(t);
				break;
			case RATIO:
				q = D1a(t);
				break;
			case SPACE:
				q = A2;
				break;
			case CR:
				q = A2;
				break;
			case SEMICOLON:
				q = I2a(t);
				break;
			case END:
				EXIT1();
				break;
			default:
				q = Error(t);
			}
			break;
		case B1:
			if (t._class == LETTER) q = M1(t);
			else q = Error(t);
			break;
		case C1:
			switch (t._class)
			{
			case SPACE:
				q = C1;
				break;
			case CR:
				q = A2;
				break;
			case SEMICOLON:
				q = I2a(t);
				break;
			case END:
				EXIT1();
				break;
			default:
				q = Error(t);
			}
			break;
		case D1:
			switch (t._class)
			{
			case RATIO:
				q = C1h(t);
				break;
			case SPACE:
				q = C1g(t);
				break;
			case CR:
				q = A2b(t);
				break;
			case SEMICOLON:
				q = I2a(t);
				break;
			case END:
				EXIT2();
				break;
			default:
				q = Error(t);
			}
			break;
		case E1:
			if (t._class == SPACE) q = F1;
			else q = Error(t);
			break;
		case E2:
			if (t._class == SPACE) q = F2;
			else q = Error(t);
			break;
		case E3:
			if (t._class == SPACE) q = F3;
			else q = Error(t);
			break;
		case F1:
			switch (t._class)
			{
			case LETTER:
				q = H1a(t);
				break;
			case DIGIT:
				q = G1a(t);
				break;
			case SPACE:
				q = F1;
				break;
			default:
				q = Error(t);
			}
			break;
		case F2:
			switch (t._class)
			{
			case DIGIT:
				q = G1a(t);
				break;
			case SPACE:
				q = F2;
				break;
			default:
				q = Error(t);
			}
			break;
		case F3:
			switch (t._class)
			{
			case LETTER:
				q = H1a(t);
				break;
			case SPACE:
				q = F3;
				break;
			default:
				q = Error(t);
			}
			break;
		case G1:
			switch (t._class)
			{
			case DIGIT:
				q = G1b(t);
				break;
			case SPACE:
				q = C1e(t);
				break;
			case CR:
				q = A2c(t);
				break;
			case SEMICOLON:
				q = I2b(t);
				break;
			case END:
				EXIT3();
				break;
			default:
				q = Error(t);
			}
			break;
		case H1:
			switch (t._class)
			{
			case LETTER:
				q = H1b(t);
				break;
			case DIGIT:
				q = H1b(t);
				break;
			case SPACE:
				q = C1f(t);
				break;
			case CR:
				q = A2d(t);
				break;
			case SEMICOLON:
				q = I2c(t);
				break;
			case END:
				EXIT4();
				break;
			default:
				q = Error(t);
			}
			break;
		case I1:
			switch (t._class)
			{
			case LETTER:
				q = I1;
				break;
			case DIGIT:
				q = I1;
				break;
			case OPERATION:
				q = I1;
				break;
			case SPACE:
				q = I1;
				break;
			case CR:
				q = A1a(t);
				break;
			case SEMICOLON:
				q = I1;
				break;
			case ERROR:
				q = I1;
				break;
			default:
				q = Error(t);
			}
			break;
		case I2:
			switch (t._class)
			{
			case LETTER:
				q = I2;
				break;
			case DIGIT:
				q = I2;
				break;
			case OPERATION:
				q = I2;
				break;
			case SPACE:
				q = I2;
				break;
			case CR:
				q = A2b(t);
				break;
			case SEMICOLON:
				q = I2;
				break;
			case ERROR:
				q = I2;
				break;
			case END:
				EXIT1();
				break;
			default:
				q = Error(t);
			}
			break;
		case J1:
			switch (t._class)
			{
			case LETTER:
				q = J1;
				break;
			case DIGIT:
				q = J1;
				break;
			case OPERATION:
				q = J1;
				break;
			case SPACE:
				q = J1;
				break;
			case CR:
				q = A2b(t);
				break;
			case SEMICOLON:
				q = J1;
				break;
			case ERROR:
				q = J1;
				break;
			case END:
				EXIT1();
				break;
			default:
				q = Error(t);
			}
			break;
		}
	}
}

void Translator()
{
	stack<integer> s;
	lexeme l;
	integer a, b;
	for (list<lexeme>::iterator i = table_lexemes.begin(); i != table_lexemes.end(); i++)
	{
		l = *i;
		switch (l.type)
		{
		case PUSH:
			s.push(integer(*l.value));
			break;
		case POP:
			*l.value = s.top().ConvertToInt();
			s.pop();
			break;
		case RAT:
			a = s.top(); s.pop();
			b = s.top(); s.pop();
			switch (*l.value)
			{
			case 1:
				s.push(a == b);
				break;
			case 2:
				s.push(a != b);
				break;
			case 3:
				s.push(a < b);
				break;
			case 4:
				s.push(a>b);
				break;
			case 5:
				s.push(a <= b);
				break;
			case 6:
				s.push(a >= b);
			}
			break;
		case OPERAT:
			a = s.top(); s.pop();
			b = s.top(); s.pop();
			switch (*l.value)
			{
			case 1:
				s.push(a + b);
				break;
			case 2:
				s.push(b - a);
				break;
			case 3:
				s.push(a*b);
				break;
			case 4:
				s.push(b / a);
			}
			break;
		case JMP:
			i = table_lexemes.begin();
			for (int j = 1; j < *l.value; j++) i++;
			break;
		case JI:
			if (s.top() == 1)
			{
				i = table_lexemes.begin();
				for (int j = 0; j < *l.value; j++) i++;
				break;
			}
		case READ:
			int n;
			cin >> n;
			s.push(integer(n));
			break;
		case WRITE:
			cout << s.top();
			break;
		case COMMENT:
		case THEEND:
			return;
		case ERR:
			continue;
		case ENDLINE:
			break;
		}
	}
}

int main()
{
	scanner();
	Translator();

	//cout << endl << REG_NUMBER << endl;
	//for (list<int>::iterator i = table_const.begin(); i != table_const.end(); i++) cout << *i << endl;
	for (list<int>::iterator i = table_const.begin(); i != table_const.end(); i++)
	{
		//	cout << (*i) << " ";
	}
	cout << endl;
	for (list<Var>::iterator i = table_variable.begin(); i != table_variable.end(); i++)
	{
		//		cout << i->name << " " << &i->value << endl;
	}
	cout << endl;
	for (list<lexeme>::iterator i = table_lexemes.begin(); i != table_lexemes.end(); i++)
	{
				//lexeme l = *i;
				//cout << l.type << " " << *l.value << endl;
	}
	cout << endl;
	//integer num(123);
	//cout << num << endl;
	//cout << num.ConvertToInt() << endl;
	return 0;
}

