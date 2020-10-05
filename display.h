#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

char zero[] = {'a', 'b', 'c', 'd', 'e', 'f'};
char one[] = {'b', 'c'};
char two[] = {'a', 'b', 'g', 'e', 'd'};
char three[] = {'a', 'b', 'g', 'c', 'd'};
char four[] = {'f', 'g', 'b', 'c'};
char five[] = {'a', 'f', 'g', 'c', 'd'};
char six[] = {'a', 'c', 'd', 'e', 'f', 'g'};
char seven[] = {'a', 'b', 'c'};
char eight[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
char nine[] = {'a', 'b', 'c', 'd', 'f', 'g'};
char display_time[10][55] = {
    {"-------------------------------------------------------"},
    {"|                                                     |"},
    {"|                                                     |"},
    {"|                 o                 o                 |"},
    {"|                                                     |"},
    {"|                 o                 o                 |"},
    {"|                                                     |"},
    {"|                                                     |"},
    {"-------------------------------------------------------"}};
char display_time_reset[10][55] = {
    {"-------------------------------------------------------"},
    {"|                                                     |"},
    {"|                                                     |"},
    {"|                 o                 o                 |"},
    {"|                                                     |"},
    {"|                 o                 o                 |"},
    {"|                                                     |"},
    {"|                                                     |"},
    {"-------------------------------------------------------"}};
char display_weather[10][28] = {
    {"----------------------------"},
    {"|                    ----- |"},
    {"|                    |     |"},
    {"|                    |     |"},
    {"|                    |     |"},
    {"|                    |     |"},
    {"|                    |     |"},
    {"|                    ----- |"},
    {"----------------------------"}};
bool sig_start = false;

// show the display
void show(char option);
// change the number in display
void display_segment(char segment, int position, char option);
// convert the time
void transform_time(void *array);
// input of clock
void number_time(int hour, int minutes, int seconds);
// input of weather
void number_weather(int decimal, int unit);
//reset the display
void reset_display();
//chronometer
void chronometer(char option, void *time);

void signal_handler(int sig);

void show(char option)
{
    if (option == 't')
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 55; j++)
            {
                printf("\033[1;33m");
                printf("%c", display_time[i][j]);
            }
            printf("\n");
        }
    }
    else if (option == 's')
    {
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 28; j++)
            {
                printf("\033[1;33m");
                printf("%c", display_weather[i][j]);
            }
            printf("\n");
        }
    }
}

void display_segment(char segment, int position, char option)
{
    if (option == 't')
    {
        switch (segment)
        {
        case 'a':
            display_time[1][3 + ((position - 1)) * 9] = '-';
            display_time[1][4 + ((position - 1)) * 9] = '-';
            display_time[1][5 + ((position - 1)) * 9] = '-';
            display_time[1][6 + ((position - 1)) * 9] = '-';
            break;
        case 'b':
            display_time[2][7 + ((position - 1)) * 9] = '|';
            display_time[3][7 + ((position - 1)) * 9] = '|';
            break;
        case 'c':
            display_time[5][7 + ((position - 1)) * 9] = '|';
            display_time[6][7 + ((position - 1)) * 9] = '|';
            break;
        case 'd':
            display_time[7][3 + ((position - 1)) * 9] = '-';
            display_time[7][4 + ((position - 1)) * 9] = '-';
            display_time[7][5 + ((position - 1)) * 9] = '-';
            display_time[7][6 + ((position - 1)) * 9] = '-';
            break;
        case 'e':
            display_time[5][2 + ((position - 1)) * 9] = '|';
            display_time[6][2 + ((position - 1)) * 9] = '|';
            break;
        case 'f':
            display_time[2][2 + ((position - 1)) * 9] = '|';
            display_time[3][2 + ((position - 1)) * 9] = '|';
            break;
        case 'g':
            display_time[4][3 + ((position - 1)) * 9] = '-';
            display_time[4][4 + ((position - 1)) * 9] = '-';
            display_time[4][5 + ((position - 1)) * 9] = '-';
            display_time[4][6 + ((position - 1)) * 9] = '-';
            break;
        }
    }
    else if (option == 's')
    {
        switch (segment)
        {
        case 'a':
            display_weather[1][3 + ((position - 1)) * 9] = '-';
            display_weather[1][4 + ((position - 1)) * 9] = '-';
            display_weather[1][5 + ((position - 1)) * 9] = '-';
            display_weather[1][6 + ((position - 1)) * 9] = '-';
            break;
        case 'b':
            display_weather[2][7 + ((position - 1)) * 9] = '|';
            display_weather[3][7 + ((position - 1)) * 9] = '|';
            break;
        case 'c':
            display_weather[5][7 + ((position - 1)) * 9] = '|';
            display_weather[6][7 + ((position - 1)) * 9] = '|';
            break;
        case 'd':
            display_weather[7][3 + ((position - 1)) * 9] = '-';
            display_weather[7][4 + ((position - 1)) * 9] = '-';
            display_weather[7][5 + ((position - 1)) * 9] = '-';
            display_weather[7][6 + ((position - 1)) * 9] = '-';
            break;
        case 'e':
            display_weather[5][2 + ((position - 1)) * 9] = '|';
            display_weather[6][2 + ((position - 1)) * 9] = '|';
            break;
        case 'f':
            display_weather[2][2 + ((position - 1)) * 9] = '|';
            display_weather[3][2 + ((position - 1)) * 9] = '|';
            break;
        case 'g':
            display_weather[4][3 + ((position - 1)) * 9] = '-';
            display_weather[4][4 + ((position - 1)) * 9] = '-';
            display_weather[4][5 + ((position - 1)) * 9] = '-';
            display_weather[4][6 + ((position - 1)) * 9] = '-';
            break;
        }
    }
}

