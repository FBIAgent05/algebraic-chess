#include "../header/chess_lib.h"


void initBoard(int board[S_BOARD][S_BOARD])
{   
    // clear board by filling with zeros
    memset(board, 0, sizeof (int) * S_BOARD * S_BOARD);

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


int outcome (int board[S_BOARD][S_BOARD], const int count_moves, const int king)
{
    if (checkmate(board, king) == CHECKMATE)
    {
        printf("\n%s is checkmated!\n", count_moves % 2 ? "Black" : "White");
        printf("%s wins the match in %d moves!\n", count_moves % 2 ? "White" : "Black", count_moves / 2 + 1);

        usleep(1000000);
        return CHECKMATE;
    }

    else if (check(board, king) == ISCHECK)
    {
        printf("\n%s is in check\n", count_moves % 2 ? "Black": "White");

        usleep(700000);
        return ISCHECK;
    }

    else if (stalemate(board, king) == IMMOBILE)
    {
        printf("\nStalemate!\n");
        printf("Game declared as draw in %d moves!\n", count_moves / 2 + 1);

        usleep(1000000);
        return STALEMATE;
    }

    return 0;
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

bool gameinProgress (void)
{
    return proceedGame(true);
}

bool proceedGame (bool x)
{
    static bool proceed = true;

    if (!x)
        proceed = false;

    return proceed;
}




// how to add the '+' when check on a string

int moveBoard(int board[S_BOARD][S_BOARD])
{
    int undo_board[S_BOARD][S_BOARD];
    memcpy(undo_board, board, sizeof undo_board);

    static char saveGame[MAX_CHAR] = {'\0'};

    if (saveGame[0] == '\0')
    {
        getTime(saveGame, MAX_CHAR);
        pushbackStr(saveGame, ".pgn", MAX_CHAR);
    }

    // responsible for recording moves to a pgn file
    FILE *record = fopen(saveGame, "a+");

    static int count_moves = 0;
    int existing_pieces_before = countExistingPieces(board);
    int existing_pieces_after = 0;

    static bool recorded_check = false;
    static bool check = false;


    bool checkmate = false;
    bool stalemate = false;

    static char input[MAX_CHAR] = {0};
    static char white_previous_input[MAX_CHAR] = {0};
    static char black_previous_input[MAX_CHAR] = {0};

    int king = count_moves % 2 ? b_K : w_K;





    switch (outcome(board, count_moves, king))
    {
        case CHECKMATE:

            popFile(saveGame, 10);
            pushbackStr(input, "#", MAX_CHAR);

            fprintf(record, "%s", input);
            fprintf(record, "%s", king ? "\n\n1-0" : "\n\n0-1");

            checkmate = true;
            break;

        case STALEMATE:

            fprintf(record, "\n\n1/2-1/2");
            stalemate = true;
            break;

        case ISCHECK:

            pawnMoved(0, UNDO);
            memcpy(board, undo_board, sizeof undo_board);

            check = true;
            break;
    }


    // records checks, the ones with '+' on
    if (check && !recorded_check)
    {
        // deletes last 10 characters of records.pgn
        popFile(saveGame, 10);

        switch (king)
        {
            case w_K:
                pushbackStr(black_previous_input, "+", MAX_CHAR);
                fprintf(record, "%-10s", black_previous_input);
                break;

            case b_K:
                pushbackStr(white_previous_input, "+", MAX_CHAR);
                fprintf(record, "%-10s", white_previous_input);
                break;
        }

        fflush(record);
        recorded_check = true;
    }



    if (checkmate || stalemate)
    {
        proceedGame(false);
        fclose(record);

        slowText("\nWe've created a PGN file for you by the way!\n");
        slowText("Exiting program...\n");

        return 0;
    }


    printf("%s%s%s >> %s\n", WHITE, displayASCII() ? "W" : "♚", RESET, white_previous_input);
    printf("%s%s%s >> %s\n", BROWN, displayASCII() ? "B" : "♚", RESET, black_previous_input);

    printf("\n%s --> Enter move #%d: ", count_moves % 2 ? "Black" : "White", count_moves / 2 + 1);
    stringInput(input, MAX_CHAR);




    moveSet newMove;
    moveSet *move = &newMove;



    if (strncmp(input, "O-O-O", MAX_CHAR) == 0 || strncmp(input, "0-0-0", MAX_CHAR) == 0)
    {
        if (castling(board, LONG_CASTLE, king) == PREVENT_CASTLE)
        {
            memcpy(board, undo_board, sizeof undo_board);
            usleep(700000);

            return -1;
        }

        count_moves++;
        return 0;
    }

    else if (strncmp(input, "O-O", MAX_CHAR) == 0 || strncmp(input, "0-0", MAX_CHAR) == 0)
    {
        if (castling(board, SHORT_CASTLE, king) == PREVENT_CASTLE)
        {
            memcpy(board, undo_board, sizeof undo_board);

            usleep(700000);
            return -1;
        }

        count_moves++;
        return 0;
    }



    if (parser(board, input, move, king))
        return -1;



    // reset
    check = false;

    switch (outcome(board, count_moves, king))
    {
        case CHECKMATE: 
            proceedGame(false);
            checkmate = true;
            break;
        
        case STALEMATE:
            proceedGame(false);
            stalemate = true;
            break;

        case ISCHECK:
            printf("Check after\n");
            pawnMoved(0, UNDO);
            memcpy(board, undo_board, sizeof undo_board);


            check = true;
            break;
    }



    if (!check)
    {
        switch (king)
        {
            case w_K:
                fprintf(record, "\n");
                fflush(record);
                break;
        }
    }


    if (!check)
    {
        existing_pieces_after = countExistingPieces(board);


        switch (king)
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

        // if number of pieces before is greater, then implies that capture took place
        if (existing_pieces_before > existing_pieces_after)
        {
            switch (king)
            {
                case w_K:
                    insertXforCapture(white_previous_input);
                    break;

                case b_K:
                    insertXforCapture(black_previous_input);
                    break;
            }
        }

        switch (king)
        {
            case w_K:
                fprintf(record, "%d. %-10s", count_moves / 2 + 1, white_previous_input);
                break;

            case b_K:
                fprintf(record, "%-10s", black_previous_input);
                break;
        }
        

        fflush(record);
        recorded_check = false;
    }


    fclose(record);
    record = NULL;

    if (check)
        return 0;

    // move success
    count_moves++; 

    return 0;
}


int parser (int board[S_BOARD][S_BOARD], char *input, moveSet *move, const int king)
{
    int invalid_move = 0;

  
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
                        move->piece = 'P';
                        move->orig_x = input[0];
                        move->dest_x = input[0];
                        move->dest_y = input[1];
                    }

                    else
                    {
                        fprintf(stdout, "%sPawn coordinates out of range%s", RED, RESET);
                        return -1;
                    }

                break;


                case 3:

                    if (isRangeLet(input[0]) && isRangeNum(input[1]) && isaPiece(input[2]))
                    {
                        move->piece = 'P';
                        move->orig_x = input[0];
                        move->dest_x = input[0];
                        move->dest_y = input[1];
                        move->promote = input[2];
                    }

                    else if (input[0] == input[1])
                    {
                        fprintf(stdout, "%sInvalid pawn move%s", RED, RESET);
                        return -1;
                    }

                    else if (isRangeLet(input[0]) && isRangeLet(input[1]) && isRangeNum(input[2]))
                    {
                        move->piece = 'P';
                        move->orig_x = input[0];
                        move->dest_x = input[1];
                        move->dest_y = input[2];
                    }


                    else
                        return -1;

                break;

                case 4:


                    if (isRangeLet(input[0]) && input[1] == 'x' && isRangeLet(input[2]) && isRangeNum(input[3]))
                    {
                        move->piece = 'P';
                        move->orig_x = input[0];
                        move->capture =  input[1];
                        move->dest_x = input[2];
                        move->dest_y = input[3];
                    }

                    else if (isRangeLet(input[0]) && isRangeNum(input[1]) && input[2] == '=' && isaPiece(input[3]))
                    {
                        move->piece = 'P';
                        move->orig_x = input[0];
                        move->capture = input[0];
                        move->dest_y = input[1];
                        move->promote = input[3];
                    }

                    else if (isRangeLet(input[0]) && isRangeLet(input[1]) && isRangeNum(input[2]) && isaPiece(input[3]))
                    {
                        move->piece = 'P';
                        move->orig_x = input[0];
                        move->dest_x = input[1];
                        move->dest_y = input[2];
                        move->promote = input[3];
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
                        move->piece = 'P';
                        move->orig_x = input[0];
                        move->capture = input[1];
                        move->dest_x = input[2];
                        move->dest_y = input[3];
                        move->promote = input[4];
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
                        move->piece = 'P';
                        move->orig_x = input[0];
                        move->capture = input[1];
                        move->dest_x = input[2];
                        move->dest_y = input[3];
                        move->promote = input[5];
                    }


                break;


                default:
                    fprintf(stdout, "%sInvalid pawn move%s", RED, RESET);
                    return -1;
            }
        }

        break;


        case 'P':

            fprintf(stdout, "%sWhen moving pawns, 'P' is ommitted, therefore type '%c%c'%s", RED, input[1], input[2], RESET);
                return -1;

        break;

        // If the first character denotes a piece other than pawn
        case 'R': case 'N': case 'B': case 'Q': case 'K':
        {
            switch (strlen(input))
            {
                case 3:

                    if (isRangeLet(input[1]) && isRangeNum(input[2]))
                    {
                        move->piece = input[0];
                        move->dest_x = input[1];
                        move->dest_y = input[2];
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
                        move->piece = input[0];
                        move->orig_x = input[1];
                        move->dest_x = input[2];
                        move->dest_y = input[3];


                        // remember to do function call here
                    }

                    // if specified rank when there are shared file
                    else if (isRangeNum(input[1]) && isRangeLet(input[2]) && isRangeNum(input[3]))
                    {
                        move->piece = input[0];
                        move->orig_y = input[1];
                        move->dest_x = input[2];
                        move->dest_y = input[3];
                    }

                    else if (input[1] == 'x' && isRangeLet(input[2]) && isRangeNum(input[3]))  // last edit
                    {
                        move->piece = input[0];
                        move->capture = input[1];
                        move->dest_x = input[2];
                        move->dest_y = input[3];
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
                        move->piece = input[0];
                        move->orig_x = input[1];
                        move->capture = input[2];
                        move->dest_x = input[3];
                        move->dest_y = input[4];
                        //insert function
                    }

                    else if (isRangeNum(input[1]) && input[2] == 'x' && isRangeLet(input[3]) && isRangeNum(input[4]))
                    {
                        move->piece = input[0];
                        move->orig_y = input[1];
                        move->capture = input[2];
                        move->dest_x = input[3];
                        move->dest_y = input[4];
                        // insert function
                    }

                    else if (isRangeLet(input[1]) && isRangeNum(input[2]) && isRangeLet(input[3]) && isRangeNum(input[4]))
                    {
                        move->piece = input[0];
                        move->orig_x = input[1];
                        move->orig_y = input[2];
                        move->dest_x = input[3];
                        move->dest_y = input[4];
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
                        move->piece = input[0];
                        move->orig_x = input[1];
                        move->orig_y = input[2];
                        move->capture = input[3];
                        move->dest_x = input[4];
                        move->dest_y = input[5];
                    }

                break;

                default:
                    fprintf(stdout, "%sInvalid move%s", RED, RESET);
                    return -1;
                break;
            }

            break;
        }

        default:
        {
            fprintf(stdout, "%sAccepted pieces are K, N, B, Q%s", RED, RESET);

            return -1;
        }
    }


    move->orig_x -= 'a';
    move->orig_y -= '0' + 1;
    move->dest_x -= 'a';
    move->dest_y -= '0' + 1;


    switch (move->piece)
    {
        case 'P': invalid_move = movePawn(board, move, king); break;
        case 'R': invalid_move = moveRook(board, move, king); break;
        case 'B': invalid_move = moveBishop(board, move, king); break;
        case 'N': invalid_move = moveKnight(board, move, king); break;
        case 'Q': invalid_move = moveQueen(board, move, king); break;
        case 'K': invalid_move = moveKing(board, move, king); break;
    }


    if (invalid_move)
    {
        fprintf(stderr, "\n%sInvalid move%s\n", RED, RESET);

        usleep(700000);
        return -1;
    }

    return 0;
}



