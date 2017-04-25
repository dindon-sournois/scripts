#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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
    int now, max;
    double percent;

    f = fopen_check("/sys/class/backlight/intel_backlight/actual_brightness", "r");
    fscanf (f, "%d", &now);
    fclose(f);

    f = fopen_check("/sys/class/backlight/intel_backlight/max_brightness", "r");
    fscanf (f, "%d", &max);
    fclose(f);

    percent = ((double)now / max)*100;

    /* i3 blocks format */
    printf("%d%%\n", (int)round(percent));
    printf("%d%%\n", (int)round(percent));
    return 0;
}
