# IN406-THL-TD1-BIS
Backup directory

# Description
Regex grammar & word reconnaissance

## Contains
 - REAMDE.me
 - makefile
 - test.c => main
 - automate.c
 - automate.h
 - pile.c
 - pile.h
 - regexp.c
 - regexp.h
 - sujet_partie2.pdf
 
## Explain
 - automate.c/.h => First automaton manipulation (automaton one letter, concatenantion, disjunction, star,  non-deterministic finite automate.
 - pile.c/.h => Stack.
 - regexp.c/.h => Regex manipulation & tree derivation.
 
 ## Usage
 In the makefile, you can use many commands :
 ```bash
 make
 or
 make run
 ```
 to compile and run the program with default test.
 
 ```bash
 make run2
 ```
 to compile and run with this regular expresion : "(a+b)*.a#"
 
 # Author
 LE DENMAT Mickael
 CAMBRESY Florian
 
 ## Last Update
 09/05/2020
