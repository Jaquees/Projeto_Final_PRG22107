#ifndef CHECKERS_GAME
#define CHECKERS_GAME

// Inclusão do arquivo de cabeçalho checkersgame.h
#include "checkersgame.h"

// Função para redefinir o tabuleiro
void resetBoard(std::map<std::pair<char, char>, char> & gameBoard) {
    // Cria um novo mapa temporário para armazenar o novo estado do tabuleiro
    std::map<std::pair<char, char>, char> temp;
    char currentPiece = '0';

    // Itera sobre as linhas do tabuleiro
    for (char y = '1'; y <= '8'; y++) {
        // Determina a peça inicial para cada linha
        if (y <= '3')
            currentPiece = pieces[Black];
        else if (y >= '6')
            currentPiece = pieces[White];
        else
            currentPiece = pieces[Empty];

        // Itera sobre as colunas do tabuleiro
        for (char x = 'a'; x <= 'h'; x++) {
            // Adiciona a peça ao novo mapa se a posição for válida
            if ((((y - 1) % 2) == 0) == (((x - 97) % 2) == 0)) {
                temp[std::make_pair(x, y)] = currentPiece;
            }
        }
    }

    // Atualiza o tabuleiro com o novo estado
    gameBoard = temp;
}

// Função para encontrar as peças restantes para um jogador
std::vector<std::pair<char, char>> findPiecesRemaining(const int & player,const std::map<std::pair<char, char>, char> & gameBoard) {
    // Vetor para armazenar as posições das peças restantes
    std::vector<std::pair<char, char>> tokens;

    // Itera sobre o tabuleiro e adiciona as peças do jogador ao vetor
    for (auto el : gameBoard) {
        if (el.second == pieces[player]) // peças normais
            tokens.push_back(el.first);
        if (el.second == pieces[player + 1]) // damas
            tokens.push_back(el.first);
    }

    // Retorna o vetor de posições das peças restantes
    return tokens;
}

// Função para verificar se há um impasse (stalemate) para o jogador especificado
bool checkStalemate(const int & playerTurn, const std::map<std::pair<char, char>, char> & gameBoard) {
    auto playerPieces = findPiecesRemaining(playerTurn, gameBoard);
    bool moveFound = false;

    // Itera sobre as peças do jogador e verifica se há movimentos válidos
    for (auto el : playerPieces) {
        if (singleSquareMove(el, std::make_pair('z', 'z'), gameBoard, true)) {
            moveFound = true;
            break;
        } else if (findJumpSquares(playerTurn, el, gameBoard).first.size() > 0) {
            moveFound = true;
            break;
        }
    }
    // Retorna verdadeiro se nenhum movimento válido for encontrado (impasse)
    return !moveFound;
}

// Função para promover fichas para damas se estiverem na última linha do adversário
void checkPromote(std::map<std::pair<char, char>, char> & gameBoard) {
    for (auto el : gameBoard) {
        if ((el.first).second == '8' && el.second == pieces[Black])
            gameBoard[el.first] = pieces[BlackKing];

        if ((el.first).second == '1' && el.second == pieces[White])
            gameBoard[el.first] = pieces[WhiteKing];
    }
}

