#include "UI.h"



void printBoard (int board[S_BOARD][S_BOARD])
{
    printf("\n    ______________________\n");

    for (int i = S_BOARD - 1; i >= 0; i--)
    {
        printf("%d  |", i + 1);

        for (int j = 0; j < S_BOARD; j++)
        {
           switch (board[j][i])
           {
                // print white pieces
                case w_P1 ... w_P8:
                    printf("%s%s%s  ", WHITE, displayASCII() ? "p" : "♟︎", RESET);
                    break;

                case w_dR: case w_lR:
                case w_R1xx ... w_R8xx:
                    printf("%s%s%s  ", WHITE, displayASCII() ? "R" : "♜", RESET);
                    break;

                case w_dN: case w_lN:
                case w_N1xx ... w_N8xx:
                    printf("%s%s%s  ", WHITE, displayASCII() ? "N" : "♞", RESET);
                    break;

                case w_dB: case w_lB:
                case w_B1xx ...  w_B8xx:
                    printf("%s%s%s  ", WHITE, displayASCII() ? "B" : "♝", RESET);
                    break;

                case w_Q:
                case w_Q1xx ... w_Q8xx:
                    printf("%s%s%s  ", WHITE, displayASCII() ? "Q" : "♛", RESET);
                    break;

                case w_K:
                    printf("%s%s%s  ", WHITE, displayASCII() ? "K" : "♚", RESET);
                    break;

                // print black pieces
                case b_P1 ... b_P8:
                    printf("%s%s%s  ", BROWN, displayASCII() ? "p" : "♟︎", RESET);
                    break;

                case b_dR: case b_lR:
                case b_R1xx ... b_R8xx:
                    printf("%s%s%s  ", BROWN, displayASCII() ? "R" : "♜", RESET);
                    break;

                case b_dN: case b_lN:
                case b_N1xx ... b_N8xx:
                    printf("%s%s%s  ", BROWN, displayASCII() ? "N" : "♞", RESET);
                    break;

                case b_dB: case b_lB:
                case b_B1xx ... b_B8xx:
                    printf("%s%s%s  ", BROWN, displayASCII() ? "B" : "♝", RESET);
                    break;

                case b_Q:
                case b_Q1xx ... b_Q8xx:
                    printf("%s%s%s  ", BROWN, displayASCII() ? "Q" : "♛", RESET);
                    break;

                case b_K:
                    printf("%s%s%s  ", BROWN, displayASCII() ? "K" : "♚", RESET);
                    break;

                default:
                    printf("%c  ", (i + j)  % 2 == 0 ? '.' : ' ');
                    break;
           }
        }
        printf("\b\b|");
        printf("\n");
    }

    printf("    ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
    printf("    a  b  c  d  e  f  g  h\n\n");
}


bool displayASCII(void)
{
    return useASCII(false);
}

bool useASCII (bool x)
{
    static int proceed = false;

    if (x)
        proceed = true;


    return proceed;
}
