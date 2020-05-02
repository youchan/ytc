require "ytc"

include Ytc

def assert(expected, input)
  asm = compile(input)
  File.write("tmp.s", asm)
  system("cc -o tmp tmp.s")
  system("./tmp")
  actual = $?.exitstatus

  if actual == expected
    puts "#{input} => #{actual}"
  else
    puts "#{input} => #{expected} expected, but got #{actual}"
    exit 1
  end
end

assert(0, "0")
assert(42, "42")

puts "OK"
