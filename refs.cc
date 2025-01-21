#include <utility>

// lvalue reference
void rizz(int /* lval */ &a) {
    a = 1;
}

void cuh() {
    int a = 1;
    rizz(a);
}

// rvalue reference
void razz(int &&a) {
    a = 1;
}

void cuh2() {
    int&& a = int(1);
    razz(std::move(a));
}

// prvalue reference
void ruzz(int &&a) {
    a = 1;
}

void cuh3() {
    ruzz(int(1));
}

// xvalue reference
class X {
public:
    X() = default;
    X(X&&) = default;
    X& operator=(X&&) = default;
};

X&& griddy() {
    return X();
}

void cuh4() {
    X&& x = griddy();
}

// glvalue reference

void diddy(int &a) {
    int x = 42;             // x is an lvalue (and also a glvalue)
    int&& y = std::move(x); // std::move(x) produces an xvalue (and also a glvalue)
}

// const overloading

class FOO {
public:

    void foo() const {
        // this->foo(); // error: foo() is const
    }
};

int main() {
    FOO x;
    x.foo();

    const FOO y;
    y.foo();
}