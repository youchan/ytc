#include "ruby.h"
#include "extconf.h"
#include <stdio.h>

VALUE rb_compile(VALUE self, VALUE value)
{
  char* code = RSTRING_PTR(value);
  VALUE return_val;
  return_val = rb_sprintf(
    ".intel_syntax noprefix\n"
    ".global main\n"
    "main:\n"
    "  mov rax, %d\n"
    "  ret\n",
    atoi(code)
  );

  return return_val;
}

void Init_ytc()
{
  VALUE mod = rb_define_module("Ytc");
  rb_define_method(mod, "compile", rb_compile, 1);
}
