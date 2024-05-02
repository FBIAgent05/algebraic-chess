# algebraic-chess
Chess made in C Language

This is a simple console-based chess game implemented in C. The game follows the standard rules of chess and allows two players to play against each other on the same computer.

~~Probably the cause of my sleep deprivation for the last two months~~


<br>


## Features

- Uses algebraic notation to control the pieces
- Implements all standard chess rules: movement, captures, check, checkmate, stalemate, castling, pawn promotion, and of course, en passant.
- Uses Unicode and ASCII characters to represent the chessboard and pieces.
- Supports two-player gameplay.
- Displays helpful messages for invalid moves and game outcomes.

<br>


**Update in 2024-05-02**
- Saves game to a PGN file
- Automatically add the 'x' or '+' when capture and check respectively in inputs to PGN file
- More informative invalid input warning messages
- Multiple source files
- ~~Less spaghetti code~~

<br>


## Installation

To compile and run the chess game, follow these steps:

1. Clone the repository:

```bash
git clone https://github.com/FBIAgent05/algebraic-chess
```

2. Navigate the repository

```bash
cd ./algebraic-chess/
```
3. Compile the source code

```bash
make
```

4. Run the executable
```bash
./chess
```


<br>


  ## How to Play

### 1) Moving

  Identify the piece notation letter and specify its destination square:<br>

  Note:
  Pawn is not denoted by 'P' as it is not in the algebraic notation standards

  Example:<br>
  Knight moves to ```c6``` would be ```Ke6```<br>
  Pawn moves to ```e4``` would be ```e4```<br>


  ### 2) Capture

  To capture, append  ```x``` as the second character input<br>
  The program automatically add this   ```x``` for you by the way

  Example:<br>
  Pawn from ```e```  captures piece in ```d4``` square     --> ```exd4```<br>
  Rook captures piece in ```a8 ``` square                                     --> ```Rxa8```<br>


  ### 3) Castling

  Long Castle  --> ```O-O-O``` or ```0-0-0```<br>
  Short Castle --> ```O-O``` or ```0-0``` <br>
  
  
### 4) Promotion

Pawn move to last rank then specify the promotion piece

Example:<br>
Pawn moves to e8 and promotes to a knight                                                                            --> ```e8N```<br>
Pawn captures piece from ```h``` file to ```g8``` and promotes to a Queen    -->  ```hxg8Q```<br>

### 5) En passant

Important note:
En passant destination is not equal to the enemy pawn piece's square

Example:<br>
Pawn from ```e``` file does an en passant to pawn in e5            --> ```exd6```

```d6``` because it is the pawn's destination
  
  <br>


 For more information, refer to this Wikipedia article  
  https://en.wikipedia.org/wiki/Algebraic_notation_(chess)#Notation_for_moves

  <br>


## Q and A
1) Is it efficient?
- No
2) Does it work?
- Yes


  <hr>


