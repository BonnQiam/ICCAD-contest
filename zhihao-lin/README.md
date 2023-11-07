
# Timing-Aware-Fill-Insertion
An EDA tool to insert fills into a chip, intending to make the chip satisfy design rule


## How to compile : 

1.Put all the *.h *.cpp Makefile in the same directory
2.Type "make" in the command line, then you get an executable program, whose name is 'cad'

## How to use:

1.Put the program 'cad' in the circuit directory which contains:
  circuit.config, circuit.cut, process.dat, rule.dat
  
2.usage:  
```./cad  <config_file_name>```
  note that <config_file_name> is the file name of config file, like 'circuit.config' 
  After execution, you would have a file named '*.fill' in current directory, 
  in which all the fills should be inserted are shown
