# CS5600 assignment-2
DUE DATE: Wed., Feb 19 2020 by 11:59:59
Shell Commands and Scripting
In this assignment you will gain experience with the bash shell scripting language. You will use bash to implement an interactive game of "tic-tac-toe", sometimes also called "naughts and crosses". The game uses a 3x3 board. Each of two players places a mark on a square of the board. The first player uses the mark "X" (capital x) and the second player uses the mark "O" (capital o). "X" always goes first. The goal is for one of the players to place three marks in a row, vertically, horizontally, or diagonally.

You are given a script with a function that implements an interactive program for playing the game by initializing the board reading player commands, marking squares, determining whether a player has won, and displaying the board on the terminal.

For this assignment, you will create two versions of the game to explore different ways of representing the board. The first version uses an array with strings to record the marks on the board. The second representation uses a single string whose characters record the marks on the board. The marks for both are in row-major order. That is, the first three marks are the first row, the next three are the second row, and the last three are the third row.

Array representation
The GitHub repository "2020SPCS500SV/assignment-2" contains the file "ttt_a" with a function play_ttt() that uses an array to represent the board. You will implement the game functions in "ttt_board_a" that work with this array representation. The "ttt_board_a" file is sourced in "ttt_a" file to make the game functions available to the game program. You will also implement unit tests for the board functions in the file 'ttt_a_test' included with this assignment. This file sources both 'ttt_board_a' and the 'unit_test' file.

params    returns    description
initBoard()    
board: reference to the board
Initialize empty board
markBoard()    
marker: marker for a player (X or O)
bpos: row major position on the board (1-9)
board: reference to the board
0 if board position marked, 1 if cannot mark position    Mark board position with marker of a player (X or O).
hasWon()    
marker: marker for a player (X or O)
board: reference to the board
0 if marker has won, 1 otherwise    Determines whether marker has 3 columns in row or 3 rows in column, or all 3 in a diagonal.
displayBoard()    
board: reference to the board
Display the board
Notes
Board representation. The board is represented as an array. Each position contains a mark ('X' or 'O') or an empty board position ('_') in row-major order.

Documentation. All files provided include a descriptive comment block at the top of the file, and a descriptive block for each function in JavaDoc/Doxygen style with the description, parameters, and return information. Any files, functions, and declarations you create should include the same kind of documentation.
Use of constants. You are strongly encouraged to declare constants within the file for values used in this module to improve readability, including the board dimension (3), size (3*3), and the special marker '_' used to indicate an empty square.

Parameters. You are strongly encouraged to copy parameters to descriptive local variables to improve readability. The game program passes the board array to functions by reference as the array name. Each function should assign this parameter value to a local nameref variable to make it available within the functions as an array. Changes made to the array change the array whose name was passed as a parameter. This is similar to how C/C++ passes arrays to functions.

Return status. Bash functions implicitly return a numeric status: 0 for normal returns, and a number between 1 and 255 in case of an error. The markBoard() function returns 1 if the position is outside the range (0..9), the marker is not 'X' or 'O', or the position is already marked, and 0 if the position was successfully marked. The hasWon() function returns 0 if the player for the marker has won the game and 1 if not. Both functions are called in tests in the game program.
String representation
The GitHub repository "2020SPCS500SV/assignment-2" also contains the file "ttt_s" with a function play_ttt() that uses a string to represent the board. You will implement the version of the game functions in "ttt_board_s" that work with this immutable string representation. The "ttt_board_s" file is sourced in "ttt_s" file to make the game functions available to the game program. You will alo implement unit tests for the board functions in the file 'ttt_s_test' included with this assignment. This file sources both 'ttt_board_s' and the 'unit_test' file.

params    returns    description
initBoard()        0 and outputs the empty board to stdout    Initialize empty board
markBoard()    
marker: marker for a player (X or O)
bpos: row major position on the board (1-9)
board: the board
0 and outputs new board to stdout if board position marked, or 1 and outputs original board to stdout if invalid marker or position    Mark board position with marker of a player (X or O).
hasWon()    
marker: marker for a player (X or O)
board: the board
0 if marker has won, 1 otherwise    Determines whether marker has 3 columns in row or 3 rows in column, or all 3 in a diagonal.
displayBoard()    
board: the board
Display the board
Notes
Board representation. The board is represented as a string. Each character is a mark ('X' or 'O') or an empty board position ('_') in row-major order.