int relocatePiece (coords find_piece[], int board[S_BOARD][S_BOARD], moveSet *move, int count_piece)
{
    bool move_success = false;

    bool specify_rank = false;
    bool specify_file = false;

    int select_piece = 0;

    if (count_piece == 0)
    {
        fprintf(stderr, "%sNo %c can move%s", RED, move -> piece, RESET);
        return -1;
    }


    if (count_piece == 1)
        move_success = true;


    else if (specify_rank)
    {
        for (int i = 0; i < count_piece; i++)
        {
            if (move->orig_y == find_piece[i].y)
            {
                select_piece = i;
                move_success = true;
            }
        }
    }


    else if (specify_file || count_piece == 2)
    {
        for (int i = 0; i < count_piece; i++)
        {
            if (move->orig_x == find_piece[i].x)
            {
                select_piece = i;
                move_success = true;
            }
        }
    }


    else if (count_piece > 2)
    {
        for (int i = 0; i < count_piece; i++)
        {
            if (move->orig_x == find_piece[i].x && move->orig_y == find_piece[i].y)
            {
                select_piece = i;
                move_success = true;
            }
        }
    }



    if (move_success)
    {
        board[move->dest_x][move->dest_y] = find_piece[select_piece].piece;
        board[find_piece[select_piece].x][find_piece[select_piece].y] = 0;

        rookorKingmoved(find_piece[select_piece].piece);

        return 0;
    }


    if (specify_rank)
    {
        printf("Specify the rank\n");
        printf("Possible moves: \n");

        for (int i = 0; i < count_piece; i++)
        {
            printf("%c%d%c%d\n", move -> piece, toNumberPos(find_piece[i].x), toLetterPos(move->dest_x), toNumberPos(move->dest_y));
        }
    }

    else if (specify_file || count_piece == 2)
    {
        printf("Specify the file\n");
        printf("Possible moves: \n");

        for (int i = 0; i < count_piece; i++)
        {
            printf("%c%c%c%d\n", move -> piece, toLetterPos(find_piece[i].x), toLetterPos(move->dest_x), toNumberPos(move->dest_y));
        }
    }



    else if (count_piece > 2)
    {
        printf("Specify rank and file ");
        printf("Possible moves: \n");

        for (int i = 0; i < count_piece; i++)
        {
            printf("%c%c%d%c%d\n", move -> piece, toLetterPos(find_piece[i].x), toNumberPos(find_piece[i].y), toLetterPos(move->dest_x), toNumberPos(move->dest_y));
        }
    }

    return -1;
}


int squareOccupiedbyAlly (int board[S_BOARD][S_BOARD], moveSet *move, const int king)
{
    int collect_ally = 0;


    switch (king)
    {
        case w_K:
            collect_ally = WHITE_P;
            break;

        case b_K:
            collect_ally = BLACK_P;
            break;
    }


    int dest_piece = board[move->dest_x][move->dest_y];

    if (selectPiece(dest_piece, collect_ally) == collect_ally)
    {
        fprintf(stderr, "%sCannot take own piece%s", RED, RESET);
        return -1;
    }

    if (board[move->dest_x][move->dest_y] == 0 && move->capture == 'x')
    {
        fprintf(stderr, "%sCannot take empty square%s", RED, RESET);
        return -1;
    }

    else


    return 0;
}


