                                                    /*** Chess: ***/
                                                /*** By: FBIAgent05 ***/
                                                /* 開始: Feb 20, 2024*/
                                                /* 終了: Apr 20, 2024*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>


#define S_BOARD 8

// including promoted pieces
#define N_PCS 48
#define MAX_CHAR 8

// white check as in, check if white is in check. Not check if white checks black
#define WHITE_CHECK -2
#define BLACK_CHECK -3
#define CHECK -5
#define CHECKMATE -10
#define STALEMATE -9
#define SCAN -12
#define VISIBLE -13
#define MAX_PCS 10
#define BLOCK 12
#define ASSASSINATE 45
#define ASSASSINATE_FAIL 46
#define ASSASSINATE_SUCCESS 47
#define MOBILE 101
#define IMMOBILE 102

#define READ 122
#define WRITE 123
#define WIPE 202

#define WHITE_P -23
#define BLACK_P -24


#define SHORT_CASTLE 2
#define LONG_CASTLE 3
#define PREVENT_CASTLE 5

#define RED "\033[31m"
#define WHITE "\033[38;2;255;253;208m"
#define BROWN "\033[38;2;150;75;0m"
#define RESET "\033[0m"

enum {a, b, c, d, e, f, g, h};
enum {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

// w -> white
// b -> black
// enums are already constant, so const isn't necessary
enum p_white 
{
    w_P1 = 100,
    w_P2 = 101,
    w_P3 = 102,
    w_P4 = 103,
    w_P5 = 104, 
    w_P6 = 105, 
    w_P7 = 106, 
    w_P8 = 107, 
    w_dR = 108, 
    w_lR = 109, 
    w_lN = 110, 
    w_dN = 111, 
    w_dB = 112, 
    w_lB = 113, 
    w_Q = 114, 
    w_K = 115
};

enum p_black 
{
    b_P1 = 200, 
    b_P2 = 201, 
    b_P3 = 202, 
    b_P4 = 203, 
    b_P5 = 204, 
    b_P6 = 205, 
    b_P7 = 206, 
    b_P8 = 207, 
    b_dR = 208, 
    b_lR = 209, 
    b_lN = 210, 
    b_dN = 211, 
    b_dB = 212, 
    b_lB = 213, 
    b_Q = 214, 
    b_K = 215
};


enum p_white_promotion
{
    w_Q1xx = 500,
    w_Q2xx = 501,
    w_Q3xx = 502,
    w_Q4xx = 503,
    w_Q5xx = 504,
    w_Q6xx = 505,
    w_Q7xx = 506,
    w_Q8xx = 507,

    w_R1xx = 508,
    w_R2xx = 509,
    w_R3xx = 510,
    w_R4xx = 511,
    w_R5xx = 512,
    w_R6xx = 513,
    w_R7xx = 514,
    w_R8xx = 515,

    w_B1xx = 516,
    w_B2xx = 517,
    w_B3xx = 518,
    w_B4xx = 519,
    w_B5xx = 520,
    w_B6xx = 521,
    w_B7xx = 522,
    w_B8xx = 523,


    w_N1xx = 524,
    w_N2xx = 525,
    w_N3xx = 526,
    w_N4xx = 527,
    w_N5xx = 528,
    w_N6xx = 529,
    w_N7xx = 530,
    w_N8xx = 531,
};



enum p_black_promotion
{
    b_Q1xx = 600,
    b_Q2xx = 601,
    b_Q3xx = 602,
    b_Q4xx = 603,
    b_Q5xx = 604,
    b_Q6xx = 605,
    b_Q7xx = 606,
    b_Q8xx = 607,

    b_R1xx = 608,
    b_R2xx = 609,
    b_R3xx = 610,
    b_R4xx = 611,
    b_R5xx = 612,
    b_R6xx = 613,
    b_R7xx = 614,
    b_R8xx = 615,

    b_B1xx = 616,
    b_B2xx = 617,
    b_B3xx = 618,
    b_B4xx = 619,
    b_B5xx = 620,
    b_B6xx = 621,
    b_B7xx = 622,
    b_B8xx = 623,


    b_N1xx = 624,
    b_N2xx = 625,
    b_N3xx = 626,
    b_N4xx = 627,
    b_N5xx = 628,
    b_N6xx = 629,
    b_N7xx = 630,
    b_N8xx = 631
};

typedef struct 
{
    int piece;
    char tile;

    int x;
    int y;
}
coords;


void markBoard(char back_board[S_BOARD][S_BOARD]);
void initBoard(int board[S_BOARD][S_BOARD]);
void printBoard (char back_board[S_BOARD][S_BOARD], int board[S_BOARD][S_BOARD]);

int moveBoard(int board[S_BOARD][S_BOARD]);


int movePawn(int board[S_BOARD][S_BOARD], int orig_let, char capture, int dest_let, int dest_num, int promote_piece, const int side);
int moveRook (int board[S_BOARD][S_BOARD], int orig_let, char orig_num, int dest_let, int dest_num, char capture, const int side, bool *W_queens_rook_moved, bool *W_kings_rook_moved, bool *B_queens_rook_moved, bool *B_kings_rook_moved);
int moveBishop (int board[S_BOARD][S_BOARD], int orig_let, char orig_num, int dest_let, int dest_num, char capture, const int side);
int moveKnight (int board[S_BOARD][S_BOARD], int orig_let, char orig_num, int dest_let, int dest_num, char capture, const int side);
int moveQueen (int board[S_BOARD][S_BOARD], int orig_let, char orig_num, int dest_let, int dest_num, char capture, const int side);
int moveKing (int board[S_BOARD][S_BOARD], int dest_let, int dest_num, char capture, const int side, bool *W_king_moved, bool *B_king_moved);


// pieces sight function as from their first person perspective
// regulates if king is check
// also takes care of preventing castling
int sightPawn (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior);
int sightRook (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior);
int sightBishop (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior);
int sightKnight (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior);
int sightQueen (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior);
int sightKing (int board[S_BOARD][S_BOARD], const int king, const int purpose);


int capturePawn (int board[S_BOARD][S_BOARD], coords find_pawn[], const int cnt_pawn, const int orig_x, const int dest_x, const int dest_y, const int king);
int promotionPawn (int board[S_BOARD][S_BOARD], const int promote_piece, const int orig_let, const int dest_let, const int dest_num, const int king);
int recentDoubleSquarePawn (int pawn, int mode);
int castling (int board[S_BOARD][S_BOARD], bool *W_king_moved, bool *W_queens_rook_moved, bool *W_kings_rook_moved, bool *B_king_moved, bool *B_queens_rook_moved, bool *B_kings_rook_moved, const int side, const int long_or_short);
int checkmate (int board[S_BOARD][S_BOARD], const int side);

int stalemate (int board[S_BOARD][S_BOARD], const int side);
int mobilityPawn (int board[S_BOARD][S_BOARD], const int side);
int mobilityRook (int board[S_BOARD][S_BOARD], const int side);
int mobilityBishop (int board[S_BOARD][S_BOARD], const int side);
int mobilityKnight (int board[S_BOARD][S_BOARD], const int side);
int mobilityQueen (int board[S_BOARD][S_BOARD], const int side);
int mobilityKing  (int board[S_BOARD][S_BOARD], const int side);


int theAllSeeingEye (int board[S_BOARD][S_BOARD], const int king, const int purpose);
int theCheckmateLastStand (int board[S_BOARD][S_BOARD], const int king, const int x_scan, const int y_scan, coords *savior,  int purpose);
int selectPiece (int piece, const int king);
void *findPiece(coords find_piece[], int board[S_BOARD][S_BOARD], int collect_piece, int *cnt, const int king);
void *findSavior(int board[S_BOARD][S_BOARD], int x_scan, int y_scan, coords *savior);


void welcome (void);
void slowText (const char *str);

void stringInput (char input[], int maxLen);
bool isRangeLet (char let);
bool isRangeNum (char num);
bool checkBounds (int n);
int isaPiece (char piece);

void clear (void);
int Pos (int n);

int main(void)
{
    clear( );
    welcome();
    
    clear();
    
    char back_board[S_BOARD][S_BOARD];
    memset(back_board, 0, sizeof back_board);

    int board[S_BOARD][S_BOARD];
    memset(board, 0, sizeof board);



    markBoard(back_board);
    initBoard(board);


    while (1)
    {
        printBoard(back_board, board);
        moveBoard(board);

        clear();
    }

    return 0;
}

void markBoard (char back_board[S_BOARD][S_BOARD])
{
    for (int i = 0; i < S_BOARD; i++)
    {
        for (int j = 0; j < S_BOARD; j++)
        {
            if ((i + j) % 2 == 0)
                back_board[j][i] = '+';

            else    
                back_board[j][i] = '.';
        }
    }
}


void initBoard(int board[S_BOARD][S_BOARD])
{
    // clear board by filling with zeros
    for (int i = 0; i < S_BOARD; i++)
    {
        for (int j = 0; j < S_BOARD; j++)
        {
            board[j][i] = 0;
        }
    }
    
    // initialize white pieces

    
    board[a][Pos(1)] = w_dR;
    board[b][Pos(1)] = w_lN;
    board[c][Pos(1)] = w_dB;
    board[d][Pos(1)] = w_Q;
    board[e][Pos(1)] = w_K;
    board[f][Pos(1)] = w_lB;
    board[g][Pos(1)] = w_dN;
    board[h][Pos(1)] = w_lR;


    // initialize black pieces
    board[a][Pos(8)] = b_lR;
    board[b][Pos(8)] = b_dN;
    board[c][Pos(8)] = b_lB;
    board[d][Pos(8)] = b_Q;
    board[e][Pos(8)] = b_K;
    board[f][Pos(8)] = b_dB;
    board[g][Pos(8)] = b_lN;
    board[h][Pos(8)] = b_dR;


    // initialize pawns on both sides
    for (int i = 0; i < S_BOARD; i++)
    {
        board[i][Pos(2)] = 100 + i;
        board[i][Pos(7)] = 200 + i;
    }

}


void printBoard (char back_board[S_BOARD][S_BOARD], int board[S_BOARD][S_BOARD])
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
                    printf("%s%s%s  ", WHITE, "🨅", RESET);
                    break;

                case w_dR: case w_lR:
                case w_R1xx ... w_R8xx:
                    printf("%s%s%s  ", WHITE, "🨂", RESET);
                    break;

                case w_dN: case w_lN:
                case w_N1xx ... w_N8xx:
                    printf("%s%s%s  ", WHITE, "🨄", RESET);
                    break;

                case w_dB: case w_lB:
                case w_B1xx ...  w_B8xx:
                    printf("%s%s%s  ", WHITE, "🨃", RESET);
                    break;

                case w_Q:
                case w_Q1xx ... w_Q8xx:
                    printf("%s%s%s  ", WHITE, "🨁", RESET);
                    break;

                case w_K:
                    printf("%s%s%s  ", WHITE, "🨀", RESET);
                    break;

                // print black pieces
                case b_P1 ... b_P8:
                    printf("%s%s%s  ", BROWN, "🨅", RESET);
                    break;

                case b_dR: case b_lR:
                case b_R1xx ... b_R8xx:
                    printf("%s%s%s  ", BROWN, "🨂", RESET);
                    break;

                case b_dN: case b_lN:
                case b_N1xx ... b_N8xx:
                    printf("%s%s%s  ", BROWN, "🨄", RESET);
                    break;

                case b_dB: case b_lB:
                case b_B1xx ... b_B8xx:
                    printf("%s%s%s  ", BROWN, "🨃", RESET);
                    break;

                case b_Q:
                case b_Q1xx ... b_Q8xx:
                    printf("%s%s%s  ", BROWN, "🨁", RESET);
                    break;

                case b_K:
                    printf("%s%s%s  ", BROWN, "🨀", RESET);
                    break; 

                default:
                    printf("%c  ", back_board[j][i]);
                    break;
           }
        }
        printf("\b\b|");
        printf("\n");
    }

    printf("    ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\n");
    printf("    a  b  c  d  e  f  g  h\n\n");
}

int moveBoard(int board[S_BOARD][S_BOARD])
{
    bool invalid_move = true;

    static int count_moves = 0;
    
    
    int undo_board[S_BOARD][S_BOARD];
    memcpy(undo_board, board, sizeof undo_board);


    char input[MAX_CHAR] = {0};
    static char white_previous_input[MAX_CHAR] = {0};
    static char black_previous_input[MAX_CHAR] = {0};

    char what_side[MAX_CHAR] = {0};
    char deliver_side[MAX_CHAR] = {0};

    int king = 0;

    int side = count_moves % 2;


    if (side)
     king = b_K;
    
    else
     king = w_K;

    
    printf("History: \n");
    printf("W >> %s\n", white_previous_input);
    printf("B >> %s\n", black_previous_input);



    switch  (king)
    {
        case w_K: 
            strncpy(what_side, "White", MAX_CHAR);
            what_side[MAX_CHAR - 1] = '\0';

            strncpy(deliver_side, "Black", MAX_CHAR);
            deliver_side[MAX_CHAR - 1] = '\0';
            break;

        case b_K: 
            strncpy(what_side, "Black", MAX_CHAR);
            what_side[MAX_CHAR - 1] = '\0'; 

            strncpy(deliver_side, "White", MAX_CHAR);
            deliver_side[MAX_CHAR - 1] = '\0';
            break;
    }





    if (checkmate(board, side) == CHECKMATE)
    {
        printf("\n%s is checkmated\n", what_side);
        printf("%s wins the match!\n", deliver_side);
        printf("Total moves: %d\n", count_moves / 2 + 1);
        getchar();
        exit (EXIT_SUCCESS);
    }

    else if (theAllSeeingEye(board, king, CHECK) == CHECK)
    {
        printf("Check before: %s\n", what_side);
        memcpy(board, undo_board, sizeof undo_board);
    }

    else if (stalemate(board, side) == IMMOBILE)
    {
        printf("\nStalemate\n");
        printf("Game declared as draw!\n");
        printf("Total moves: %d\n", count_moves / 2 + 1);
        getchar();

        exit (EXIT_SUCCESS);
    }


    

    printf("\n%s --> Enter move #%d: ", what_side, count_moves / 2 + 1);
    stringInput(input, MAX_CHAR);    



    /* Welcome to input refinery!*/

    
    char piece =        '\0';
    int orig_let =     '\0';
    int orig_num =     '\0';
    char capture =      'o';
    int dest_let =     '\0';
    int dest_num =     '\0';
    char promote_piece = 0;



    //bool move_success = false;

    static bool W_king_moved = false;
    static bool W_queens_rook_moved = false;
    static bool W_kings_rook_moved = false;

    static bool B_king_moved = false;
    static bool B_queens_rook_moved = false;
    static bool B_kings_rook_moved = false;


    if (strncmp(input, "O-O-O", MAX_CHAR) == 0 || strncmp(input, "0-0-0", MAX_CHAR) == 0)
    {
        if (castling(board, &W_king_moved, &W_queens_rook_moved, &W_kings_rook_moved, &B_king_moved, &B_queens_rook_moved, &B_kings_rook_moved, side, LONG_CASTLE) == PREVENT_CASTLE)
        {
            memcpy(board, undo_board, sizeof undo_board);

            return 1;
        }
        

        switch (king)
        {
            case w_K:
                W_king_moved = true;
            break;

            case b_K:
                B_king_moved = true;
            break;
        }


        count_moves++;
        return 0;
    }

    if (strncmp(input, "O-O", MAX_CHAR) == 0 || strncmp(input, "0-0", MAX_CHAR) == 0)
    {
        if (castling(board, &W_king_moved, &W_queens_rook_moved, &W_kings_rook_moved, &B_king_moved, &B_queens_rook_moved, &B_kings_rook_moved, side, SHORT_CASTLE) == PREVENT_CASTLE)
        {
            memcpy(board, undo_board, sizeof undo_board);

            return 1;
        }





        switch (king)
        {
            case w_K:
                W_king_moved = true;
            break;

            case b_K:
                B_king_moved = true;
            break;
        }


        count_moves++;
        return 0;
    }
    

    /* Great Barrier of Input Validation */

    switch (input[0])
    {
        // FOR PAWNS ONLY
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
        {
            switch (strlen(input))
            {
                // pawn move
                case 2:
                
                    if (isRangeLet(input[0]) && isRangeNum(input[1]))
                    {
                        piece = 'P';
                        orig_let = input[0];
                        dest_let = input[0];
                        dest_num = input[1];
                    }
                        
                    else
                    {
                        fprintf(stdout, "%sPawn coordinates out of range%s", RED, RESET);
                        return -1;
                    }
                    // function to pawn
                    // goto statement

                break;
                

                case 3:
                
                    if (isRangeLet(input[0]) && isRangeNum(input[1]) && isaPiece(input[2]))
                    {
                        piece = 'P';
                        orig_let = input[0];
                        dest_let = input[0];
                        dest_num = input[1];
                        promote_piece = input[2];
                    }

                    else if (isRangeLet(input[0]) && isRangeLet(input[1]) && isRangeNum(input[2]))
                    {
                        piece = 'P';
                        orig_let = input[0];
                        dest_let = input[1];
                        dest_num = input[2];
                    }


                    else
                        return -1;
                
                break;

                case 4:
                

                    if (isRangeLet(input[0]) && input[1] == 'x' && isRangeLet(input[2]) && isRangeNum(input[3]))
                    {
                        piece = 'P';
                        orig_let = input[0];
                        capture =  input[1];
                        dest_let = input[2];
                        dest_num = input[3];
                    }

                    else if (isRangeLet(input[0]) && isRangeNum(input[1]) && input[2] == '=' && isaPiece(input[3]))
                    {
                        piece = 'P';
                        orig_let = input[0];
                        dest_let = input[0];
                        dest_num = input[1];
                        promote_piece = input[3];
                    }

                    else if (isRangeLet(input[0]) && isRangeLet(input[1]) && isRangeNum(input[2]) && isaPiece(input[3]))
                    {
                        piece = 'P';
                        orig_let = input[0];
                        dest_let = input[1];
                        dest_num = input[2];
                        promote_piece = input[3];
                    }

                    else
                    {
                        fprintf(stdout, "%sPawn capture coordinates out of range%s", RED, RESET);
                        return -1;
                    }
                    
                break;
                
                case 5:

                    if (isRangeLet(input[0]) && input[1] == 'x' && isRangeLet(input[2]) && isRangeNum(input[3]) && isaPiece(input[4]))
                    {
                        piece = 'P';
                        orig_let = input[0];
                        capture = input[1];
                        dest_let = input[2];
                        dest_num = input[3];
                        promote_piece = input[4];
                    }

                    else
                    {
                        fprintf(stdout, "%sPawn capture promotion attempt fail%s\n", RED, RESET);
                        return -1;
                    }

                break;


                case 6:

                    if (isRangeLet(input[0]) && input[1] == 'x' && isRangeLet(input[2]) && isRangeNum(input[3]) && input[4] == '=' && isaPiece(input[5]))
                    {
                        piece = 'P';
                        orig_let = input[0];
                        capture = input[1];
                        dest_let = input[2];
                        dest_num = input[3];
                        promote_piece = input[5];
                    }


                break;


                default:
                    fprintf(stdout, "%sInvalid pawn move%s", RED, RESET);
                    return -1;
            }
        }    

        break;

        // If the first character denotes a piece other than pawn
        case 'R': case 'N': case 'B': case 'Q': case 'K':
        {
            switch (strlen(input))
            {
                case 3:

                    if (isRangeLet(input[1]) && isRangeNum(input[2]))
                    {
                        piece = input[0];
                        dest_let = input[1];
                        dest_num = input[2];
                    }

                    else
                    {
                        // segmentation fault
                        fprintf(stdout, "%s move out of range%s", RED, RESET);
                        return -1;
                    }

                    // dont forget to pass capture
                break;

                case 4:

                    // if specified file when there are shared ranks
                    if (isRangeLet(input[1]) && isRangeLet(input[2]) && isRangeNum(input[3]))
                    {
                        piece = input[0];
                        orig_let = input[1];
                        dest_let = input[2];
                        dest_num = input[3];
                        

                        // remember to do function call here
                    }

                    // if specified rank when there are shared file
                    else if (isRangeNum(input[1]) && isRangeLet(input[2]) && isRangeNum(input[3]))
                    {
                        piece = input[0];
                        orig_num = input[1];
                        dest_let = input[2];
                        dest_num = input[3];
                    }

                    else if (input[1] == 'x' && isRangeLet(input[2]) && isRangeNum(input[3]))  // last edit
                    {                    
                        piece = input[0];
                        capture = input[1];
                        dest_let = input[2];
                        dest_num = input[3];
                    }

                    else
                    {
                        fprintf(stdout, "%sAppend 'x' after the piece to capture%s", RED, RESET);
                        return -1;
                    }
                    // insert function
                break;
        
                case 5:

                    if (isRangeLet(input[1]) && input[2] == 'x' && isRangeLet(input[3]) && isRangeNum(input[4]))
                    {
                        piece = input[0];
                        orig_let = input[1];
                        capture = input[2];
                        dest_let = input[3];
                        dest_num = input[4];
                        //insert function
                    }

                    else if (isRangeNum(input[1]) && input[2] == 'x' && isRangeLet(input[3]) && isRangeNum(input[4]))
                    {
                        piece = input[0];
                        orig_num = input[1];
                        capture = input[2];
                        dest_let = input[3];
                        dest_num = input[4];
                        // insert function
                    }

                    else if (isRangeLet(input[1]) && isRangeNum(input[2]) && isRangeLet(input[3]) && isRangeNum(input[4]))
                    {
                        piece = input[0];
                        orig_let = input[1];
                        orig_num = input[2];
                        dest_let = input[3];
                        dest_num = input[4];
                    }

                    else
                    {
                        fprintf(stdout, "%sInvalid capture%s", RED, RESET);
                        return -1;
                    }


                break;

                case 6:

                    // check if coordinates are in the right range (happens in Qh4xe1)
                    if (isRangeLet(input[1]) && isRangeNum(input[2]) && input[3] == 'x')
                    {
                        piece = input[0];
                        orig_let = input[1];
                        orig_num = input[2];
                        capture = input[3];
                        dest_let = input[4];
                        dest_num = input[5];
                    }

                break;
            }

            break;
        }

        default:
        {
            fprintf(stdout, "%sPieces are denoted in uppercase%s", RED, RESET);
            return -1;
        }
    }    


    // decrement moves here


    switch (piece)
    {
        case 'P': invalid_move = movePawn(board, orig_let, capture, dest_let, dest_num, promote_piece, side); break;
        case 'R': invalid_move = moveRook(board, orig_let, orig_num, dest_let, dest_num, capture, side, &W_queens_rook_moved, &W_kings_rook_moved, &B_queens_rook_moved, &B_kings_rook_moved); break;
        case 'B': invalid_move = moveBishop(board, orig_let, orig_num, dest_let, dest_num, capture, side); break;
        case 'N': invalid_move = moveKnight(board, orig_let, orig_num, dest_let, dest_num, capture, side); break;
        case 'Q': invalid_move = moveQueen(board, orig_let, orig_num, dest_let, dest_num, capture, side); break;
        case 'K': invalid_move = moveKing(board, dest_let, dest_num, capture, side, &W_king_moved, &B_king_moved); break;
    }


    if (invalid_move)
    {
        fprintf(stderr, "\n%sInvalid move%s\n", RED, RESET);
        printf("Press enter to continue: ");
        getchar();

        // count moves doesnt count if move is invalid
        count_moves--;
        goto end;
    }

    else
    {
        switch  (king)
        {
            case w_K:
                strncpy(white_previous_input, input, MAX_CHAR);
                white_previous_input[MAX_CHAR - 1] = '\0';
            break;

            case b_K:
                strncpy(black_previous_input, input, MAX_CHAR);
                black_previous_input[MAX_CHAR - 1] = '\0';
            break;
        }
    }



    if (checkmate(board, count_moves % 2) == CHECKMATE)
    {
        printf("%s is Checkmated\n", what_side);
        printf("%s wins the match!\n", deliver_side);
        printf("Total moves: %d\n", count_moves / 2 + 1);

        getchar();
        exit (EXIT_SUCCESS);
    }

    else if (theAllSeeingEye(board, king, CHECK) == CHECK)
    {
        printf("Check after \n");
        memcpy(board, undo_board, sizeof undo_board);
        
        return -1;
    }


    else if (stalemate(board, side) == IMMOBILE)
    {
        printf("\nStalemate\n");
        printf("Game declared as draw!\n");
        printf("Total moves: %d\n", count_moves / 2 + 1);


        getchar();

        exit (EXIT_SUCCESS);
    }


    end:




    count_moves++;

    return 0;
}



