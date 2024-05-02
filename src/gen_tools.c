#include "../header/gen_tools.h"


int popFile (char *filename, int delete_ch)
{
    FILE *file = fopen(filename, "r");
    FILE *tmp_file = fopen("tmp.txt", "w");

    if (!file)
        return 1;


    int length = 0;
    int remain = 0;

    fseek(file, 0, SEEK_END);
    length = ftell(file);

    remain = length - delete_ch;

    rewind(file);

    for (int i = 0; i < remain; i++)
    {
        int ch = fgetc(file);
        fputc(ch, tmp_file);
    }


    fclose(tmp_file);
    fclose(file);


    file = fopen(filename, "w");
    tmp_file = fopen("tmp.txt", "r");

    for (int i = 0; i < remain; i++)
    {
        int ch = fgetc(tmp_file);
        fputc(ch, file);
    }

    fclose(tmp_file);
    fclose(file);

    tmp_file = NULL;
    file = NULL;

    remove("tmp.txt");

    return 0;
}


#ifdef _WIN32

    void clear (void)
    {
        system("cls");
    }

#else

    void clear (void)
    {
        system("clear");
    }

#endif



void slowText (const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i]);
        fflush(stdout);
        usleep(5000);
    }
}


void stringInput (char input[], int maxLen)
{
    int i = 0;
    int ch = 0;


    while (1)
    {
        ch = getchar();

        if (ch == '\n')
            break;

        else if (i < maxLen - 1 && ch != EOF)
        {
            input[i] = ch;
            i++;
        }
    }

    input[i] = '\0';
}

void getTime (char *str, const int maxLen)
{
    char time_string[MAX_CHAR];
    
    time_t universalTime;
    struct tm *localTime;

    time(&universalTime);

    localTime = localtime(&universalTime);

    strftime(time_string, MAX_CHAR, "%Y-%M-%d %H:%M:%S", localTime);

    strncpy(str, time_string, maxLen);
    str[maxLen - 1] = '\0';
}


void pushbackStr (char *str, char *push_ch, const int maxLen)
{
    strncat(str, push_ch, maxLen);
    str[maxLen - 1] = '\0';
}



