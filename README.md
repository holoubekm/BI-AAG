# BI-AAG Course

### Progtest
Folder contains solutions and source codes for assignments we were assigned to during the winter semester.

These folders contain `assignment.html` filed with instructions and description as well.

### Postfix calculator
This folder contains custom postfix calculator implementation
It was used for lecturing purposes to let students understand how basic lexer, parser and stack automaton works.
For each character in the input there is debbuging log to tell in which state the automaton is.

Supported operations:
```
+ - * /
```
Supported functions:
```
sin cos
```
There is also builtin support for decimal numbers.
**Each computation has to be finished using the EOF marker (CTRL+D on Linux)**

Using the special command `p` you can print current content of the stack

#### Example
```bash
$ ./Lexer
1 2.0 +

NUM
FINISH
[1]
NUM
DOT
DECIMAL
FINISH
[2]
FINISH
[+]
START

CTRL+D

Result: [3]
```

#### How to build
##### Linux
```bash
cd calculator
mkdir debug && cd debug
cmake ..
make
```

---
##### Linux
Open the solution placed in `lexer` folder in Visual Studio and build it.

### Script converting NFA to DFA
Script implements powerset algorithm converting nondeterministic finite-state automata to an appropriate deterministic variant.

Refer to:
https://www.tutorialspoint.com/automata_theory/ndfa_to_dfa_conversion.htm
and
https://en.wikipedia.org/wiki/Powerset_construction

Input format should be clear from examples.

#### Example
```bash
$ cat nka1.txt 
		1	2	3	eps
<>	A	B	-	-	-
	B	-	C	-	-
	C	-	-	D	-
<	D	-	-	-	-

$ python convertor.py nka1.txt
		1	2	3
<	A	B	-	-
	B	-	C	-
	C	-	-	D
<	D	-	-	-
>	S'	B	-	-
```
