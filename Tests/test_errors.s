"no error here";
int x = 1;

"variable already declared!";
int x = 2;

"variable used before declaration!";
print z;

"no error here";
int z;

"variable used before initialization!";
print z;

"Type mismatch!";
int y = 'c';

"Type mismatch!";
x = "sad";

"no error here";
x = 2 + 3;

"Type mismatch!";
x = 2 + 'c' + 3;

"Type mismatch!";
x = 2 + "sd";

"Type mismatch!";
x = 2 + 2.4;

"variable used before declaration!";
b = 5;

"variable used before declaration!";
b = 5 + 3;

"syntax error";
x ++;

"syntax error";
x += 1;

"syntax error";
x = x/% 5;

"Type mismatch!";
string str = 'a';

"syntax error";
x = 23 4;

"Type mismatch!";
float flo = 5;

"Type mismatch!";
char c = "ds";

"no error here";
float f = 0.2;

"operation not defined";
f = f % 5.6;

"no error here";
string newStr = "im";
newStr = newStr + "@";

"operation not defined";
newStr = newStr - "@";

"no error here";
f = f + 5.6;