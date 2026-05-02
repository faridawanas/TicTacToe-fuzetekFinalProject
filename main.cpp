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
    AIPlayer(const string& name, char symbol, Difficulty difficulty);

    void getMove(int& row, int& col) override;
    void setDifficulty(Difficulty newDifficulty);

private:
    void getRandomMove(const Board& board, int& row, int& col) const;
    void getBestMove(const Board& board, int& row, int& col) const; // Minimax
    int evaluateBoard(const Board& board) const;
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
    void setupPvC(Difficulty difficulty);
    void switchPlayer();
    void handleHumanMove(Player* player);
    void handleAIMove(AIPlayer* aiPlayer);
    bool checkGameEnd();
    void displayResult() const;
    void reset();
};

// Board::isValidMove
bool Board::isValidMove(int row, int col) const
{
    if (row < 0 || row >= size || col < 0 || col >= size)
        return false;

    return grid[row][col] == ' ';
}

// Board::makeMove
bool Board::makeMove(int row, int col, char symbol)
{
    if (!isValidMove(row, col))
        return false;

    grid[row][col] = symbol;
    return true;
}

// Game::handleHumanMove
void Game::handleHumanMove(Player* player)
{
    int row, col;

    while (true)
    {
        player->getMove(row, col);

        row--;
        col--;

        if (board.makeMove(row, col, player->getSymbol()))
            break;
    }
}

int main()
{
    Game ticTacToe;
    ticTacToe.start();
    return 0;
}
