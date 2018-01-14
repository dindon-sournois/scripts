#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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

    int block_button = -1;
    char * value = getenv("BLOCK_BUTTON");
    if (value)
      sscanf(value, "%d", &block_button);

    switch (block_button) {
      case (4): // scroll up
        if (fork() == 0) {
          execl("/usr/bin/xbacklight", "/usr/bin/xbacklight", "-inc", "10", NULL);
          perror("execl");
          return 0;
        }
        break;
      case (5): // scroll down
        if (fork() == 0) {
          execl("/usr/bin/xbacklight", "/usr/bin/xbacklight", "-dec", "10", NULL);
          perror("execl");
          return 0;
        }
        break;
      default:
        break;
    }

    wait(NULL); // to sync display and value change

    f = fopen_check("/sys/class/backlight/intel_backlight/actual_brightness", "r");
    fscanf (f, "%d", &now);
    fclose(f);

    f = fopen_check("/sys/class/backlight/intel_backlight/max_brightness", "r");
    fscanf (f, "%d", &max);
    fclose(f);

    percent = ((double)now / max)*100;

    /* i3 blocks format */
    printf("%d%%\n", (int)round(percent));
    return 0;
}
