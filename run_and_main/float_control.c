int MAIN(){
    float a, b, c;
    a = 1.0;
    b = 2.0;
    if( a > b || 1.0 > 2.0)
    {
        a = 3.14;
    }
    else{
        a = -3.14;
    }
    write(a);
    
    a = 1.0;
    b = 333.333;
    while( a < b ){
        write(a);
        a = a + 100.3;
    }
}