int movePawn(int board[S_BOARD][S_BOARD], int orig_let, char capture, int dest_let, int dest_num, int promote_piece, const int side)
{
    coords find_pawn[8];
    coords chosen_pawn;
    //int chosen_pawn_id = 0;

    static bool pawn_moved_white[S_BOARD] = {false};
    static bool pawn_moved_black[S_BOARD] = {false};
    int pawn_range_white[S_BOARD];
    int pawn_range_black[S_BOARD];

    memset(pawn_range_white, 0, sizeof pawn_range_white);
    memset(pawn_range_black, 0, sizeof pawn_range_black);


    bool found_pawn = false;
    bool double_square = false;


    int pawn_last_id = 0;

    int cnt_pawn = 0;

    int king = 0;
    int collect_enemy = 0;
    int collect_ally = 0;
    int enemy_king = 0;

    int last_rank = 0;

    orig_let -= 'a';
    dest_let -= 'a';
    dest_num -= '0' + 1;

    int orig_x = orig_let;
    int dest_x = dest_let;
    int dest_y = dest_num;





    switch (side)
    {
        case 0:
            king = w_K;
            collect_ally = WHITE_P;
            enemy_king = b_K;
            collect_enemy = BLACK_P;
            last_rank = Pos(8);
        break;


        case 1:
            king = b_K;
            collect_ally = BLACK_P;
            enemy_king = w_K;
            collect_enemy = WHITE_P;
            last_rank = Pos(1);
        break;
    }




    // search for pawns
    switch (king)
    {
        case w_K:

            for (int x_pos = 0; x_pos < S_BOARD; x_pos++)
            {
                // dest_y - 1 because dest_y has no pieces for obstacles
                for (int y_pos = dest_y - 1; y_pos >= 0; y_pos--)
                {
                    int piece = board[x_pos][y_pos];
                    
                    if (selectPiece(piece, king) == PAWN)
                    {
                        find_pawn[cnt_pawn].x = x_pos;
                        find_pawn[cnt_pawn].y = y_pos;
                        find_pawn[cnt_pawn].piece = piece;

                        cnt_pawn++;
                    }

                    else if (!piece)
                    {

                    }

                    else
                        break;
                }
            }

        break;

        case b_K:

            for (int x_pos = 0; x_pos < S_BOARD; x_pos++)
            {
                for (int y_pos = dest_y + 1; y_pos < S_BOARD; y_pos++)
                {
                    int piece = board[x_pos][y_pos];

                    if (selectPiece(piece, king) == PAWN)
                    {
                        find_pawn[cnt_pawn].x = x_pos;
                        find_pawn[cnt_pawn].y = y_pos;
                        find_pawn[cnt_pawn].piece = piece;

                        cnt_pawn++;
                    }

                    else if (!piece)
                    {

                    }

                    else    
                        break;
                }
            }

        break;
    }


    if (dest_y == last_rank)
    {
        if (promotionPawn(board, promote_piece, orig_x, dest_x, dest_y, king))
        {
            fprintf(stderr, "%sInvalid promotion%s\n", RED, RESET);
            printf("Valid format: %c%dQ or %c%d=Q", dest_x + 'a', dest_y + 1, dest_x + 'a', dest_y + 1);
            return -1;
        }
        return 0;
    }

    // if gap is 1, then capture
    if (capturePawn(board, find_pawn, cnt_pawn, orig_x, dest_x, dest_y, king))
        return 0;


    // if there is no pawn capture, then there should be no gap between orig_x and dest_x
    if (abs(orig_x - dest_x) != 0)
        return -1;


    // pawns cannot capture forward
    if (board[dest_let][dest_num])
        return -1;


    switch (king)
    {
        case w_K:

            for (int i = 0; i < cnt_pawn; i++)
            {
                int pawn_last_id = find_pawn[i].piece % 10;
                
                if (pawn_moved_white[pawn_last_id])
                    pawn_range_white[pawn_last_id] = 1;

                else
                    pawn_range_white[pawn_last_id] = 2;
            }

        break;


        case b_K:

            for (int i = 0; i < cnt_pawn; i++)
            {
                int pawn_last_id = find_pawn[i].piece % 10;

                if (pawn_moved_black[pawn_last_id])
                    pawn_range_black[pawn_last_id] = 1;
                
                else
                    pawn_range_black[pawn_last_id] = 2;
            }

        break;
    }



    switch (king)
    {
        case w_K:

            for (int i = 0; i < cnt_pawn; i++)
            {
                int piece = find_pawn[i].piece % 10;

                for (int y_range = 1; y_range <= pawn_range_white[piece]; y_range++)
                {
                    if (find_pawn[i].x == orig_let && find_pawn[i].y == dest_y - y_range)
                    {
                        chosen_pawn.x = find_pawn[i].x;
                        chosen_pawn.y = find_pawn[i].y;
                        chosen_pawn.piece = find_pawn[i].piece;

                        pawn_last_id = find_pawn[i].piece % 10;
                        found_pawn = true;

                        if (y_range == 2)
                            double_square = true;

                        break;
                    }



                }

                if (found_pawn)
                    break;
            }

        break;


        case b_K:

            for (int i = 0; i < cnt_pawn; i++)
            {
                int piece = find_pawn[i].piece % 10;



                for (int y_range = 1; y_range <= pawn_range_black[piece]; y_range++)
                {
                    if (find_pawn[i].x == orig_let && find_pawn[i].y == dest_y + y_range)
                    {
                        chosen_pawn.x = find_pawn[i].x;
                        chosen_pawn.y = find_pawn[i].y;
                        chosen_pawn.piece = find_pawn[i].piece;

                        pawn_last_id = find_pawn[i].piece % 10;
                        found_pawn = true;

                        if (y_range == 2)
                            double_square = true;

                        break;
                    }
                }

                if (found_pawn)
                    break;
            }

        break;
    }

   



   


    if (found_pawn)
    {

        switch (king)
        {
            case w_K:
                pawn_moved_white[pawn_last_id] = true;
            break;

            case b_K:
                pawn_moved_black[pawn_last_id] = true;
            break;
        }
    

        if (double_square)
            recentDoubleSquarePawn(chosen_pawn.piece, WRITE);      

        board[chosen_pawn.x][dest_y] = chosen_pawn.piece;
        board[chosen_pawn.x][chosen_pawn.y] = 0;
    }

    else
        return -1;


    if (abs(orig_let - dest_let) == 1)
    {
        // for capture
    }




    
    return 0;
}





