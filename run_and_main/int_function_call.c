int one(){
    int a;
    a = 333;
    write(a);
    return a + 33;
}

int MAIN(){
    int a;
    float b;
    b = 3.14;
    a = one();
    write(a);
    one();
    write(b);
}
