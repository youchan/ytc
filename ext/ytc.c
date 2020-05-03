#include "ruby.h"
#include "extconf.h"
#include <stdio.h>

VALUE rb_compile(VALUE self, VALUE value)
{
  rb_yield(rb_str_new_cstr(".intel_syntax noprefix\n"));
  rb_yield(rb_str_new_cstr(".global main\n"));
  rb_yield(rb_str_new_cstr("main:\n"));
  rb_yield(rb_sprintf("  mov rax, %"PRIsVALUE"\n", value));
  rb_yield(rb_str_new_cstr("  ret\n"));

  return Qnil;
}

void Init_ytc()
{
  VALUE mod = rb_define_module("Ytc");
  rb_define_method(mod, "compile", rb_compile, 1);
}
