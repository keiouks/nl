x1 = 23 + 7;
print(x1);
print(10 * 10);
function f1() {
    print(x1);
    x2 = 10;
    return 20;
    x3 = 20;
}
print(-f1() *10);
x1 = x1 - f1();
print(x1);
print(x2);

function f2() {
    print(2);
    function f3() {
        return x1 + 10;
    }
    return f3() * 2;
}

print(f2() + 3);