void transform_time(void *array)
{
    int(*n_array) = array;

    if (n_array[2] >= 60)
    {
        n_array[1] += n_array[2] / 60;
        n_array[2] %= 60;
    }
    if (n_array[1] >= 60)
    {
        n_array[0] += n_array[1] / 60;
        n_array[1] %= 60;
    }
}

void number_time(int hour, int minutes, int seconds)
{
    int hour_display[] = {hour / 10, hour % 10};
    int minutes_display[] = {minutes / 10, minutes % 10};
    int seconds_display[] = {seconds / 10, seconds % 10};
    /*
    printf("%i %i\n", hour_display[0], hour_display[1]);
    printf("%i %i\n", minutes_display[0], minutes_display[1]);
    printf("%i %i\n", seconds_display[0], seconds_display[1]);
    */
    switch (hour_display[0])
    {
    case 0:
        for (int i = 0; i < sizeof(zero); i++)
        {
            display_segment(zero[i], 1, 't');
        }
        break;
    case 1:
        for (int i = 0; i < sizeof(one); i++)
        {
            display_segment(one[i], 1, 't');
        }
        break;
    case 2:
        for (int i = 0; i < sizeof(two); i++)
        {
            display_segment(two[i], 1, 't');
        }
        break;
    case 3:
        for (int i = 0; i < sizeof(three); i++)
        {
            display_segment(three[i], 1, 't');
        }
        break;
    case 4:
        for (int i = 0; i < sizeof(four); i++)
        {
            display_segment(four[i], 1, 't');
        }
        break;
    case 5:
        for (int i = 0; i < sizeof(five); i++)
        {
            display_segment(five[i], 1, 't');
        }
        break;
    case 6:
        for (int i = 0; i < sizeof(six); i++)
        {
            display_segment(six[i], 1, 't');
        }
        break;
    case 7:
        for (int i = 0; i < sizeof(seven); i++)
        {
            display_segment(seven[i], 1, 't');
        }
        break;
    case 8:
        for (int i = 0; i < sizeof(eight); i++)
        {
            display_segment(eight[i], 1, 't');
        }
        break;
    case 9:
        for (int i = 0; i < sizeof(nine); i++)
        {
            display_segment(nine[i], 1, 't');
        }
        break;
    }

    switch (hour_display[1])
    {
    case 0:
        for (int i = 0; i < sizeof(zero); i++)
        {
            display_segment(zero[i], 2, 't');
        }
        break;
    case 1:
        for (int i = 0; i < sizeof(one); i++)
        {
            display_segment(one[i], 2, 't');
        }
        break;
    case 2:
        for (int i = 0; i < sizeof(two); i++)
        {
            display_segment(two[i], 2, 't');
        }
        break;
    case 3:
        for (int i = 0; i < sizeof(three); i++)
        {
            display_segment(three[i], 2, 't');
        }
        break;
    case 4:
        for (int i = 0; i < sizeof(four); i++)
        {
            display_segment(four[i], 2, 't');
        }
        break;
    case 5:
        for (int i = 0; i < sizeof(five); i++)
        {
            display_segment(five[i], 2, 't');
        }
        break;
    case 6:
        for (int i = 0; i < sizeof(six); i++)
        {
            display_segment(six[i], 2, 't');
        }
        break;
    case 7:
        for (int i = 0; i < sizeof(seven); i++)
        {
            display_segment(seven[i], 2, 't');
        }
        break;
    case 8:
        for (int i = 0; i < sizeof(eight); i++)
        {
            display_segment(eight[i], 2, 't');
        }
        break;
    case 9:
        for (int i = 0; i < sizeof(nine); i++)
        {
            display_segment(nine[i], 2, 't');
        }
        break;
    }

    switch (minutes_display[0])
    {
    case 0:
        for (int i = 0; i < sizeof(zero); i++)
        {
            display_segment(zero[i], 3, 't');
        }
        break;
    case 1:
        for (int i = 0; i < sizeof(one); i++)
        {
            display_segment(one[i], 3, 't');
        }
        break;
    case 2:
        for (int i = 0; i < sizeof(two); i++)
        {
            display_segment(two[i], 3, 't');
        }
        break;
    case 3:
        for (int i = 0; i < sizeof(three); i++)
        {
            display_segment(three[i], 3, 't');
        }
        break;
    case 4:
        for (int i = 0; i < sizeof(four); i++)
        {
            display_segment(four[i], 3, 't');
        }
        break;
    case 5:
        for (int i = 0; i < sizeof(five); i++)
        {
            display_segment(five[i], 3, 't');
        }
        break;
    case 6:
        for (int i = 0; i < sizeof(six); i++)
        {
            display_segment(six[i], 3, 't');
        }
        break;
    case 7:
        for (int i = 0; i < sizeof(seven); i++)
        {
            display_segment(seven[i], 3, 't');
        }
        break;
    case 8:
        for (int i = 0; i < sizeof(eight); i++)
        {
            display_segment(eight[i], 3, 't');
        }
        break;
    case 9:
        for (int i = 0; i < sizeof(nine); i++)
        {
            display_segment(nine[i], 3, 't');
        }
        break;
    }

    switch (minutes_display[1])
    {
    case 0:
        for (int i = 0; i < sizeof(zero); i++)
        {
            display_segment(zero[i], 4, 't');
        }
        break;
    case 1:
        for (int i = 0; i < sizeof(one); i++)
        {
            display_segment(one[i], 4, 't');
        }
        break;
    case 2:
        for (int i = 0; i < sizeof(two); i++)
        {
            display_segment(two[i], 4, 't');
        }
        break;
    case 3:
        for (int i = 0; i < sizeof(three); i++)
        {
            display_segment(three[i], 4, 't');
        }
        break;
    case 4:
        for (int i = 0; i < sizeof(four); i++)
        {
            display_segment(four[i], 4, 't');
        }
        break;
    case 5:
        for (int i = 0; i < sizeof(five); i++)
        {
            display_segment(five[i], 4, 't');
        }
        break;
    case 6:
        for (int i = 0; i < sizeof(six); i++)
        {
            display_segment(six[i], 4, 't');
        }
        break;
    case 7:
        for (int i = 0; i < sizeof(seven); i++)
        {
            display_segment(seven[i], 4, 't');
        }
        break;
    case 8:
        for (int i = 0; i < sizeof(eight); i++)
        {
            display_segment(eight[i], 4, 't');
        }
        break;
    case 9:
        for (int i = 0; i < sizeof(nine); i++)
        {
            display_segment(nine[i], 4, 't');
        }
        break;
    }

    switch (seconds_display[0])
    {
    case 0:
        for (int i = 0; i < sizeof(zero); i++)
        {
            display_segment(zero[i], 5, 't');
        }
        break;
    case 1:
        for (int i = 0; i < sizeof(one); i++)
        {
            display_segment(one[i], 5, 't');
        }
        break;
    case 2:
        for (int i = 0; i < sizeof(two); i++)
        {
            display_segment(two[i], 5, 't');
        }
        break;
    case 3:
        for (int i = 0; i < sizeof(three); i++)
        {
            display_segment(three[i], 5, 't');
        }
        break;
    case 4:
        for (int i = 0; i < sizeof(four); i++)
        {
            display_segment(four[i], 5, 't');
        }
        break;
    case 5:
        for (int i = 0; i < sizeof(five); i++)
        {
            display_segment(five[i], 5, 't');
        }
        break;
    case 6:
        for (int i = 0; i < sizeof(six); i++)
        {
            display_segment(six[i], 5, 't');
        }
        break;
    case 7:
        for (int i = 0; i < sizeof(seven); i++)
        {
            display_segment(seven[i], 5, 't');
        }
        break;
    case 8:
        for (int i = 0; i < sizeof(eight); i++)
        {
            display_segment(eight[i], 5, 't');
        }
        break;
    case 9:
        for (int i = 0; i < sizeof(nine); i++)
        {
            display_segment(nine[i], 5, 't');
        }
        break;
    }

    switch (seconds_display[1])
    {
    case 0:
        for (int i = 0; i < sizeof(zero); i++)
        {
            display_segment(zero[i], 6, 't');
        }
        break;
    case 1:
        for (int i = 0; i < sizeof(one); i++)
        {
            display_segment(one[i], 6, 't');
        }
        break;
    case 2:
        for (int i = 0; i < sizeof(two); i++)
        {
            display_segment(two[i], 6, 't');
        }
        break;
    case 3:
        for (int i = 0; i < sizeof(three); i++)
        {
            display_segment(three[i], 6, 't');
        }
        break;
    case 4:
        for (int i = 0; i < sizeof(four); i++)
        {
            display_segment(four[i], 6, 't');
        }
        break;
    case 5:
        for (int i = 0; i < sizeof(five); i++)
        {
            display_segment(five[i], 6, 't');
        }
        break;
    case 6:
        for (int i = 0; i < sizeof(six); i++)
        {
            display_segment(six[i], 6, 't');
        }
        break;
    case 7:
        for (int i = 0; i < sizeof(seven); i++)
        {
            display_segment(seven[i], 6, 't');
        }
        break;
    case 8:
        for (int i = 0; i < sizeof(eight); i++)
        {
            display_segment(eight[i], 6, 't');
        }
        break;
    case 9:
        for (int i = 0; i < sizeof(nine); i++)
        {
            display_segment(nine[i], 6, 't');
        }
        break;
    }
}

