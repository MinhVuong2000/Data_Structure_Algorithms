#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <math.h>
#include <stack>

using namespace std;

//find character in string, if isn't return -1
int findChar(string a, int posStart, int posEnd, char c) {
    for (int i = posStart;i <= posEnd;i++)
        if (a[i] == c) return i;
    return -1;
}

//change string to Int
int StringToInt(string a, int posStart, int posEnd) {
    int value = 0;
    for (int i = posStart;i <= posEnd;i++) {
        value *= 10;
        value = value + a[i] - 48;
    }
    return value;
}

//change string to int or float
float StringToNumber(string a, int start, int end) {
    float value = 0;
    int posDot = findChar(a, start, end, '.');
    if (posDot == -1)
        value = StringToInt(a, start, end);
    else {

        value += StringToInt(a, start, posDot - 1);
        for (int i = posDot + 1;i <= end;i++) {
            value = value + (a[i] - 48) * 1.0 / pow(10, i - posDot);
        }
    }
    return value;
}

//calculate expression by using string input is postfix expression
string calExpress(string str, int n) {
    int pSpace;//find index of space in string
    stack<float> values;//contain operands
    for (int i = 0;i < n;) {
        i == n - 1 ? pSpace = n:pSpace = findChar(str, i, n - 1, ' ');
        if (pSpace - i > 1 || (str[i] >= '0' && str[i] <= '9'))
            values.push(StringToNumber(str, i, pSpace-1));
        else {
            float v2 = values.top();
            values.pop();
            float v1 = values.top();
            values.pop();
            switch (str[i]) {
            case '+': values.push(v1 + v2); break;
            case '-': values.push(v1 - v2); break;
            case '*': values.push(v1 * v2); break;
            case '/': 
                if (v2 == 0)
                    return "E";
                values.push(v1 / v2); break;
            case '^': values.push(pow(v1, v2)); break;
            }
        }
        i = pSpace + 1;
    }
    return to_string(values.top());
}

//arrange order of operator
int prior(char c) {
    if (c == '+' || c == '-')
        return 1;
    else if (c == '*' || c == '/')
        return 2;
    else if (c == '^')
        return 3;
    return 0;
}

//check if char is digit, return 1 if yes
bool isdigit(char c) {
    return (c >= '0' && c <= '9') ? 1 : 0;
}

//check order punctuation
bool validPunc(string str, int n) {
    stack<char> a;
    int check=0; //to check order of {}, [], ()
    for (int i = 0;i < n;i++) {
        if (str[i] == '(' || str[i] == '[' || str[i] == '{')//open is push into stack
        {
            if (str[i] == '{' && check == 0) check = 2;
            if (str[i] == '[') {
                if (check == 0) check = 1;//don't have {}
                else check--;//have {}
            }
            if (str[i] == '(' && check != 0) check -- ;//have {} or []
            a.push(str[i]);
        }
        else if (str[i] == ')' || str[i] == ']' || str[i] == '}') {
            if (!a.empty()) {
                if (str[i] == ')')
                    if (a.top() != '(') 
                        return 0;
                if (str[i] == ']')
                    if (a.top() != '[')
                        return 0;
                if (str[i] == '}')
                    if (a.top() != '{')
                        return 0;
                a.pop();
            }
            else return 0; //if stack is empty, false because has open not close
        }
    }
    return (check==0 && a.empty() == 1);//check if final stack is empty is all is suitable
}

//check expression is error, if yes return 1
bool errorExpression(string str) {
    int size = str.length();

    if (!validPunc(str, size)) return 1;

    //check -(a) and +(a)
    if (strstr(str.c_str(), "+(") != NULL ||
        strstr(str.c_str(), "-(") != NULL)
        return 1;

    for (int i = 0;i < size-1;i++) {
        //check -digit
        if (str[i] == '-' && isdigit(str[i + 1]))
            return 1;

        //check 2 operands near together?
        int posSpace = findChar(str, i, size - 1, ' ');
        if (posSpace == -1)
            return 0;
        if ((isdigit(str[posSpace - 1]) && isdigit(str[posSpace + 1])) || 
            (isdigit(str[posSpace - 1]) && str[posSpace + 1]=='(') ||
            (str[posSpace - 1]==')' && isdigit(str[posSpace + 1])))
            return 1;
    }

    return 0;
}

//exchange [], {} in string into all ()
string rePunc(string str, int size) {
    for (int i = 0;i < size;i++) {
        if (str[i] == '[' || str[i] == '{')
            str[i] = '(';
        else if (str[i] == ']' || str[i] == '}')
            str[i] = ')';
    }
    return str;
}

//infix to postfix
string infixToPostfix(string infix) {
    if (errorExpression(infix))//check if is error?
        return "E";
    infix = "(" + infix + ")";
    int size = infix.length();
    infix = rePunc(infix, size);//change punctation into all ()
    stack<char> st;
    string out="";//string for output
    for (int i = 0;i < size;) {
        if (infix[i] == ' ')
            i++;
        else {
            if (isdigit(infix[i])) {
                do {
                    out += infix[i];
                    i++;
                } while (infix[i] != ' ' && infix[i] != ')');//get number string
                out += " ";

            }
            else if (infix[i] == '(') {
                st.push('(');
                i++;
            }
            else if (infix[i] == ')') {
                while (st.top() != '(') {
                    out += st.top();
                    out += " ";
                    st.pop();
                }
                st.pop();
                i++;
            }
            else {//when is operator or punctuation
                while (prior(infix[i]) <= prior(st.top())) {
                    out += st.top();
                    out += " ";
                    st.pop();
                }
                st.push(infix[i]);
                i++;
            }
        }
    }
    return out;
}


int main(int argc, char *argv[])
{
    string line;
    ifstream file(argv[1]);
    if (file.fail())
        cout << "Fail file";
    ofstream out(argv[4]);
    for (int i = 0;i<StringToInt(argv[2], 0, strlen(argv[2]) - 1);i++) {
        getline(file,line);
        line = infixToPostfix(line);//change string to postfix expression
        if (!strcmp(argv[3],"-c") && line.compare("E")) {
            string calc = calExpress(line, line.length());
            if (calc != "E")
                out << stof(calc) << endl;//convert from string to float, then print
            else out << calc << endl;
        }
        else //when "-t"
            out << line << endl;
    }

    file.close();
    out.close();

    return 0;
}
