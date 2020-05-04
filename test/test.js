let a = 5;
let b = 0;
let c = -3;
while (a > 0) {
    a = a - 1;
    b = b + 1;
    if(b > 2) {
        a = -4;
        c = a * b * c;
    }
}