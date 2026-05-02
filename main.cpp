#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

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
    Player(const string& name, char symbol)
    {
        this->name = name;
        this->symbol = symbol;
    }
    virtual ~Player() {} // Good practice for inheritance

    // Pure virtual method
    virtual void getMove(const Board& board, int& row, int& col) = 0;

    string getName() const
    {
        return name;
    }
    char getSymbol() const
    {
        return symbol;
    }
    void setName(const string& name)
    {
        this->name = name;
    }
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(const string& name, char symbol)
        : Player(name, symbol) {}

    void getMove(const Board& board, int& row, int& col) override
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
    AIPlayer(const string& name, char symbol, Difficulty difficulty):Player(name,symbol)
    {
        setDifficulty(difficulty);
    };

    void getMove(const Board& board, int& row, int& col) override
    {
        if (difficulty == EASY)
            getRandomMove(board, row, col);
        else
            getBestMove(board, row, col);
    }
    void setDifficulty(Difficulty newDifficulty)
    {
        difficulty = newDifficulty;

    };

private:
    void getRandomMove(const Board& board, int& row, int& col) const
    {

        row=(rand()%3);
        col=(rand()%3);
        while(!board.isValidMove(row,col))
        {
            row=(rand()%3);
            col=(rand()%3);
        };

    };

    int minimax(Board b, bool isMax)
    {

        int score = evaluateBoard(b);

        if (score == 10 || score == -10 || b.isFull())
            return score;

        if (isMax)
        {
            int best = -1000;

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {

                    if (b.isValidMove(i, j))
                    {
                        Board newBoard = b;
                        newBoard.makeMove(i, j, symbol);

                        best = max(best, minimax(newBoard, false));
                    }
                }
            }

            return best;
        }
        else
        {
            int best = 1000;
            char opp = (symbol == 'X') ? 'O' : 'X';

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {

                    if (b.isValidMove(i, j))
                    {
                        Board newBoard = b;
                        newBoard.makeMove(i, j, opp);

                        best = min(best, minimax(newBoard, true));
                    }
                }
            }

            return best;
        }
    }
    void getBestMove(const Board& board, int& row, int& col)
    {

        int bestVal = -1000;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {

                if (board.isValidMove(i, j))
                {

                    Board newBoard = board;
                    newBoard.makeMove(i, j, symbol);

                    int moveVal = minimax(newBoard, false);

                    if (moveVal > bestVal)
                    {
                        bestVal = moveVal;
                        row = i;
                        col = j;
                    }
                }
            }
        }
    }



    int evaluateBoard(const Board& board) const
    {
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
    Game(): board(3), player1(nullptr), player2(nullptr), currentPlayer(nullptr) {}
    ~Game()
    {
        delete player1;
        delete player2;
    }

    void start();
    void showMenu()
    {
        cout << "1. Player vs Player\n";
        cout << "2. Player vs AI (Easy)\n";
        cout << "3. Player vs AI (Hard)\n";
    }

private:
    void setupPvP(const string& p1_name, const string& p2_name, char p1_sym, char p2_sym)
    {
        player1 = new HumanPlayer(p1_name, p1_sym);
        player2 = new HumanPlayer(p2_name, p2_sym);
    }
    void setupPvC(Difficulty difficulty)
    {
        string name;
        cout << "Enter your name: ";
        cin >> name;

        player1 = new HumanPlayer(name, 'X');
        player2 = new AIPlayer("Computer", 'O', difficulty);
    }
    void switchPlayer()
    {
        if(currentPlayer == nullptr)
            return;
        if(currentPlayer == player1)
            currentPlayer = player2;
        else
            currentPlayer = player1;
    }
    void handleHumanMove(Player* player);
    void handleAIMove(AIPlayer* aiPlayer)
    {

        int r, c;

        while (true)
        {
            aiPlayer->getMove(board, r, c);

            if (board.makeMove(r, c, aiPlayer->getSymbol()))
                break;
        }

    }

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


void Game::handleHumanMove(Player* player)
{
    int row, col;

    while (true)
    {
        cout << "Enter row and column (1-" << board.getSize() << "): ";

        if (!(cin >> row >> col))
        {
            cout << "Invalid input! Please enter numbers.\n";

            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        row--;
        col--;

        if (!board.isValidMove(row, col))
        {
            cout << "Invalid move! Try again.\n";
            continue;
        }
        board.makeMove(row, col, player->getSymbol());
        break;
    }
}


void Game::start()
{
    int choice;
    showMenu();
    cin >> choice;

    if (choice == 1)
    {
        string name1, name2;
        cout << "Enter Player name(symbol = 'o'): ";
        cin >> name1;
        cout << "Enter Player name(symbol = 'x'): ";
        cin >> name2;
        setupPvP(name1, name2, 'O', 'X');
    }
    else if (choice == 2) setupPvC(EASY);
    else setupPvC(HARD);

    currentPlayer = player1;

    while (true)
    {
        board.display();

        if (dynamic_cast<AIPlayer*>(currentPlayer))
        {
            cout << currentPlayer->getName()
                 << " (" << currentPlayer->getSymbol() << ") is thinking...\n";

            handleAIMove((AIPlayer*)currentPlayer);
        }
        else
        {
            cout << "Enter row and column (1-3): " << endl;
            cout << currentPlayer->getName()
                 << " (" << currentPlayer->getSymbol() << ") turn: ";

            handleHumanMove(currentPlayer);
        }

        if (checkGameEnd())
        {
            board.display();
            displayResult();
            break;
        }

        switchPlayer();
    }
}

int main()
{
    Game ticTacToe;
    ticTacToe.start();
    return 0;
}