void cardinalFindPiece (int board[S_BOARD][S_BOARD], coords find_piece[], moveSet *move, const int piece, int *count_piece, const int king)
{
    int boundary = 0;
    bool limited_range = false;


    switch (piece)
    {
        case KNIGHT: case KING:
            limited_range = true;
            break;
    }

    switch (piece)
    {
        case ROOK: case BISHOP: case QUEEN: case KING:
            boundary = 1;
            break;

        case KNIGHT:
            boundary = 2;
            break;
    }



    for (int x_modif = boundary * -1; x_modif <= boundary; x_modif++)
    {
        for (int y_modif = boundary * -1; y_modif <= boundary; y_modif++)
        {
            if (skipLoopPiece(x_modif, y_modif, piece))
                continue;

            for (int x_pos = move -> dest_x + x_modif, y_pos = move -> dest_y + y_modif; checkBounds(x_pos) && checkBounds(y_pos); x_pos += x_modif, y_pos += y_modif)
            {
                int select_piece = board[x_pos][y_pos];

                if (selectPiece(select_piece, king) == piece)
                {
                    find_piece[*count_piece].x = x_pos;
                    find_piece[*count_piece].y = y_pos;
                    find_piece[*count_piece].piece = select_piece;

                    *count_piece += 1;
                    break;
                }

                else if (select_piece || limited_range)
                    break;
            }
        }
    }
}




int movePawn(int board[S_BOARD][S_BOARD], moveSet *move, const int king)
{
    coords find_pawn;

    bool found_pawn = false;
    bool double_square = false;
    bool pawn_moved = false;

    int pawn_range = 0;

    int y_modif = 0;
    int last_rank = 0;


    switch (king)
    {
        case w_K:
            last_rank = Pos(8);
            y_modif = -1;
            break;

        case b_K:
            last_rank = Pos(1);
            y_modif = 1;
            break;
    }


    for (int y_pos = move->dest_y + y_modif; checkBounds(y_pos); y_pos += y_modif)
    {
        int piece = board[move -> orig_x][y_pos];

        if (selectPiece(piece, king) == PAWN)
        {
            find_pawn.x = move -> orig_x;
            find_pawn.y = y_pos;
            find_pawn.piece = piece;

            move -> orig_y = y_pos;

            found_pawn = true;
            break;
        }

        else if (piece)
            break;
        // should have an else if break here for obstacles but nah, too many variables
    }


    if (!found_pawn)
    {
        fprintf(stderr, "%sNo pawn can move%s", RED, RESET);
        return -1;
    }


    if (move->dest_y == last_rank)
    {
        if (promotionPawn(board, move->promote, move, king))
        {
            fprintf(stderr, "%sInvalid promotion%s\n", RED, RESET);
            printf("Valid format: %c%dQ or %c%d=Q", toLetterPos(move->dest_x), toNumberPos(move->dest_y), toLetterPos(move->dest_x), toNumberPos(move->dest_y));
            return -1;
        }
        return 0;
    }

    // if gap is 1, then capture
    // found_pawn is a bool, taking place of an int
    if (capturePawn(board, &find_pawn, found_pawn, move, king))
        return 0;

    // if there is no pawn capture, then there should be no gap between orig_x and dest_x
    if (abs(move->orig_x - move->dest_x) != 0)
        return -1;


    if (move->capture == 'x' && abs(move->orig_x - move->dest_x) == 0)
        return -1;

    if (squareOccupiedbyAlly(board, move, king))
        return -1;

    // pawns cannot capture forward
    if (board[move->dest_x][move->dest_y])
        return -1;


    if (abs(move -> orig_y - move -> dest_y) == 1)
        pawn_range = 1;

    else if (pawnMoved(find_pawn.piece, READ))
        pawn_range = 1;

    else
        pawn_range = 2;



    for (int y_range = pawn_range; y_range >= 0; y_range--)
    {
        if (find_pawn.y == move -> dest_y - (y_range * y_modif * -1))
        {
            pawn_moved = true;

            if (abs(y_range == 2))
                double_square = true;

            break;
        }
    }


    if (pawn_moved)
    {
        pawnMoved(find_pawn.piece, WRITE);

        if (double_square)
            recentDoubleSquarePawn(find_pawn.piece, WRITE);

        // reset double square pawn to 0
        else
            recentDoubleSquarePawn(0, WRITE);

        board[move->dest_x][move->dest_y] = find_pawn.piece;
        board[find_pawn.x][find_pawn.y] = 0;

        return 0;
    }

    return -1;
}


int moveRook (int board[S_BOARD][S_BOARD], moveSet *move, const int king)
{
    coords find_rook[MAX_PCS];
    int count_rook = 0;

    if (squareOccupiedbyAlly(board, move, king))
        return -1;

    cardinalFindPiece(board, find_rook, move, ROOK, &count_rook, king);

    if (relocatePiece(find_rook, board, move, count_rook))
        return -1;

    return 0;
}


int moveBishop (int board[S_BOARD][S_BOARD], moveSet *move, const int king)
{
    coords find_bishop[MAX_PCS];
    int count_bishop = 0;

    if (squareOccupiedbyAlly(board, move, king))
        return -1;

    cardinalFindPiece(board, find_bishop, move, BISHOP, &count_bishop, king);

    // return -1 if relocatePiece function is not satisfied
    if (relocatePiece(find_bishop, board, move, count_bishop))
        return -1;

    return 0;
}



int moveKnight (int board[S_BOARD][S_BOARD], moveSet *move, const int king)
{
    coords find_knight[MAX_PCS];
    int count_knight = 0;

    if (squareOccupiedbyAlly(board, move, king))
        return -1;

    cardinalFindPiece(board, find_knight, move, KNIGHT, &count_knight, king);

    if (relocatePiece(find_knight, board, move, count_knight))
        return -1;

    return 0;
}


int moveQueen (int board[S_BOARD][S_BOARD], moveSet *move, const int king)
{
    coords find_queen[MAX_PCS];
    int count_queen = 0;

    if (squareOccupiedbyAlly(board, move, king))
        return -1;

    cardinalFindPiece(board, find_queen, move, QUEEN, &count_queen, king);

    if (relocatePiece(find_queen, board, move, count_queen))
        return -1;

    return 0;
}


int moveKing (int board[S_BOARD][S_BOARD], moveSet *move, const int king)
{
    coords find_king[MAX_PCS];

    // tmp variable, there is only one king
    int count_king = 0;

    if (squareOccupiedbyAlly(board, move, king))
        return -1;

    cardinalFindPiece(board, find_king, move, KING, &count_king, king);

    if (relocatePiece(find_king, board, move, count_king))
        return -1;

    return 0;
}


int pawnMoved (const int pawn, const int mode)
{
    static bool pawn_moved[S_BOARD + S_BOARD] = {false};
    int pawn_last_id = 0;
    int side = pawn / 100;
    static int final_id = 0;


    if (pawn != 0)
    {
        pawn_last_id = pawn % 10;
        side = pawn / 100;


        switch (side)
        {
            case 2:
                pawn_last_id += 8;
                break;
        }

        final_id = pawn_last_id;
    }


    switch (mode)
    {
        case READ:
            return pawn_moved[final_id];

        case WRITE:
            pawn_moved[final_id] = true;
            break;

        case UNDO:
            pawn_moved[final_id] = false;
            break;
    }

    return 0;
}


