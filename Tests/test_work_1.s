
"This program mainly test the integers/floats operations";


print "integers part first";
"normal operactions parts";

int x = 10;
int y = 1;
print x;
print "complex equation";
print y + 10 * 20 - x * 3;
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


print "";
print "";
print "";
print "";
print "floats part";

"normal operactions parts";

float fx = 10.0;
float fy = 1.0;
print fx;
print "complex equation";
print fy + 10.0 * 20.0 - fx * 3.5;
print fy / 10.0;

print "now let's make complex condition and print the value of x";
"this condition will be (True and (Flase or True) and True) so it will print the value of x = 10";
"will not print y as it go into the first condition";
if ((fx + 10.0 > fy + 2.0) && ((fx + 10.0 < fy + 2.0) || (fx >= fy)) && (fy <= fx)){
    print fx;
} else if(fx){
    print fy;
}

"loops part";
float fi = 0.0;
float fpow = 1.0;
"lets see 2 power 10 in a loop";
for(fi=0.0; fi < 10.0; fi = fi + 1.0){
    fpow = fpow * 2.0;
}

print "power is here => fpow(2,10) = 1024";
print fpow;


"try subtract in different loop";
float fsub = 0.0;
fi = 0.0;
while(fi < 10.0){
    fsub = fsub - 2.0;
    fi = fi + 1.0;
}

print "fsub is here => 2*-10 = -20";
print fsub;


"add 2 * 10 in a loop";
fi = 0.0; 
float fadd = 0.0;
dowhile(fi < 10.0){
    fadd = fadd + 2.0;
    fi = fi + 1.0;
}
print "fadd is here => 2*10 = 20";
print fadd;