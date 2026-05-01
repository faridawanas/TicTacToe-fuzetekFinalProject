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
    Player(const string& name, char symbol){
        this->name = name;
        this->symbol = symbol;
    }
    virtual ~Player() {} // Good practice for inheritance

    // Pure virtual method
    virtual void getMove(int& row, int& col) = 0;

    string getName() const{
        return name;
    }
    char getSymbol() const{
        return symbol;
    }
    void setName(const string& name){
        this->name = name;
    }
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
    void setupPvP(const string& name p1_name, const string& name p2_name, char p1_sym, char p2_sym){
        player1 = new Player(p1_name, p1_sym);
        player2 = new Player(p2_name, p2_sym);
    }
    void setupPvC(Difficulty difficulty);
    void switchPlayer(){
        if(currentPlayer == nullptr)
            return;
        if(currentPlayer == player1)
            currentPlayer = player2;
        else
            currentPlayer = player1;
    }
    void handleHumanMove(Player* player);
    void handleAIMove(AIPlayer* aiPlayer);
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
