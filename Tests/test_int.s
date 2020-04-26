"This program mainly test the integers operations";

"normal operactions parts";

int x = 10;
int y = 1;
print x;
print "print complex equation";
print y + 10 * 20 + x * 3;
"y * 100 / 2 % 3 = 100 / 2 % 3 = 50 % 3 = 2";
print y * 100 / 2 % 3;
"-x%3 = - 10 % 3 = -1";
print - x % 3;

print "now let's make complex condition and print the value of x";
"this condition will be (True and (Flase or True) and True) so it will print the value of x = 10";
"will not print y as it go into the first condition";
if ((x + 10 > y + 2) && ((x + 10 < y + 2) || (x >= y)) && (y <= x)){
    print x;
} else if(x){
    print y;
}

"loops part";
int i = 0;
int pow = 1;
"lets see 2 power 10 in a loop";
for(i=0; i < 10; i = i + 1){
    pow = pow * 2;
}

print "power is here => pow(2,10) = 1024";
print pow;


"try subtract in different loop";
int sub = 0;
i = 0;
while(i < 10){
    sub = sub - 2;
    i = i + 1;
}

print "sub is here => 2*-10 = -20";
print sub;


"add 2 * 10 in a loop";
i = 0; 
int add = 0;
dowhile(i < 10){
    add = add + 2;
    i = i + 1;
}
print "add is here => 2*10 = 20";
print add;