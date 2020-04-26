"This program mainly test the integers operations";

"normal operactions parts";

float x = 10.0;
float y = 1.0;
print x;
print "print complex equation";
print y + 10.0 * 20.0 + x * 3.5;
print y / 10.0;

print "now let's make complex condition and print the value of x";
"this condition will be (True and (Flase or True) and True) so it will print the value of x = 10";
"will not print y as it go into the first condition";
if ((x + 10.0 > y + 2.0) && ((x + 10.0 < y + 2.0) || (x >= y)) && (y <= x)){
    print x;
} else if(x){
    print y;
}

"loops part";
float i = 0.0;
float pow = 1.0;
"lets see 2 power 10 in a loop";
for(i=0.0; i < 10.0; i = i + 1.0){
    pow = pow * 2.0;
}

print "power is here => pow(2,10) = 1024";
print pow;


"try subtract in different loop";
float sub = 0.0;
i = 0.0;
while(i < 10.0){
    sub = sub - 2.0;
    i = i + 1.0;
}

print "sub is here => 2*-10 = -20";
print sub;


"add 2 * 10 in a loop";
i = 0.0; 
float add = 0.0;
dowhile(i < 10.0){
    add = add + 2.0;
    i = i + 1.0;
}
print "add is here => 2*10 = 20";
print add;