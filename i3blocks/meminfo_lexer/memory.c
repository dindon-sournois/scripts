#include <stdio.h>

#define COL_WARN "#b58900"
#define COL_CRIT "#dc322f"
#define COL_SAFE "#93a1a1"

extern int yylex(void);
extern FILE* yyin;

double total_mem;
double free_mem;

int main()
{
    yyin = fopen("/proc/meminfo", "r");
    yylex();

    double used_mem = (total_mem - free_mem) / 1024 / 1024;
    int percent = ((total_mem - free_mem) / total_mem) * 100;
    free_mem = free_mem / 1024 / 1024;

    char *color;
    if (percent > 85)
        color = COL_CRIT;
    else if (percent > 70)
        color = COL_WARN;
    else
        color = COL_SAFE;

    /* i3blocks pango format */
    printf("<span foreground=\"%s\"> %.1fG</span>\n", color, free_mem);
    return 0;
}
