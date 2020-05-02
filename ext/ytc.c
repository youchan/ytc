#include "ruby.h"
#include "extconf.h"
#include <stdio.h>

VALUE rb_compile(VALUE self, VALUE value)
{
  char* code = RSTRING_PTR(value);
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");
  printf("  mov rax, %d\n", atoi(code));
  printf("  ret\n");

  return Qnil;
}

void Init_ytc()
{
  VALUE mod = rb_define_module("Ytc");
  rb_define_method(mod, "compile", rb_compile, 1);
}
