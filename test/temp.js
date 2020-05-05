function add(p1, p2) {
    var e = p1 + p2;
    return e;
}

while (true) {
    log(a + b);
}

var a = "i am a string";
var b = true;
var c = 1234;
var d = 5678;
var e = 'c';
if (b) {
    var f = c * d;
}

log();
var g = add(c, d);

a = add(1, 2);

let a = 2;
if (a < 4) {
    if ( a < 0) {
        a = 666;
    } else {
        a = 999;
    }
} else {
    a = 3;
}

let a = 3 * 2;
let b = a * 3 - 6;

function add(a, b) {
    let temp = a + b;
    return temp;
}

function minus(a, b) {
    return a - b;
}

let c = add(a, b);
let d = minus(c, 7);

function fibonacci(n) {
    if (n == 0) {
        return 0;
    } else {
        if (n == 1) {
            return 1;
        } else {
            return fibonacci(n - 1) + fibonacci(n - 2);
        }
    }
}

let e = fibonacci(0);
let f = fibonacci(6);
let g = fibonacci(10);

let h = 10;

while (h > 0) {
    h = h - 1.5;
}