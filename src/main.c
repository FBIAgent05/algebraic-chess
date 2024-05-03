                                                    /*** Chess: ***/
                                                /*** By: FBIAgent05 ***/
                                                /* 開始: Feb 20, 2024*/
                                                /* 終了: Apr, 20, 2024*/
                                                /* 再改修: May, 02, 2024*/


#include "chess_lib.h"
#include "UI.h"

int main(void)
{
    int board[S_BOARD][S_BOARD];
    initBoard(board);

    clear();
    welcome();


    while (gameinProgress())
    {
        clear();

        printBoard(board);
        recordInput(board);
    }

    return 0;
}
