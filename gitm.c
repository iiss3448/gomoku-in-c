// name: IMRAN ISSA
// unikey: iiss3448
// SID: 500459100

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT 4096
#define MAX_HISTORY 982

char board[19][20] = {0};
char current_player;
int is_game_ended = 0;

void load_board(void) {
    for (size_t i=0; i < 19; i++) {
        memset(board[i], '.', 19);
    }
}

void who(void) {
    printf("%c\n", current_player);
}

void view(char *pos) {
    char mist_centre[5];
    int mist_x, mist_y;
    if (strlen(pos) == 0) {
        mist_y = 10, mist_x = 10;
        sprintf(mist_centre, "%c%d", mist_y + '@', mist_x);
    }
    else {
        int col = pos[0] - '@'; // This is a capital letter, must obtain range of 1-19
        char *strrow = pos + 1;
        int row = atoi(strrow);

        // Calculate mist centre
        mist_y = 1 + (5*col*col + 3*col + 4) % 19;
        mist_x = 1 + (4*row*row + 2*row - 4) % 19;
        sprintf(mist_centre, "%c%d", mist_y + '@', mist_x);
    }

    // Calculate index for board
    if (mist_x > 10) {
        int diff = mist_x - 10;
        mist_x = 10 - diff;
    }
    else if (mist_x < 10) {
        int diff = 10 - mist_x;
        mist_x = 10 + diff;
    }
    mist_y--, mist_x--;

    // Retrieve each row in mist hole
    char mist_hole[50] = {};
    for (int i = mist_x - 3; i <= mist_x + 3; i++) {
        char current_row[8] = "";
        int current_col = 0;
        for (int j = mist_y - 3; j <= mist_y + 3; j++) {
            if (i > 18 || j > 18 || i < 0 || j < 0) {
                current_row[current_col] = 'x';
            }
            else {
                current_row[current_col] = board[i][j];
            }
            current_col++;
        }
        strcat(mist_hole, current_row);
    }
    printf("%s,%s\n", mist_centre, mist_hole);
}

void end_game(char result) {
    switch (result) {
    case 'B':
        printf("Black wins!\n");
        break;
    case 'W':
        printf("White wins!\n");
        break;
    case 'T':
        printf("Wow, a tie!\n");
        break;
    }
    is_game_ended = 1;
}

