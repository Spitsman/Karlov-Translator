#pragma once
#include "Transliterator.h"
#include <string>
using namespace std;

enum symbols { LETTER, DIGIT, OPERATION, RATIO, SPACE, CR, SEMICOLON, ERROR, END };
enum states { A1, A2, B1, C1, D1, E1, E2, E3, F1, F2, F3, G1, H1, I1, I2, J1 };
enum lexemes { PUSH, POP, OPERAT, RAT, JMP, JI, READ, WRITE, THEEND, COMMENT, ERR, ENDLINE };

struct Var
{
	string name;
	int value;
};

struct lexeme
{
	lexemes type;
	int* value;
};

struct Token
{
	symbols _class;
	int value;
};

struct tabl_per
{
	char letter;
	int alt;
	states(*func)(Token t);
};

//Начальный вектор
int init_vector[26] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 6, 0, 11, 0, 0, 0, 0, 14, 0, 0, 0 };

//Таблица переходов
states B1b(Token t);
states C1d(Token t);
states E1a(Token t);
states E2b(Token t);
states E3a(Token t);
states C1b(Token t);
states C1c(Token t);
states E2a(Token t);
tabl_per tabl[17] = { { 'n', 0, B1b }, { 'd', 0, C1b }, { 'i', 4, E2a }, { 'm', 0, B1b }, { 'p', 0, E2b }, { 'o', 8, B1b }, { 'p', 0, E3a }, { 'u', 0, B1b }, { 's', 0, B1b }, { 'h', 0, E1a }, { 'e', 0, B1b }, { 'a', 0, B1b }, { 'd', 0, C1c }, { 'r', 0, B1b }, { 'i', 0, B1b }, { 't', 0, B1b }, { 'e', 0, C1d } };
