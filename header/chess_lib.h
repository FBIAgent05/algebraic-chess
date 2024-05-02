// all functions and libraries necessary for chess

#ifndef CHESS_LIB_H
#define CHESS_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "gen_tools.h"

#define S_BOARD 8
#define MAX_CHAR 256


enum return_val
{
    NONE,
    WHITE_CHECK,
    BLACK_CHECK,
    ISCHECK,
    CHECKMATE,
    STALEMATE,
    SCAN,
    VISIBLE,
    MAX_PCS,
    BLOCK,
    ASSASSINATE,
    ASSASSINATE_FAIL,
    ASSASSINATE_SUCCESS,
    MOBILE,
    IMMOBILE,
    FOUND_SAVIOR,
    WHITE_KINGS_ROOK,
    WHITE_QUEENS_ROOK,
    BLACK_KINGS_ROOK,
    BLACK_QUEENS_ROOK,
    WHITE_KING,
    BLACK_KING,
    READ,
    WRITE,
    WIPE,
    UNDO,
    WHITE_P,
    BLACK_P,
    SHORT_CASTLE,
    LONG_CASTLE,
    PREVENT_CASTLE
};

#define RED "\033[31m"
#define WHITE "\x1b[38;5;230m"
#define BROWN "\x1b[38;5;94m"
#define RESET "\033[0m"

enum {a, b, c, d, e, f, g, h};
enum {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};


// pawn is too complex for integrating to unified for loop

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
    char piece;
    char capture;
    char promote;

    int orig_x;
    int orig_y;
    int dest_x;
    int dest_y;
}
moveSet;


typedef struct
{
    int piece;
    int x;
    int y;
}
coords;


typedef struct
{
    int min_gap;
    int max_gap;
    int y_modif;
    int first_rank;
}
castlingGap;


// Pretend that this is some split file project

// board.h
void initBoard(int board[S_BOARD][S_BOARD]);
void printBoard (int board[S_BOARD][S_BOARD]);
int outcome (int board[S_BOARD][S_BOARD], int count_moves, const int king);

bool displayASCII();
bool useASCII (bool x);

bool gameinProgress();
bool proceedGame (bool x);



// move.h
int moveBoard(int board[S_BOARD][S_BOARD]);
int relocatePiece (coords find_piece[], int board[S_BOARD][S_BOARD], moveSet *move, int count_piece);
int squareOccupiedbyAlly (int board[S_BOARD][S_BOARD], moveSet *move, const int king);

void cardinalFindPiece (int board[S_BOARD][S_BOARD], coords find_piece[], moveSet *move, const int piece, int *count_piece, const int king);

int movePawn(int board[S_BOARD][S_BOARD], moveSet *move, const int king);
int moveRook (int board[S_BOARD][S_BOARD], moveSet *move, const int king);
int moveBishop (int board[S_BOARD][S_BOARD], moveSet *move, const int king);
int moveKnight (int board[S_BOARD][S_BOARD], moveSet *move, const int king);
int moveQueen (int board[S_BOARD][S_BOARD], moveSet *move, const int king);
int moveKing (int board[S_BOARD][S_BOARD], moveSet *move, const int king);

int pawnMoved (const int pawn, const int mode);
int capturePawn (int board[S_BOARD][S_BOARD], coords find_pawn[], const int cnt_pawn, moveSet *move, const int king);
int promotionPawn (int board[S_BOARD][S_BOARD], const int promote_piece, moveSet *move, const int king);
int recentDoubleSquarePawn (int pawn, int mode);




// castling.h
int castling (int board[S_BOARD][S_BOARD], const int long_or_short, const int king);
int rookorKingmoved (int rook_or_king);

int aimBetweenKingAndRook (coords find_piece[], int board[S_BOARD][S_BOARD], const int piece, const int count_piece, const castlingGap *castling);

int aimCastle(int board[S_BOARD][S_BOARD], const int long_or_short, const int king);
int aimCastlePawn(int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king);
int aimCastleRook(int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king);
int aimCastleBishop(int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king);
int aimCastleKnight(int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king);
int aimCastleQueen(int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king);
int aimCastleKing(int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king);




// logic.h
int check (int board[S_BOARD][S_BOARD], const int king);

int checkPawn(int board[S_BOARD][S_BOARD], const int king);
int checkRook(int board[S_BOARD][S_BOARD], const int king);
int checkBishop(int board[S_BOARD][S_BOARD], const int king);
int checkKnight(int board[S_BOARD][S_BOARD], const int king);
int checkQueen(int board[S_BOARD][S_BOARD], const int king);

int checkmate (int board[S_BOARD][S_BOARD], const int king);
int theCheckmateLastStand (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king);

// pieces sight function as from their first person perspective
int sightPawn (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king);
int sightRook (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king);
int sightBishop (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king);
int sightKnight (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king);
int sightQueen (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king);


int stalemate (int board[S_BOARD][S_BOARD], const int side);
int canMoveNextTurn (coords find_piece[], int board[S_BOARD][S_BOARD], const int piece, const int count_piece, const int king);

int mobilityPawn (int board[S_BOARD][S_BOARD], const int side);
int mobilityRook (int board[S_BOARD][S_BOARD], const int side);
int mobilityBishop (int board[S_BOARD][S_BOARD], const int side);
int mobilityKnight (int board[S_BOARD][S_BOARD], const int side);
int mobilityQueen (int board[S_BOARD][S_BOARD], const int side);

void findSavior(int board[S_BOARD][S_BOARD], int x_scan, int y_scan, coords *savior);
int sightsforSavior (int board[S_BOARD][S_BOARD], coords find_piece[], const int piece, const int piece_count, const int x_scan, const int y_scan, coords *savior);

coords searchAttacker(int board[S_BOARD][S_BOARD], coords find_king, int king);
coords searchSavior (int board[S_BOARD][S_BOARD], coords find_king, coords attacker, int king);
int assassinateAttacker (int board[S_BOARD][S_BOARD], coords attacker, coords savior, int king);

int searchKingtoCheck (coords find_piece[], int board[S_BOARD][S_BOARD], const int count_piece, const int piece_checker, const int king);




// utils.h
void welcome (void);

int kingOpponent (const int king);
int collectEnemy (const int king);
int leadDirection (int gap);
int skipLoopPiece (const int x_modif, const int y_modif, const int piece);
int selectPiece (int piece, const int king);
void findPiece(coords find_piece[], int board[S_BOARD][S_BOARD], int collect_piece, int *cnt, const int king);
int countExistingPieces (int board[S_BOARD][S_BOARD]);
void insertXforCapture (char *str);



// bounds.h
int isaPiece (char piece);
bool isRangeLet (char let);
bool isRangeNum (char num);
bool checkBounds (int n);
char toLetterPos (int n);
int toNumberPos (int n);
int Pos (int n);

#endif