// Função para verificar e processar um movimento, retornando se foi bem-sucedido e os detalhes do movimento
std::pair<bool, std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>>> checkMove(const int & player, const std::pair<char, char> & from, const std::pair<char, char> & to,const std::map<std::pair<char, char>, char> & gameBoard) {

    // Flag para indicar erros durante o movimento
    bool errors = false;
    // Estrutura para armazenar o resultado do movimento
    std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>> returnResult;

    // Verifica se as posições de origem e destino estão no tabuleiro
    auto fromIt = gameBoard.find(from);
    if (fromIt == gameBoard.end()) {
            errors = true;
    } else {
        // Verifica se o jogador está movendo sua própria peça
        if ((fromIt->second != pieces[player]) && (fromIt->second != pieces[player + 1])) {
                errors = true;
        }
    }

    auto toIt = gameBoard.find(to);
    if (toIt == gameBoard.end()) {
            errors = true;
    } else {
        // Verifica se o espaço de destino está vazio
        if (toIt->second != pieces[Empty]) {
                errors = true;
        }
    }

    // Verifica se o movimento é de captura
    if (to.first == from.first + 2 && to.second == from.second + 2) {
        // Verifica se a casa à frente da peça está vazia
        auto it = gameBoard.find({from.first + 1, from.second + 1});
        if (it != gameBoard.end() && it->second != pieces[Empty]) {
            // Verifica se a peça capturada é uma peça inimiga
            if (it->second == pieces[player ^ 1]) {
                // O movimento é válido
                return {true, {{}, {}}};
            }
        }
    } else if (to.first == from.first - 2 && to.second == from.second + 2) {
        // Verifica se a casa à frente da peça está vazia
        auto it = gameBoard.find({from.first - 1, from.second + 1});
        if (it != gameBoard.end() && it->second != pieces[Empty]) {
            // Verifica se a peça capturada é uma peça inimiga
            if (it->second == pieces[player ^ 1]) {
                // O movimento é válido
                return {true, {{}, {}}};
            }
        }
    }

    // Existe um caminho de salto válido?
    if (!errors) { // Não se preocupe em fazer a busca se o movimento já for conhecido como inválido
        if (singleSquareMove(from, to, gameBoard)) {
            returnResult.first.push_back(to);
        } else {
            auto result = jumpPathSearch(from, to, gameBoard);
            if (!result.first) {
                errors = true;
            } else {
                returnResult = result.second;
            }
        }
    }
    return {!errors, returnResult};
}

// Função para procurar um caminho de salto
std::pair<bool, std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>>> jumpPathSearch(const std::pair<char, char> &from, const std::pair<char, char> &to,const std::map<std::pair<char, char>, char> &gameBoard) {

    std::set<std::pair<char, char>> possibilities{};        // casas que precisam ser pesquisadas
    std::set<std::pair<char, char>> searchedPossibilities{}; // casas que já foram pesquisadas

    std::vector<std::pair<char, char>> path{};       // casas saltadas no caminho para a casa "TO"
    std::vector<std::pair<char, char>> jumpedActual{}; // peças inimigas saltadas no processo
    bool pathFound = false;
    int playerPiece = gameBoard.at(from);

    // Estrutura de nó para armazenar informações sobre as casas durante a busca
    struct Node {
    public:
        std::pair<char, char> square;                 // Qual casa é esta
        std::stack<std::pair<char, char>> jumpPoints; // Onde este nó pode saltar
        std::pair<char, char> jumpedOver = {0, 0};    // Qual peça inimiga este nó saltou
        bool searched = false;
        std::shared_ptr<Node> fromPtr = nullptr;
        Node(std::pair<char, char> square, std::shared_ptr<Node> fromPtr, std::pair<char, char> jumpedOver) {
            this->square = square;
            this->fromPtr = fromPtr;
            this->jumpedOver = jumpedOver;
        }
        Node(std::pair<char, char> x) {
            square = x;
        }
        Node() {
        }
    };

    Node current(from);
    possibilities.insert(from);

    while (possibilities != searchedPossibilities) {
        auto temp = findJumpSquares(playerPiece, current.square, gameBoard);
        if (pathFound) {
            path.push_back(current.square);
            if (current.fromPtr == nullptr) {
                break;
            } else {
                jumpedActual.push_back(current.jumpedOver);
                current = *(current.fromPtr);
            }

        } else {
            if (!current.searched) {
                for (unsigned int i = 0; i < temp.first.size(); i++) {
                    auto el = temp.first.at(i);
                    current.jumpPoints.push(el);
                    possibilities.insert(el);
                    if (el == to) {
                        path.push_back(el);
                        jumpedActual.push_back(temp.second.at(i));
                        pathFound = true;
                        break;
                    }
                }
                current.searched = true;
            }
            if (!current.jumpPoints.empty()) {
                auto top = current.jumpPoints.top();

                while (searchedPossibilities.find(top) != searchedPossibilities.end()) {
                    if (!current.jumpPoints.empty()) // evita falha
                        current.jumpPoints.pop();

                    if (current.jumpPoints.empty()) // evita falha
                        break;
                    else
                        top = current.jumpPoints.top();
                }
            }
            if (current.jumpPoints.empty()) { // Verifica se chegou ao final do caminho
                if (current.fromPtr == nullptr) {
                    break;
                } else {
                    current = *(current.fromPtr); // Retorna ao final do caminho
                }
            } else {
                auto top = current.jumpPoints.top();
                current.jumpPoints.pop();
                searchedPossibilities.insert(current.square);
                // Cria um novo nó e continua a busca
                current = Node(top, std::make_shared<Node>(current), temp.second.at(current.jumpPoints.size()));
            }
        }
    }
    // Retorna:
    // - booleano: foi encontrado um caminho?
    // - par: vetor de pontos -> casas que a peça saltou a caminho do destino
    //       vetor de pontos -> casas das peças inimigas saltadas ao longo do caminho
    //
    // Nota: o primeiro vetor de pontos é retornado para a futura implementação da animação da sequência de saltos.
    return {pathFound, {path, jumpedActual}};
}

