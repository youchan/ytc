require "ytc"

include Ytc

def assert(expected, input)
  File.open("tmp/tmp.s", "w") do |tmp|
    compile(input) do |str|
      tmp << str
    end
  end
  system("cc -o tmp/tmp tmp/tmp.s")
  system("./tmp/tmp")
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
