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