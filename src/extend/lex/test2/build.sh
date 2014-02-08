#flex test.lex
#cc -o test1 lex.yy.c

bison -d Name.y
sed -i 's/typedef int YYSTYPE/typedef char* YYSTYPE/g' Name.tab.h
flex Name.lex
cc -o test2 lex.yy.c Name.tab.c
