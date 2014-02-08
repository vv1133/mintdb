%{
	int wordCount = 0;
%}
chars [A-Za-z\_\'\.\"]
numbers ([0-9])+
delim [" "\n\t]
whitespace {delim}+
words {chars}+

%%
{words} { wordCount++; }
{whitespace} { }
{numbers} { }
%%

void main(void)
{
	yylex(); /* start the analysis */
	printf (" No of words: %d\n", wordCount);
}

int yywrap(void)
{
	return 1;
}
