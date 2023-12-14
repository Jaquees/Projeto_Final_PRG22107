#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H
#include <stdio.h>
#include <tchar.h>
#include <ctype.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <exception>
#include <stack>
#include <memory>

typedef enum Move_State {
    InvalidMove = 0,
    ValidMove,
    WhiteWin,
    BlackWin,
    Draw
} Move_State;

typedef enum Pieces_List {
    Empty = 0,
    Black,
    BlackKing,
    White,
    WhiteKing
} Pieces_List;

// Representação visual das peças no tabuleiro
static const std::vector<char> pieces = { '.', 'x', 'X', 'o', 'O' };

// Reinicializa o tabuleiro
void resetBoard(std::map<std::pair<char, char>, char> &gameBoard);

// Imprime o tabuleiro na saída padrão
void printBoard(const std::map<std::pair<char, char>, char> &gameBoard);

// Encontra as peças restantes de um determinado tipo no tabuleiro
std::vector<std::pair<char, char>> findPiecesRemaining(const char &piece, const std::map<std::pair<char, char>, char> &gameBoard);

// Verifica se há um impasse no jogo
bool checkStalemate(const int &playerTurn, const std::map<std::pair<char, char>, char> &gameBoard);

// Promove peças para "reis" quando alcançam a última linha
void checkPromote(std::map<std::pair<char, char>, char> &gameBoard);

// Busca o caminho para um salto entre duas posições no tabuleiro
std::pair<bool, std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>>> jumpPathSearch(const std::pair<char, char> &from,const std::pair<char, char> &to,const std::map<std::pair<char, char>, char> &gameBoard);

// Verifica a validade de um movimento
std::pair<bool, std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>>> checkMove(const int &player,const std::pair<char, char> &from,const std::pair<char, char> &to,const std::map<std::pair<char, char>, char> &gameBoard);

// Encontra as posições de salto disponíveis para uma peça
std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>> findJumpSquares(const char &playerPiece,const std::pair<char, char> &square, const std::map<std::pair<char, char>, char> &gameBoard);

// Verifica se um movimento é de uma única casa
bool singleSquareMove(const std::pair<char, char> &from,const std::pair<char, char> &to,const std::map<std::pair<char, char>, char> &gameBoard,const bool &findAllSquares = false);

// Imprime uma mensagem de erro
void printError();

// Move uma peça no tabuleiro
void movePiece(std::pair<char, char> &to,std::pair<char, char> &from,std::map<std::pair<char, char>, char> &gameBoard);

// Obtém o movimento do usuário
std::pair<std::pair<char, char>, std::pair<char, char>> getUserMove(std::string &prompt);

// Verifica o estado do jogo (vitória, derrota, empate, movimento válido)
int checkWinStatus(std::map<std::pair<char, char>, char> &gameBoard, int &playerTurn);

// Realiza a vez de um jogador
int takeTurn(std::map<std::pair<char, char>, char> &gameBoard,std::pair<std::pair<char, char>, std::pair<char, char>> playerMove,int &playerTurn);

#endif