int capturePawn (int board[S_BOARD][S_BOARD], coords find_pawn[], const int cnt_pawn, moveSet *move, const int king)
{
    if (abs(move -> orig_x - move -> dest_x) != 1)
        return 0;


    coords chosen_pawn;
    coords en_passant_target;

    int en_passant_pos_orig = 0;
    int en_passant_pos_y = 0;
    int direction_capture = 0;

    bool en_passant_proceed = false;
    bool capture = false;

    bool en_passant = false;

    int y_increment = 0;
    int collect_enemy = collectEnemy(king);


    // if orig_x is greater then, left capture
    if (move -> orig_x > move -> dest_x)
        direction_capture = -1;

    // else, then right capture
    else
        direction_capture = 1;


    switch (king)
    {
        case w_K:
            y_increment = 1;
            en_passant_pos_orig = Pos(5);
            en_passant_pos_y = Pos(6);
        break;

        case b_K:
            y_increment = -1;
            en_passant_pos_orig = Pos(4);
            en_passant_pos_y = Pos(3);
        break;
    }


    if (move -> orig_x == en_passant_pos_orig && move -> dest_y == en_passant_pos_y)
        en_passant = true;


    for (int i = 0; i < cnt_pawn; i++)
    {
        if (find_pawn[i].x == move -> orig_x)
        {
            int enemy_pawn = 0;
            int enemy_enpassant = 0;

            if (checkBounds(find_pawn[i].x + direction_capture) && checkBounds(find_pawn[i].y + y_increment))
                enemy_pawn = board[find_pawn[i].x + direction_capture][find_pawn[i].y + y_increment];

            if (checkBounds(find_pawn[i].x + direction_capture) && checkBounds(find_pawn[i].y))
                enemy_enpassant = board[find_pawn[i].x + direction_capture][find_pawn[i].y];


             // en passant
            if (enemy_enpassant != 0 && en_passant && recentDoubleSquarePawn(enemy_enpassant, READ))
            {
                chosen_pawn.x = find_pawn[i].x;
                chosen_pawn.y = find_pawn[i].y;
                chosen_pawn.piece = find_pawn[i].piece;


                en_passant_target.x = find_pawn[i].x + direction_capture;
                en_passant_target.y = find_pawn[i].y;
                en_passant_target.piece = enemy_enpassant;

                en_passant_proceed = true;

            }

            else if (enemy_pawn != 0 && selectPiece(enemy_pawn, collect_enemy) == collect_enemy)
            {
                chosen_pawn.x = find_pawn[i].x;
                chosen_pawn.y = find_pawn[i].y;
                chosen_pawn.piece = find_pawn[i].piece;

                capture = true;
            }
        }
    }


    if (en_passant_proceed && !capture)
    {
        board[move -> dest_x][move -> dest_y] = chosen_pawn.piece;
        board[chosen_pawn.x][chosen_pawn.y] = 0;
        board[en_passant_target.x][en_passant_target.y] = 0;

        return 1;
    }


    else if (!en_passant_proceed && capture)
    {
        board[move -> dest_x][move -> dest_y] = chosen_pawn.piece;
        board[chosen_pawn.x][chosen_pawn.y] = 0;

        return 1;
    }

    return 0;
}


int promotionPawn (int board[S_BOARD][S_BOARD], const int promote_piece, moveSet *move, const int king)
{
    if (!isaPiece(promote_piece))
        return -1;

    // gap too large
    if (abs(move -> orig_x - move -> dest_x) > 1)
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

            find_pawn.x = move -> orig_x;
            find_pawn.y = Pos(7);
            find_pawn.piece = board[move -> orig_x][Pos(7)];

            collect_enemy = BLACK_P;
            enemy_rank = Pos(8);

        break;

        case b_K:

            find_pawn.x = move -> orig_x;
            find_pawn.y = Pos(2);
            find_pawn.piece = board[move -> orig_x][Pos(2)];

            collect_enemy = WHITE_P;
            enemy_rank = Pos(1);

        break;
    }


    if (selectPiece(find_pawn.piece, king) != PAWN)
        return -1;



    if (move -> orig_x > move -> dest_x)
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



    if (abs(move -> orig_x - move -> dest_x) == 1)
    {
        if (checkBounds(move -> orig_x + direction_capture))
        {
            enemy_piece = board[move -> orig_x + direction_capture][move -> dest_y];

            if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[move -> orig_x + direction_capture][move -> dest_y] = pawn_last_id + increment_piece;
            }
        }
    }


    else
        board[move -> orig_x][enemy_rank] = pawn_last_id + increment_piece;


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


    if (check(board, king) == ISCHECK)
    {
        memcpy(board, tmp_board, sizeof tmp_board);
        return -1;
    }


    return 0;
}

int recentDoubleSquarePawn (int pawn, const int mode)
{
    static int double_square_pawn = 0;
    int return_pawn = double_square_pawn;

    switch (mode)
    {
        case READ:

            if (pawn == return_pawn)
                return 1;

        break;

        case WRITE:
            double_square_pawn = pawn;
        break;
    }

    return 0;
}




int castling (int board[S_BOARD][S_BOARD], const int long_or_short, const int king)
{
    int select_king = 0;

    // reads rook if it has moved or not
    int select_rook = 0;

    // writes rook into board
    int write_rook = 0;

    int first_rank = 0;

    int min_gap = 0;
    int max_gap = 0;


    char rook_belonging[MAX_CHAR] = {'\0'};

    switch (king)
    {
        case w_K:
            select_king = WHITE_KING;
            first_rank = Pos(1);
            break;

        case b_K:
            select_king = BLACK_KING;
            first_rank = Pos(8);
            break;
    }


    switch (select_king)
    {
        case WHITE_KING:

            switch (long_or_short)
            {
                case LONG_CASTLE:
                    select_rook = WHITE_QUEENS_ROOK;
                    write_rook = w_dR;
                    break;

                case SHORT_CASTLE:
                    select_rook = WHITE_KINGS_ROOK;
                    write_rook = w_lR;
                    break;
            }

        break;

        case BLACK_KING:

            switch (long_or_short)
            {
                case LONG_CASTLE:
                    select_rook = BLACK_QUEENS_ROOK;
                    write_rook = b_lR;
                    break;

                case SHORT_CASTLE:
                    select_rook = BLACK_KINGS_ROOK;
                    write_rook = b_dR;
                    break;
            }

        break;
    }

    switch (long_or_short)
    {
        case LONG_CASTLE:
            strncpy(rook_belonging, "Queen's", MAX_CHAR);
            rook_belonging[MAX_CHAR - 1] = '\0';

            min_gap = b;
            max_gap = d;
        break;

        case SHORT_CASTLE:
            strncpy(rook_belonging, "King's", MAX_CHAR);
            rook_belonging[MAX_CHAR - 1] = '\0';

            min_gap = f;
            max_gap = g;
        break;
    }


    if (rookorKingmoved(select_king))
    {
        fprintf(stderr, "%sCastling is not possible as the king already moved%s", RED, RESET);
        return PREVENT_CASTLE;
    }

    if (rookorKingmoved(select_rook))
    {
        fprintf(stderr, "%sCastling is not possible since %s rook has already moved%s", RED, rook_belonging, RESET);
        return PREVENT_CASTLE;
    }



    for (int i = min_gap; i <= max_gap; i++)
    {
        if (board[i][first_rank])
        {
            fprintf(stderr, "%sPieces are blocking the king and rook in castle%s", RED, RESET);
            return PREVENT_CASTLE;
        }
    }


    if (aimCastle(board, long_or_short, king) == PREVENT_CASTLE)
    {
        fprintf(stderr, "%sCannot castle. Some enemy piece is aiming down the sights between king and rook%s", RED, RESET);
        return PREVENT_CASTLE;
    }



    switch (long_or_short)
    {
        case LONG_CASTLE:

            board[c][first_rank] = king;
            board[d][first_rank] = write_rook;
            board[e][first_rank] = 0;
            board[a][first_rank] = 0;

        break;

        case SHORT_CASTLE:

            board[g][first_rank] = king;
            board[f][first_rank] = write_rook;
            board[e][first_rank] = 0;
            board[h][first_rank] = 0;

        break;
    }

    // set castling pieces as moved
    rookorKingmoved(write_rook);
    rookorKingmoved(king);

    return 0;
}


int rookorKingmoved (int rook_or_king)
{
    static bool white_kings_rook_moved = false;
    static bool white_queens_rook_moved = false;
    static bool black_kings_rook_moved = false;
    static bool black_queens_rook_moved = false;
    static bool white_king_moved = false;
    static bool black_king_moved = false;


    switch (rook_or_king)
    {
        case w_K:
            white_king_moved = true;
            break;

        case b_K:
            black_king_moved = true;
            break;

        case w_dR:
            white_queens_rook_moved = true;
            break;

        case w_lR:
            white_kings_rook_moved = true;
            break;

        case b_dR:
            black_kings_rook_moved = true;
            break;

        case b_lR:
            black_queens_rook_moved = true;
            break;


        case WHITE_KING:
            return white_king_moved;

        case BLACK_KING:
            return black_king_moved;

        case WHITE_QUEENS_ROOK:
            return white_queens_rook_moved;

        case WHITE_KINGS_ROOK:
            return white_kings_rook_moved;

        case BLACK_QUEENS_ROOK:
            return black_queens_rook_moved;

        case BLACK_KINGS_ROOK:
            return black_kings_rook_moved;
    }

    return 0;
}


