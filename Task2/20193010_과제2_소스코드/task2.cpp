#include <iostream>
#include <cstring>
#include <queue>
using namespace std;

queue<string> Q;

//function
void error();
int DIGIT(string, int);
void tokenParse(string);
double expr();
double term();
double factor();
int number();
string lex();
string nextValue();
void queueClear();

void error() {
	cout << "syntax error!!\n";
	queueClear();
	//process end
	exit(0);
}

int DIGIT(string str, int index) {
	string temp = "";
	int cnt = 0;
	for(int i=index; i<str.length(); i++, cnt++) {
		if(str[i] >= '0' && str[i] <= '9') temp += str[i];
		else if(str[i] == ' ') continue;
		else break;
	}
	Q.push(temp);
	return cnt-1; //i++ is already in tokenParse()
}

void tokenParse(string str) {
	for(int i=0; i<str.length(); i++) {
		if(str[i] == ' ') continue;
		else if(str[i] >= '0' && str[i] <= '9') {
			i += DIGIT(str, i);
		}
		else if(str[i] == '*' || str[i] == '/' || str[i] == '+' ||
				str[i] == '-' || str[i] == '(' || str[i] == ')') {
			string temp = "";
			temp += str[i];
			Q.push(temp);
		}
		else {
			error();
		}
	}
}

double expr() {
	double data = 0;
	data = term();
	
	while(nextValue() == "+" || nextValue() == "-") {
		char oper;
		oper = lex()[0];
		
		if(oper == '+') data += term();
		else if(oper == '-') data -= term();
	}
	return data;
}

double term() {
	double data = 0;
	data = factor();
	while(nextValue() == "*" || nextValue() == "/") {
		char oper;
		oper = lex()[0];
		if(oper == '*') data *= factor();
		else if(oper == '/') data /= factor();
	}
	return data;
}

double factor() {
	double data = 0;
	//if nextValue is '-' (option)
	char oper = '+';
	if(nextValue() == "-") {
		oper = lex()[0];
	}
	//if nextValue is digit
	if(nextValue()[0] >= '0' && nextValue()[0] <= '9') data = number();
	//( <expr> )
	else {
		if(nextValue() == "(") {
			lex();
			data = expr();
			if(nextValue() == ")") {
				lex();
			}
			else error();
		}
		else error();
	}
	if(oper == '-') return -data;
	else return data;
}

int number() {
	string NUM = lex();
	return stoi(NUM);
}

string lex() {
	if(!Q.empty()) {
		string value = Q.front();
		Q.pop();
		return value;
	}
	else return "";
}

string nextValue() {
	if (!Q.empty())	return Q.front();
	else return "";
}

void queueClear() {
	while(!Q.empty()) Q.pop();
}

int main() {
	//ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	while(1) {
		double ans = 0;
		string str;
		queueClear();
		cout << ">> ";
		getline(cin, str);
		tokenParse(str);
		ans = expr();
		//if input is left
		if(!Q.empty()) error();
		cout << ans << '\n';
	}
}
