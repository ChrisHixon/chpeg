#include <string.h>
#include "chpeg_all.h"
#include "calc_bytecode.h"

int calc_val(ChpegNode *node, unsigned char *input) {
    int value = 0;

#ifdef DEBUG_CALC
    fprintf(stderr, "%s: def=%d\n", __func__, node->def);
#endif

    switch (node->def) {
        case CALC_DEF_EXPR:
            value = calc_val(node->head, input);
            break;
        case CALC_DEF_SUM:
        case CALC_DEF_PRODUCT:
            {
                ChpegNode *child = node->head, *op = NULL;

                value = calc_val(child,  input);
                op = child->next;

                while (op) {
                    child = op->next;
                    switch (input[op->offset]) {
                        case '+':
                            value += calc_val(child, input);
                            break;
                        case '-':
                            value -= calc_val(child, input);
                            break;
                        case '*':
                            value *= calc_val(child, input);
                            break;
                        case '/':
                            value /= calc_val(child, input);
                            break;
                        default:
                            chpeg_abort("unhandled op case '%c'\n", input[op->offset]);
                            break;
                    }
                    op = child->next;
                }
            }
            break;

        case CALC_DEF_NUMBER:
            {
                char *token = chpeg_token(input, node->offset, node->length);
                value = atoi(token);
#ifdef DEBUG_CALC
                fprintf(stderr, "%s: str='%s', number=%d\n", __func__, token, value);
#endif
                CHPEG_FREE(token);
            }
            break;

        default:
            chpeg_abort("unhandled def '%d'\n", node->def);
            break;
    }

#ifdef DEBUG_CALC
    fprintf(stderr, "%s: def=%d, return: %d\n", __func__, node->def, value);
#endif
    return value;
}

int main(int argc, const char *argv[])
{
    int err = 0;

    const char *input_filename = "-";
    unsigned char *input = NULL;
    size_t length = 0;

    ChpegParser *parser = NULL;
    int parse_result = 0;

    if (argc > 2) {
        fprintf(stderr, "usage: %s [FILE]\n", argv[0]);
        err = -1;
        goto done;
    }

    if (argc == 2) {
        input_filename = argv[1];
    }

    // Read the file to parse into input
    if (chpeg_read_file(input_filename, &input, &length) != 0) {
        fprintf(stderr, "Could not read file: %s\n", input_filename);
        err = -2;
        goto done;
    }

    // Parse the data file using calc_bytecode
    parser = ChpegParser_new(&calc_bytecode);

#if CHPEG_PACKRAT
    // set to non-zero to enable packrat
    parser->packrat = 1;
#endif

    size_t consumed = 0;
    parse_result = ChpegParser_parse(parser, input, length, &consumed);
    if (parse_result == 0) {
        if (consumed == length) {
            printf("Parse successful.\n");
        }
        ChpegParser_print_tree(parser, input, stdout);
    }
    else {
        if (parse_result == CHPEG_ERR_EXTRANEOUS_INPUT) {
            printf("Extraneous input: parse consumed %lu bytes out of %lu\n", consumed, length);
        }
        else {
            printf("Parse failed with result: %d\n", parse_result);
        }
        ChpegParser_print_error(parser, input);
        err = parse_result;
        goto done;
    }

    int value = calc_val(parser->tree_root, input);
    printf("Result: %d\n", value);

    err = 0;

done:

    if (input) {
        CHPEG_FREE(input);
        input = NULL;
    }
    if (parser) {
        ChpegParser_free(parser);
        parser = NULL;
    }

    return err;
}