int aimBetweenKingAndRook (coords find_piece[], int board[S_BOARD][S_BOARD], int piece, const int count_piece, const castlingGap *castling)
{
    bool boundary = 1;
    bool limited_range = false;


    switch (piece)
    {
        case KNIGHT: case KING: case PAWN:
            limited_range = true;
            break;
    }

    switch (piece)
    {
        case KNIGHT:
            boundary = 2;
            break;
    }


    for (int x_modif = boundary * -1; x_modif <= boundary; x_modif++)
    {
        for (int y_modif = boundary * -1; y_modif <= boundary; y_modif++)
        {
            if (skipLoopPiece(x_modif, y_modif, piece))
                continue;

            for (int i = 0; i < count_piece; i++)
            {
                for (int x_pos = find_piece[i].x + x_modif, y_pos = find_piece[i].y + y_modif; checkBounds(x_pos) && checkBounds(y_pos); x_pos += x_modif, y_pos += y_modif)
                {
                    for (int gap = castling -> min_gap; gap <= castling -> max_gap; gap++)
                    {
                        if (x_pos == gap && y_pos == castling -> first_rank)
                            return PREVENT_CASTLE;
                    }

                    if (limited_range || board[x_pos][y_pos])
                        break;
                }
            }
        }
    }

    return 0;
}



int aimCastle (int board[S_BOARD][S_BOARD], const int long_or_short, const int king)
{
    castlingGap castling;

    castling.min_gap = 0;
    castling.max_gap = 0;
    castling.y_modif = 0;
    castling.first_rank = 0;

    int enemy_king = 0;

    switch (long_or_short)
    {
        case LONG_CASTLE:
            castling.min_gap = b;
            castling.max_gap = d;
            break;

        case SHORT_CASTLE:
            castling.min_gap = f;
            castling.max_gap = g;
            break;
    }

    switch (king)
    {
        case w_K:
            enemy_king = b_K;
            castling.y_modif = -1;
            castling.first_rank = Pos(1);
            break;

        case b_K:
            enemy_king = w_K;
            castling.y_modif = 1;
            castling.first_rank = Pos(8);
            break;
    }


    if (aimCastlePawn(board, &castling, enemy_king) == PREVENT_CASTLE)
    {
        printf("pawn aiming");
        return PREVENT_CASTLE;
    }

    else if (aimCastleRook(board, &castling, enemy_king) == PREVENT_CASTLE)
    {
        printf("rook aiming");
        return PREVENT_CASTLE;
    }

    else if (aimCastleBishop(board, &castling, enemy_king) == PREVENT_CASTLE)
    {
        printf("bishop aiming");
        return PREVENT_CASTLE;
    }

    else if (aimCastleKnight(board, &castling, enemy_king == PREVENT_CASTLE))
    {
        printf("knight aiming");
        return PREVENT_CASTLE;
    }

    else if (aimCastleQueen(board, &castling, enemy_king) == PREVENT_CASTLE)
    {
        printf("queen aiming");
        return PREVENT_CASTLE;
    }

    else if (aimCastleKing(board, &castling, enemy_king) == PREVENT_CASTLE)
    {
        printf("king aiming");
        return PREVENT_CASTLE;
    }

    return 0;
}


int aimCastlePawn (int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king)
{
    coords find_pawn[S_BOARD];
    int count_pawn = 0;

    findPiece(find_pawn, board, PAWN, &count_pawn, enemy_king);


    for (int x_modif = -1; x_modif <= 1; x_modif++)
    {
        if (x_modif == 0)
            continue;

        for (int i = 0; i < count_pawn; i++)
        {
            int x_pos = find_pawn[i].x + x_modif;
            int y_pos = find_pawn[i].y + castling -> y_modif;


            for (int gap = castling -> min_gap; gap <= castling -> max_gap; gap++)
            {
                if (x_pos == gap && y_pos == castling -> first_rank)
                    return PREVENT_CASTLE;
            }

            if (board[x_pos][y_pos])
                break;
        }
    }

    return 0;
}


int aimCastleRook (int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king)
{
    coords find_rook[MAX_PCS];
    int count_rook = 0;

    findPiece(find_rook, board, ROOK, &count_rook, enemy_king);

    if (aimBetweenKingAndRook(find_rook, board, ROOK, count_rook, castling) == PREVENT_CASTLE)
        return PREVENT_CASTLE;

    return 0;
}


int aimCastleBishop (int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king)
{
    coords find_bishop[MAX_PCS];
    int count_bishop = 0;

    findPiece(find_bishop, board, BISHOP, &count_bishop, enemy_king);

    if (aimBetweenKingAndRook(find_bishop, board, BISHOP, count_bishop, castling) == PREVENT_CASTLE)
        return PREVENT_CASTLE;

    return 0;
}


int aimCastleKnight(int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king)
{
    coords find_knight[MAX_PCS];
    int count_knight = 0;

    findPiece(find_knight, board, KNIGHT, &count_knight, enemy_king);

    if (aimBetweenKingAndRook(find_knight, board, KNIGHT, count_knight, castling) == PREVENT_CASTLE)
        return PREVENT_CASTLE;

    return 0;
}


int aimCastleQueen (int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king)
{
    coords find_queen[MAX_PCS];
    int count_queen = 0;

    findPiece(find_queen, board, QUEEN, &count_queen, enemy_king);

    if (aimBetweenKingAndRook(find_queen, board, QUEEN, count_queen, castling) == PREVENT_CASTLE)
        return PREVENT_CASTLE;

    return 0;
}

int aimCastleKing (int board[S_BOARD][S_BOARD], const castlingGap *castling, const int enemy_king)
{
    coords find_king[MAX_PCS];
    int count_king = 0;

    findPiece(find_king, board, KING, &count_king, enemy_king);

    if (aimBetweenKingAndRook(find_king, board, KING, count_king, castling) == PREVENT_CASTLE)
        return PREVENT_CASTLE;

    return 0;
}





int check (int board[S_BOARD][S_BOARD], const int king)
{
    if (checkPawn(board, king) == ISCHECK)
        return ISCHECK;

    else if (checkRook(board, king) == ISCHECK)
        return ISCHECK;

    else if (checkKnight(board, king) == ISCHECK)
        return ISCHECK;

    else if (checkBishop(board, king) == ISCHECK)
        return ISCHECK;

    else if (checkQueen(board, king) == ISCHECK)
        return ISCHECK;

    return 0;
}



int checkPawn (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_pawn[S_BOARD];
    int count_pawn = 0;

    int y_modif = 0;

    switch (king)
    {
        case w_K:
            y_modif = 1;
            break;

        case b_K:
            y_modif = -1;
    }


    findPiece(find_pawn, board, PAWN, &count_pawn, kingOpponent(king));


    for (int i = 0; i < count_pawn; i++)
    {
        for (int x_modif = -1; x_modif <= 1; x_modif++)
        {
            if (x_modif == 0)
                continue;

            int x_pos = find_pawn[i].x + x_modif;
            int y_pos = find_pawn[i].y + y_modif;

            if (!checkBounds(x_pos) || !checkBounds(y_pos))
                continue;

            int piece = board[x_pos][y_pos];

            if (selectPiece(piece, king) == KING)
                return ISCHECK;
        }
    }

    return 0;
}


int checkRook (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_rook[MAX_PCS];
    int count_rook = 0;

    findPiece(find_rook, board, ROOK, &count_rook, kingOpponent(king));

    if (searchKingtoCheck(find_rook, board, count_rook, ROOK, king) == ISCHECK)
        return ISCHECK;

    return 0;
}


