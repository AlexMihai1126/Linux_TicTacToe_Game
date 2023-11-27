#include <ncurses.h>

void deseneazaTablaJoc(char tablaJoc[3][3]) {
    clear();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printw("%c", tablaJoc[i][j]);
            if (j < 2) {
                printw(" | ");
            }
        }
        printw("\n");
        if (i < 2) {
            printw("---------\n");
        }
    }

    refresh();
}

bool verificaCastig(char tablaJoc[3][3], char jucatorId) {
    for (int i = 0; i < 3; ++i) {
        if ((tablaJoc[i][0] == jucatorId && tablaJoc[i][1] == jucatorId && tablaJoc[i][2] == jucatorId) ||
            (tablaJoc[0][i] == jucatorId && tablaJoc[1][i] == jucatorId && tablaJoc[2][i] == jucatorId)) {
            return true;
        }
    } //randuri si coloane

    if ((tablaJoc[0][0] == jucatorId && tablaJoc[1][1] == jucatorId && tablaJoc[2][2] == jucatorId) ||
        (tablaJoc[0][2] == jucatorId && tablaJoc[1][1] == jucatorId && tablaJoc[2][0] == jucatorId)) {
        return true;
    } //diagonale

    return false;
}

int main() {
    char tablaJoc[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    char jucatorCurent = 'X';
    int randTabla, coloanaTabla;
    MEVENT miscareMouse;

    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    mousemask(ALL_MOUSE_EVENTS, nullptr);

    while (true) {
        deseneazaTablaJoc(tablaJoc);
        printw("Player %c's turn. Click on a cell to make a move.\n", jucatorCurent);
        refresh();

        int ch = getch(); //citeste inputurile si verifica daca este mouse
        if (ch == KEY_MOUSE) {
            if (getmouse(&miscareMouse) == OK) {
                randTabla = miscareMouse.y / 2; // Each randTabla is separated by 2 lines
                coloanaTabla = miscareMouse.x / 4; // Each column is separated by 4 characters

                // Check if the selected cell is empty
                if (randTabla >= 0 && randTabla < 3 && coloanaTabla >= 0 && coloanaTabla < 3 && tablaJoc[randTabla][coloanaTabla] == ' ') {
                    tablaJoc[randTabla][coloanaTabla] = jucatorCurent;

                    // Check for a win
                    if (verificaCastig(tablaJoc, jucatorCurent)) {
                        deseneazaTablaJoc(tablaJoc);
                        printw("Player %c wins!\n", jucatorCurent);
                        printw("Press any key to exit.\n");
                        refresh();
                        getch();
                        break;
                    }

                    // Check for a tie
                    bool remiza = true;
                    for (auto & i : tablaJoc) {
                        for (char j : i) {
                            if (j == ' ') {
                                remiza = false;
                                break;
                            }
                        }
                        if (!remiza) {
                            break;
                        }
                    }

                    if (remiza) {
                        deseneazaTablaJoc(tablaJoc);
                        printw("It's a tie!\n");
                        printw("Press any key to exit.\n");
                        refresh();
                        getch();
                        break;
                    }

                    // Switch to the other player
                    jucatorCurent = (jucatorCurent == 'X') ? 'O' : 'X';
                }
            }
        }
    }

    // Clean up ncurses
    endwin();

    return 0;
}
