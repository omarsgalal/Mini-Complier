

int hash(char* myString, int modulo) {
    char * t;
    int sum = 0;

    for (t = myString; *t != '\0'; t++) {
        sum = sum + *t;
    }

    return sum % modulo;
}