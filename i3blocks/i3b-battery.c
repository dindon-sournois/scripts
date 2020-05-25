#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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
    int capacity, ac_status;

    int block_button = -1;
    char * value = getenv("BLOCK_BUTTON");
    if (value)
      sscanf(value, "%d", &block_button);

    switch (block_button) {
    case (3): // right clic
      if (fork() == 0) {
        char *cmd = "/home/dindon/bin/i3b-notify-battery";
        execl(cmd, cmd, NULL);
        perror("execl");
        return 0;
      }
      break;
    default:
      break;
    }

    f = fopen_check("/sys/class/power_supply/AC/online", "r");
    fscanf (f, "%d", &ac_status);
    fclose(f);

    f = fopen_check("/sys/class/power_supply/BAT0/capacity", "r");
    fscanf (f, "%d", &capacity);
    fclose(f);

    char *icon, *color;
    if (ac_status)
        {
            icon = ICON_AC;
            if (capacity >= 85)
                color = AC_FULL;
            else
                color = AC_CHR;
        }
    else if (capacity < 10)
        {
            icon = ICON_L_10;
            color = BAT_CRIT;
        }
    else if (capacity < 25)
        {
            icon = ICON_L_25;
            color = BAT_WARN;
        }
    else if (capacity < 50)
        {
            icon = ICON_L_50;
            color = BAT_SAFE;
        }
    else if (capacity < 85)
        {
            icon = ICON_L_85;
            color = BAT_SAFE;
        }
    else if (capacity >= 85)
        {
            icon = ICON_GE_85;
            color = BAT_SAFE;
        }

    /* i3blocks format */
    printf("%s %d%%\n\n%s\n", icon, capacity, color);
    return 0;
}