int moveRook (int board[S_BOARD][S_BOARD], int orig_let, char orig_num, int dest_let, int dest_num, char capture, const int side, bool *W_queens_rook_moved, bool *W_kings_rook_moved, bool *B_queens_rook_moved, bool *B_kings_rook_moved)
{
    coords find_rook[4];
    size_t find_rook_s = sizeof find_rook / sizeof find_rook[0];


    bool found_up = false;
    bool found_down = false;
    bool found_right = false;
    bool found_left = false;


    //int cnt_rook = 0;
    int king = 0;
    int collect_ally = 0;

    memset(find_rook, -1, sizeof find_rook);

    // plus 1 because findrook is resetted to - 1, represents if piece is there or not


    int x = dest_let - 'a';
    int y = Pos(dest_num - '0');

    orig_let -= 'a';
    orig_num -= '0' + 1;


    switch (side)
    {
        case 0: 
            king = w_K;
            collect_ally = WHITE_P;
            break;

        case 1:
            king = b_K;
            collect_ally = BLACK_P;
            break;
    }



    int dest_piece = board[x][y];

    if (selectPiece(dest_piece, collect_ally) == collect_ally)
    {
        return fprintf(stderr, "%sCannot take own piece%s", RED, RESET);
    }

    if (board[x][y] == 0 && capture == 'x')
    {
        return fprintf(stderr, "%sCannot take empty square%s", RED, RESET);
    }


    // search up of destination square
    for (int i = y + 1; i < S_BOARD; i++)
    {
        int piece = board[x][i];
        
        if (selectPiece(piece, king) == ROOK)
        {
            find_rook[0].x = x;
            find_rook[0].y = i;

            find_rook[0].piece = board[x][i];
            found_up = true;
            break;
        }

        else if (board[x][i])
            break;
    }

    // search down of destination square
    for (int i = y - 1; i >= 0; i--)
    {
        int piece = board[x][i];
        
        if (selectPiece(piece, king) == ROOK)
        {
            find_rook[1].x = x;
            find_rook[1].y = i;

            find_rook[1].piece = board[x][i];
            found_down = true;
            break;
        }

        else if (board[x][i])
            break;    
    }

    // search right 
    for (int i = x + 1; i < S_BOARD; i++)
    {
        int piece = board[i][y];
        
        if (selectPiece(piece, king) == ROOK)
        {
            find_rook[2].x = i;
            find_rook[2].y = y;

            find_rook[2].piece = board[i][y];
            found_right = true;
            break;
        }

        else if (board[i][y])
            break;
    }

    // search left
    for (int i = x - 1; i >= 0; i--)
    {
        int piece = board[i][y];
        
        if (selectPiece(piece, king) == ROOK)
        {
            find_rook[3].x = i;
            find_rook[3].y = y;

            find_rook[3].piece = board[i][y];
            found_left = true;
            break;
        }

        else if (board[i][y])
            break;
    }


    for (int i = 0; i < find_rook_s; i++)
    {
        if (find_rook[i].x >= 0)
        {
            printf("(%d, %d)\n", find_rook[i].x, find_rook[i].y);
        }
    }

    


    int count_present_rooks = 0;

    for (int i = 0; i < find_rook_s; i++)
    {
        if (find_rook[i].piece >= 0)
            count_present_rooks++;
    }



    if (count_present_rooks <= 0)
    {
        fprintf(stderr, "%sNo rook can move%s", RED, RESET);
        return 1;
    }



    if (count_present_rooks == 1)
    {
                        // other founds are zero, determine which one with value
        for (int i = 0; i < find_rook_s; i++)
        {
            if (find_rook[i].x >= 0 && find_rook[i].y >= 0)
            {
                board[x][y] = find_rook[i].piece;
                board[find_rook[i].x][find_rook[i].y] = 0;


                if (find_rook[i].piece == w_dR)
                    *W_queens_rook_moved = true;

                else if (find_rook[i].piece == w_lR)
                    *W_kings_rook_moved = true;

                break;
            }
        }
    }

    

    else if ((found_left && found_right) || (found_down && found_right) || (found_down && found_left) || (found_up && found_right) || (found_up && found_left))
    {

        for (int i = 0; i < find_rook_s; i++)
        {
            if (orig_let == find_rook[i].x)
            {
                board[x][y] = find_rook[i].piece;
                board[find_rook[i].x][find_rook[i].y] = 0;
                return 0;
            }
        }

        // failed searching
        printf("File should be specified\n");

        printf("Possible moves: \n");

        for (int i = 0; i < find_rook_s; i++)
        {
            if (find_rook[i].x >= 0)
            {
                printf("R%c%c%d\n", find_rook[i].x + 'a', x + 'a', y + 1);
            } 
        }

        return 1;
    }


    else if (found_up && found_down)
    {
        for (int i = 0; i < find_rook_s; i++)
        {
            if (orig_num == find_rook[i].y)
            {
                board[x][y] = find_rook[i].piece;
                board[find_rook[i].x][find_rook[i].y] = 0;
                return 0;
            }
        }

        printf("Rank should be specified\n");

        printf("Possible moves: \n");

        for (int i = 0; i < find_rook_s; i++)
        {
            if (find_rook[i].x >= 0)
            {
                printf("R%d%c%d\n", find_rook[i].y + 1, x + 'a', y + 1);
            } 
        }

        
        return 1;
    }

    else if (count_present_rooks >= 3)
    {
        printf("Specify file and rank");

        for (int i = 0; i < find_rook_s; i++)
        {
            if (orig_let == find_rook[i].x && orig_num == find_rook[i].y)
            {
                board[x][y] = find_rook[i].piece;
                board[find_rook[i].x][find_rook[i].y] = 0;
                
                return 0;
            }
        }

        printf("Possible moves: \n");

        for (int i = 0; i < find_rook_s; i++)
        {
            if (find_rook[i].x >= 0)
            {
                printf("R%c%d%c%d\n", find_rook[i].x + 'a', find_rook[i].y + 1, x + 'a', y + 1);
            }
        }

        return 1;
    }



    return 0;
}


int moveBishop (int board[S_BOARD][S_BOARD], int orig_let, char orig_num, int dest_let, int dest_num, char capture, const int side)
{
    coords find_bishop[4];
    int find_bishop_s =  sizeof (find_bishop) / sizeof (find_bishop[0]);

    memset(find_bishop, -1 , sizeof(find_bishop));

    int king = 0;
    int collect_ally = 0;


    bool up_left = false;
    bool up_right = false;
    bool down_left = false;
    bool down_right = false;


    int x = dest_let - 'a';
    int y = dest_num - '0' - 1;
    
    orig_let -= 'a';
    orig_num -= '0' + 1;


    switch (side)
    {
        case 0:
            king = w_K;
            collect_ally = WHITE_P;
        break;

        case 1:
            king = b_K;
            collect_ally = BLACK_P;
        break;
    }


    // check if destination square is occupied by ally or enemy


    int dest_enemy = board[x][y];

    if (selectPiece(dest_enemy, collect_ally) == collect_ally)
    {
        fprintf(stderr, "%sCannot capture ally%s", RED, RESET);
        return -1;
    }

    if (dest_enemy == 0 && capture == 'x')
    {
        fprintf(stderr, "%sCannot capture an empty square%s", RED, RESET);
        return -1;
    }




    for (int i = x + 1, j = y + 1; i < S_BOARD && j < S_BOARD; i++, j++)
    {
        int piece = board[i][j];
        
        if (selectPiece(piece, king) == BISHOP)
        {
            find_bishop[0].x = i;
            find_bishop[0].y = j;
            find_bishop[0].piece = board[i][j];

            up_right = true;
            break;
        }

        else if (board[i][j])
            break;
    }

    for (int i = x - 1, j = y + 1; i >= 0 && j < S_BOARD; i--, j++)
    {
        int piece = board[i][j];
        
        if (selectPiece(piece, king) == BISHOP)
        {
            find_bishop[1].x = i;
            find_bishop[1].y = j;
            find_bishop[1].piece = board[i][j];

            up_left = true;
            break;
        }

        else if (board[i][j])
            break;
    }


    for (int i = x + 1, j = y - 1; i < S_BOARD && j >= 0; i++, j--)
    {
        int piece = board[i][j];
        
        if (selectPiece(piece, king) == BISHOP)
        {
            find_bishop[2].x = i;
            find_bishop[2].y = j;
            find_bishop[2].piece = board[i][j];

            down_right = true;
            break;
        }

        else if (board[i][j])
            break;
    }


    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--)
    {
        int piece = board[i][j];
        
        if (selectPiece(piece, king) == BISHOP)
        {
            find_bishop[3].x = i;
            find_bishop[3].y = j;
            find_bishop[3].piece = board[i][j];

            down_left = true;
            break;
        }

        else if (board[i][j])
            break;
    }




    int count_present_bishop = 0;

    for (int i = 0 ; i < find_bishop_s; i++)
    {
        if (find_bishop[i].piece >= 0)
            count_present_bishop++;
    }


    if (count_present_bishop == 0)
    {
        return fprintf(stderr, "%sNo bishop can move%s", RED, RESET);
    }

    else if (count_present_bishop == 1)
    {
        // if one bishop can move
        for (int i = 0; i < find_bishop_s; i++)
        {
             if (find_bishop[i].x >= 0 && find_bishop[i].y >= 0)
             {
                   board[x][y] = find_bishop[i].piece;
                   board[find_bishop[i].x][find_bishop[i].y] = 0;
                 break;
            }
         }
    }


    else if ((up_left && up_right) || (down_left && down_right) || (up_left && down_right) || (up_right && down_left))
    {
        printf("Specify the file\n");

        for (int i = 0; i < find_bishop_s; i++)
        {
            if (orig_let == find_bishop[i].x)
            {

                board[x][y] = find_bishop[i].piece;
                board[find_bishop[i].x][find_bishop[i].y] = 0;

                return 0;   
            }
        }


        printf("Possible moves: \n");

        for (int i = 0; i < find_bishop_s; i++)
        {
            if (find_bishop[i].x >= 0)
            {
                printf("B%c%c%d\n", find_bishop[i].x + 'a', x + 'a', y + 1);
            }
        }

        return 1;
    }


    else if ((up_left && down_left) || (up_right && down_right))
    {
        //printf("Specify the rank\n");

        for (int i = 0; i < find_bishop_s; i++)
        {
            if (orig_num == find_bishop[i].y)
            {
                board[x][y] = find_bishop[i].piece;
                board[find_bishop[i].x][find_bishop[i].y] = 0;
                
                return 0;
            }
        }


        printf("Possible moves: \n");

        for (int i = 0; i < find_bishop_s; i++)
        {
            if (find_bishop[i].x >= 0)
            {
                printf("B%d%c%d\n", find_bishop[i].y + 1, x, y);
            }
        }

    }

    else if (count_present_bishop >= 3)
    {
        printf("Specify rank and file ");


        for (int i = 0; i < find_bishop_s; i++)
        {
            if (orig_let == find_bishop[i].x && orig_num == find_bishop[i].y)
            {
                board[x][y] = find_bishop[i].piece;
                board[find_bishop[i].x][find_bishop[i].y] = 0;
                
                return 0;
            }
        }


        printf("Possible moves: \n");

        for (int i = 0; i < find_bishop_s; i++)
        {
            if (find_bishop[i].x >= 0)
            {
                printf("B%c%d%c%d\n", find_bishop[i].x, find_bishop[i].y + 1, x, y);
            }
        }
    }

    return 0;
}



int moveKnight (int board[S_BOARD][S_BOARD], int orig_let, char orig_num, int dest_let, int dest_num, char capture, const int side)
{
    coords find_knight[8];

    memset(find_knight, -1, sizeof(find_knight));

    int x = dest_let - 'a';
    int y = dest_num - '0' - 1;

    orig_let -= 'a';
    orig_num -= '0' + 1;


    int king = 0;
    int collect_ally = 0;


    int found = 0;



    switch (side)
    {
        case 0:
            king = w_K;
            collect_ally = WHITE_P;
        break;

        case 1:
            king = b_K;
            collect_ally = BLACK_P;
        break;
    }


    int dest_enemy = board[x][y];

    if (selectPiece(dest_enemy, collect_ally) == collect_ally)
    {
        return fprintf(stderr, "%sKnight cannot take own piece%s", RED, RESET);
    }


    if (dest_enemy == 0 && capture == 'x')
    {
        return fprintf(stderr, "%sCannot take empty square%s", RED, RESET);
    }


    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            int piece = board[i + x][j + y];
            
            if (i + x < 0 || i + x >= S_BOARD)
                continue;
        
            if (j + y < 0 || j + y >= S_BOARD)
                continue;

            if (abs(i) == abs(j))
                continue;

            if (i == 0 || j == 0)
                continue;



            if (selectPiece(piece, king) == KNIGHT)
            {
                find_knight[found].piece = board[i + x][j + y];
                
                find_knight[found].x = i + x;
                find_knight[found].y = j + y;

                found++;
            }    
        }
    }
    



    if (found == 0)
        return fprintf(stderr, "%sKnight move is not possible%s", RED, RESET);



    bool specify_file = false;
    bool specify_rank = false;

    for (int i = 0; i < found; i++)
    {
        for (int j = i + 1; j < found; j++)
        {
            if (find_knight[i].y == find_knight[j].y)
               specify_file = true;
        
            else if (find_knight[i].x == find_knight[j].x)
                specify_rank = true;
        }
    }
    


    if (found == 1)
    {
        board[x][y] = find_knight[0].piece;
        board[find_knight[0].x][find_knight[0].y] = 0;

    }


    else if (specify_rank)
    {   
        for (int i = 0; i < found; i++)
        {
            if (orig_num == find_knight[i].y)
            {
                board[x][y] = find_knight[i].piece;
                board[find_knight[i].x][find_knight[i].y] = 0;
                
                goto move_finish;
            }
        }

        printf("Specify rank.\nPossible moves:\n");

        for (int i = 0; i < found; i++)
        {
            printf("N%d%c%d\n", find_knight[i].y + 1, x + 'a', y + 1);
        }

        return 1;
    }


    else if (found == 2)
    {
        for (int i = 0 ; i < found; i++)
        {
            if (orig_let == find_knight[i].x)
            {
                board[x][y] = find_knight[i].piece;
                board[find_knight[i].x][find_knight[i].y] = 0;
                

                goto move_finish;
            }
        }


        printf("Specify the file.\nPossible moves: \n");
    
        for (int i = 0; i < found; i++)
        {
            printf("N%c%c%d\n", find_knight[i].x + 'a', x + 'a', y + 1);
        }

        return 1;
    }




    else if (found >= 2)
    {
        for (int i = 0; i < found; i++)
        {
            if (orig_let == find_knight[i].x && orig_num == find_knight[i].y)
            {
                board[x][y] = find_knight[i].piece;
                board[find_knight[i].x][find_knight[i].y] = 0;

                goto move_finish;
            }
        }


        printf("Specify file and rank.\nPossible moves: \n");

        for (int i = 0; i < found; i++)
        {
            printf("N%c%d%c%d\n", find_knight[i].x + 'a', find_knight[i].y + 1, x + 'a', y + 1);
        }

        return 1;
    }



    move_finish:
    return 0;
}


int moveQueen (int board[S_BOARD][S_BOARD], int orig_let, char orig_num, int dest_let, int dest_num, char capture, const int side)
{
    
    // lol there is only one queen, why size array 9
    // oh for the potential promotion of pawns
    coords find_queen[9];
    memset(find_queen, 0, sizeof find_queen);

    int count = 0;


    int x = dest_let - 'a';
    int y = dest_num - '0' - 1;

    orig_let -= 'a';
    orig_num -= '0' + 1;


    int king = 0;
    int collect_ally = 0;


    switch (side)
    {
        case 0:
            king = w_K;
            collect_ally = WHITE_P;
        break;

        case 1:
            king = b_K;
            collect_ally = BLACK_P;
        break;
    }


    int dest_enemy = board[x][y];


    if (selectPiece(dest_enemy, collect_ally) == collect_ally)
        return fprintf(stderr, "%sQueen cannot take her own allies%s", RED, RESET);

    if (dest_enemy == 0 && capture == 'x')
        return fprintf(stderr, "%sQueen can't take blank square%s", RED, RESET);


    for (int i = x + 1; i < S_BOARD; i++)
    {
        int piece = board[i][y];
        
        if (selectPiece(piece, king) == QUEEN)
        {
            find_queen[count].x = i;
            find_queen[count].y = y;

            find_queen[count].piece = board[i][y];

            count++;
            break;
        }

        else if (board[i][y])
            break;
    }


    for (int i = x - 1; i >= 0; i--)
    {
        int piece = board[i][y];
        
        if (selectPiece(piece, king) == QUEEN)
        {
            find_queen[count].x = i;
            find_queen[count].y = y;

            find_queen[count].piece = board[i][y];

            count++;
            break;
        }

        else if (board[i][y])
            break;
    }



    for (int i = y - 1; i >= 0; i--)
    {
        int piece = board[x][i];
        
        if (selectPiece(piece, king) == QUEEN)
        {
            find_queen[count].x = x;
            find_queen[count].y = i;

            find_queen[count].piece = board[x][i];

            count++;
            break;
        }

        else if (piece)
            break;
    }


    for (int i = y + 1; i < S_BOARD; i++)
    {
        int piece = board[x][i];
        
        if (selectPiece(piece, king) == QUEEN)
        {
            find_queen[count].x = x;
            find_queen[count].y = i;

            find_queen[count].piece = board[x][i];

            count++;
            break;
        }

        else if (piece)
            break;
    }



    for (int i = x + 1, j = y + 1; i < S_BOARD && j < S_BOARD; i++, j++)
    {
        int piece = board[i][j];
        
        if (selectPiece(piece, king) == QUEEN)
        {
            find_queen[count].x = i;
            find_queen[count].y = j;

            find_queen[count].piece = board[i][j];

            count++;
            break;
        }

        else if (piece)
            break;
    }


    for (int i = x + 1, j = y - 1; i < S_BOARD && j >= 0; i++, j--)
    {
        int piece = board[i][j];
        
        if (selectPiece(piece, king) == QUEEN)
        {
            find_queen[count].x = i;
            find_queen[count].y = j;

            find_queen[count].piece = board[i][j];

            count++;
            break;
        }

        else if (piece)
            break;
    }


    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--)
    {
        int piece = board[i][j];
        
        if (selectPiece(piece, king) == QUEEN)
        {
            find_queen[count].x = i;
            find_queen[count].y = j;

            find_queen[count].piece = board[i][j];

            count++;
            break;
        }

        else if (piece)
            break;
}


    for (int i = x - 1, j = y + 1; i >= 0 && j < S_BOARD; i--, j++)
    {
        int piece = board[i][j];
        
        if (selectPiece(piece, king) == QUEEN)
        {
            find_queen[count].x = i;
            find_queen[count].y = j;

            find_queen[count].piece = board[i][j];

            count++;
            break;
        }

        else if (piece)
            break;
    }



    bool specify_file = false;
    bool specify_rank = false;



    for (int i = 0; i < count; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (find_queen[i].x == find_queen[j].x)
                specify_rank = true;

            if (find_queen[i].y == find_queen[j].y)
                specify_file = true;
        }
    }


    if (count == 0)
        return fprintf(stderr, "%sNo queen can move%s", RED, RESET);

    if (count == 1)
    {
        board[x][y] = find_queen[0].piece;
        board[find_queen[0].x][find_queen[0].y] = 0;
    }
    
    else if (specify_rank)
    {
        for (int i = 0; i <= count; i++)
        {
            if (find_queen[i].y == orig_num)
            {
                board[x][y] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;

                goto success;
            }
        }


        printf("Available moves: \n");

        for (int i = 0; i < count; i++)
        {
            printf("Q%d%c%d\n", find_queen[i].y + 1, x + 'a', y + 1);
        }

        return 1;
    }

    else if (specify_file)
    {
        for (int i = 0; i < count; i++)
        {
            if (find_queen[i].x == orig_let)
            {
                board[x][y] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;

                goto success;
            }
        }


        printf("Available moves: \n");

        for (int i = 0; i < count; i++)
        {
            printf("Q%c%c%d\n", find_queen[i].x + 'a', x + 'a', y + 1);
        }

        return 1;
    }



    else if (count > 2)
    {
        for (int i = 0; i <= count; i++)
        {
            if (find_queen[i].x == orig_let && find_queen[i].y == orig_num)
            {
                board[x][y] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;

                goto success;
            }
        }


        printf("Available moves: \n");

        for (int i = 0; i < count; i++)
        {
            printf("Q%c%d%c%d\n", find_queen[i].x + 'a', find_queen[i].y + 1, x + 'a', y + 1);
        }

        return 1;
    }


    success:
    return 0;
}


