/*
 * Copyright (C) 2014,2015 levi0x0 with enhancements by ProgrammerNerd
 *
 * barM (bar_monitor or BarMonitor) is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 *  This is a new version of bar monitor, even less lines of code more effective.
 *
 *  Read main() to configure your new status Bar.
 *
 *  compile: gcc -o barM barM.c -O2 -s -lX11
 *
 *  mv barM /usr/local/bin/
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <X11/Xlib.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"
#define MAXSTR  1024

static const char * date(void);
static const char * ram(void);
static void XSetRoot(const char *name);
/*Append here your functions.*/
static const char*(*const functab[])(void)={
        ram,date
};

int main(int argc, char *argv[]){
        char status[MAXSTR];
        char* off = status;
        int left = sizeof(status);
        char* sta = off;
        for(int i = 1; i < argc && left > 0;  ++i) {
                int ret = snprintf(sta, left,"[%s] ", argv[i]);
                sta += ret;
                left -= ret;
        }
        int funtab_size = sizeof(functab)/sizeof(functab[0]);
        for(int i = 0; i < funtab_size && left > 0; ++i ) {
                int ret = snprintf(sta, left, "[%s] ", functab[i]());
                sta += ret;
                left -= ret;
        }
        XSetRoot(status);
        printf("%s\n", status);
        return 0;
}

/* Returns the date*/
static const char * date(void){
        static char date[MAXSTR];
        time_t now = time(0);

        strftime(date, MAXSTR, TIME_FORMAT, localtime(&now));
        return date;
}
/* Returns a string that contains the amount of free and available ram in megabytes*/
static const char * ram(void){
        static char ram[MAXSTR];
        struct sysinfo s;
        sysinfo(&s);
        snprintf(ram,sizeof(ram),"%.1f%% %.1fG",
                 (((double)s.freeram)/((double)s.totalram))*100,
                  ((double)s.freeram)/1048576./1024.);
        return ram;
}

static void XSetRoot(const char *name){
        Display *display;

        if (( display = XOpenDisplay(0x0)) == NULL ) {
                fprintf(stderr, "[barM] cannot open display!\n");
                exit(1);
        }

        XStoreName(display, DefaultRootWindow(display), name);
        XSync(display, 0);

        XCloseDisplay(display);
}

