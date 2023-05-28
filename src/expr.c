#include <stdio.h>


typedef struct Parser {
    char *src;
    int i;
    float vars[64];
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
    if (cur == '$') {
        adv;
        int index = 0;
        while (cur >= '0' && cur <= '9') {
            index *= 10;
            index += cur - '0';
            adv;
        }
        return p->vars[index];
    }

    if (cur == '-') {
        div = -1;
        adv;
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

    return value;
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


void program(Parser *p) {
    while (cur != '\0') {
        skip
        int start = p->i;
        float value;
        if (cur == '$') {
            adv;
            int index = 0;
            while (cur >= '0' && cur <= '9') {
                index *= 10;
                index += cur - '0';
                adv;
            }
            skip
            if (cur == '=') {
                adv;
                value = expr_top(p);
                p->vars[index] = value;
                printf("> $%d = %f\n", index, value);
                continue;
            }
        }
        p->i = start;
        value = expr_top(p);
        printf("> %f\n", value);
    }
}




int main(void) {
    char buffer[128];
    Parser p = {buffer, 0};
    while (1) {
        char* eof = fgets(buffer, sizeof(buffer), stdin);
        p.i = 0;
        program(&p);
    }
}