int moveKing (int board[S_BOARD][S_BOARD], int dest_let, int dest_num, char capture, const int side, bool *W_king_moved, bool *B_king_moved)
{
    coords find_king;

    int ally_side = 0;


    bool king_can_move = false;
    bool king_found = false;

    int x = dest_let - 'a';
    int y = dest_num - '0' - 1;


    int collect_ally = 0;



    switch (side)
    {
        case 0:
            ally_side = w_K;
        break;

        case 1:
            ally_side = b_K;
        break;
    }


    switch (ally_side)
    {
        case w_K:
            collect_ally = WHITE_P;
        break;
        
        case b_K:
            collect_ally = BLACK_P;
        break;
    }


   
    int capture_piece = board[x][y];

    if (selectPiece(capture_piece, collect_ally) == collect_ally)
        return fprintf(stderr, "%sKing cannot take his subjects%s", RED, RESET);

    
    if (capture_piece == 0 && capture == 'x')
        return fprintf(stderr, "%sKing cannot take empty square%s", RED, RESET);




    for (int i = 0; i < S_BOARD; i++)
    {
        for (int j = 0; j < S_BOARD; j++)
        {
            if (board[i][j] == ally_side)
            {
                find_king.x = i;
                find_king.y = j;
                find_king.piece = board[i][j];

                king_found = true;

                break;
            }

            if (king_found)
                break;
        }

        if (king_found)
            break;
    }

    printf("King is in %c, %d\n", find_king.x + 'a', find_king.y + 1);


   

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int piece = 0;
            
            if (i == 0 && j == 0)
                continue;

           
            if (find_king.x + i >= S_BOARD || find_king.x + i < 0 )
                continue;

            if (find_king.y + j >= S_BOARD || find_king.y + j < 0)
                continue;


            piece = board[find_king.x + i][find_king.y + j];

            if (piece == 0)
            {
                king_can_move = true;
                break;
            }

            else if (ally_side == w_K)
            {
                if (selectPiece(piece, BLACK_P) == BLACK_P)
                {
                    king_can_move = true;
                    break;
                }
            }

            else if (ally_side == b_K)
            {
                if (selectPiece(piece, WHITE_P) == WHITE_P)
                {
                    king_can_move = true;
                    break;
                }
            }
        }

        if (king_can_move)
            break;
    }


    if (!king_can_move)
    {
        return fprintf(stderr, "%sKing cannot move%s", RED, RESET);
    }

    board[x][y] = find_king.piece;
    board[find_king.x][find_king.y] = 0;

    switch (side)
    {
        case 0:
            *W_king_moved = true;
            break;
        
        case 1:
            *B_king_moved = true;
            break;
    }

    return 0;
}


int castling (int board[S_BOARD][S_BOARD], bool *W_king_moved, bool *W_queens_rook_moved, bool *W_kings_rook_moved, bool *B_king_moved, bool *B_queens_rook_moved, bool *B_kings_rook_moved, const int side, const int long_or_short)
{
        
    switch (side)
    {
        case 0:
            if (*W_king_moved)
            {
                fprintf(stderr, "%sCastling is not possible%s", RED, RESET);
                getchar();

                return PREVENT_CASTLE;
            }
        break;

        case 1:
            if (*B_king_moved)
            {
                fprintf(stderr, "%sCastling cannot take place%s", RED, RESET);
                getchar();

                return PREVENT_CASTLE;
            }
        break;
    }
    
    
    
    switch (side)
    {
        case 0:

            
            switch (long_or_short)
            {
                case LONG_CASTLE:
                
                    for (int i = b; i < e; i++)
                    {
                        if (board[i][Pos(1)])
                        {
                            fprintf (stderr, "%sPieces are blocking the king and rook in long castle%s", RED, RESET);
                            return PREVENT_CASTLE;
                        }
                    }

                break;

                case SHORT_CASTLE:

                    for (int i = f; i < h; i++)
                    {
                        if (board[i][Pos(1)])
                        {
                            fprintf (stderr, "%sPieces are blocking the king and rook in short castle%s", RED, RESET);
                            return PREVENT_CASTLE;
                        }
                    }

                break;
            }

        break;


        case 1:

            switch (long_or_short)
            {
                case LONG_CASTLE:
                    
                    for (int i = b; i < e; i++)
                    {
                        if (board[i][Pos(8)])
                        {
                            fprintf(stderr, "%sPieces are blocking the king and the rook in long castle%s", RED, RESET);
                            return PREVENT_CASTLE;
                        }
                    }

                break;

                case SHORT_CASTLE:

                    for (int i = f; i < h; i++)
                    {
                        if (board[i][Pos(8)])
                        {
                            fprintf(stderr, "%sPieces are blocking the king and the rook in short castle%s", RED, RESET);
                            return PREVENT_CASTLE;
                        }
                    }

                break;
            }
    }



    switch (side)
    {
        case 0:

            switch (long_or_short)
            {
                case LONG_CASTLE:

                    if (*W_queens_rook_moved)
                    {
                        fprintf(stderr, "%sQueen's rook already made it's move, therefore long castle is invalid%s", RED, RESET); 
                        return PREVENT_CASTLE;
                    }

                    board[c][Pos(1)] = w_K;
                    board[d][Pos(1)] = w_dR;
                    board[e][Pos(1)] = 0;
                    board[a][Pos(1)] = 0;

                break;

                case SHORT_CASTLE:
                    if (*W_kings_rook_moved)
                    {
                        fprintf(stderr, "%sKing's rook already has made a move, therefore short castle is invalid%s", RED, RESET);
                        return PREVENT_CASTLE;
                    }

                    board[g][Pos(1)] = w_K;
                    board[f][Pos(1)] = w_lR;
                    board[e][Pos(1)] = 0;
                    board[h][Pos(1)] = 0;

                break;
            }


        break;

        case 1:

            switch (long_or_short)
            {
                case LONG_CASTLE:

                    if (*B_queens_rook_moved)
                    {
                        fprintf (stderr, "%sQueen's rook has already made a move, thus long castle cannot be done%s", RED, RESET);
                        return PREVENT_CASTLE;
                    }

                    board[c][Pos(8)] = b_K;
                    board[d][Pos(8)] = b_lR;
                    board[e][Pos(8)] = 0;
                    board[a][Pos(8)] = 0;

                break;

                case SHORT_CASTLE:

                    if (*B_kings_rook_moved)
                    {
                        fprintf (stderr, "%sKing's rook has already made a move, thus short castle is illegal%s", RED, RESET);
                        return PREVENT_CASTLE;
                    }

                    board[g][Pos(8)] = b_K;
                    board[f][Pos(8)] = b_dR;
                    board[e][Pos(8)] = 0;
                    board[h][Pos(8)] = 0;
                    
                break;
            }
    }


    // check if enemy pieces are aiming between king and rook



    switch (side)
    {
        case 0:
            if (sightPawn(board, w_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightRook(board, w_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightBishop(board, w_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightKnight(board, w_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightQueen(board, w_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightKing(board, w_K, long_or_short) == PREVENT_CASTLE)
                return PREVENT_CASTLE;
                
        break;

        case 1:
            if (sightPawn(board, b_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightRook(board, b_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightBishop(board, b_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightKnight(board, b_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightQueen(board, b_K, long_or_short, 0, 0, NULL) == PREVENT_CASTLE)
                return PREVENT_CASTLE;

            if (sightKing(board, b_K, long_or_short) == PREVENT_CASTLE)
                return PREVENT_CASTLE;
        break;
    }

    return 0;
}


int sightPawn (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior)
{
    coords find_pawn[8];
    int count = 0;

    bool check = false;
    bool prevent_castle = false;

    int enemy_king = 0;


    if (savior != NULL)
    {
        savior -> x = 0;
        savior -> y = 0;
        savior -> piece = 0;
    }


    switch (king)
    {
        case w_K:
            enemy_king = b_K;
        break;

        case b_K:
            enemy_king = w_K;
        break;
    }



    findPiece(find_pawn, board, PAWN, &count, enemy_king);



    if (count == 0)
        goto end;


    switch (purpose)
    {
        case LONG_CASTLE:

            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int j = b; j <= d; j++)
                    {
                        if (find_pawn[i].x - 1 == j && find_pawn[i].y - 1 == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }


                        if (find_pawn[i].x + 1 == j && find_pawn[i].y - 1 == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }
                    }
                }
            }


            else if (king == b_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int j = b; j <= d; j++)
                    {
                        if (find_pawn[i].x - 1 == j && find_pawn[i].y + 1  == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        if (find_pawn[i].x + 1 == j && find_pawn[i].y + 1 == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }
                    }
                }
            }

        break;


        case SHORT_CASTLE:

            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int j = f; j <= g; j++)
                    {
                        if (find_pawn[i].x - 1 == j && find_pawn[i].y - 1 == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        if (find_pawn[i].x + 1 == j && find_pawn[i].y - 1 == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }
                    }                    
                }
            }


            else if (king == b_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int j = f; j <= g; j++)
                    {
                        if (find_pawn[i].x + 1 == j && find_pawn[i].y + 1 == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        if (find_pawn[i].x - 1 == j && find_pawn[i].y + 1  == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }
                    }
                }
            }
        
        break;

        case SCAN:

            coords pawn_scan[8];
            int cnt_scan = 0;

            int collect_enemy = 0;


            switch (king)
            {
                case w_K:
                    collect_enemy = BLACK_P;
                break;

                case b_K:
                    collect_enemy = WHITE_P;
                break;
            }

            if (savior != NULL)
            {
                savior -> x = -1;
                savior -> y = -1;
                savior -> piece = -1;   
            }

            findPiece(pawn_scan, board, PAWN, &cnt_scan, king);


            int range = 1;

            if (king == w_K)
            {
                for (int i = 0; i < cnt_scan; i++)
                {
                    int x_pos = pawn_scan[i].x;
                    int y_pos = pawn_scan[i].y;
                    //int piece = pawn_scan[i].piece;

                    if (y_pos == Pos(2))
                        range = 2;



                    for (int idx_range = 1; idx_range <= range; idx_range++)
                    {
                        if (y_pos + idx_range < 0 || y_pos + idx_range >= S_BOARD)
                            break;
                    
                        if (board[x_pos][y_pos + idx_range])
                            break;

                        if (x_pos == x_scan && y_pos + idx_range == y_scan)
                            return VISIBLE;
                    }


                    // attempts to capture the attacker
                    if (x_pos + 1 == x_scan && y_pos - 1 == y_scan)
                    {
                        int enemy_piece = 0;

                        enemy_piece = board[x_scan][y_scan];

                        if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
                        {
                            findSavior(board, x_pos, y_pos, savior);
                            return VISIBLE;

                        }
                    }

                    if (x_pos + 1 == x_scan && y_pos + 1 == y_scan)
                    {
                        int enemy_piece = 0;

                        enemy_piece = board[x_scan][y_scan];

                        if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
                        {
                            findSavior(board, x_pos, y_pos, savior);
                            return VISIBLE;
                        }
                    }

                    range = 1;
                }
            }


            else if (king == b_K)
            {
                for (int i = 0; i < cnt_scan; i++)
                {
                    int x_pos = pawn_scan[i].x;
                    int y_pos = pawn_scan[i].y;
                    //int piece = pawn_scan[i].piece;
                    


                    if (y_pos == Pos(7))
                        range = 2;


                    for (int idx_range = 1; idx_range <= range; idx_range++)
                    {
                        if (y_pos - idx_range < 0 || y_pos - idx_range >= S_BOARD)
                            break;

                        if (board[x_pos][y_pos - idx_range])
                            break;
                    
                        if (x_pos == x_scan && y_pos - idx_range == y_scan)
                            return VISIBLE;

                    }

                    // attempts to capture the attacker
                    if (x_pos - 1 == x_scan && y_pos - 1 == y_scan)
                    {
                        int piece = 0;

                        piece = board[x_scan][y_scan];


                        if (selectPiece(piece, collect_enemy) == collect_enemy)
                        {
                            findSavior(board, x_pos, y_pos, savior);
                            return VISIBLE;
                        }

                    }

                    if (x_pos + 1 == x_scan && y_pos - 1 == y_scan)
                    {
                        int piece = 0;


                        piece = board[x_scan][y_scan];

                        if (selectPiece(piece, collect_enemy) == collect_enemy)
                        {
                            findSavior(board, x_pos, y_pos, savior);
                            return VISIBLE;
                        }
                    }


                    range = 1;
                }
            }

          

        return 0;
        break;
    }

    if (purpose != CHECK)
        goto end;



    if (king == w_K)
    {
        for (int i = 0; i < count; i++)
        {
            if (!(find_pawn[i].x - 1 < 0 || find_pawn[i].x - 1 >= S_BOARD || find_pawn[i].y - 1 < 0 || find_pawn[i].y - 1 >= S_BOARD))
            {
                if (board[find_pawn[i].x - 1][find_pawn[i].y - 1] == w_K)
                {
                    check = true;
                    goto end;
                }
            }
            
            

            if (!(find_pawn[i].x + 1 < 0 || find_pawn[i].x + 1 >= S_BOARD || find_pawn[i].y - 1 < 0 || find_pawn[i].y - 1 >= S_BOARD))
            {
                if (board[find_pawn[i].x + 1][find_pawn[i].y - 1] == w_K)
                {
                    check = true;
                    goto end;
                }
            }
        }
    }


    else if (king == b_K)
    {
        for (int i = 0; i < count; i++)
        {
            if (!(find_pawn[i].x + 1 < 0 || find_pawn[i].x + 1 >= S_BOARD || find_pawn[i].y + 1 < 0 || find_pawn[i].y + 1 >= S_BOARD))
            {
                if (board[find_pawn[i].x + 1][find_pawn[i].y + 1] == b_K)
                {
                    check = true;
                    goto end;
                }
            }


            if (!(find_pawn[i].x - 1 < 0 || find_pawn[i].x - 1 >= S_BOARD || find_pawn[i].y + 1 < 0 || find_pawn[i].y + 1 >= S_BOARD))
            {
                if (board[find_pawn[i].x - 1][find_pawn[i].y + 1] == b_K)
                {
                    check = true;
                    goto end;
                }
            }
        }
    }

    end:

    if (prevent_castle)
        return PREVENT_CASTLE;

    if (check)
        return CHECK;

    return 0;
}


int sightRook (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior)
{
    coords find_rook[MAX_PCS];
    int count = 0;

    int enemy_king = 0;

    bool prevent_castle = false;
    bool check = false;

    if (savior != NULL)
    {
        savior -> x = 0;
        savior -> y = 0;
        savior -> piece = 0;
    }

    switch (king)
    {
        case w_K:
            enemy_king = b_K;
        break;

        case b_K:
            enemy_king = w_K;
        break;
    }

    findPiece(find_rook, board, ROOK, &count, enemy_king);



    if (count == 0)
        goto end;


    switch (purpose)
    {
        case LONG_CASTLE:

            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    // check if rook is aiming to 1st rank
                    for (int y_pos = find_rook[i].y - 1; y_pos >= 0; y_pos--)
                    {
                        if ((find_rook[i].x >= b && find_rook[i].x <= d) && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        if (board[find_rook[i].x][y_pos])
                            goto end;
                    }
                }
            }

            else if (king == b_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int y_pos = find_rook[i].y + 1; y_pos < S_BOARD; y_pos++)
                    {
                        if ((find_rook[i].x >= b && find_rook[i].x <= d) && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        if (board[find_rook[i].x][y_pos])
                            goto end;
                    }
                }
            }

        break;


        case SHORT_CASTLE:

            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int y_pos = find_rook[i].y - 1; y_pos >= 0; y_pos--)
                    {
                        if ((find_rook[i].x >= f && find_rook[i].x <= g) && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[find_rook[i].x][y_pos])
                            goto end;
                    }
                }
            }

            else if (king == b_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int y_pos = find_rook[i].y + 1; y_pos < S_BOARD; y_pos++)
                    {
                        if ((find_rook[i].x >= f && find_rook[i].x <= g) && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[find_rook[i].x][y_pos])
                            goto end;
                    }
                }
            }

        break;




        case SCAN:

            coords rook_scan[MAX_PCS];

            if (savior != NULL)
            {
                savior -> x = -1;
                savior -> y = -1;
                savior -> piece = -1;   
            }


            int cnt_scan = 0;

            findPiece(rook_scan, board, ROOK, &cnt_scan, king);

            for (int i = 0; i < cnt_scan; i++)
            {
                for (int x_pos = rook_scan[i].x + 1, y_pos = rook_scan[i].y; x_pos < S_BOARD; x_pos++)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, rook_scan[i].x, rook_scan[i].y, savior);

                        return VISIBLE;
                    }
                        
                        

                    else if (board[x_pos][y_pos])
                        break;
                }

                for (int x_pos = rook_scan[i].x - 1, y_pos = rook_scan[i].y; x_pos >= 0; x_pos--)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, rook_scan[i].x, rook_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


                for (int x_pos = rook_scan[i].x, y_pos = rook_scan[i].y + 1; y_pos < S_BOARD; y_pos++)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, rook_scan[i].x, rook_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


                for (int x_pos = rook_scan[i].x, y_pos = rook_scan[i].y - 1; y_pos >= 0; y_pos--)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, rook_scan[i].x, rook_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }
            }


        return 0;
        break;
    }

    if (purpose != CHECK)
        goto end;


    for (int i = 0; i < count; i++)
    {
        for (int pos_x = find_rook[i].x + 1; pos_x < S_BOARD; pos_x++)
        {
            if (board[pos_x][find_rook[i].y] == king)
            {
                check = true;
                goto end;
            }

            else if (board[pos_x][find_rook[i].y])
                break;
        }

        for (int pos_x = find_rook[i].x - 1; pos_x >= 0; pos_x--)
        {
            if (board[pos_x][find_rook[i].y] == king)
            {
                check = true;
                goto end;
            }

            else if (board[pos_x][find_rook[i].y])
                break;
        }

        for (int pos_y = find_rook[i].y + 1; pos_y < S_BOARD; pos_y++)
        {
            if (board[find_rook[i].x][pos_y] == king)
            {
                check = true;
                goto end;
            }

            else if (board[find_rook[i].x][pos_y])
                break;
        }   

        for (int pos_y = find_rook[i].y - 1; pos_y >= 0; pos_y--)
        {
            if (board[find_rook[i].x][pos_y] == king)
            {
                check = true;
                goto end;
            }

            else if (board[find_rook[i].x][pos_y])
                break;
        }
    }


    end:


    if (prevent_castle)
    {
        printf("castling not possible\n");
        getchar();
        return PREVENT_CASTLE;
    }

    if (check)
        return CHECK;

    return 0;
}

