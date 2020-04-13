var a = "i am a string";
var b = true;
var c = 1234;
var d = 5678;
var e = 'c';
if(b) {
    var f = c*d;
}

function add(p1, p2) {
    var e = p1 + p2;
    return e;
}
log();
var g = add(c, d);

a = add(1, 2);

while (true) {
    log(a+b);
}