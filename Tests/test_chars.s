"This program mainly test the character operations";

char x = 'z';
x = x + 'a';
char dummy; 
dummy = 'c';
x = x - dummy;
print "here is the value of x => ";
print x;

print "now test the conditions with the characters";
print "if it print 'y' thats means okay, if it print 'n' thats means there is error";
if (x < 'z'){
    print 'y';
}

if (x > 'z'){
    print 'n';
} else if (x != 'x'){
    print 'n';
} else if (x == 'x'){
    print 'y';
}


"Now let's try + and - with characters";
x = 'Z';
x = x + 'A';
dummy = 'C';
x = x - dummy;
print x;
"It'll print X";


char a = 'b';
char capitalize = 'A' - 'a';
a = capitalize + a;
print a;
"It'll print B";

"It'll print Z";
print 'z' + capitalize;

"It'll print z";
print 'Z' - capitalize;


print "let's try to use loops over all characters";
for(char small = 'a'; small <= 'z'; small = small + 'b' - 'a'){
    print small;
}

print "let's try to use loops over all capital characters";
char c = 'A';
while(c <= 'Z'){
    print c;
    c = c + 'b' - 'a';
}

print "what about print all digits as characters";
char dig = '1';
dowhile (dig <= '9'){
    print dig;
    dig = dig + '2' - '1';
}