int substr(char sub[], const char str[]) {
    int str_len = 0, sub_len = 0;
    while (str[str_len] != '\0')
    {
        str_len++;
    }
    while (sub[sub_len] != '\0')
    {
        sub_len++;
    }
    for (int i = 0; i < str_len; i++)
    {
        if (str[i] == sub[0])
        {
            int exists = 1;
            int j;
            for (j = 0; j < sub_len; j++)
            {
                if (str[i + j] != sub[j])
                {
                    exists = 0;
                    break;
                }
            }
            if (i + j < str_len)
            {
                if (exists && str[i + j] != sub[0] && str[i + j - sub_len - 1] != sub[0])
                {
                    return 1;
                }
            }
            else if (i + j == str_len) {
                if (exists && str[i + j - sub_len - 1] != sub[0])
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void check_win(int row, int col) {
    // Assume the game is tied and thus the board is full
    int is_tie = 1;
    // Check if there are any empty spaces left, update is_tie to 0 if an empty space is found
    for (size_t i = 0; i < 19; i++)
    {
        char *line = strchr(board[i], '.');
        if (line)
        {
            is_tie = 0;
            break;
        }
    }
    if (is_tie)
    {
        end_game('T');
    }
    // Check if there is a winner
    else
    {
        char full_row[20], full_col[20] = {0}, positive_diag[20] = {0}, negative_diag[20] = {0};
        // Calculate result of entire row of stone's coordinate
        memcpy(full_row, board[row], 20);
        // Calculate result of entire column of stone's coordinate
        for (size_t i = 0; i < 19; i++)
        {
            full_col[i] = board[i][col];
        }

        // Calculate result of diagonal with positive gradient of stone's coordinate
        if (row >= col) { // A
            // Before stone
            for (size_t i = 0; i <= col && row - col + i <= row; i++)
            {
                positive_diag[i] = board[row - col + i][i];
            }
            // After stone
            for (size_t i = 1; col + i < 19 && row + i < 19; i++)
            {
                positive_diag[col + i] = board[row + i][col + i];
            }
        }
        else { // B
            // Before stone
            for (size_t i = 0; i <= col && col - row + i <= row; i++)
            {
                positive_diag[i] = board[col - row + i][i];
            }
            // After stone
            for (size_t i = 1; col + i < 19 && row + i < 19; i++)
            {
                positive_diag[col + i] = board[row + i][col + i];
            }
        }

        // Calculate result of diagonal with negative gradient of stone's coordinate
        // Rows before stone
        if (row + col < 19) {
            for (size_t i = 0; i <= col; i++) {
                negative_diag[i] = board[row + col - i][i];
            }
        }
        else {
            for (size_t i = 0; i <= col; i++) {
                negative_diag[i] = board[18 - i][i];
            }
        }
        // Rows after stone
        for (size_t i = 1; col + i < 19 && row - i > -1; i++) {
            negative_diag[col + i] = board[row - i][col + i];
        }

        // Check for winning combination
        if (current_player == 'B')
        {
            if (substr("#####", full_row))
            {
                end_game(current_player);
            }
            else if (substr("#####", full_col))
            {
                end_game(current_player);
            }
            else if (substr("#####", positive_diag))
            {
                end_game(current_player);
            }
            else if (substr("#####", negative_diag))
            {
                end_game(current_player);
            }
        }
        else
        {
            if (substr("ooooo", full_row))
            {
                end_game(current_player);
            }
            else if (substr("ooooo", full_col))
            {
                end_game(current_player);
            }
            else if (substr("ooooo", positive_diag))
            {
                end_game(current_player);
            }
            else if (substr("ooooo", negative_diag))
            {
                end_game(current_player);
            }
        }        
    }
}

int place(char *pos) {
    // Subtract 1 from both coordinate values are the coordinates are given as one-based indexing
    int col = pos[0] - '@'; // This is a capital letter, must obtain range of 1-19
    char *strrow = pos + 1;
    int row = atoi(strrow);

    // Check for valid coordinates given
    if (col > 19 || row > 19 || col < 1 || row < 1)
    {
        printf("Invalid coordinate\n");
        return 0;
    }
    else
    {
        if (row > 10)
        {
            int diff = row - 10;
            row = 10 - diff;
        }
        else if (row < 10)
        {
            int diff = 10 - row;
            row = 10 + diff;
        }
        row--, col--;
        // Check if coordinate is occupied
        if (board[row][col] != '.')
        {
            printf("Occupied coordinate\n");
            return 0;
        }
        else
        {
            // Place stone on board based on current player
            if (current_player == 'B')
            {
                board[row][col] = '#';
            }
            else
            {
                board[row][col] = 'o';
            }
            check_win(row, col);
            return 1;
        }
    }
}

int main(int argc, char** argv) {
    // Initialise game
    char input[MAX_INPUT];
    load_board();
    current_player = 'B';
    char history[MAX_HISTORY] = "";
    char recent_stone[4] = "";

    // Loop for input
    while (1) {
        fgets(input, MAX_INPUT, stdin);

        // Error checking input length
        if (strlen(input) < 3) {
            printf("Invalid!\n");
            continue;
        }

        // Error checking alphanumeric characters
        int non_alnum = 0;
        for (size_t i = 0; i < strlen(input); i++) {
            if (isalnum(input[i]) == 0 && isspace(input[i]) == 0 && input[i] != '\0') {
                non_alnum = 1;
                break;
            }
        }
        if (non_alnum) {
            printf("Invalid!\n");
            continue;
        }

        // Error checking whitespace
        int whitespace_count = 0;
        for (size_t i = 0; i < strlen(input); i++) {
            if (input[i] == ' ') {
                whitespace_count++;
            }
        }
        if (whitespace_count > 1) {
            printf("Invalid!\n");
            continue;
        }

        // If whitespace is not detected, no parameters are parsed
        if (strchr(input, ' ') == NULL) {
            if (strcmp(input, "who\n") == 0) {
                who();
            }
            else if (strcmp(input, "term\n") == 0) {
                return 1;
            }
            else if (strcmp(input, "resign\n") == 0) {
                if (current_player == 'B') {
                    end_game('W');
                    return 0;
                }
                else {
                    end_game('B');
                }
                printf("%s\n", history);
                printf("Thank you for playing!\n");
                return 0;
            }
            else if (strcmp(input, "view\n") == 0) {
                if (strlen(history) == 0) {
                    view("");
                }
                else {
                    view(recent_stone);
                }
            }
            else if (strcmp(input, "history\n") == 0) {
                printf("%s\n", history);
            }
            else {
                printf("Invalid!\n");
                continue;
            }
        }

        // Else, whitespace is detected, parameters are parsed
        else {
            char *token = strtok(input, " ");
            if (strcmp(token, "place") == 0) {
                token = strtok(NULL, " ");
                // Check coordinate length
                if (strlen(token) < 3) {
                    printf("Invalid!\n");
                    continue;
                }

                // Check if exactly one alphabet character is given
                int alpha = 0;
                for (size_t i = 0; i < strlen(token); i++) {
                    if (isalpha(token[i]) != 0) {
                        alpha++;
                    }
                }
                if (alpha != 1) {
                    printf("Invalid coordinate\n");
                    continue;
                }

                // Check if digits are given
                int is_digit = 0;
                for (size_t i = 0; i < strlen(token); i++) {
                    if (isdigit(token[i]) != 0) {
                        is_digit = 1;
                        break;
                    }
                }
                if (is_digit == 0) {
                    printf("Invalid coordinate\n");
                    continue;
                }

                // Coordinates cannot have 0 as the first digit
                if (token[1] == '0') {
                    printf("Invalid coordinate\n");
                    continue;
                }

                token[strcspn(token, "\n")] = '\0';
                int is_placed = place(token);
                if (is_game_ended && is_placed) {
                    strcat(history, token);
                    printf("%s\n", history);
                    printf("Thank you for playing!\n");
                    return 0;
                }
                // Change player after current player has made their move
                // Record move in history
                else if (is_placed) {
                    strcat(history, token);
                    strcpy(recent_stone, token);
                    if (current_player == 'B') {
                        current_player = 'W';
                    }
                    else {
                        current_player = 'B';
                    }
                }
                token = strtok(NULL, " ");
            }
            else {
                printf("Invalid!\n");
                continue;
            }
        }   
    }
}
