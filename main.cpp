//TEMA 1 SO, ANUL 3, MIHAI ALEXANDRU-MARIO GRUPA 363
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
    for (int i = 0; i < 3; i++) {
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
    char tablaJoc[3][3] = {{' ', ' ', ' '},
                           {' ', ' ', ' '},
                           {' ', ' ', ' '}};
    char jucatorCurent = 'X';
    int randTabla, coloanaTabla;
    MEVENT miscareMouse;

    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    mousemask(ALL_MOUSE_EVENTS, nullptr); //configuram NCurses sa foloseasca mouse-ul

    while (true) {
        deseneazaTablaJoc(tablaJoc);
        printw("Randul jucatorului %c. Apasa pe o casuta pentru a face o mutare.\n", jucatorCurent);
        refresh();

        int ch = getch(); //citeste inputurile si verifica daca este de la mouse
        if (ch == KEY_MOUSE) {
            if (getmouse(&miscareMouse) == OK) {
                randTabla = miscareMouse.y / 2;
                coloanaTabla = miscareMouse.x / 4; //trecem in indici pt matricea care memoreaza jocul

                if (randTabla >= 0 && randTabla < 3 && coloanaTabla >= 0 && coloanaTabla < 3 &&
                    tablaJoc[randTabla][coloanaTabla] == ' ') {
                    tablaJoc[randTabla][coloanaTabla] = jucatorCurent; //vedem daca casuta in care vrem sa inseram este libera

                    if (verificaCastig(tablaJoc, jucatorCurent)) {
                        deseneazaTablaJoc(tablaJoc);
                        printw("A castigat %c !\n", jucatorCurent);
                        printw("Apasa orice tasta pentru a iesi din program.\n");
                        refresh();
                        getch();
                        break; //verificam daca jucatorul curent a castigat
                    }

                    bool remiza = true;
                    for (auto &i: tablaJoc) {
                        for (char j: i) {
                            if (j == ' ') {
                                remiza = false;
                                break;
                            } //plecam de la presupunerea ca jocul este remiza, daca gasim cel putin o casuta libera inseamna ca jocul inca nu s-a terminat
                        }
                        if (!remiza) {
                            break;
                        }
                    }

                    if (remiza) {
                        deseneazaTablaJoc(tablaJoc);
                        printw("Este remiza!\n");
                        printw("Apasa orice tasta pentru a iesi din program.\n");
                        refresh();
                        getch();
                        break;
                    } //in caz de remiza jocul se incheie

                    jucatorCurent = (jucatorCurent == 'X') ? 'O' : 'X'; //schimbam jucatorul curent dupa o runda.
                }
            }
        }
    }

    endwin();

    return 0;
}