// Função para encontrar casas que podem ser saltadas
std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>> findJumpSquares(const char &playerPiece, const std::pair<char, char> &square,const std::map<std::pair<char, char>, char> &gameBoard) {

    // Vetores de retorno
    std::vector<std::pair<char, char>> output;
    std::vector<std::pair<char, char>> jumped;

    int enemy;
    // Acompanha quais peças podem ser saltadas legalmente
    if (playerPiece == pieces[Black] || playerPiece == pieces[BlackKing])
        enemy = White;
    else
        enemy = Black;

    char possibleJumpedPiece;

    if (playerPiece != pieces[White]) { // peças que podem avançar
        if ((square.first < 'g') && (square.second < '7')) {
            // Casa superior direita
            possibleJumpedPiece = gameBoard.at({square.first + 1, square.second + 1});
            if ((possibleJumpedPiece == pieces[enemy]) || (possibleJumpedPiece == pieces[enemy + 1] /* rei */)) {
                if (gameBoard.find({square.first + 2, square.second + 2})->second == pieces[Empty]) {
                    output.push_back({square.first + 2, square.second + 2});
                    jumped.push_back({square.first + 1, square.second + 1});
                }
            }
        }
        if ((square.first > 'b') && (square.second < '7')) { // Certifica-se de estar saltando sobre uma peça inimiga
            // Casa superior esquerda
            possibleJumpedPiece = gameBoard.at({square.first - 1, square.second + 1});
            if ((possibleJumpedPiece == pieces[enemy]) || (possibleJumpedPiece == pieces[enemy + 1])) {
                if (gameBoard.find({square.first - 2, square.second + 2})->second == pieces[Empty]) {
                    output.push_back({square.first - 2, square.second + 2});
                    jumped.push_back({square.first - 1, square.second + 1});
                }
            }
        }
    }

    if (playerPiece != pieces[Black]) { // peças que podem retroceder
        if ((square.first < 'g') && (square.second > '2')) {
            // Casa inferior direita
            possibleJumpedPiece = gameBoard.at({square.first + 1, square.second - 1});
            if ((possibleJumpedPiece == pieces[enemy]) || (possibleJumpedPiece == pieces[enemy + 1])) {
                if (gameBoard.find({square.first + 2, square.second - 2})->second == pieces[Empty]) {
                    output.push_back({square.first + 2, square.second - 2});
                    jumped.push_back({square.first + 1, square.second - 1});
                }
            }
        }

        if ((square.first > 'b') && (square.second > '2')) {
            // Casa inferior esquerda
            possibleJumpedPiece = gameBoard.at({square.first - 1, square.second - 1});
            if ((possibleJumpedPiece == pieces[enemy]) || (possibleJumpedPiece == pieces[enemy + 1])) {
                if (gameBoard.find({square.first - 2, square.second - 2})->second == pieces[Empty]) {
                    output.push_back({square.first - 2, square.second - 2});
                    jumped.push_back({square.first - 1, square.second - 1});
                }
            }
        }
    }

    return {output, jumped};
}

