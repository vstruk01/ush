#include "ush.h"

static void print_esc(char *s) {
    char temp = 27;

    write(1, &temp, 1);
    write(1, "[", 1);
    mx_printstr(s);
}

void out_monitor(int position, char *str, int count, int ch) {
    struct winsize w;
    int temp;
    char *chars = (char*)(&ch);

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    temp = (count + 5 - position) / w.ws_col;
    if ((count + 5 - position) % w.ws_col == 0 && !(chars[2] == 65 || chars[2] == 66))
        temp--;
    if  (((count + 5 - position) % w.ws_col == 0 && chars[2] == 68) || ((count + 5 - position) % w.ws_col  - w.ws_col == -1 && ch == 127))
        temp++;
    if ((count + 5 - position) % w.ws_col == 0 && ch == 127)
        temp++;
    for (int i = temp; i > 0; i--)
        print_esc("1A");
    write(1,"\r",1);
    mx_printstr("u$l = ");
    mx_printstr(str);
    mx_printstr(" ");
    for (int i = (count + 5) / w.ws_col; i > 0; i--)
        print_esc("1A");
    write(1,"\r",1);
    mx_printstr("u$l = ");
    write(1, str, count - position - 1);
    if ((count + 5 - position) % w.ws_col == 0) {
        if (position == 0)
            write(1, " ", 1);
        else
            write(1, &str[count - position - 1], 1);
    write(1, "\b", 1);
    }
}

void clean_monitor(char *str, int *table, char *new_str) {
    struct winsize w;
    int temp;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    temp = (table[2] + 5 - table[3]) / w.ws_col;
    for (int i = temp; i > 0; i--) {
        print_esc("1A");
    }
    write(1,"\r",1);
    print_esc("J");
    mx_printstr("u$l = ");
    mx_printstr(new_str);
    mx_printstr(" ");
}