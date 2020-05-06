function test(arr) {
    arr[1] = -6;
    return arr;
}

let a = [1, 2, 3];
let b = test(a);
let c = b[1];