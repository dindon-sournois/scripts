#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ICON_AC ""
#define ICON_L_10 ""
#define ICON_L_25 ""
#define ICON_L_50 ""
#define ICON_L_85 ""
#define ICON_GE_85 ""
#define AC_FULL "#dc322f"
#define AC_CHR "#859900"
#define BAT_WARN "#b58900"
#define BAT_CRIT "#dc322f"
#define BAT_SAFE "#859900"

FILE *fopen_check(char *path, char *mode)
{
    FILE *f;
    f = fopen(path, mode);
    if (!f) {
        perror(path);
        exit(EXIT_FAILURE);
    }
    return f;
}

int main(void)
{
    FILE *f;
    int now, full, ac_status, rounded;
    double percent;

    f = fopen_check("/sys/class/power_supply/AC/online", "r");
    fscanf (f, "%d", &ac_status);
    fclose(f);

    f = fopen_check("/sys/class/power_supply/BAT0/energy_full", "r");
    fscanf (f, "%d", &full);
    fclose(f);

    f = fopen_check("/sys/class/power_supply/BAT0/energy_now", "r");
    fscanf (f, "%d", &now);
    fclose(f);

    percent = ((double)now / full)*100;
    rounded = (int)round(percent);

    char *icon, *color;
    if (ac_status)
        {
            icon = ICON_AC;
            if (rounded >= 85)
                color = AC_FULL;
            else
                color = AC_CHR;
        }
    else if (rounded < 10)
        {
            icon = ICON_L_10;
            color = BAT_CRIT;
        }
    else if (rounded < 25)
        {
            icon = ICON_L_25;
            color = BAT_WARN;
        }
    else if (rounded < 50)
        {
            icon = ICON_L_50;
            color = BAT_SAFE;
        }
    else if (rounded < 85)
        {
            icon = ICON_L_85;
            color = BAT_SAFE;
        }
    else if (rounded >= 85)
        {
            icon = ICON_GE_85;
            color = BAT_SAFE;
        }

    /* i3blocks format */
    printf("%s %d%%\n\n%s\n", icon, rounded, color);
    return 0;
}
