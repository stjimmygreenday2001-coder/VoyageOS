/*
 * VoyageC Compiler
 * 
 * A lightweight C-like compiler with Assembly interop
 * Designed to run both on the host (for building VoyageOS)
 * and within the OS itself (for development)
 */

#include <stdint.h>
#include <stddef.h>

/* Lexer token types */
typedef enum {
    TOK_EOF = 0,
    TOK_NUMBER,
    TOK_STRING,
    TOK_IDENTIFIER,
    TOK_KEYWORD,
    TOK_OPERATOR,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_SEMICOLON,
    TOK_COMMA,
    TOK_DOT,
    TOK_COLON,
    TOK_NEWLINE,
} TokenType;

/* Keywords */
typedef enum {
    KW_VOID,
    KW_INT,
    KW_CHAR,
    KW_FLOAT,
    KW_DOUBLE,
    KW_STRUCT,
    KW_UNION,
    KW_ENUM,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_FOR,
    KW_DO,
    KW_SWITCH,
    KW_CASE,
    KW_DEFAULT,
    KW_RETURN,
    KW_BREAK,
    KW_CONTINUE,
    KW_SIZEOF,
    KW_TYPEOF,
    KW_INLINE,
    KW_ASM,
    KW_IMPORT,
    KW_EXPORT,
} KeywordType;

typedef struct {
    TokenType type;
    KeywordType keyword;
    const char *value;
    int line;
    int column;
} Token;

typedef struct {
    const char *input;
    int position;
    int line;
    int column;
    Token current_token;
} Lexer;

/* AST Node types */
typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_VARIABLE,
    NODE_BLOCK,
    NODE_IF_STMT,
    NODE_WHILE_STMT,
    NODE_FOR_STMT,
    NODE_RETURN_STMT,
    NODE_EXPRESSION,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_CALL,
    NODE_ARRAY_ACCESS,
    NODE_STRUCT_MEMBER,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    void *data;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;
} ASTNode;

/* Compiler context */
typedef struct {
    Lexer lexer;
    ASTNode *ast;
    int error_count;
    int warning_count;
} Compiler;

/* Lexer functions */
void lexer_init(Lexer *lex, const char *input) {
    lex->input = input;
    lex->position = 0;
    lex->line = 1;
    lex->column = 1;
}

Token lexer_next_token(Lexer *lex) {
    Token tok = {0};
    
    // Skip whitespace
    while (lex->input[lex->position] == ' ' ||
           lex->input[lex->position] == '\t') {
        lex->position++;
        lex->column++;
    }
    
    if (lex->input[lex->position] == '\0') {
        tok.type = TOK_EOF;
        return tok;
    }
    
    char c = lex->input[lex->position];
    
    switch (c) {
        case '(':
            tok.type = TOK_LPAREN;
            break;
        case ')':
            tok.type = TOK_RPAREN;
            break;
        case '{':
            tok.type = TOK_LBRACE;
            break;
        case '}':
            tok.type = TOK_RBRACE;
            break;
        case '[':
            tok.type = TOK_LBRACKET;
            break;
        case ']':
            tok.type = TOK_RBRACKET;
            break;
        case ';':
            tok.type = TOK_SEMICOLON;
            break;
        case ',':
            tok.type = TOK_COMMA;
            break;
        case '.':
            tok.type = TOK_DOT;
            break;
        case ':':
            tok.type = TOK_COLON;
            break;
        case '\n':
            tok.type = TOK_NEWLINE;
            lex->line++;
            lex->column = 0;
            break;
        default:
            tok.type = TOK_IDENTIFIER;
            break;
    }
    
    lex->position++;
    lex->column++;
    
    tok.line = lex->line;
    tok.column = lex->column;
    
    return tok;
}

/* Parser stub */
ASTNode *parse_program(Compiler *comp) {
    // Parse tokens into AST
    return NULL;
}

/* Code generator */
const char *codegen_x86(ASTNode *ast) {
    // Generate x86-64 assembly from AST
    return "";
}

/* Main compiler function */
int compilec_file(const char *filename, const char *source) {
    Compiler comp = {0};
    
    lexer_init(&comp.lexer, source);
    comp.ast = parse_program(&comp);
    
    if (comp.error_count > 0) {
        return 1;  // Compilation failed
    }
    
    const char *assembly = codegen_x86(comp.ast);
    
    // Save assembly to .s file
    
    return 0;  // Success
}

int main(void) {
    // Demo compilation
    const char *sample = 
        "fn main() {\n"
        "    num x = 42;\n"
        "    return x;\n"
        "}\n";
    
    return compilec_file("main.vc", sample);
}
