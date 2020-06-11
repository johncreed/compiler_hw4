/* Test OR AND stmt */
int MAIN(){
    int a, b;
    
    /* CMP */
    a = 2;
    if( a == 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 1;
    if( a == 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 2;
    if( a >= 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 1;
    if( a >= 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 2;
    if( a <= 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 3;
    if( a <= 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 2;
    if( a != 1 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 2;
    if( a != 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 3;
    if( a > 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 100;
    if( a < 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 3;
    if( a > 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 1;
    if( a > 2 ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);

    /* OR AND */
    a = 1;
    b = 1;
    if( a || b ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 0;
    b = 1;
    if( a || b ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 1;
    b = 0;
    if( a || b ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 0;
    b = 0;
    if( a || b ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 1;
    b = 1;
    if( a && b ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 0;
    b = 1;
    if( a && b ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 1;
    b = 0;
    if( a && b ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 0;
    b = 0;
    if( a && b ){
        a = 666;
    }
    else{
        a = 333;
    }
    
    write(a);
    
    a = 1;
    if(!a){
        a  = 666;
    }
    else{
        a = 333;
    }
    write(a);

    /* while */

    a = 1;
    while( a < 10 ){
        if( (a - (a/2)*2) == 0){
            write(a);
        }
        a = a + 1;
    }
}