int checkBishop (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_bishop[MAX_PCS];
    int count_bishop = 0;

    findPiece(find_bishop, board, BISHOP, &count_bishop, kingOpponent(king));

    if (searchKingtoCheck(find_bishop, board, count_bishop, BISHOP, king) == ISCHECK)
        return ISCHECK;

    return 0;
}

int checkKnight (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_knight[MAX_PCS];
    int count_knight = 0;

    findPiece(find_knight, board, KNIGHT, &count_knight, kingOpponent(king));

    if (searchKingtoCheck(find_knight, board, count_knight, KNIGHT, king) == ISCHECK)
        return ISCHECK;

    return 0;
}

int checkQueen (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_queen[MAX_PCS];
    int count_queen = 0;

    findPiece(find_queen, board, QUEEN, &count_queen, kingOpponent(king));

    if (searchKingtoCheck(find_queen, board, count_queen, QUEEN, king) == ISCHECK)
        return ISCHECK;

    return 0;
}


int checkmate (int board[S_BOARD][S_BOARD], const int king)
{
    int tmp_board[S_BOARD][S_BOARD];

    memcpy(tmp_board, board, sizeof tmp_board);

    coords find_king;
    int isthekingthere = 0;


    coords attacker;
    attacker.x = attacker.y = attacker.piece = -1;

    coords savior;
    savior.x = savior.y = savior.piece = -1;


    bool curr_position_safe = true;
    bool surrounded = false;

    int collect_enemy = collectEnemy(king);
    int count_invalid_moves = 0;

    bool attacker_found = false;
    bool savior_found = false;
    bool can_block_checkmate = false;


    findPiece(&find_king, board, KING, &isthekingthere, king);


    for (int pos_x = -1; pos_x <= 1; pos_x++)
    {
        for (int pos_y = -1; pos_y <= 1; pos_y++)
        {
            if (pos_x == 0 && pos_y == 0)
            {
                if (check(board, king) != ISCHECK)
                    curr_position_safe = true;

                else
                    curr_position_safe = false;

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


            int enemy_piece = board[find_king.x + pos_x][find_king.y + pos_y];

            if (!enemy_piece || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
            {
                board[find_king.x][find_king.y] = 0;
                board[find_king.x + pos_x][find_king.y + pos_y] = king;


                if (check(board, king) == ISCHECK)
                    count_invalid_moves++;

            }

            else
                count_invalid_moves++;


            memcpy(board, tmp_board, sizeof tmp_board);
        }
    }


    if (count_invalid_moves >= 8 && curr_position_safe == false)
        surrounded = true;


    if (surrounded)
    {
        attacker = searchAttacker(board, find_king, king);

        if (attacker.piece > 0)
            attacker_found = true;
    }


    if (attacker_found && surrounded)
    {
        savior = searchSavior(board, find_king, attacker, king);

        // ensures that the savior is found
        if (savior.piece > 0)
            savior_found = true;
    }


    // assume that if savior is found, then checkmate can be prevented
    if (savior_found)
        can_block_checkmate = true;


    // check if savior is attacker, which shouldnt even happen
    if (savior_found)
    {
        // bug where savior is attacker itself fix
        if (savior.piece == attacker.piece)
        {
            savior_found = false;
            can_block_checkmate = false;
        }
    }


    if (attacker_found && savior_found)
    {
        if (assassinateAttacker(board, attacker, savior, king) == ASSASSINATE_FAIL)
            can_block_checkmate = false;
    }


    if (surrounded && can_block_checkmate)
    {
        printf("\nsavior in: %c%d\n", toLetterPos(savior.x), toNumberPos(savior.y));
        printf("attacker in: %c%d\n", toLetterPos(attacker.x), toNumberPos(attacker.y));
    }



    if (!can_block_checkmate && surrounded && !curr_position_safe)
        return CHECKMATE;


    return 0;
}


int theCheckmateLastStand (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king)
{
    if (sightPawn(board, x_scan, y_scan, savior, king) == VISIBLE)
        return VISIBLE;

    if (sightBishop(board, x_scan, y_scan, savior, king) == VISIBLE)
        return VISIBLE;

    if (sightRook(board, x_scan, y_scan, savior, king) == VISIBLE)
        return VISIBLE;

    if (sightQueen(board, x_scan, y_scan, savior, king) == VISIBLE)
        return VISIBLE;

    if (sightKnight(board, x_scan, y_scan, savior, king) == VISIBLE)
        return VISIBLE;


    return 0;
}




int sightPawn (int board[S_BOARD][S_BOARD],const int x_scan, const int y_scan, coords *savior, const int king)
{
    coords find_pawn[S_BOARD];
    int count_pawn = 0;

    findPiece(find_pawn, board, PAWN, &count_pawn, king);

    memset(savior, -1, sizeof(coords));

    int collect_enemy = collectEnemy(king);
    int y_modif = 0;

    switch (king)
    {
        case w_K:
            y_modif = 1;
            break;

        case b_K:
            y_modif = -1;
            break;
    }


    for (int i = 0; i < count_pawn; i++)
    {
        int x_pos = find_pawn[i].x;
        int y_pos = find_pawn[i].y;
        int pawn = find_pawn[i].piece;

        // block check
        for (int range = y_modif; pawnMoved(pawn, READ) ? range <= 1 : range <= 2; range += y_modif)
        {
            if (!checkBounds(y_pos + range))
                break;

            else if (board[x_pos][y_pos + range])
                break;

            else if (x_pos == x_scan && y_pos + range == y_scan)
                return VISIBLE;
        }

        // capture attacker
        for (int capture = -1; capture <= 1; capture += 2)
        {
            if (x_pos + capture == x_scan && y_pos + y_modif == y_scan)
            {
                if (!checkBounds(x_pos + capture) || !checkBounds(y_pos + y_modif))
                    continue;

                int enemy_piece = board[x_pos + capture][y_pos + y_modif];

                if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
                {
                    findSavior(board, x_scan, y_scan, savior);
                    return VISIBLE;
                }
            }
        }
    }

    return 0;
}


int sightRook (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king)
{
    coords find_rook[MAX_PCS];
    int count_rook = 0;

    findPiece(find_rook, board, ROOK, &count_rook, king);

    if (sightsforSavior(board, find_rook, ROOK, count_rook, x_scan, y_scan, savior) == VISIBLE)
        return VISIBLE;

    return 0;
}

int sightBishop (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king)
{
    coords find_bishop[MAX_PCS];
    int count_bishop = 0;

    findPiece(find_bishop, board, BISHOP, &count_bishop, king);

    if (sightsforSavior(board, find_bishop, BISHOP, count_bishop, x_scan, y_scan, savior) == VISIBLE)
        return VISIBLE;


    return 0;
}



int sightKnight (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king)
{
    coords find_knight[MAX_PCS];
    int count_knight = 0;

    findPiece(find_knight, board, KNIGHT, &count_knight, king);

    if (sightsforSavior(board, find_knight, KNIGHT, count_knight, x_scan, y_scan, savior) == VISIBLE)
        return VISIBLE;

    return 0;
}


int sightQueen (int board[S_BOARD][S_BOARD], const int x_scan, const int y_scan, coords *savior, const int king)
{
    coords find_queen[MAX_PCS];
    int count_queen = 0;

    findPiece(find_queen, board, QUEEN, &count_queen, king);

    if (sightsforSavior(board, find_queen, QUEEN, count_queen, x_scan, y_scan, savior) == VISIBLE)
        return VISIBLE;


    return 0;
}


int stalemate (int board[S_BOARD][S_BOARD], const int side)
{
    if (mobilityPawn(board, side) == MOBILE)
        return MOBILE;

    if (mobilityRook(board, side) == MOBILE)
        return MOBILE;

    if (mobilityBishop(board, side) == MOBILE)
        return MOBILE;

    if (mobilityKnight(board, side) == MOBILE)
        return MOBILE;

    if (mobilityQueen(board, side) == MOBILE)
        return MOBILE;

    return IMMOBILE;
}


int canMoveNextTurn (coords find_piece[], int board[S_BOARD][S_BOARD], int piece, const int count_piece, const int king)
{
    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    int boundary = 1;
    bool limited_range = false;

    int collect_enemy = collectEnemy(king);

    switch (piece)
    {
        case KNIGHT: case KING:
            limited_range = true;
            break;
    }

    switch (piece)
    {
        case KNIGHT:
            boundary = 2;
            break;
    }


    for (int i = 0; i < count_piece; i++)
    {
        for (int x_modif = boundary * -1; x_modif <= boundary; x_modif++)
        {
            for (int y_modif = boundary * -1; y_modif <= boundary; y_modif++)
            {
                if (skipLoopPiece(x_modif, y_modif, piece))
                    continue;

                for (int x_pos = find_piece[i].x + x_modif, y_pos = find_piece[i].y + y_modif; checkBounds(x_pos) && checkBounds(y_pos); x_pos += x_modif, y_pos += y_modif)
                {
                    int enemy_piece = board[x_pos][y_pos];
                    bool blank_square = board[x_pos][y_pos] ? false : true;

                    if (blank_square || selectPiece(enemy_piece, collect_enemy) == collect_enemy)
                    {
                        board[x_pos][y_pos] = find_piece[i].piece;
                        board[find_piece[i].x][find_piece[i].y] = 0;

                        if (check(board, king) != ISCHECK)
                        {
                            memcpy(board, tmp_board, sizeof tmp_board);
                            return MOBILE;
                        }

                        memcpy(board, tmp_board, sizeof tmp_board);
                    }

                    else if (limited_range || board[x_pos][y_pos])
                        break;
                }
            }
        }
    }

    return 0;
}


int mobilityPawn (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_pawn[S_BOARD];

    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    int cnt_pawn = 0;
    int range = 0;
    int y_modif = 0;

    int collect_enemy = collectEnemy(king);

    int enemy_left = -1;
    int enemy_right = -1;
    int en_passant_left = -1;
    int en_passant_right = -1;


    switch (king)
    {
        case w_K:
            y_modif = 1;
            break;

        case b_K:
            y_modif = -1;
            break;
    }

    findPiece(find_pawn, board, PAWN, &cnt_pawn, king);



    for (int i = 0; i < cnt_pawn; i++)
    {
        int x_pos = find_pawn[i].x;
        int y_pos = find_pawn[i].y;
        int piece = find_pawn[i].piece;


        if (pawnMoved(piece, king))
            range = 1;

        else
            range = 2;


        for (int range_check = y_modif; abs(range_check) <= range; range_check += y_modif)
        {
            if (!checkBounds(y_pos + range_check))
                break;

            // obstacle checking
            if (board[x_pos][y_pos + range_check])
                break;

            if (board[x_pos][y_pos + range_check] == 0)
            {
                board[x_pos][y_pos] = 0;
                board[x_pos][y_pos + range_check] = piece;
            }

            if (check(board, king) != ISCHECK)
            {
                memcpy(board, tmp_board, sizeof tmp_board);
                return MOBILE;
            }

            memcpy(board, tmp_board, sizeof tmp_board);


            // pawn capture mobility

            if (checkBounds(x_pos - 1) && checkBounds(y_pos + y_modif))
                enemy_left = board[x_pos - 1][y_pos + y_modif];

            if (checkBounds(x_pos + 1) && checkBounds(y_pos + y_modif))
                enemy_right = board[x_pos + 1][y_pos + y_modif];

            if (checkBounds(x_pos - 1))
                en_passant_left = board[x_pos - 1][y_pos];

            if (checkBounds(x_pos + 1))
                en_passant_right = board[x_pos + 1][y_pos];


            if (selectPiece(enemy_left, collect_enemy) == collect_enemy)
            {
                board[x_pos - 1][y_pos + y_modif] = piece;
                board[x_pos][y_pos] = 0;

                if (check(board, king) != ISCHECK)
                {
                    memcpy(board, tmp_board, sizeof tmp_board);
                    return MOBILE;
                }

                memcpy(board, tmp_board, sizeof tmp_board);
            }

            if (selectPiece(enemy_right, collect_enemy) == collect_enemy)
            {
                board[x_pos + 1][y_pos + y_modif] = piece;
                board[x_pos][y_pos] = 0;


                if (check(board, king) != ISCHECK)
                {
                    memcpy(board, tmp_board, sizeof tmp_board);
                    return MOBILE;
                }

                memcpy(board, tmp_board, sizeof tmp_board);
            }

            if (recentDoubleSquarePawn(en_passant_left, READ))
            {
                board[x_pos - 1][y_pos + y_modif] = piece;
                board[x_pos][y_pos] = 0;
                board[x_pos - 1][y_pos] = 0;

                if (check(board, king) != ISCHECK)
                {
                    memcpy(board, tmp_board, sizeof tmp_board);
                    return MOBILE;
                }

                memcpy(board, tmp_board, sizeof tmp_board);
            }


            if (recentDoubleSquarePawn(en_passant_right, READ))
            {
                board[x_pos + 1][y_pos + y_modif] = piece;
                board[x_pos][y_pos] = 0;
                board[x_pos - 1][y_pos] = 0;

                if (check(board, king) != ISCHECK)
                {
                    memcpy(board, tmp_board, sizeof tmp_board);
                    return MOBILE;
                }

                memcpy(board, tmp_board, sizeof tmp_board);
            }

            // reset pieces
            enemy_left = -1;
            enemy_right = -1;
            en_passant_left = -1;
            en_passant_right = -1;
        }
    }


    return IMMOBILE;
}


int mobilityRook (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_rook[MAX_PCS];
    int cnt_rook = 0;

    findPiece(find_rook, board, ROOK, &cnt_rook, king);

    if (canMoveNextTurn(find_rook, board, ROOK, cnt_rook, king) == MOBILE)
        return MOBILE;

    return IMMOBILE;
}



int mobilityKnight (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_knight[MAX_PCS];
    int cnt_knight = 0;

    findPiece(find_knight, board, KNIGHT, &cnt_knight, king);

    if (canMoveNextTurn(find_knight, board, KNIGHT, cnt_knight, king) == MOBILE)
        return MOBILE;

    return IMMOBILE;
}



int mobilityBishop (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_bishop[MAX_PCS];
    int cnt_bishop = 0;

    findPiece(find_bishop, board, BISHOP, &cnt_bishop, king);

    if (canMoveNextTurn(find_bishop, board, BISHOP, cnt_bishop, king) == MOBILE)
        return MOBILE;

    return IMMOBILE;
}


int mobilityQueen (int board[S_BOARD][S_BOARD], const int king)
{
    coords find_queen[MAX_PCS];
    int cnt_queen = 0;

    findPiece(find_queen, board, QUEEN, &cnt_queen, king);

    if (canMoveNextTurn(find_queen, board, BISHOP, cnt_queen, king) == MOBILE)
        return MOBILE;

    return IMMOBILE;
}

void findSavior (int board[S_BOARD][S_BOARD], int x_scan, int y_scan, coords *savior)
{
    if (savior == NULL)
        return;


    savior -> x = x_scan;
    savior -> y = y_scan;
    savior -> piece = board[x_scan][y_scan];

    return;
}

int sightsforSavior (int board[S_BOARD][S_BOARD], coords find_piece[], const int piece, const int piece_count, const int x_scan, const int y_scan, coords *savior)
{
    bool limited_range = false;
    int boundary = 1;

    switch (piece)
    {
        case KING: case KNIGHT:
            limited_range = true;
            break;
    }

    switch (piece)
    {
        case KNIGHT:
            boundary = 2;
            break;
    }

    // reset piece
    savior -> piece = -1;

    for (int i = 0; i < piece_count; i++)
    {
        for (int x_modif = boundary * -1; x_modif <= boundary; x_modif++)
        {
            for (int y_modif = boundary * -1; y_modif <= boundary; y_modif++)
            {
                if (skipLoopPiece(x_modif, y_modif, piece))
                    continue;

                for (int x_pos = find_piece[i].x + x_modif, y_pos = find_piece[i].y + y_modif; checkBounds(x_pos) && checkBounds(y_pos); x_pos += x_modif, y_pos += y_modif)
                {
                    if (x_pos == x_scan && y_pos == y_scan)
                    {
                        findSavior(board, find_piece[i].x, find_piece[i].y, savior);
                        return VISIBLE;
                    }

                    else if (limited_range || board[x_pos][y_pos])
                        break;
                }
            }
        }
    }


    return 0;
}


coords searchAttacker(int board[S_BOARD][S_BOARD], coords find_king, int king)
{
    coords attacker;
    attacker.piece = -1;

    int collect_enemy = collectEnemy(king);


    for (int x_modif = -1; x_modif <= 1; x_modif++)
    {
        for (int y_modif = -1; y_modif <= 1; y_modif++)
        {
            if (x_modif == 0 && y_modif == 0)
                continue;


            for (int x_pos = find_king.x + x_modif, y_pos = find_king.y + y_modif; checkBounds(x_pos) && checkBounds(y_pos); x_pos += x_modif, y_pos += y_modif)
            {
                int enemy_piece = board[x_pos][y_pos];

                if (selectPiece(enemy_piece, collect_enemy) == collect_enemy)
                {
                    attacker.x = x_pos;
                    attacker.y = y_pos;
                    attacker.piece = enemy_piece;

                    return attacker;
                }

                else if (enemy_piece)
                    break;
            }
        }
    }


    // if no attacker found, then it must be a knight
    for (int x_modif = -2; x_modif <= 2; x_modif++)
    {
        for (int y_modif = -2; y_modif <= 2; y_modif++)
        {
            if (abs(x_modif) == abs(y_modif))
                continue;

            else if (x_modif == 0 || y_modif == 0)
                continue;


            int x_pos = find_king.x + x_modif;
            int y_pos = find_king.y + y_modif;

            if (!checkBounds(x_pos) || !checkBounds(y_pos))
                continue;


            int enemy_knight_piece = board[x_pos][y_pos];

            if (selectPiece(enemy_knight_piece, kingOpponent(king)) == KNIGHT)
            {
                attacker.x = x_pos;
                attacker.y = y_pos;
                attacker.piece = enemy_knight_piece;

                return attacker;
            }
        }
    }


    return attacker;
}


coords searchSavior (int board[S_BOARD][S_BOARD], coords find_king, coords attacker, int king)
{
    coords savior;
    savior.x = -1;
    savior.y = -1;
    savior.piece = -1;


    int x_modif = leadDirection(attacker.x - find_king.x);
    int y_modif = leadDirection(attacker.y - find_king.y);

    // in case if knight, knight cannot block pieces anyway
    // directly attack attacker

    if (selectPiece(attacker.piece, kingOpponent(king)) == KNIGHT)
    {
        if (theCheckmateLastStand(board, attacker.x, attacker.y, &savior, king) == VISIBLE)
            return savior;
    }

    else
    {
        //  block attacker
        for (int x_scan = find_king.x + x_modif, y_scan = find_king.y + y_modif; checkBounds(x_scan) && checkBounds(y_scan); x_scan += x_modif, y_scan += y_modif)
        {
            int piece = board[x_scan][y_scan];

            if (theCheckmateLastStand(board, x_scan, y_scan, &savior, king) == VISIBLE)
                return savior;

            else if (piece == attacker.piece)
                break;
        }
    }


    return savior;
}

int assassinateAttacker (int board[S_BOARD][S_BOARD], coords attacker, coords savior, int king)
{
    int tmp_board[S_BOARD][S_BOARD];
    memcpy(tmp_board, board, sizeof tmp_board);

    int return_val = 0;

    // pretend to capture
    board[attacker.x][attacker.y] = savior.piece;
    board[savior.x][savior.y] = 0;

    if (check(board, king) == ISCHECK)
        return_val = ASSASSINATE_FAIL;

    memcpy(board, tmp_board, sizeof tmp_board);

    return return_val;
}


int searchKingtoCheck (coords find_piece[], int board[S_BOARD][S_BOARD], const int count_piece, const int piece_checker, const int king)
{
    int boundary = 1;
    bool limited_range = false;

    switch (piece_checker)
    {
        case KNIGHT: case PAWN: case KING:
            limited_range = true;
            break;
    }

    switch (piece_checker)
    {
        case KNIGHT:
            boundary = 2;
            break;
    }


    for (int i = 0; i < count_piece; i++)
    {
        for (int x_modif = boundary * -1; x_modif <= boundary; x_modif++)
        {
            for (int y_modif = boundary * -1; y_modif <= boundary; y_modif++)
            {
                if (skipLoopPiece(x_modif, y_modif, piece_checker))
                    continue;


                for (int x_pos = find_piece[i].x + x_modif, y_pos = find_piece[i].y + y_modif; checkBounds(x_pos) && checkBounds(y_pos); x_pos += x_modif, y_pos += y_modif)
                {
                    int piece = board[x_pos][y_pos];

                    if (selectPiece(piece, king) == KING)
                        return ISCHECK;

                    else if (piece || limited_range)
                        break;
                }
            }
        }
    }


    return 0;
}



void welcome (void)
{
    char ch;

    slowText("algebraic-chess v1.5\n");
    slowText("By: FBIAgent05\n");
    slowText("Source Code: https://github.com/FBIAgent05/algebraic-chess/\n\n");

    slowText("Display ASCII pieces? (y / N): ");
    stringInput(&ch, sizeof(char) * 2);

    switch (ch)
    {
        case 'Y': case 'y':
            useASCII(true);
        break;
    }

    slowText("\nLoading Chess\n");

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



int kingOpponent (const int king)
{
    switch (king)
    {
        case w_K:
            return b_K;

        case b_K:
            return w_K;
    }

    return 0;
}


int collectEnemy (const int king)
{
    switch (king)
    {
        case w_K:
            return BLACK_P;

        case b_K:
            return WHITE_P;
    }

    return 0;
}

int leadDirection (int gap)
{
    if (gap > 0)
        return 1;

    else if (gap < 0)
        return -1;

    else
        return 0;
}



int skipLoopPiece (const int x_modif, const int y_modif, const int piece)
{
    switch (piece)
    {
        case ROOK:

            if (abs(x_modif) == abs(y_modif))
                return 1;

        break;

        case BISHOP:

            if (abs(x_modif) != abs(y_modif))
                return 1;

            else if (x_modif == 0 || y_modif == 0)
                return 1;

        break;

        case KNIGHT:

            if (abs(x_modif) == abs(y_modif))
                return 1;

            else if (x_modif == 0 || y_modif == 0)
                return 1;

        break;

        case QUEEN: case KING:

            if (x_modif == 0 && y_modif == 0)
                return 1;
    }


    return 0;
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


void findPiece(coords find_piece[], int board[S_BOARD][S_BOARD], int collect_piece, int *cnt, const int king)
{
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
}


int countExistingPieces (int board[S_BOARD][S_BOARD])
{
    int existing = 0;
    
    for (int i = 0; i < S_BOARD; i++)
    {
        for (int j = 0; j < S_BOARD; j++)
        {
            if (board[i][j])
                existing++;
        }
    }

    return existing;
}


void insertXforCapture (char *str)
{
    char tmp_str[MAX_CHAR];
    int cnt = 0;

    if (str[1] == 'x')
        return;
    
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (i == 1)
        {
            tmp_str[cnt] = 'x';
            cnt++;
        }

        tmp_str[cnt] = str[i];
        cnt++;
    }

    strncpy(str, tmp_str, MAX_CHAR);
    str[cnt] = '\0';
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


char toLetterPos (int n)
{
    return n + 'a';
}

int toNumberPos (int n)
{
    return n + 1;
}


int Pos (int n)
{
    if (n < 0)
        return 0;

    if (n > S_BOARD)
        return 7;

    return n - 1;
}
