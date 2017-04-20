%{
extern double total_mem;
extern double free_mem;
%}

%option nounput noinput
%x TOTAL FREE CACHED BUFFERS

%%

^MemTotal BEGIN TOTAL;
<TOTAL>[0-9]+ total_mem = atoi(yytext);
<TOTAL>\n BEGIN INITIAL;
<TOTAL>. ;

^MemFree BEGIN FREE;
<FREE>[0-9]+ free_mem += atoi(yytext);
<FREE>\n BEGIN INITIAL;
<FREE>. ;

^Buffers BEGIN BUFFERS;
<BUFFERS>[0-9]+ free_mem += atoi(yytext);
<BUFFERS>\n BEGIN INITIAL;
<BUFFERS>. ;

^Cached BEGIN CACHED;
<CACHED>[0-9]+ free_mem += atoi(yytext);
<CACHED>\n BEGIN INITIAL;
<CACHED>. ;

.|\n ;

%%
