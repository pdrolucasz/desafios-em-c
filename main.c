#include "display.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <signal.h>
#include <stdbool.h>

void signal_handler(int sig);
struct tm *timeinfo;
int numbers_clock[3];
int numbers_weather[2];
int chronus[] = {0, 0, 0};
char option;

int main()
{
again:
    setlocale(LC_ALL, "Portuguese");
    char option;
    printf("\033[1;033m");
    printf("c - para cronômetro\n");
    printf("r - para relógio\n");
    printf("Coloque a opção\n");
    scanf("%c", &option);
    if (option == 'r' || option == 'R')
    {
        time_t now;
        time(&now);
        timeinfo = localtime(&now);
        numbers_clock[0] = timeinfo->tm_hour;
        numbers_clock[1] = timeinfo->tm_min;
        numbers_clock[2] = timeinfo->tm_sec;

        while (1)
        {
            printf("Aperte ctrl+c para voltar ao menu de seleção\n");
            if (signal(SIGINT, signal_handler) == SIG_ERR)
            {
                fprintf(stderr, "signal func registration failed\n");
            }
            numbers_clock[2]++;
            transform_time(numbers_clock);
            number_time(numbers_clock[0], numbers_clock[1], numbers_clock[2]);
            printf("\033[1;032m");
            show('t');
            reset_display();
            sleep(1);
            system("clear");
            if (sig_start)
            {
                sig_start = false;
                goto again;
            }
        }
    }
    else if (option == 'c' || option == 'C')
    {
    start:
        system("clear");
        printf("+ é para contagem progressiva\n");
        printf("- é para contagem regressiva\n");
        printf("Coloque no seguinte formato.\nhh mm ss +/-\n");
        printf("Ex.: 0 0 5 -\n");
        scanf("%i %i %i %c", &chronus[0], &chronus[1], &chronus[2], &option);
        printf("%i %i %i\n", chronus[0], chronus[1], chronus[2]);
        chronometer(option, chronus);
        printf("%i %i %i\n", chronus[0], chronus[1], chronus[2]);
        if (sig_start)
        {
            sig_start = false;
            goto start;
        }
    }
    else
    {
        system("clear");
        goto again;
    }

    /*
    scanf("%i %i %i %c", &chronus[0], &chronus[1], &chronus[2], &option);
    printf("%i %i %i %c\n", chronus[0], chronus[1], chronus[2]);
    chronometer(option, chronus);
    */
    /*
    if (signal(SIGINT, signal_handler) == SIG_ERR)
    {
        fprintf(stderr, "signal func registration failed\n");
        return 1;
    }

    time_t now;
    time(&now);
    timeinfo = localtime(&now);
    numbers_clock[0] = timeinfo->tm_hour;
    numbers_clock[1] = timeinfo->tm_min;
    numbers_clock[2] = timeinfo->tm_sec;

    while (1)
    {
        numbers_clock[2]++;
        transform_time(numbers_clock);
        number_time(numbers_clock[0], numbers_clock[1], numbers_clock[2]);
        printf("\033[1;032m");
        printf("Para ativar o cronômetro aperte crtl+c\n");
        show('t');
        reset_display();
        sleep(1);
        system("clear");
        if (sig_start)
        {
            printf("+ é para contagem progressiva\n");
            printf("- é para contagem regressiva\n");
            printf("Coloque no seguinte formato.\nhh mm ss +/-\n");
            printf("Ex.: 0 0 5 -\n");
            scanf("%i %i %i %c", &chronus[0], &chronus[1], &chronus[2], &option);
            printf("%i %i %i\n", chronus[0], chronus[1], chronus[2]);
            chronometer(option, chronus);
            printf("%i %i %i\n", chronus[0], chronus[1], chronus[2]);
            break;
        }
    }
    */
    /*
    for (;;)
    {

        printf("OK!\n");
        sleep(1);
        if (sig_caught)
        {
            break;
            return 0;
        }
    }
    printf("done!\n");
    */
    return 0;
}
/*
void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        sig_start = true;
    }
}
*/