int sightBishop (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior)
{
    coords find_bishop[MAX_PCS];
    int count = 0;

    int enemy_king = 0;

    if (savior != NULL)
    {
        savior -> x = 0;
        savior -> y = 0;
        savior -> piece = 0;
    }


    switch (king)
    {
        case w_K:
            enemy_king = b_K;
        break;

        case b_K:
            enemy_king = w_K;
        break;
    }



    findPiece(find_bishop, board, BISHOP, &count, enemy_king);


    bool prevent_castle = false;
    bool check = false;




    if (count == 0)
        goto end;


    switch (purpose)
    {
        case LONG_CASTLE:

            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int x_pos = find_bishop[i].x - 1, y_pos = find_bishop[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            prevent_castle = false;
                            break;
                        }
                    }


                    for (int x_pos = find_bishop[i].x + 1, y_pos = find_bishop[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            prevent_castle = false;
                            goto end;
                        }
                    }
                }


            }

            else if (king == b_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int x_pos = find_bishop[i].x - 1, y_pos = find_bishop[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            prevent_castle = false;
                            break;
                        }
                    }


                    for (int x_pos = find_bishop[i].x + 1, y_pos = find_bishop[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            prevent_castle = false;
                            goto end;
                        }
                    }
                }
            }
        break;


        case SHORT_CASTLE:

            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int x_pos = find_bishop[i].x - 1, y_pos = find_bishop[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            prevent_castle = false;
                            break;
                        }
                    }

                    for (int x_pos = find_bishop[i].x + 1, y_pos = find_bishop[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            prevent_castle = false;
                            break;
                        }
                    }
                }
            }


            else if (king == b_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int x_pos = find_bishop[i].x - 1, y_pos = find_bishop[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            prevent_castle = false;
                            break;
                        }
                    }

                    for (int x_pos = find_bishop[i].x + 1, y_pos = find_bishop[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            prevent_castle = false;
                            break;
                        }
                    }
                }
            }

        break;


        case SCAN:

            coords bishop_scan[MAX_PCS];
            int cnt_scan = 0;
            
            findPiece(bishop_scan, board, BISHOP, &cnt_scan, king);

            if (savior != NULL)
            {
                savior -> x = -1;
                savior -> y = -1;
                savior -> piece = -1;   
            }
    
            for (int i = 0; i < cnt_scan; i++)
            {
                for (int x_pos = bishop_scan[i].x + 1, y_pos = bishop_scan[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, bishop_scan[i].x , bishop_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }

                for (int x_pos = bishop_scan[i].x + 1, y_pos = bishop_scan[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, bishop_scan[i].x, bishop_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


                for (int x_pos = bishop_scan[i].x - 1, y_pos = bishop_scan[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, bishop_scan[i].x, bishop_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }

                for (int x_pos = bishop_scan[i].x - 1, y_pos = bishop_scan[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, bishop_scan[i].x, bishop_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


            }
        
        return 0;
        break;
    }



    if (purpose != CHECK)
        goto end;

    for (int i = 0; i < count; i++)
    {
        for (int x_pos = find_bishop[i].x - 1, y_pos = find_bishop[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }

        for (int x_pos = find_bishop[i].x + 1, y_pos = find_bishop[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }

        for (int x_pos = find_bishop[i].x - 1, y_pos = find_bishop[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }

        for (int x_pos = find_bishop[i].x + 1, y_pos = find_bishop[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }
    }


    end:

    if (prevent_castle)
    {
        fprintf(stderr, "%sCastling not allowed%s", RED, RESET);
        return PREVENT_CASTLE;
    }

    if (check)
        return CHECK;

    return 0;
}



int sightKnight (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior)
{
    coords find_knight[MAX_PCS];


    int enemy_king = 0;

    int count = 0;
    bool prevent_castle = false;
    bool check = false;

    
    
    if (savior != NULL)
    {
        savior -> x = 0;
        savior -> y = 0;
        savior -> piece = 0;
    }


    switch (king)
    {
        case w_K:
            enemy_king = b_K;
        break;

        case b_K:
            enemy_king = w_K;
        break;
    }


    findPiece(find_knight, board, KNIGHT, &count, enemy_king);


    if (count == 0)
        goto end;


    switch (purpose)
    {
        case LONG_CASTLE:

            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    if (find_knight[i].x < 0 || find_knight[i].x >= S_BOARD)
                        continue;

                    if (find_knight[i].y < 0 || find_knight[i].y >= S_BOARD)
                        continue;

                    if (find_knight[i].x >= b && find_knight[i].x <= d && find_knight[i].y == Pos(1))
                    {
                        prevent_castle = true;
                        goto end;
                    }
                }
            }

            else if (king == b_K)
            {
                for (int i = 0; i < count; i++)
                {
                    if (find_knight[i].x < 0 || find_knight[i].x >= S_BOARD)
                        continue;

                    if (find_knight[i].y < 0 || find_knight[i].y >= S_BOARD)
                        continue;

                    if (find_knight[i].x >= b && find_knight[i].x <= d && find_knight[i].y == Pos(8))
                    {
                        prevent_castle = true;
                        goto end;
                    }
                }
            }

        break;

        case SHORT_CASTLE:

            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    if (find_knight[i].x < 0 || find_knight[i].x >= S_BOARD)
                        continue;

                    if (find_knight[i].y < 0 || find_knight[i].y >= S_BOARD)
                        continue;

                    if (find_knight[i].x >= f && find_knight[i].x <= g && find_knight[i].y == Pos(1))
                    {
                        prevent_castle = true;
                        goto end;
                    }
                }
            }

            else if (king == b_K)
            {
                for (int i = 0; i < count; i++)
                {
                    if (find_knight[i].x < 0 || find_knight[i].x >= S_BOARD) 
                        continue;   
                    
                    if (find_knight[i].y < 0 || find_knight[i].y >= S_BOARD)
                        continue;

                    if (find_knight[i].x >= f && find_knight[i].y <= g && find_knight[i].y == Pos(8))
                    {
                        prevent_castle = true;
                        goto end;
                    }
                }
            }

        break;


        case SCAN:

            coords knight_scan[MAX_PCS];
            int cnt_scan = 0;

            if (savior != NULL)
            {
                savior -> x = -1;
                savior -> y = -1;
                savior -> piece = -1;   
            }

            findPiece(knight_scan, board, KNIGHT, &cnt_scan, king);


            for (int i = 0; i < cnt_scan; i++)
            {
                for (int x_pos = -2; x_pos <= 2; x_pos++)
                {
                    for (int y_pos = -2; y_pos <= 2; y_pos++)
                    {
                        if (abs(x_pos) == abs(y_pos))
                            continue;

                        if (x_pos == 0 || y_pos == 0)
                            continue;

                        if (knight_scan[i].x + x_pos < 0 || knight_scan[i].x + x_pos >= S_BOARD)
                            continue;

                        if (knight_scan[i].y + y_pos < 0 || knight_scan[i].y + y_pos >= S_BOARD)
                            continue;


                        if (knight_scan[i].x + x_pos == x_scan && knight_scan[i].y + y_pos == y_scan)
                        {
                            findSavior(board, knight_scan[i].x, knight_scan[i].y, savior);

                            return VISIBLE;
                        }

                        else if (board[knight_scan[i].x + x_pos][knight_scan[i].y + y_pos])
                            break;
                    }
                }
            }


            break;
            return 0;
    }

    if (purpose != CHECK)
        goto end;


    for (int i = 0; i < count; i++)
    {
        for (int x_pos = -2; x_pos <= 2; x_pos++)
        {
            for (int y_pos = -2; y_pos <= 2; y_pos++)
            {
                if (abs(x_pos) == abs(y_pos))
                    continue;

                if (x_pos == 0 || y_pos == 0)
                    continue;

                if (x_pos + find_knight[i].x < 0 || x_pos + find_knight[i].x >= S_BOARD)
                    continue;

                if (y_pos + find_knight[i].y < 0 || y_pos + find_knight[i].y >= S_BOARD)
                    continue;

                if (board[x_pos + find_knight[i].x][y_pos + find_knight[i].y] == king)
                {
                    check = true;
                    goto end;
                }
            }
        }        
    }


    end:

    if (prevent_castle)
        return PREVENT_CASTLE;


    if (check)
        return CHECK;

    return 0;
}


int sightQueen (int board[S_BOARD][S_BOARD], const int king, const int purpose, const int x_scan, const int y_scan, coords *savior)
{
    coords find_queen[MAX_PCS];
    int count = 0;

    int enemy_king = 0;

    bool prevent_castle = false;
    bool check = false;

    if (savior != NULL)
    {
        savior -> x = 0;
        savior -> y = 0;
        savior -> piece = 0;
    }


    switch (king)
    {
        case w_K:
            enemy_king = b_K;
        break;

        case b_K:
            enemy_king = w_K;
        break;
    }


    findPiece(find_queen, board, QUEEN, &count, enemy_king);


    if (count == 0)
        goto end;


    switch (purpose)
    {
        case LONG_CASTLE:
            
            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                            break;
                    }

                    for (int x_pos = find_queen[i].x, y_pos = find_queen[i].y - 1; y_pos >= 0; y_pos--)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[find_queen[i].x][y_pos])
                            break;
                    }

                    for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                            break;
                    }
                }
            }

            else if (king == b_K) 
            {
                for (int i = 0; i < count; i++)
                {
                    for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            break;
                        }
                    }

                    for (int x_pos = find_queen[i].x, y_pos = find_queen[i].y + 1; y_pos < S_BOARD; y_pos++)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            break;
                        }
                    }

                    for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
                    {
                        if (x_pos >= b && x_pos <= d && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            break;
                        }
                    }
                    
                }                
            }           

            
        break;
    
    
        case SHORT_CASTLE:
            
            if (king == w_K)
            {
                for (int i = 0; i < count; i++)
                {
                    for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                            break;
                    }

                    for (int x_pos = find_queen[i].x, y_pos = find_queen[i].y - 1; y_pos >= 0; y_pos--)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[find_queen[i].x][y_pos])
                            break;
                    }

                    for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(1))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                            break;
                    }
                }
            }

            else if (king == b_K) 
            {
                for (int i = 0; i < count; i++)
                {
                    for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            break;
                        }
                    }

                    for (int x_pos = find_queen[i].x, y_pos = find_queen[i].y + 1; y_pos < S_BOARD; y_pos++)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            break;
                        }
                    }

                    for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
                    {
                        if (x_pos >= f && x_pos <= g && y_pos == Pos(8))
                        {
                            prevent_castle = true;
                            goto end;
                        }

                        else if (board[x_pos][y_pos])
                        {
                            break;
                        }
                    }
                    
                }                
            }           

        break;


        case SCAN:

            coords queen_scan[MAX_PCS];
            int cnt_scan = 0;

            if (savior != NULL)
            {
                savior -> x = -1;
                savior -> y = -1;
                savior -> piece = -1;   
            }

            findPiece(queen_scan, board, QUEEN, &cnt_scan, king);
           

            for (int i = 0; i < cnt_scan; i++)
            {
                for (int x_pos = queen_scan[i].x + 1, y_pos = queen_scan[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, queen_scan[i].x, queen_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


                for (int x_pos = queen_scan[i].x + 1, y_pos = queen_scan[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, queen_scan[i].x, queen_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }

                for (int x_pos = queen_scan[i].x - 1, y_pos = queen_scan[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, queen_scan[i].x, queen_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }



                for (int x_pos = queen_scan[i].x - 1, y_pos = queen_scan[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, queen_scan[i].x, queen_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


                for (int x_pos = queen_scan[i].x + 1, y_pos = queen_scan[i].y; x_pos < S_BOARD; x_pos++)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, queen_scan[i].x, queen_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


                for (int x_pos = queen_scan[i].x - 1, y_pos = queen_scan[i].y; x_pos >= 0; x_pos--)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, queen_scan[i].x, queen_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


                for (int x_pos = queen_scan[i].x, y_pos = queen_scan[i].y + 1; y_pos < S_BOARD; y_pos++)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, queen_scan[i].x, queen_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }


                for (int x_pos = queen_scan[i].x, y_pos = queen_scan[i].y - 1; y_pos >= 0; y_pos--)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, queen_scan[i].x, queen_scan[i].y, savior);

                        return VISIBLE;
                    }

                    else if (board[x_pos][y_pos])
                        break;
                }

                
            }

        

        return 0;
        break;
    }


    if (purpose != CHECK)
        goto end; 

    for (int i = 0; i < count; i++)
    {
        for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }

        for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }


        for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }


        for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }


        for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y; x_pos < S_BOARD; x_pos++)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }


        for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y; x_pos >= 0; x_pos--)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }

        
        for (int x_pos = find_queen[i].x, y_pos = find_queen[i].y + 1; y_pos < S_BOARD; y_pos++)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }


        for (int x_pos = find_queen[i].x, y_pos = find_queen[i].y - 1; y_pos >= 0; y_pos--)
        {
            if (board[x_pos][y_pos] == king)
            {
                check = true;
                goto end;
            }

            else if (board[x_pos][y_pos])
                break;
        }
    }

    end:

    if (prevent_castle)
        return PREVENT_CASTLE;

    if (check)
        return CHECK;
    
    return 0;
}