// Função para verificar se o movimento é de uma casa de distância e se é válido.
bool singleSquareMove(const std::pair<char, char> &from, const std::pair<char, char> &to,const std::map<std::pair<char, char>, char> &gameBoard,const bool &findAllSquares) {

    char playerPiece = gameBoard.find(from)->second;
    int amtSquares = 0;
    if (playerPiece == pieces[Black] || playerPiece == pieces[BlackKing] || playerPiece == pieces[WhiteKing]) {
        // Peças que podem se mover para cima
        // Casa superior direita
        if (findAllSquares) {
            auto it = gameBoard.find(std::make_pair(from.first + 1, from.second + 1));
            if (it != gameBoard.end()) {
                if (it->second == pieces[Empty]) { // se a casa estiver vazia
                    amtSquares++;
                }
            }
        } else if ((from.first + 1) == (to.first)) {
            if ((from.second + 1) == (to.second)) {
                return true;
            }
        }

        // Casa superior esquerda
        if (findAllSquares) {
            auto it = gameBoard.find(std::make_pair(from.first - 1, from.second + 1));
            if (it != gameBoard.end()) {
                if (it->second == pieces[Empty]) { // se a casa estiver vazia
                    amtSquares++;
                }
            }
        } else if ((from.first - 1) == (to.first)) {
            if ((from.second + 1) == (to.second)) {
                return true;
            }
        }
    }
    if (playerPiece == pieces[White] || playerPiece == pieces[BlackKing] || playerPiece == pieces[WhiteKing]) {
        // Peças que podem se mover para baixo
        // Casa inferior esquerda
        if (findAllSquares) {
            auto it = gameBoard.find(std::make_pair(from.first - 1, from.second - 1));
            if (it != gameBoard.end()) {
                if (it->second == pieces[Empty]) { // se a casa estiver vazia
                    amtSquares++;
                }
            }
        } else if ((from.first - 1) == (to.first)) {
            if ((from.second - 1) == (to.second)) {
                return true;
            }
        }

        // Casa inferior direita
        if (findAllSquares) {
            auto it = gameBoard.find(std::make_pair(from.first + 1, from.second - 1));
            if (it != gameBoard.end()) {
                if (it->second == pieces[Empty]) { // se a casa estiver vazia
                    amtSquares++;
                }
            }
        } else if ((from.first + 1) == (to.first)) {
            if ((from.second - 1) == (to.second)) {
                return true;
            }
        }
    }

    if (!findAllSquares) // Se estivermos procurando por um movimento específico
        return false;    // Se chegamos aqui, não é um movimento de uma casa
    else
        return amtSquares != 0; // Se estivermos apenas procurando por qualquer movimento de uma casa
}

// remove a peça
void removeSquare(const std::pair<char, char> &square, std::map<std::pair<char, char>, char> &gameBoard) {
    gameBoard.at(square) = pieces[Empty];
}

// Move a peça sem verificar se o movimento é legal.
void movePiece(std::pair<char, char> &from,std::pair<char, char> &to,std::map<std::pair<char, char>, char> &gameBoard) {

    gameBoard.at(to) = gameBoard.at(from);
    gameBoard.at(from) = pieces[Empty];
}

// Verifica se os jogadores têm peças restantes para jogar
int checkWinStatus(std::map<std::pair<char, char>, char> &gameBoard, int &playerTurn) {
    if ((findPiecesRemaining(White, gameBoard)).size() == 0) {
        if ((findPiecesRemaining(Black, gameBoard)).size() == 0) {
            return Draw;
        } else {
            return BlackWin;
        }
    } else if ((findPiecesRemaining(Black, gameBoard)).size() == 0) {
        return WhiteWin;
    } else if (checkStalemate(playerTurn, gameBoard)) {
        if (checkStalemate(((playerTurn == Black) ? White : Black), gameBoard))
            return Draw; // Ambos estão em um impasse
        return ((playerTurn == Black) ? WhiteWin : BlackWin);
    } else {
        return ValidMove;
    }
}

// Manipula a vez do jogador
int takeTurn(std::map<std::pair<char, char>, char> &gameBoard, std::pair<std::pair<char, char>, std::pair<char, char>> playerMove,int &playerTurn) {


    auto result = checkMove(playerTurn, playerMove.first, playerMove.second, gameBoard);
    // O resultado também contém o caminho percorrido para a jogada, para possível animação no futuro

    if (result.first) { // Se a jogada for legal
        movePiece(playerMove.first, playerMove.second, gameBoard);
        for (auto el : result.second.second) { // Exclui quaisquer tokens "pulados"
            removeSquare(el, gameBoard);
        }
    } else {
        return InvalidMove;
    }

    // Agora é a vez do outro jogador
    if (playerTurn == Black)
        playerTurn = White;
    else
        playerTurn = Black;

    checkPromote(gameBoard);

    return checkWinStatus(gameBoard, playerTurn);
}


#endif
