#include <stdio.h>

typedef struct Parser {
    char *src;
    int i;
} Parser;


#define cur (p->src[p->i])
#define adv (++p->i)
#define skip while (cur > 0 && cur <= 32) {adv;}

float expr_top(Parser *p);

float expr_unit(Parser *p) {
    float value = 0.;
    float div = 1.;
    skip
    if (cur == '(') {
        adv;
        value = expr_top(p);
        skip
        if (cur == ')') {
            adv;
        }
        return value;
    }
    if (cur == '-') {
        div = -1;
    }
    while (cur >= '0' && cur <= '9') {
        value *= 10;
        value += cur - '0';
        adv;
    }
    if (cur != '.') {
        return value * div;
    }
    adv;
    while (cur >= '0' && cur <= '9') {
        value *= 10;
        value += cur - '0';
        div *= 10;
        adv;
    }
    return value / div;
}

float expr_mult(Parser *p) {
    float value = expr_unit(p);
    while (1) {
        skip
        char c = cur;
        if (c != '*' && c != '/') {
            return value;
        }
        adv;
        float right = expr_unit(p);
        switch (c) {
            case '*': value *= right; break;
            case '/': value /= right; break;
            default: return value;
        }
    }
    return value;
}

float expr_top(Parser *p) {
    float value = expr_mult(p);
    while (1) {
        skip
        char c = cur;
        if (c != '+' && c != '-') {
            return value;
        }
        adv;
        float right = expr_mult(p);
        switch (c) {
            case '+': value += right; break;
            case '-': value -= right; break;
            default: return value;
        }
    }
    return value;
}



int main(void) {
    Parser p = {"1*4 + 2*5 + 3*6 + (3+4)/6", 0};
    printf("%f\n", expr_top(&p));
}