int sightKing (int board[S_BOARD][S_BOARD], const int king, const int purpose)
{
    coords find_king;
    bool found_king = false;

    bool prevent_castle = false;
    bool check = false;


    int ally_side = 0;

    if (king == w_K)
        ally_side = b_K;
    
    else if (king == b_K)
        ally_side = w_K;

    switch (king)
    {
        case w_K:

            for (int i = 0; i < S_BOARD; i++)
            {
                for (int j = 0; j < S_BOARD; j++)
                {
                    if (board[i][j] == b_K)
                    {
                        find_king.x = i;
                        find_king.y = j;
                        find_king.piece = board[i][j];

                        found_king = true;
                    }

                    if (found_king)
                        break;
                }

                if (found_king)
                    break;
            }

        break;


        case b_K:

            for (int i = 0; i < S_BOARD; i++)
            {
                for (int j = 0; j < S_BOARD; j++)
                {
                    if (board[i][j] == w_K)
                    {
                        find_king.x = i;
                        find_king.y = j;
                        find_king.piece = board[i][j];

                        found_king = true;
                    }

                    if (found_king)
                        break;
                }

                if (found_king)
                    break;
            }

        break;
    }



    int which_rank = 0;


    if (king == w_K)
        which_rank = Pos(1);

    else if (king == b_K)
        which_rank = Pos(8);

    switch (purpose)
    {
        case LONG_CASTLE:

            for (int x_pos = -1; x_pos <= 1; x_pos++)
            {
                for (int y_pos = -1; y_pos <= -1; y_pos++)
                {
                    if (x_pos == 0 && y_pos == 0)
                        continue;

                    if (find_king.x + x_pos < 0 || find_king.x + x_pos >= S_BOARD)
                        continue;

                    if (find_king.y + y_pos < 0 || find_king.y + y_pos >= S_BOARD)
                        continue;

                    if (find_king.x + x_pos >= b && find_king.x + x_pos <= d && find_king.y + y_pos == which_rank)
                    {
                        prevent_castle = true;
                        goto end;
                    }
                }
            }

        break;

        case SHORT_CASTLE:

            for (int x_pos = -1; x_pos <= 1; x_pos++)
            {
                for (int y_pos = -1; y_pos <= 1; y_pos++)
                {
                    if (x_pos == 0 && y_pos == 0)
                        continue;

                    if (find_king.x + x_pos < 0 || find_king.x + x_pos >= S_BOARD)
                        continue;

                    if (find_king.y + y_pos < 0 || find_king.y + y_pos >= S_BOARD)
                        continue;

                    if (find_king.x + x_pos >= f && find_king.x + x_pos <= g && find_king.y + y_pos == which_rank)
                    {
                        prevent_castle = true;
                        goto end;
                    }
                }
            }
            
        break;
    }

    if (purpose != CHECK)
        goto end;

    for (int x_pos = -1; x_pos <= 1; x_pos++)
    {
        for (int y_pos = -1; y_pos <= 1; y_pos++)
        {
            if (x_pos == 0 && y_pos == 0)
                continue;

            if (find_king.x + x_pos < 0 || find_king.x + x_pos >= S_BOARD)
                continue;

            if (find_king.y + y_pos < 0 || find_king.y + y_pos >= S_BOARD)
                continue;

            if (board[find_king.x + x_pos][find_king.y + y_pos] == ally_side)
            {
                check = true;
                goto end;
            }
        }
    }

        
    end:

    if (prevent_castle)
        return PREVENT_CASTLE;

    if (check)
        return CHECK;


    return 0;
}


int checkmate (int board[S_BOARD][S_BOARD], const int side)
{
    int dupli_board[S_BOARD][S_BOARD];
    int backup_dupli_board[S_BOARD][S_BOARD];

    memcpy(dupli_board, board, sizeof dupli_board);
    memcpy(backup_dupli_board, board, sizeof backup_dupli_board);

    coords find_king; 
    find_king.x = -1;
    find_king.y = -1;
    find_king.piece = -1;

    bool isthekingthere = false;
    bool curr_position_safe = true;
    bool surrounded = false;

    int king = 0;
    int collect_ally = 0;
    int collect_enemy = 0;
    int enemy_king = 0;


    int count_valid_moves = 0;
    int count_invalid_moves = 0;






    coords attacker;

    attacker.x = -1;
    attacker.y = -1;
    attacker.piece = -1;

    bool king_found = false;
    bool attacker_found = false;
    bool search_left = false;
    bool search_right = false;
    bool search_up = false;
    bool search_bottom = false;
    bool search_leftup = false;
    bool search_rightup = false;
    bool search_leftbottom = false;
    bool search_rightbottom = false;

    bool can_block_checkmate = true;






    switch (side)
    {
        case 0:
            king = w_K;
            enemy_king = b_K;
            collect_ally = WHITE_P;
            collect_enemy = BLACK_P;
            break;
        
        case 1:
            king = b_K;
            enemy_king = w_K;
            collect_ally = BLACK_P;
            collect_enemy = WHITE_P;
            break;
    }


    for (int i = 0; i < S_BOARD; i++)
    {
        for (int j = 0; j < S_BOARD; j++)
        {
            if (board[i][j] == king)
            {
                find_king.x = i;
                find_king.y = j;
                find_king.piece = board[i][j];

                isthekingthere = true;
            }

            if (isthekingthere)
                break;
        }

        if (isthekingthere)
            break;
    }


    if (isthekingthere == false)
        goto end;



    
    for (int pos_x = -1; pos_x <= 1; pos_x++)
    {
        for (int pos_y = -1; pos_y <= 1; pos_y++)
        {
            if (pos_x == 0 && pos_y == 0)
            {
                if (theAllSeeingEye(dupli_board, king, CHECK) != CHECK)
                {
                    curr_position_safe = true;
                }

                else
                {
                    curr_position_safe = false;
                }

                continue;
            }


            


            if (find_king.x + pos_x < 0 || find_king.x + pos_x >= S_BOARD)
            {
                count_invalid_moves++;
                continue;
            }

            if (find_king.y + pos_y < 0 || find_king.y + pos_y >= S_BOARD)
            {
                count_invalid_moves++;
                continue;
            }

            /*   if (curr_position_safe == false)
            {
                count_invalid_moves++;
            }
*/
            int enemy_piece = dupli_board[find_king.x + pos_x][find_king.y + pos_y];



            if (!enemy_piece || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                dupli_board[find_king.x][find_king.y] = 0;
                dupli_board[find_king.x + pos_x][find_king.y + pos_y] = king;


                if (theAllSeeingEye(dupli_board, king, CHECK) == CHECK)
                    count_invalid_moves++;
            
            }

            else
                count_invalid_moves++;


            memcpy(dupli_board, backup_dupli_board, sizeof dupli_board);
        }
    }




    if (count_invalid_moves >= 8 && curr_position_safe == false)
    {
        surrounded = true;
    }




    if (surrounded)
    {
        for (int i = 0; i < S_BOARD; i++)
        {
            for (int j = 0; j < S_BOARD; j++)
            {
                if (board[i][j] == king)
                {
                    find_king.x = i;
                    find_king.y = j;
                    find_king.piece = board[i][j];

                    king_found = true;
                }

                if (king_found)
                    break;
            }

            if (king_found)
                break;
        }


        if (king_found == false)
            goto end;


        for (int pos_x = find_king.x + 1, pos_y = find_king.y + 1; pos_x < S_BOARD && pos_y < S_BOARD; pos_x++, pos_y++)
        {
            int enemy_piece = board[pos_x][pos_y];

            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                attacker.x = pos_x;
                attacker.y = pos_y;
                attacker.piece = board[pos_x][pos_y];

                attacker_found = true;

                goto attack_located;
            }

            else if (enemy_piece)
                break;
        }



        for (int pos_x = find_king.x + 1, pos_y = find_king.y - 1; pos_x < S_BOARD && pos_y >= 0; pos_x++, pos_y--)
        {
            int enemy_piece = board[pos_x][pos_y];
            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                attacker.x = pos_x;
                attacker.y = pos_y;
                attacker.piece = board[pos_x][pos_y];

                attacker_found = true;

                goto attack_located;
            }

            else if (enemy_piece)
                break;
        }




        for (int pos_x = find_king.x - 1, pos_y = find_king.y + 1; pos_x >= 0 && pos_y < S_BOARD; pos_x--, pos_y++)
        {
            int enemy_piece = board[pos_x][pos_y];
            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                attacker.x = pos_x;
                attacker.y = pos_y;
                attacker.piece = board[pos_x][pos_y];

                attacker_found = true;

                goto attack_located;
            }

            else if (enemy_piece)
                break;
        }


        for (int pos_x = find_king.x - 1, pos_y = find_king.y - 1; pos_x >= 0 && pos_y >= 0; pos_x--, pos_y--)
        {
            int enemy_piece = board[pos_x][pos_y];
            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                attacker.x = pos_x;
                attacker.y = pos_y;
                attacker.piece = board[pos_x][pos_y];

                attacker_found = true;

                goto attack_located;
            }

            else if (enemy_piece)
                break;
        }



        for (int pos_x = find_king.x, pos_y = find_king.y + 1; pos_y < S_BOARD; pos_y++)
        {
            int enemy_piece = board[pos_x][pos_y];
            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                attacker.x = pos_x;
                attacker.y = pos_y;
                attacker.piece = board[pos_x][pos_y];

                attacker_found = true;

                goto attack_located;
            }

            else if (enemy_piece)
                break;
        }



        for (int pos_x = find_king.x, pos_y = find_king.y - 1; pos_y >= 0; pos_y--)
        {
            int enemy_piece = board[pos_x][pos_y];
            
            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                attacker.x = pos_x;
                attacker.y = pos_y;
                attacker.piece = board[pos_x][pos_y];

                attacker_found = true;

                goto attack_located;
            }

            else if (enemy_piece)
                break;
        }



        for (int pos_x = find_king.x + 1, pos_y = find_king.y; pos_x < S_BOARD; pos_x++)
        {
            int enemy_piece = board[pos_x][pos_y];
            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                attacker.x = pos_x;
                attacker.y = pos_y;
                attacker.piece = board[pos_x][pos_y];

                attacker_found = true;

                goto attack_located;
            }

            else if (enemy_piece)
                break;
        }


        for (int pos_x = find_king.x - 1, pos_y = find_king.y; pos_x >= 0; pos_x--)
        {
            int enemy_piece = board[pos_x][pos_y];
            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                attacker.x = pos_x;
                attacker.y = pos_y;
                attacker.piece = board[pos_x][pos_y];

                attacker_found = true;

                goto attack_located;
            }

            else if (enemy_piece)
                break;
        }


        attack_located:


        if (find_king.x == attacker.x && find_king.y > attacker.y)
            search_bottom = true;

        else if (find_king.x == attacker.x && find_king.y < attacker.y)
            search_up = true;

        else if (find_king.x < attacker.x && find_king.y == attacker.y)
            search_right = true;

        else if (find_king.x > attacker.x && find_king.y == attacker.y)
            search_left = true;

        else if (find_king.x > attacker.x && find_king.y > attacker.y)
            search_leftbottom = true;

        else if (find_king.x < attacker.x && find_king.y > attacker.y)
            search_rightbottom = true;

        else if (find_king.x < attacker.x && find_king.y < attacker.y)
            search_rightup = true;

        else if (find_king.x > attacker.x && find_king.y < attacker.y)
            search_leftup = true;


    }





    coords savior;



    if (search_bottom && attacker_found)
    {
/*         
        printf("Search bottom\n");
        getchar();
 */
        for (int x_scan = find_king.x, y_scan = find_king.y - 1; y_scan >= 0; y_scan--)
        {
            int piece = board[x_scan][y_scan];
            
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &savior, BLOCK) == VISIBLE)
            {
                can_block_checkmate = true;
                goto end;
            }

            else if (selectPiece(piece, collect_ally) == collect_ally)
                break;
        }
    }

    else if (search_up && attacker_found)
    {
/*         printf("Search up\n");
        getchar();
 */
        for (int x_scan = find_king.x, y_scan = find_king.y + 1; y_scan < S_BOARD; y_scan++)
        {
            int piece = board[x_scan][y_scan];
            
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &savior, BLOCK) == VISIBLE)
            {
                can_block_checkmate = true;
                goto end;
            }

            else if (selectPiece(piece, collect_ally) == collect_ally)
                break;

        }
    }

    else if (search_left && attacker_found)
    {
/*         
        printf("Search left\n");
        getchar();
 */
        for (int x_scan = find_king.x - 1, y_scan = find_king.y; x_scan >= 0; x_scan--)
        {
            int piece = board[x_scan][y_scan];
            
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &savior, BLOCK) == VISIBLE)
            {
                can_block_checkmate = true;
                goto end;
            }

            else if (selectPiece(piece, collect_ally) == collect_ally)
                break;
        } 
    }


    else if (search_right && attacker_found)
    {
/*         printf("Search right\n");
        getchar();
 */
        for (int x_scan = find_king.x + 1, y_scan = find_king.y; x_scan < S_BOARD; x_scan++)
        {
            int piece = board[x_scan][y_scan];
            
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &savior, BLOCK) == VISIBLE)
            {
                can_block_checkmate = true;
                goto end;
            }

            else if (selectPiece(piece, collect_ally) == collect_ally)
                break;
        }
    }


    else if (search_leftbottom && attacker_found)
    {
/*         printf("Search left bottom\n");
        getchar();
 */
        for (int x_scan = find_king.x - 1, y_scan = find_king.y - 1; x_scan >= 0 && y_scan >= 0; x_scan--, y_scan--)
        {
            int piece = board[x_scan][y_scan];
            
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &savior, BLOCK) == VISIBLE)
            {
                can_block_checkmate = true;
                goto end;
            }

            else if (selectPiece(piece, collect_ally) == collect_ally)
                break;
        }
    }


    else if (search_rightbottom && attacker_found)
    {
/*         printf("Search right bottom\n");
        getchar();
 */
        for (int x_scan = find_king.x + 1, y_scan = find_king.y - 1; x_scan < S_BOARD && y_scan >= 0; x_scan++, y_scan--)
        {
            int piece = board[x_scan][y_scan];
            
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &savior, BLOCK) == VISIBLE)
            {
                can_block_checkmate = true;
                goto end;
            }

            else if (selectPiece(piece, collect_ally) == collect_ally)
                break;
        }
    }

    else if (search_leftup && attacker_found)
    {
/*         printf("Search left up\n");
        getchar();
 */
        for (int x_scan = find_king.x - 1, y_scan = find_king.y + 1; x_scan >= 0 && y_scan < S_BOARD; x_scan--, y_scan++)
        {
            int piece = board[x_scan][y_scan];
            
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &savior, BLOCK) == VISIBLE)
            {
                can_block_checkmate = true;
                goto end;
            }

            else if (selectPiece(piece, collect_ally) == collect_ally)
                break;
        }
    }

    else if (search_rightup && attacker_found)
    {
/*         printf("Search right up\n");
        getchar();
 */
        for (int x_scan = find_king.x + 1, y_scan = find_king.y + 1; x_scan < S_BOARD && y_scan < S_BOARD; x_scan++, y_scan++)
        {
            int piece = board[x_scan][y_scan];
            
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &savior, BLOCK) == VISIBLE)
            {
                can_block_checkmate = true;
                goto end;
            }
            

            else if (selectPiece(piece, collect_ally) == collect_ally)
                break;
        }
    }


    end:


    // prevent side effects, ensures that the savior is found
    // if there is no savior, then blocking the checkmate is futile
    if (savior.x < 0 || savior.y < 0 || savior.piece < 0)
        can_block_checkmate = false;

  

    if (surrounded && !can_block_checkmate)
    {                
        int verdict = theCheckmateLastStand(board, king, attacker.x, attacker.y, NULL, ASSASSINATE);

        //printf("Surrounded");

        if (verdict == ASSASSINATE_FAIL)
        {
            //printf("Cannot block checkmate");
            can_block_checkmate = false;
        }

        
        else if (verdict == CHECK)
        {
            can_block_checkmate = false;
        }
    }

  



    if (surrounded && can_block_checkmate)
        printf("savior in: %c%d\n", savior.x + 'a', savior.y + 1);


    if (!can_block_checkmate && surrounded && !curr_position_safe)
        return CHECKMATE;


    return 0;
}

int theAllSeeingEye (int board[S_BOARD][S_BOARD], const int king, const int purpose)
{
    if (sightPawn(board, king, purpose, 0, 0, NULL))
        return CHECK;

    if (sightKnight(board, king, purpose, 0, 0, NULL))
        return CHECK;

    if (sightBishop(board, king, purpose, 0, 0, NULL))
        return CHECK;

    if (sightRook(board, king, purpose, 0, 0, NULL))
        return CHECK;

    if (sightQueen(board, king, purpose, 0, 0, NULL))
        return CHECK;

    if (sightKing(board, king, purpose))
        return CHECK;

    return 0;
}


