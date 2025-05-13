
require 'optparse'

def printNumber(num)
	print "%03d %08b %03o %02X %s\n" % [num,num,num,num, toNegative(num)]
end	


def printNumberLatex(num)
	print "$%03d$&$%08b$&$%03o$&$%02X$&$%s$\\\\\n" % [num,num,num,num,toNegative(num)]
end	


count=20
def printConsole
	print "dec bin      oct hex\n"
	i=0
	$foo.each do |num|
		if (i==5)
			print "\n"
			i=0
		end
		printNumber(num)
		i+=1
	end
end

def printConsoleDec
	print "dec\n"
	$foo.each do |num|
		print"%03d\n" %[num]
	end
end


def toNegative (num)
	invert=false
	rv ="";
	foo="%08b" %num
	foo.split("").reverse.each do |val|
		if (invert==true)
			if (val=="0")
				rv="1"+rv
			else
				rv="0"+rv
			end		
		else
			if (val=="0")
				rv=val+rv
			else
				rv=val+rv
				invert=true
			end			
		end
	end
	rv
end

def printLatex
	print"\\begin{tabular}{c|c|c|c|c}\n"
    print"dec & bin & oct & hex & negative \\\\\\hline\n" 
	$foo.each do |num|
		printNumberLatex(num)
	end
	  print"\\end{tabular}\n"
end


latex=false
interactive=false

OptionParser.new do |opts|
  opts.banner = "Usage: generator.rb [options]"
  opts.on("-l", "--latex", "Generate LaTex Table") {latex=true}
  opts.on("-i", "--interactive", "Run the program in the interactive mode") {interactive=true}
  opts.on("-n", "--num=val", "Number of values to generate", Integer) {|val| count=val}
end.parse!


$foo=Array.new(count) { rand(20...127) }

if (latex==true)
	printLatex
elsif (interactive==true)
	printConsoleDec
	print "Show results? (type \"yes\" to show)\n"
	while (STDIN.gets!="yes\n")
		print "Show results? (type \"yes\" to show)\n"
	end
	printConsole
else
	printConsole
end
	