#include "ush.h"

void mx_segfault() {
    mx_printerr("OOPS! SEGMENT FAULT\n");
    exit(1);
}
void mx_ctrl_c() {
    // t_info *info = mx_get_info(0);
    // t_list *tmp = info->processes;
    // pid_t pid = 0;
    // while(tmp) {
    //     pid = (pid_t) tmp->data;
    //     kill(pid, SIGKILL);
    // }
    printf("CTRL+C\n");
}


static void init_var() {
    // int initCount = 10;
    
    // *variables = (t_variable **) malloc (sizeof(t_variable*) * initCount); // initCount -vars
    // for (int i = 0; i < initCount; i++) {
    //     (*variables)[i] = (t_variable *) malloc (sizeof(t_variable));
    // }
}

static void shlvl_new(char **s) {
    int a = 0;
    char *temp = 0;

    for (int i = 0; i < mx_strlen(*s); i++){
        a = a * 10 + (*s[i] - '0');
    }
    mx_strdel(s);
    temp = mx_itoa(++a);
    *s = temp;
}


static void start_program(t_list **var_tree, char **env) {
    int i = -1;
    char **envvar = 0;
    char *temp = 0;
    t_variable *var = 0;

    while (env[++i]) {
        var = (t_variable*) malloc(sizeof(t_variable));
        envvar = mx_strsplit(env[i], '=');
        if (mx_strcmp(envvar[0], "SHLVL") == 0) {
            shlvl_new(&envvar[1]);
            temp = mx_strjoin("SHLVL=", envvar[1]);
        }
        else if (mx_strcmp(envvar[0], "PWD") == 0) {
            mx_strdel(&envvar[1]);
            envvar[1] = getcwd(NULL, 0);
            temp = mx_strjoin("PWD=", envvar[1]);
        }
        else if (mx_strcmp(envvar[0], "OLDPWD") == 0) {
            mx_strdel(&envvar[1]);
            envvar[1] = getcwd(NULL, 0);
            temp = mx_strjoin("OLDPWD=", envvar[1]);
        }
        else
            temp = mx_strdup(env[i]);
        var->is_env = true;
        var->name = envvar[0];
        var->value = envvar[1];
        var->mem = temp;
        putenv(temp);
        mx_push_front(var_tree, var);
        free(envvar);
    }
}

void mx_ush_init(t_info **info, char **env) {
    t_info *newInfo = (t_info*) malloc(sizeof(t_info));
    newInfo->env = env;
    newInfo->processes = (pid_t*) malloc(sizeof(pid_t) * 11);
    char *buff = 0;
    int i = 0;
    for(i = 0; i < 10; i++)
        newInfo->processes[i] = 0; // empty
    newInfo->isExit = false;
    newInfo->exit_status = 0;
    newInfo->path = mx_strdup(getenv("PATH"));
    newInfo->pwd = getcwd(NULL, 0);
    newInfo->pwdP = getcwd(NULL, 0);
    newInfo->pwdL = getcwd(NULL, 0);
    newInfo->old_pwd = getcwd(NULL, 0);
    //  printf("pwd = %s\n", newInfo->old_pwd);
    newInfo->home = mx_strdup(getenv("HOME"));
    (*info) = newInfo;
    start_program(&(newInfo->var_tree), env);
    // mx_get_info(*info);
    signal(SIGSEGV, mx_segfault);
    signal(SIGINT, mx_ctrl_c);
}
