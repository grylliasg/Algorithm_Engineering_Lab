LEDA_LIB="/usr/local/LEDA"
LEDA_INCL="/usr/local/LEDA/incl/"


list: leda_list.cpp
	g++ -o leda_list leda_list.cpp  -L$(LEDA_LIB) -I$(LEDA_INCL) -lleda


stack: leda_stack.cpp
	g++ -o leda_stack leda_stack.cpp -L$(LEDA_LIB) -I$(LEDA_INCL) -lleda

run_list:
	./leda_list

run_stack:
	./leda_stack

clean:
	rm leda_list leda_stack