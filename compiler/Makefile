all: bin/compiler

bin/compiler : build/y.tab.c build/lex.yy.c  src/AbsSynTree.c include/AbsSynTree.h src/symbolTable.c include/symbolTable.h | build bin
	@gcc build/lex.yy.c build/y.tab.c src/symbolTable.c src/AbsSynTree.c -o bin/compiler

build/y.tab.c : src/compiler.y | build
	@yacc -d src/compiler.y -o build/y.tab.c

build/lex.yy.c : src/compiler.l build/y.tab.h | build
	@lex -o build/lex.yy.c src/compiler.l 	

build/y.tab.h : src/compiler.y | build
	@yacc -d src/compiler.y -o src/y.tab.c 

build:
	@mkdir -p build
	
bin: 
	@mkdir -p bin
 
test1 : bin/compiler
	@./bin/compiler < testcases/test1.sil
test2 : bin/compiler
	@./bin/compiler < testcases/test2.sil
test3 : bin/compiler
	@./bin/compiler < testcases/test3.sil
test4 : bin/compiler
	@./bin/compiler < testcases/test4.sil
test5 : bin/compiler
	@./bin/compiler < testcases/test5.sil
test6 : bin/compiler
	@./bin/compiler < testcases/test6.sil
test7 : bin/compiler
	@./bin/compiler < testcases/test7.sil
test8 : bin/compiler
	@./bin/compiler < testcases/test8.sil
test9 : bin/compiler
	@./bin/compiler < testcases/test9.sil
test10 : bin/compiler
	@./bin/compiler < testcases/test10.sil
test11 : bin/compiler
	@./bin/compiler < testcases/test11.sil

clean: 
	rm -rf bin build

.PHONY:  clean test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11
