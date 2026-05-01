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
    Board(int size = 3): size(size)
    {
        grid.resize(size, vector<char>(size, ' '));
    }

    void display() const
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                cout << grid[i][j];
                if (j < size - 1) cout << " | ";
            }
            cout << endl;

            if (i < size - 1)
            {
                for (int k = 0; k < size * 4 - 3; k++) cout << "-";
                cout << endl;
            }
        }
    }

    bool makeMove(int row, int col, char symbol);
    bool isValidMove(int row, int col) const;
    bool checkWin(char symbol) const
    {
        for (int i = 0; i < size; i++)
        {
            bool rowWin = true, colWin = true;

            for (int j = 0; j < size; j++)
            {
                if (grid[i][j] != symbol)
                    rowWin = false;
                if (grid[j][i] != symbol)
                    colWin = false;
            }

            if (rowWin || colWin)
                return true;
        }

        bool diag1 = true, diag2 = true;

        for (int i = 0; i < size; i++)
        {
            if (grid[i][i] != symbol)
                diag1 = false;
            if (grid[i][size - i - 1] != symbol)
                diag2 = false;
        }

        return diag1 || diag2;
    }

    bool isFull() const
    {
        for (auto& row : grid)
            for (auto cell : row)
                if (cell == ' ') return false;

        return true;
    }

    char getCell(int row, int col) const
    {
        return grid[row][col];
    }

    void reset()
    {
        for (auto& row : grid)
            for (auto& cell : row)
                cell = ' ';

    }
    int getSize() const
    {
        return size;
    }
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
    void showMenu()
    {
        cout << "1. Player vs Player\n";
        cout << "2. Player vs AI (Easy)\n";
        cout << "3. Player vs AI (Hard)\n";
    }

private:
    void setupPvP();
    void setupPvC(Difficulty difficulty);
    void switchPlayer();
    void handleHumanMove(Player* player);
    void handleAIMove(AIPlayer* aiPlayer);

    bool checkGameEnd()
    {
        return board.checkWin('X') || board.checkWin('O') || board.isFull();
    }

    void displayResult() const
    {
        if (board.checkWin('X')) cout << "X wins!\n";
        else if (board.checkWin('O')) cout << "O wins!\n";
        else cout << "Draw!\n";
    }

    void reset()
    {
        board.reset();
    }
};

int main()
{
    Game ticTacToe;
    ticTacToe.start();
    return 0;
}
