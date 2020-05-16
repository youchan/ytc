#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ruby.h"
#include "extconf.h"

typedef enum {
  TK_RESERVED, // 記号
  TK_NUM,      // 整数トークン
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

Token *token;

void error(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buf[128];
  vsnprintf(buf, 128, fmt, ap);
  rb_raise(rb_eRuntimeError, buf);
  va_end(ap);
}

bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) {
    return false;
  }
  token = token->next;
  return true;
}

void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) {
    error("'%c'ではありません", op);
  }
  token = token->next;
}

long expect_number() {
  if (token->kind != TK_NUM) {
    rb_raise(rb_eRuntimeError, "数ではありません");
    error("数ではありません");
  }
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eol() {
  return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *current_token, char *str) {
  Token *token = calloc(1, sizeof(Token));
  token->kind = kind;
  token->str = str;
  current_token->next = token;
  return token;
}

Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *current_token = &head;
  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      current_token = new_token(TK_RESERVED, current_token, p++);
      continue;
    }

    if (isdigit(*p)) {
      current_token = new_token(TK_NUM, current_token, p);
      current_token->val = strtol(p, &p, 10);
      continue;
    }

    error("トークナイズできません");
  }

  new_token(TK_EOF, current_token, p);
  return head.next;
}

VALUE rb_compile(VALUE self, VALUE value)
{
  token = tokenize(StringValuePtr(value));

  rb_yield(rb_str_new_cstr(".intel_syntax noprefix\n"));
  rb_yield(rb_str_new_cstr(".global main\n"));
  rb_yield(rb_str_new_cstr("main:\n"));

  rb_yield(rb_sprintf("  mov rax, %ld\n", expect_number()));


  while (!at_eol()) {
    if (consume('+')) {
      rb_yield(rb_sprintf("  add rax, %ld\n", expect_number()));
      continue;
    }

    expect('-');
    rb_yield(rb_sprintf("  sub rax, %ld\n", expect_number()));
  }

  rb_yield(rb_str_new_cstr("  ret\n"));

  return Qnil;
}

void Init_ytc()
{
  VALUE mod = rb_define_module("Ytc");
  rb_define_method(mod, "compile", rb_compile, 1);
}