int theCheckmateLastStand (int board[S_BOARD][S_BOARD], const int king, const int x_scan, const int y_scan, coords *savior, int purpose)
{
    switch (purpose)
    {
        case BLOCK:
        
            if (sightPawn(board, king, SCAN, x_scan, y_scan, savior) == VISIBLE)
                return VISIBLE;

            if (sightBishop(board, king, SCAN, x_scan, y_scan, savior) == VISIBLE)
                return VISIBLE;

            if (sightRook(board, king, SCAN, x_scan, y_scan, savior) == VISIBLE)
                return VISIBLE;

            if (sightQueen(board, king, SCAN, x_scan, y_scan, savior) == VISIBLE)
                return VISIBLE;

            if (sightKnight(board, king, SCAN, x_scan, y_scan, savior) == VISIBLE)
                return VISIBLE;


        break;



        case ASSASSINATE:
            
            
            
            int tmp_board[S_BOARD][S_BOARD];
            memcpy(tmp_board, board, sizeof tmp_board);

            coords find_king;

            //bool found_attacker = false;



            int enemy_king = 0;

            switch (king)
            {
                case w_K:
                    enemy_king = b_K;
                break;
            
                case b_K:
                    enemy_king = w_K;
                break;
            }



            findPiece(&find_king, board, KING, NULL, king);


            
            // must be a knight 
            // search for knights
            coords find_knight;
            bool found_knight = false;

            for (int i = -2; i <= 2; i++)
            {
                for (int j = -2; j <= 2; j++)
                {
                    int piece = 0;

                    if (find_king.x + i < 0 || find_king.x + i >= S_BOARD)
                        continue;

                    if (find_king.y + j < 0 || find_king.y + j >= S_BOARD)
                        continue;

                    if (abs(i) == abs(j))
                        continue;

                    if (i == 0 || j == 0)
                        continue;

                    piece = board[find_king.x + i][find_king.y + j];

                    if (selectPiece(piece, enemy_king) == KNIGHT)
                    {
                        find_knight.x = find_king.x + i;
                        find_knight.y = find_king.y + j;
                        find_knight.piece = piece;

                        found_knight = true;
                    }

                    if (found_knight)
                        break;

                }

                if (found_knight)
                    break;

            }


            if (found_knight)
                printf("Knight attacker in %c%d\n", find_knight.x + 'a', find_knight.y + 1);



            coords find_savior;

              
            if (found_knight)
            {
                // find enemy pieces that can capture the knight

                printf("I see you knight\n");
               
                if (theCheckmateLastStand(board, king, find_knight.x, find_knight.y, &find_savior, BLOCK) == VISIBLE)
                {
                    board[find_knight.x][find_knight.y] = find_savior.piece;
                    board[find_savior.x][find_savior.y] = 0;


                      // in some cases, savior is zero??? How to prevent this, where does this come from
                    if (find_savior.piece < 0 && find_savior.x < 0 && find_savior.y < 0)
                    {
                        // no one came to save
                        return CHECK;
                    }

                    printf("Savior is in %c%d\n", find_savior.x + 'a', find_savior.y + 1);


                    if (theAllSeeingEye(board, king, CHECK) == CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);

                        printf("Attempt to capture knight still in check\n");
                        getchar();

                        return CHECK;
                    }

                    memcpy(board, tmp_board, sizeof tmp_board);

                }

                
                if (theAllSeeingEye(board, king, CHECK) == CHECK)
                    return CHECK;
                    

                return ASSASSINATE_FAIL;
            }


            printf("Attacker is in %c%d\n", x_scan + 'a', y_scan + 1);

       
            if (theCheckmateLastStand(board, king, x_scan, y_scan, &find_savior, BLOCK) == VISIBLE)
            {
                board[x_scan][y_scan] = find_savior.piece;
                board[find_savior.x][find_savior.y] = 0;

                printf("Attacker is in %c%d\n", x_scan + 'a', y_scan + 1);

                printf("Savior is in %c%d\n", find_savior.x + 'a', find_savior.y + 1);
                printf("Savior is a %d\n", find_savior.piece);


                // in some cases, savior is zero??? How to prevent this, where does this come from
                if (find_savior.piece < 0 && find_savior.x < 0 && find_savior.y < 0)
                {
                    // no one came to save
                    return CHECK;
                }

                if (theAllSeeingEye(board, king, CHECK) == CHECK)
                {
                    
                    memcpy(board, tmp_board, sizeof tmp_board);
                    return CHECK;
                }


                memcpy(board, tmp_board, sizeof tmp_board);

            }


            if (theAllSeeingEye(board, king, CHECK) == CHECK)
                return CHECK;

                    
            return ASSASSINATE_SUCCESS;
        break;

    }

    return 0;
}


int stalemate (int board[S_BOARD][S_BOARD], const int side)
{
    if (mobilityPawn(board, side) == MOBILE)
    {
        //printf("pawn can move\n");
        return MOBILE;
    }

    if (mobilityRook(board, side) == MOBILE)
    {
        //printf("rook can move\n");
        return MOBILE;
    }

    if (mobilityBishop(board, side) == MOBILE)
    {
        //printf("bishop can move\n");
        return MOBILE;
    }

    if (mobilityKnight(board, side) == MOBILE)
    {
        //printf("knight can move\n");
        return MOBILE;
    }

    if (mobilityQueen(board, side) == MOBILE)
    {
        //printf("queen can move\n");
        return MOBILE;
    }

    if (mobilityKing(board, side) == MOBILE)
    {
        //printf("king can move\n");
        return MOBILE;
    }

    return IMMOBILE;
}


int mobilityPawn (int board[S_BOARD][S_BOARD], const int side)
{
    coords find_pawn[8];
    
    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);


    

    int king = 0;
    int cnt_pawn = 0;
    int range = 1;

    switch (side)
    {
        case 0:
            king = w_K;
        break;

        case 1:
            king = b_K;
        break;
    }

    findPiece(find_pawn, board, PAWN, &cnt_pawn, king);


    switch (king)
    {
        case w_K:

            for (int i = 0; i < cnt_pawn; i++)
            {
                int x_pos = find_pawn[i].x;
                int y_pos = find_pawn[i].y;
                int piece = find_pawn[i].piece;

                int enemy_piece_left = -1;
                int enemy_piece_right = -1;
                int en_passant_left = -1;
                int en_passant_right = -1;
                
                if (x_pos == Pos(2))
                    range = 2;


                for (int range_check = 1; range_check <= range; range_check++)
                {
                    if (y_pos + range_check < 0 || y_pos + range_check >= S_BOARD)
                        continue;

                    // obstacle checking
                    if (board[x_pos][y_pos + range_check])
                        break;
                    
                    if (board[x_pos][y_pos + range_check] == 0)
                    {
                        board[x_pos][y_pos] = 0;
                        board[x_pos][y_pos + range_check] = piece;
                    }

                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }

                    memcpy(board, tmp_board, sizeof tmp_board);
                }
                

                if (checkBounds(x_pos - 1) && checkBounds(y_pos + 1))
                    enemy_piece_left = board[x_pos - 1][y_pos + 1];

                if (checkBounds(x_pos + 1) && checkBounds(y_pos + 1))
                    enemy_piece_right = board[x_pos + 1][y_pos + 1];

                if (checkBounds(x_pos - 1))
                    en_passant_left = board[x_pos - 1][y_pos];

                if (checkBounds(x_pos + 1))
                    en_passant_right = board[x_pos + 1][y_pos];
    


                if (selectPiece(enemy_piece_left, BLACK_P) == BLACK_P)
                {
                    board[x_pos - 1][y_pos + 1] = piece;
                    board[x_pos][y_pos] = 0;

                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }
                }

                else if (selectPiece(enemy_piece_right, BLACK_P) == BLACK_P)
                {
                    board[x_pos + 1][y_pos + 1] = piece;
                    board[x_pos][y_pos] = 0;
                    
                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }
                }

                else if (recentDoubleSquarePawn(en_passant_left, READ) == en_passant_left)
                {
                    board[x_pos - 1][y_pos] = 0;
                    board[x_pos][y_pos] = 0;
                    board[x_pos - 1][y_pos + 1] = piece;

                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }
                }

                else if (recentDoubleSquarePawn(en_passant_right, READ) == en_passant_right)
                {
                    board[x_pos + 1][y_pos] = 0;
                    board[x_pos][y_pos] = 0;
                    board[x_pos + 1][y_pos + 1] = piece;

                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }
                }


                range = 1;
                enemy_piece_left = -1;
                enemy_piece_right = -1;
                en_passant_left = -1;
                en_passant_right = -1;


                memcpy(board, tmp_board, sizeof tmp_board);
            }

        break;

        case b_K:

            for (int i = 0; i < cnt_pawn; i++)
            {
                int x_pos = find_pawn[i].x;
                int y_pos = find_pawn[i].y;
                int piece = find_pawn[i].piece;

                int enemy_piece_left = -1;
                int enemy_piece_right = -1;
                int en_passant_left = -1;
                int en_passant_right = -1;

                
                if (x_pos == Pos(7))
                    range = 2;


                for (int range_check = 1; range_check <= range; range_check++)
                {
                    if (y_pos - range_check < 0 || y_pos - range_check >= S_BOARD)
                        continue;

                    // obstacle checking
                    if (board[x_pos][y_pos - range_check])
                        break;
                    
                    if (board[x_pos][y_pos - range_check] == 0)
                    {
                        board[x_pos][y_pos] = 0;
                        board[x_pos][y_pos - range_check] = piece;
                    }
                    
                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }

                    memcpy(board, tmp_board, sizeof tmp_board);
                }
                

                if (checkBounds(x_pos - 1) && checkBounds(y_pos - 1))
                    en_passant_left = board[x_pos - 1][y_pos - 1];

                if (checkBounds(x_pos + 1) && checkBounds(y_pos - 1))
                    en_passant_right = board[x_pos + 1][y_pos - 1];

                if (checkBounds(x_pos - 1))
                    en_passant_left = board[x_pos - 1][y_pos];

                if (checkBounds(x_pos + 1))
                    en_passant_right = board[x_pos + 1][y_pos];
    


                if (selectPiece(enemy_piece_left, WHITE_P) == WHITE_P)
                {
                    board[x_pos - 1][y_pos - 1] = piece;
                    board[x_pos][y_pos] = 0;


                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }
                }

                else if (selectPiece(enemy_piece_right, WHITE_P) == WHITE_P)
                {
                    board[x_pos + 1][y_pos - 1] = piece;
                    board[x_pos][y_pos] = 0;

                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }
                    
                }


                else if (recentDoubleSquarePawn(en_passant_left, READ) == en_passant_left)
                {
                    board[x_pos - 1][y_pos] = 0;
                    board[x_pos][y_pos] = 0;
                    board[x_pos - 1][y_pos - 1] = piece;

                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }
                }

                else if (recentDoubleSquarePawn(en_passant_right, READ) == en_passant_right)
                {
                    board[x_pos + 1][y_pos] = 0;
                    board[x_pos][y_pos] = 0;
                    board[x_pos + 1][y_pos - 1] = piece;

                    if (theAllSeeingEye(board, king, CHECK) != CHECK)
                    {
                        memcpy(board, tmp_board, sizeof tmp_board);
                        return MOBILE;
                    }
                }

                range = 1;
                enemy_piece_left = -1;
                enemy_piece_right = -1;
                en_passant_left = -1;
                en_passant_right = -1;

                memcpy(board, tmp_board, sizeof tmp_board);
            }

        break;
    }

    return IMMOBILE;
}


int mobilityRook (int board[S_BOARD][S_BOARD], const int side)
{
    coords find_rook[MAX_PCS];

    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    int cnt_rook = 0;

    int king = 0;
    int collect_enemy = 0;

    switch (side)
    {
        case 0:
            king = w_K;
            collect_enemy = BLACK_P;
            break;

        case 1:
            king = b_K;
            collect_enemy = WHITE_P;
            break;
    }

    findPiece(find_rook, board, ROOK, &cnt_rook, king);



    for (int i = 0; i < cnt_rook; i++)
    {
        for (int x_pos = find_rook[i].x + 1, y_pos = find_rook[i].y; x_pos <= S_BOARD; x_pos++)
        {
            int enemy_piece = 0;
            
            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;
            
            enemy_piece = board[x_pos][y_pos];
            
            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_rook[i].piece;
                board[find_rook[i].x][find_rook[i].y] = 0;
            }

            // break if ally is encountered
            else
                break;


            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }

        for (int x_pos = find_rook[i].x - 1, y_pos = find_rook[i].y; x_pos >= 0; x_pos--)
        {
            int enemy_piece = 0;
            
            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;
            
            enemy_piece = board[x_pos][y_pos];
            
            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_rook[i].piece;
                board[find_rook[i].x][find_rook[i].y] = 0;
            }

            // break if ally is encountered
            else
                break;


            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }


            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_rook[i].x, y_pos = find_rook[i].y + 1; y_pos < S_BOARD; y_pos++)
        {
            int enemy_piece = 0;
            
            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;
            
            enemy_piece = board[x_pos][y_pos];
            
            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_rook[i].piece;
                board[find_rook[i].x][find_rook[i].y] = 0;
            }

            // break if ally is encountered
            else
                break;


            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }


            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_rook[i].x, y_pos = find_rook[i].y - 1; y_pos >= 0; y_pos--)
        {
            int enemy_piece = 0;
            
            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;
            
            enemy_piece = board[x_pos][y_pos];
            
            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_rook[i].piece;
                board[find_rook[i].x][find_rook[i].y] = 0;
            }

            // break if ally is encountered
            else
                break;


            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }


            memcpy(board, tmp_board, sizeof tmp_board);
        }
    }

    return IMMOBILE;
}



int mobilityKnight (int board[S_BOARD][S_BOARD], const int side)
{
    coords find_knight[MAX_PCS];

    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    int cnt_knight = 0;

    int king = 0;
    int collect_enemy = 0;


    switch (side)
    {
        case 0:
            king = w_K;
            collect_enemy = BLACK_P;
            break;

        case 1:
            king = b_K;
            collect_enemy = WHITE_P;
            break;
    }


    findPiece(find_knight, board, KNIGHT, &cnt_knight, king);


    for (int i = 0; i < cnt_knight; i++)
    {
        for (int x_pos = -2; x_pos <= 2; x_pos++)
        {
            for (int y_pos = -2; y_pos <= 2; y_pos++)
            {
                int enemy_piece = 0;
                
                if (abs(x_pos) == abs(y_pos))
                    continue;
                
                if (x_pos == 0 || y_pos == 0)
                    continue;

                if (find_knight[i].x + x_pos < 0 || find_knight[i].x + x_pos >= S_BOARD)
                    continue;

                if (find_knight[i].y + y_pos < 0 || find_knight[i].y + y_pos >= S_BOARD)
                    continue;


                enemy_piece = board[find_knight[i].x + x_pos][find_knight[i].y + y_pos];

                if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
                {
                    board[find_knight[i].x + x_pos][find_knight[i].y + y_pos] = find_knight[i].piece;
                    board[find_knight[i].x][find_knight[i].y] = 0;
                }

                else
                    break;
                
                if (theAllSeeingEye(board, king, CHECK) != CHECK)
                {
                    memcpy(board, tmp_board, sizeof tmp_board);
                    return MOBILE;
                }

                memcpy(board, tmp_board, sizeof tmp_board);
            }
        }
    }


    return IMMOBILE;
}



