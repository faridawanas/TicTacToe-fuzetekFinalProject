#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Board
{
private:
    vector<vector<char>> grid;
    const int size;

public:
    Board(int size = 3);

    void display() const;
    bool makeMove(int row, int col, char symbol);
    bool isValidMove(int row, int col) const;
    bool checkWin(char symbol) const;
    bool isFull() const;
    char getCell(int row, int col) const;
    void reset();
    int getSize() const;
};


// Abstract Base Class
class Player
{
protected:
    string name;
    char symbol;

public:
    Player(const string& name, char symbol);
    virtual ~Player() {} // Good practice for inheritance

    // Pure virtual method
    virtual void getMove(int& row, int& col) = 0;

    string getName() const;
    char getSymbol() const;
    void setName(const string& name);
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(const string& name, char symbol)
        : Player(name, symbol) {}

    void getMove(int& row, int& col) override
    {
        cin >> row >> col;
    }
};

enum Difficulty { EASY, HARD };

// Derived AI Class
class AIPlayer : public Player
{
private:
    Difficulty difficulty;

public:
    AIPlayer(const string& name, char symbol, Difficulty difficulty):Player(name,symbol) {
        setDifficulty(difficulty);
    };

    void getMove(int& row, int& col) override;
    void setDifficulty(Difficulty newDifficulty) {
        difficulty = newDifficulty;

    };

private:
    void getRandomMove(const Board& board, int& row, int& col) const {

        row=(rand()%3);
        col=(rand()%3);
        while(!board.isValidMove(row,col)) {
            row=(rand()%3);
            col=(rand()%3);
        };

    };

    int minimax(Board b, bool isMax) {

        int score = evaluateBoard(b);

        if (score == 10 || score == -10 || b.isFull())
            return score;

        if (isMax) {
            int best = -1000;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {

                    if (b.isValidMove(i, j)) {
                        Board newBoard = b;
                        newBoard.makeMove(i, j, symbol);

                        best = max(best, minimax(newBoard, false));
                    }
                }
            }

            return best;
        }
        else {
            int best = 1000;
            char opp = (symbol == 'X') ? 'O' : 'X';

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {

                    if (b.isValidMove(i, j)) {
                        Board newBoard = b;
                        newBoard.makeMove(i, j, opp);

                        best = min(best, minimax(newBoard, true));
                    }
                }
            }

            return best;
        }
    }
    void getBestMove(const Board& board, int& row, int& col) {

        int bestVal = -1000;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                if (board.isValidMove(i, j)) {

                    Board newBoard = board;
                    newBoard.makeMove(i, j, symbol);

                    int moveVal = minimax(newBoard, false);

                    if (moveVal > bestVal) {
                        bestVal = moveVal;
                        row = i;
                        col = j;
                    }
                }
            }
        }
    }



    int evaluateBoard(const Board& board) const {
        if (board.checkWin('X')) return (symbol == 'X' ? 10 : -10);
        if (board.checkWin('O')) return (symbol == 'O' ? 10 : -10);
        return 0;
    }
};

class Game
{
private:
    Board board;
    Player* player1;
    Player* player2;
    Player* currentPlayer;

public:
    Game();
    ~Game(); // To clean up pointers if needed

    void start();
    void showMenu();

private:
    void setupPvP();
    void setupPvC(Difficulty difficulty) {
        if(difficulty == EASY) {

            player2=new AIPlayer(player2->getName(),player2->getSymbol(),EASY);
        }
        else if(difficulty == HARD) {
            player2=new AIPlayer(player2->getName(),player2->getSymbol(),HARD);
        }
    };
    void switchPlayer();
    void handleHumanMove(Player* player);
    void handleAIMove(AIPlayer* aiPlayer) {
        int r,c;
        aiPlayer->getMove(r,c);
          board.makeMove(r,c,aiPlayer->getSymbol());
    };
    bool checkGameEnd();
    void displayResult() const;
    void reset();
};

int main()
{
    Game ticTacToe;
    ticTacToe.start();
    return 0;
}