void reset_display()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 55; j++)
        {
            display_time[i][j] = display_time_reset[i][j];
        }
    }
}

void chronometer(char option, void *array)
{
    int(*time) = array;
    int chronometer[] = {0, 0, 0};
    if (option == '+')
    {

        while (1)
        {
            if (signal(SIGINT, signal_handler) == SIG_ERR)
            {
                fprintf(stderr, "signal func registration failed\n");
            }
            printf("Aperte ctrl+c para voltar ao menu de seleção\n");
            transform_time(time);
            transform_time(chronometer);
            //printf("%i %i %i\n", chronometer[0], chronometer[1], chronometer[2]);
            //printf("%i %i %i\n", time[0], time[1], time[2]);

            if (time[0] == chronometer[0] && time[1] == chronometer[1] && time[2] == chronometer[2])
            {
                number_time(chronometer[0], chronometer[1], chronometer[2]);
                show('t');
                break;
            }
            number_time(chronometer[0], chronometer[1], chronometer[2]);
            show('t');
            reset_display();
            sleep(1);
            system("clear");
            chronometer[2]++;
            if (sig_start)
            {
                break;
            }
        }
    }
    if (option == '-')
    {
        transform_time(time);
        chronometer[0] = time[0];
        chronometer[1] = time[1];
        chronometer[2] = time[2];
        //printf("%i %i %i\n", chronometer[0], chronometer[1], chronometer[2]);
        //printf("%i %i %i\n", time[0], time[1], time[2]);

        while (1)
        {
            printf("Aperte ctrl+c para voltar ao menu de seleção\n");
            if (signal(SIGINT, signal_handler) == SIG_ERR)
            {
                fprintf(stderr, "signal func registration failed\n");
            }
            //printf("%i %i %i\n", chronometer[0], chronometer[1], chronometer[2]);
            //printf("%i %i %i\n", time[0], time[1], time[2]);
            number_time(chronometer[0], chronometer[1], chronometer[2]);
            show('t');
            reset_display();
            sleep(1);
            system("clear");
            if (chronometer[0] == 0 && chronometer[1] == 0 && chronometer[2] == 0)
            {
                number_time(chronometer[0], chronometer[1], chronometer[2]);
                show('t');
                break;
            }
            if (chronometer[0] != 0 && chronometer[1] == 0)
            {

                chronometer[0]--;
                chronometer[1] = 59;
                chronometer[2] = 59;
                number_time(chronometer[0], chronometer[1], chronometer[2]);
                show('t');
                reset_display();
                sleep(1);
                system("clear");
            }
            if (chronometer[1] != 0 && chronometer[2] == 0)
            {
                chronometer[1]--;
                chronometer[2] = 59;
                number_time(chronometer[0], chronometer[1], chronometer[2]);
                show('t');
                reset_display();
                sleep(1);
                system("clear");
            }
            chronometer[2]--;
            if (sig_start)
            {
                break;
            }
        }
    }
}

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        sig_start = true;
    }
}