int mobilityBishop (int board[S_BOARD][S_BOARD], const int side)
{
    coords find_bishop[MAX_PCS];

    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    int cnt_bishop = 0;

    int king = 0;
    int collect_enemy = 0;

    switch (side)
    {
        case 0:
            king = w_K;
            collect_enemy = BLACK_P;
            break;

        case 1:
            king = b_K;
            collect_enemy = WHITE_P;
            break;
    }

    findPiece(find_bishop, board, BISHOP, &cnt_bishop, king);


    for (int i = 0; i < cnt_bishop; i++)
    {
        for (int x_pos = find_bishop[i].x + 1, y_pos = find_bishop[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
        {
            int enemy_piece = 0;
            
            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_bishop[i].piece;
                board[find_bishop[i].x][find_bishop[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_bishop[i].x + 1, y_pos = find_bishop[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
        {
            int enemy_piece = 0;
            
            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_bishop[i].piece;
                board[find_bishop[i].x][find_bishop[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_bishop[i].x - 1, y_pos = find_bishop[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
        {
            int enemy_piece = 0;
            
            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_bishop[i].piece;
                board[find_bishop[i].x][find_bishop[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_bishop[i].x - 1, y_pos = find_bishop[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
        {
            int enemy_piece = 0;
            
            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_bishop[i].piece;
                board[find_bishop[i].x][find_bishop[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }
    }

    return IMMOBILE;
}


int mobilityQueen (int board[S_BOARD][S_BOARD], const int side)
{
    coords find_queen[MAX_PCS];

    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    int cnt_queen = 0;

    int king = 0;
    int collect_enemy = 0;

    switch (side)
    {
        case 0:
            king = w_K;
            collect_enemy = BLACK_P;
        break;


        case 1:
            king = b_K;
            collect_enemy = WHITE_P;
        break;
    }


    findPiece(find_queen, board, QUEEN, &cnt_queen, king);


    for (int i = 0; i < cnt_queen; i++)
    {
        for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y + 1; x_pos < S_BOARD && y_pos < S_BOARD; x_pos++, y_pos++)
        {
            int enemy_piece = 0;

            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y - 1; x_pos < S_BOARD && y_pos >= 0; x_pos++, y_pos--)
        {
            int enemy_piece = 0;

            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y + 1; x_pos >= 0 && y_pos < S_BOARD; x_pos--, y_pos++)
        {
            int enemy_piece = 0;

            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }

        for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y - 1; x_pos >= 0 && y_pos >= 0; x_pos--, y_pos--)
        {
            int enemy_piece = 0;

            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_queen[i].x + 1, y_pos = find_queen[i].y; x_pos < S_BOARD; x_pos++)
        {
            int enemy_piece = 0;

            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }

        
        for (int x_pos = find_queen[i].x - 1, y_pos = find_queen[i].y; x_pos >= 0; x_pos--)
        {
            int enemy_piece = 0;

            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }


        for (int x_pos = find_queen[i].x, y_pos = find_queen[i].y + 1; y_pos < S_BOARD; y_pos++)
        {
            int enemy_piece = 0;

            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }

        for (int x_pos = find_queen[i].x, y_pos = find_queen[i].y - 1; y_pos >= 0; y_pos--)
        {
            int enemy_piece = 0;

            if (x_pos < 0 || x_pos >= S_BOARD)
                continue;

            if (y_pos < 0 || y_pos >= S_BOARD)
                continue;

            enemy_piece = board[x_pos][y_pos];

            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[x_pos][y_pos] = find_queen[i].piece;
                board[find_queen[i].x][find_queen[i].y] = 0;
            }

            else
                break;

            if (theAllSeeingEye(board, king, CHECK) != CHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);
        }
    }


    return IMMOBILE;
}


int mobilityKing (int board[S_BOARD][S_BOARD], const int side)
{
    coords find_king;

    find_king.x = -1;
    find_king.y = -1;
    find_king.piece = -1;

    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    int king = 0;
    int collect_enemy = 0;


    switch (side)
    {
        case 0:
            king = w_K;
            collect_enemy = BLACK_P;
        break;

        case 1:
            king = b_K;
            collect_enemy = WHITE_P;
        break;
    }
    

    findPiece(&find_king, board, KING, NULL, king);


    if (find_king.x < 0 && find_king.y < 0 && find_king.piece < 0)
    {
        printf("King is missing??\n");
        exit (EXIT_FAILURE);
    }



    if (theAllSeeingEye(board, king, CHECK) == CHECK)
    {
        // it is stalemate if king is not in check and king cannot move on the next turn
        return CHECK;
    }



    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int enemy_piece = 0;
            
            
            if (i == 0 && j == 0)
                continue;


            if (find_king.x + i < 0 || find_king.x + i >= S_BOARD)
                continue;

            if (find_king.y + j < 0 || find_king.y + j >= S_BOARD)
                continue;


            enemy_piece = board[find_king.x + i][find_king.y + j];


            if (enemy_piece == 0 || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[find_king.x + i][find_king.y + j] = king;
                board[find_king.x][find_king.y] = 0;



                if (theAllSeeingEye(board, king, CHECK) != CHECK)
                {
                    memcpy(board, tmp_board, sizeof tmp_board);
                    return MOBILE;
                }
            }


            memcpy(board, tmp_board, sizeof tmp_board);
        }
    }

    return IMMOBILE;
}


int selectPiece (int piece, const int king)
{
    
    if (king == w_K)
    {
        switch (piece)
        {
            case w_P1 ... w_P8:
                return PAWN;
            break;

            case w_lN: case w_dN:
                return KNIGHT;
            break;

            case w_N1xx ... w_N8xx:
                return KNIGHT;
            break;

            case w_lR: case w_dR:
                return ROOK;
            break;

            case w_R1xx ... w_R8xx:
                return ROOK;
            break;

            case w_lB: case w_dB:
                return BISHOP;
            break;

            case w_B1xx ... w_B8xx:
                return BISHOP;
            break;

            case w_Q:
                return QUEEN;
            break;

            case w_Q1xx ... w_Q8xx:
                return QUEEN;
            break;

            case w_K:
                return KING;
            break;

        }

    }

    else if (king == b_K)
    {
        switch (piece)
        {
            case b_P1 ... b_P8:
                return PAWN;
            break;

            case b_lN: case b_dN:
                return KNIGHT;
            break;

            case b_N1xx ... b_N8xx:
                return KNIGHT;
            break;

            case b_lR: case b_dR:
                return ROOK;
            break;

            case b_R1xx ... b_R8xx:
                return ROOK;
            break;

            case b_lB: case b_dB:
                return BISHOP;
            break;

            case b_B1xx ... b_B8xx:
                return BISHOP;
            break;

            case b_Q:
                return QUEEN;
            break;

            case b_Q1xx ... b_Q8xx:
                return QUEEN;
            break;

            case b_K:
                return KING;
            break;
        }
    }


    else if (king == WHITE_P || king == BLACK_P)
    {
        switch (piece)
        {
            case w_P1 ... w_P8:
            case w_lN: case w_dN:
            case w_N1xx ... w_N8xx:
            case w_lR: case w_dR:
            case w_R1xx ... w_R8xx:
            case w_lB: case w_dB:
            case w_B1xx ... w_B8xx:
            case w_Q:
            case w_Q1xx ... w_Q8xx:
            case w_K:
               return WHITE_P;

            break;



            case b_P1 ... b_P8:
            case b_lN: case b_dN:
            case b_N1xx ... b_N8xx:
            case b_lR: case b_dR:
            case b_R1xx ... b_R8xx:
            case b_lB: case b_dB:
            case b_B1xx ... b_B8xx:
            case b_Q:
            case b_Q1xx ... b_Q8xx:
            case b_K:
                return BLACK_P;

            break;
        }
    }


    return -1;
}


void *findPiece(coords find_piece[], int board[S_BOARD][S_BOARD], int collect_piece, int *cnt, const int king)
{
    if (cnt == NULL)
    {
        // if finding the king
        // doesn't need a counter because there is only one king per side

        for (int i = 0; i < S_BOARD; i++)
        {
            for (int j = 0; j < S_BOARD; j++)
            {
                int piece = board[i][j];

                if (selectPiece(piece, king) == collect_piece)
                {
                    find_piece -> x  = i;
                    find_piece -> y = j;
                    find_piece -> piece = piece;

                    return NULL;
                }
            }
        }

        return NULL;
    }
    
    *cnt = 0;
    
    for (int i = 0; i < S_BOARD; i++)
    {
        for (int j = 0; j < S_BOARD; j++)
        {
            int piece = board[i][j];

            if (selectPiece(piece, king) == collect_piece)
            {
                find_piece[*cnt].x = i;
                find_piece[*cnt].y = j;
                find_piece[*cnt].piece = piece;

                *cnt += 1;
            }
        }
    }

    return NULL;
}

void *findSavior (int board[S_BOARD][S_BOARD], int x_scan, int y_scan, coords *savior)
{
    if (savior == NULL)
        return NULL;


    savior -> x = x_scan;
    savior -> y = y_scan;
    savior -> piece = board[x_scan][y_scan];
                        
    return NULL;
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

        input[i] = '\0';
    }
}



int capturePawn (int board[S_BOARD][S_BOARD], coords find_pawn[], const int cnt_pawn, const int orig_x, const int dest_x, const int dest_y, const int king)
{
    // to capture, there must be a gap of original position x and destination position x of 1
    
/*     printf("orig x: %d\n", orig_x);
    printf("dest_x: %d\n", dest_x);

    printf("gap: %d\n", abs(orig_x - dest_x));
     */

    if (abs(orig_x - dest_x) != 1)
        return 0;


    int direction_capture = 0;
    int direction_find = 0;

    int last_rank = 0;

    int en_passant_pos = 0;

    
    bool en_passant_proceed = false;
    bool left_capture = false;
    bool right_capture = false;

    // if orig_x is greater then, left capture
    if (orig_x > dest_x)
    {
        direction_find = 1;
        direction_capture = -1;
        left_capture = true;
    }

    // else, then right capture
    else
    {
        direction_find = -1;
        direction_capture = 1;
        right_capture = true;
    }

    



    coords chosen_pawn;
    coords en_passant_target;


    int y_increment = 0;


    
    int enemy_king = 0;
    int collect_enemy = 0;


    bool capture = false;

    switch (king)
    {
        case w_K:
            enemy_king = b_K;
            collect_enemy = BLACK_P;
            y_increment = 1;
            last_rank = Pos(8);
            en_passant_pos = Pos(6);
        break;

        case b_K:
            enemy_king = w_K;
            collect_enemy = WHITE_P;
            y_increment = -1;
            last_rank = Pos(1);
            en_passant_pos = Pos(3);
        break;
    }


    // for en passant
    if (dest_y == en_passant_pos)
    {
        for (int i = 0; i < cnt_pawn; i++)
        {
            if (find_pawn[i].x == orig_x)
            {
                int enemy_pawn = 0;

                if (checkBounds(find_pawn[i].x + direction_capture))
                {
                    enemy_pawn = board[find_pawn[i].x + direction_capture][find_pawn[i].y];

                    if (recentDoubleSquarePawn(enemy_pawn, READ) == enemy_pawn)
                    {
                        chosen_pawn.x = find_pawn[i].x;
                        chosen_pawn.y = find_pawn[i].y;
                        chosen_pawn.piece = find_pawn[i].piece;


                        en_passant_target.x = find_pawn[i].x + direction_capture;
                        en_passant_target.y = find_pawn[i].y;
                        en_passant_target.piece = enemy_pawn;

                        en_passant_proceed = true;

                        break;
                    }
                }
            }
        }
    }



    else
    {
        for (int i = 0; i < cnt_pawn; i++)
        {
            if (find_pawn[i].x == orig_x)
            {
                int enemy = 0;

                if (checkBounds(find_pawn[i].x + direction_capture) && checkBounds(find_pawn[i].y + y_increment))
                    enemy = board[find_pawn[i].x + direction_capture][find_pawn[i].y + y_increment];
                    

                if (selectPiece(enemy, collect_enemy) == collect_enemy)
                {
                    chosen_pawn.x = find_pawn[i].x;
                    chosen_pawn.y = find_pawn[i].y;
                    chosen_pawn.piece = find_pawn[i].piece;

                    capture = true;

                    break;
                }
            }
        }
    }
    


    if (capture)
    {
        board[dest_x][dest_y] = chosen_pawn.piece;
        board[chosen_pawn.x][chosen_pawn.y] = 0;

        return 1;
    }

    else if (en_passant_proceed)
    {
        board[dest_x][dest_y] = chosen_pawn.piece;
        board[chosen_pawn.x][chosen_pawn.y] = 0;
        board[en_passant_target.x][en_passant_target.y] = 0;


        return 1;
    }



    return 0;
}


int promotionPawn (int board[S_BOARD][S_BOARD], const int promote_piece, const int orig_let, const int dest_let, const int dest_num, const int king)
{
    if (!isaPiece(promote_piece))
        return -1;

    // gap too large
    if (abs(orig_let - dest_let) > 1)
        return -1;





    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    coords find_pawn;

    int pawn_last_id = 0;

    int collect_enemy = 0;
    int enemy_piece = 0;


    int enemy_rank = 0;
    int increment_piece = 0;


    int direction_capture = 0;


    switch (king)
    {
        case w_K:

            find_pawn.x = orig_let;
            find_pawn.y = Pos(7);
            find_pawn.piece = board[orig_let][Pos(7)];

            collect_enemy = BLACK_P;
            enemy_rank = Pos(8);

        break;

        case b_K:

            find_pawn.x = orig_let;
            find_pawn.y = Pos(2);
            find_pawn.piece = board[orig_let][Pos(2)];

            collect_enemy = WHITE_P;
            enemy_rank = Pos(1);

        break;
    }


    if (selectPiece(find_pawn.piece, king) != PAWN)
        return -1;



    if (orig_let > dest_let)
        direction_capture = -1;

    else
        direction_capture = 1;




    pawn_last_id = find_pawn.piece % 8;
    

    if (promote_piece == 'K')
        return 0;


    switch (king)
    {
        case w_K:

            switch (promote_piece)
            {
                case 'Q':
                    increment_piece = w_Q1xx;
                    break;

                case 'R':
                    increment_piece = w_R1xx;
                    break;

                case 'B':
                    increment_piece = w_B1xx;
                    break;

                case 'N':
                    increment_piece = w_N1xx;
                    break;
            }

        break;

        case b_K:

            switch (promote_piece)
            {
                case 'Q':
                    increment_piece = b_Q1xx;
                    break;

                case 'R':
                    increment_piece = b_R1xx;
                    break;

                case 'B':
                    increment_piece = b_B1xx;
                    break;
                
                case 'N':
                    increment_piece = b_N1xx;
                    break;
            }

        break;
    }




    if (abs(orig_let - dest_let) == 1)
    {
        if (checkBounds(orig_let + direction_capture))
        {
            enemy_piece = board[orig_let + direction_capture][dest_num];
            
            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[orig_let + direction_capture][dest_num] = pawn_last_id + increment_piece;
            }
        }
    }


    else
    {
        board[orig_let][enemy_rank] = pawn_last_id + increment_piece;
    }

    
    // delete previous position

    switch (king)
    {
        case w_K:

            for (int i = 0; i < S_BOARD; i++)
            {
                if (board[i][Pos(7)] == find_pawn.piece)
                {
                    board[i][Pos(7)] = 0;
                    break;
                }
            }

        break;

        case b_K:

            for (int i = 0; i < S_BOARD; i++)
            {
                if (board[i][Pos(2)] == find_pawn.piece)
                {
                    board[i][Pos(2)] = 0;
                    break;
                }
            }

        break;
    }


    if (theAllSeeingEye(board, king, CHECK) == CHECK)
    {
        memcpy(board, tmp_board, sizeof tmp_board);
        return -1;
    }


    return 0;
}

int recentDoubleSquarePawn (int pawn, const int mode)
{
    static int cnt = 0;
    static int double_square_pawn = 0;
    int return_pawn = double_square_pawn;
    bool wipe = false;

    // wipe data if next turn
    if (cnt % 3 == 2)
        wipe = true;

    if (wipe)
        double_square_pawn = 0;

    switch (mode)
    {
        case READ:
            return return_pawn;
        break;

        case WRITE:
            double_square_pawn = pawn;
            cnt++;
        break;
    }

    return 0;
}


bool isRangeLet (char let)
{    
    switch (let)
    {
        case 'a' ... 'h':
            break;

        default:
            return false;
    }

    return true;
}

bool isRangeNum (char num)
{
    switch (num)
    {
        case '1' ... '8':
            break;
        
        default:
            return false;
    }

    return true;
}


bool checkBounds (int n)
{
    switch (n)
    {
        case 0 ... 7:
            break;

        default:
            return false;
    }

    return true;
}

int isaPiece (char piece)
{
    switch (piece)
    {
        case 'Q': case 'B': case 'R': case 'N':
            break;

        case 'K':
            return KING;

        default:
            return 0;
    }

    return 1;
}


void slowText (const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i]);
        fflush(stdout);
        usleep(5000);
    }
}

void welcome (void)
{
    slowText("Chess v1.0\n");
    slowText("Author: FBIAgent05\n\n");
    slowText("Loading Chess\n");
    
    for (int i = 0; i <= 100; i++)
    {
        printf("<%d%%> ", i);


        for (int j = 1; j <= i + 1; j++)
        {
            if (j % 4 == 0)
            {
                putchar('=');

            }
        }

        if (i <= 100)
        {
            putchar('>');
            printf("\r");

            fflush(stdout);
            usleep(20000);
        }

        if (i == 100)
            printf("\n");

    }
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




int Pos (int n)
{
    if (n < 0)
        return 0;

    if (n > S_BOARD)
        return 7;

    // if return 0, king is missing
    return n - 1;
}

