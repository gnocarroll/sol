#include <stdio.h>

#include "expr_node.h"
#include "token.h"

int main(void) {
    for (int token_idx = 0; token_idx < TOK_COUNT; token_idx++) {
        const char *text = get_token_text(token_idx);

        if (text) {
            printf("TOKEN TEXT: %s\n", text);
        }
    }

    return 0;
}