#include <stdio.h>

char inputChar() {
    return 'h';
}

int main(void) {
    char *src = "+[-->-[>>+>-----<<]<--<---]>-.>>>+.>>..+++[.>]<<<<.+++.------.<<-.>>>>+.";
    // char *src = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    // char *src = "++ > +++++ [<+>-] ++++ ++++ [< +++ +++ > -] <.";

    char buffer[64] = {0};

    int pc = 0;
    int dp = 32;
    int depth = 0;


    while (1) {
        // printf("%d", pc);
        switch (src[pc]) {
            case '>':  {++dp; ++pc;} break;
            case '<':  {--dp; ++pc;} break;
            case '+':  {++buffer[dp]; ++pc;} break;
            case '-':  {--buffer[dp]; ++pc;} break;
            case '.': {printf("%c", buffer[dp]); ++pc;} break;
            case ',': {buffer[dp] = inputChar(); ++pc;} break;
            case '[': {
                if (buffer[dp] == 0) {
                    while (src[pc] != '\0') {
                        if (src[pc] == '[') {
                            ++depth;
                        } 
                        if (src[pc] == ']' && --depth == 0) {
                            break;
                        }
                        ++pc;
                    }
                } else {
                    ++pc;
                }
            } break;
            case ']': {
                if (buffer[dp] != 0) {
                    while (pc > 0) {
                        if (src[pc] == ']') {
                            ++depth;
                        }
                        if (src[pc] == '[' && --depth == 0) {
                            break;
                        }
                        --pc;
                    }
                } else {
                    ++pc;
                }
            } break;
            case '\0': goto exit;
            default: ++pc; break;
        }
    }
    exit:
    printf("\nfinished\n");
}
