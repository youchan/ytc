file "ext/Makefile" => "ext/extconf.rb" do
  Dir.chdir("ext") do
    system "ruby extconf.rb"
  end
end

file "ext/ytc.so" => ["ext/Makefile", "ext/ytc.c"] do
  Dir.chdir("ext") do
    system "make"
  end
end

task :test => ["ext/ytc.so", "test.rb"] do
  ruby "-I. test.rb"
end

task :default => :test
