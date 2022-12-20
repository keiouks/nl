let x = 10;
function fun1() {
    let x = 20;
    x = x + 20;
    print(x);
}
fun1();

function fun2() {
    let x = 30;
    function fun3() {
        print(x + 5);
    }
    fun3();
}

fun2();

print(x);

function fun4() {
    x = 900;
}
fun4();

print(x);

fun3();