Documentation. All files provided include a descriptive comment block at the top of the file, and a descriptive block for each function in JavaDoc/Doxygen style with the description, parameters, and return information. Any files, functions, and declarations you create should include the same kind of documentation.
Use of constants. You are strongly encouraged to declare constants within the file for values used in this module to improve readability, including the board dimension (3), size (3*3), and the special marker '_' used to indicate an empty square.

Parameters. You are strongly encouraged to copy parameters to descriptive local variables to improve readability. The game program passes the board string to functions by value

Return status. Bash functions implicitly return a numeric status: 0 for normal returns, and a number between 1 and 255 in case of an error. The markBoard() function returns 1 if the position is outside the range (0..9), the marker is not 'X' or 'O', or the position is already marked, and 0 if the position was successfully marked. The hasWon() function returns 0 if the player for the marker has won the game and. 1 if not. Both functions are called in tests in the game program.

Return output. Functions that modify the board must output the new board string to standard output. The game program captures the value and assigns it to a local board variable.

Testing
You will test your functions in two ways.

Unit tests
After implementing a set of unit tests for the board functions, run the unit tests to ensure that the unit tests all pass. The initial unit test files for the two representations have an incomplete implementaiton of a unit test where X has won. This unit test will fail the assertion the X has won until you complete this unit test.

Interactively
Test your implementations by interactively playing the game and observing the outcome.

Board display. The initial board should show a matrix of positions like this:

-------------
| 1 | 2 | 3 |
-------------
| 4 | 5 | 6 |
-------------
| 7 | 8 | 9 |
-------------                

This also helps ensure no logic errors in the initBoard() function.

When plays are made, the indexes are replaced by X's and O's. Here is the board after the first moves of the two players.

-------------
| X | 2 | 3 |
-------------
| 4 | 5 | 6 |
-------------
| 7 | 8 | O |
-------------                

This helps ensure no logic errors in the displayBoard() function.

Input validation. Verify that selecting the index of an open square is accepted and updates the board display correctly. Also verify that selecting a position that is not empty or a position that is out of bounds or not an integer is rejected and the player is prompted again to enter a valid position This helps ensure no logic errors in the markBoard() function.

X or O wins. Play a game where X wins, and one where O wins. Unless X makes a mistake, X can always win. If X does make a mistake, it is possible for O to win. After testing X wins, make a mistake and have O win to ensure no logic errors in the hasWon() function. The program will announce which player won and terminate.

No winner. Play a game where there is no winner after all of the positions are filled. This can happen if X recovers from an error but not soon enough to win. The program will announce a tie game and terminate.

Project Staffing
This is an individual assignment, so you will be working on your own.

Turning in the assignment
Turn your assignment in by uploading the two bash files you created and the two from the assignment-2 repository to your 2020SPCS5600SV/assignment-2-ccis_id repository. The graders will download and run your code, so be sure that all the files are avaiable and no extraneous files have been checked in. Once you have checked in your code, please verify that what you checked in is actually in the repository to avoid delays in grading your assignment.

You are welcome to check in intermediate versions during development as a backup to what is on your computer. Only the final version will be graded.

Extras
Once your code is completely working, you might want to experiment with using color and text attributes to make the X and O marks and positions stand out on the board display. This part of the assignment is totally optional.

A program called tput provides a portable way to create special character strings for your terminal to output text with different foreground and background colors and enhancements. Here are a few options. See online documentation for tput for a complete description.

description
tput bold    Returns character string to display future output in double-intensity
tput dim    Returns character string to display future output in half-intensity
tput rev    Returns character string to display future output in reverse video (swaps foreground and background colors)
tput smul    Returns character string to display future output underlined
tput setab [0-7]    Returns character string to display future output with the specified background color (0=black, 1=red, 2=green, 3=yellow, 4=blue, 5=magenta, 6-cyan, 7-white).
tput setaf [0-7]    Returns character string to display future output with the specified foreground color (0=black, 1=red, 2=green, 3=yellow, 4=blue, 5=magenta, 6-cyan, 7-white).
tput sgr0    Returns character string to reset all display attributes of future output
For example, you can create bash strings to display future output in reverse video magenta and to restore normal video as:

rv_magenta=$(tput rev)$(tput setaf 5)
reset_video=$(tput srg0)

Here is a printf command using these strings that displays "Hello World" in reverse video magenta.

printf "%b HelloWorld %b\n", "$rv_magenta" "$reset_video"
The "%b" format specifier is like "%s" but allows it to interpret escape sequences like the ones produced by tput. Try this example on your bash terminal first before modifying your code.

Have fun experimenting with different forgrounds, backgrounds, and text enhancements to display the X and O marks and positions on the board.
