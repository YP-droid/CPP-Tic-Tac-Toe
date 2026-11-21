#include <bits/stdc++.h>
using namespace std;

class Board {
public:
  vector<vector<char>> b;
  Board() : b(3, vector<char>(3, '.')) {}
  void print_board() {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        cout << b[i][j];
      }
      cout << '\n';
    }
  }
};

class Player {
  // player will have symbols associated to them
public:
  char symbol;
  Player(char c) : symbol(c) {}
};

// prevent duplicate
bool dupe(int num, vector<vector<char>> &v) {
  int i = (num - 1) / 3;
  int j = (num - 1) % 3;

  return v[i][j] != '.'; // duplicate
}

bool win_check(int i, int j, Board &board) {

  // check row

  int count = 0;
  for (int x = 0; x < 3; x++)
    if (board.b[i][x] == board.b[i][j])
      count++;

  if (count == 3)
    return 1;

  // check column

  count = 0;
  for (int x = 0; x < 3; x++)
    if (board.b[x][j] == board.b[i][j])
      count++;

  if (count == 3)
    return 1;

  // diagonals check only if at (1,1)

  // right diagonal start at (2,0)
  count = 0;
  int r_i = 2, r_j = 0;
  for (int x = 0; x < 3; x++) {
    if (board.b[r_i][r_j] == board.b[i][j])
      count++;
    --r_i;
    ++r_j;
  }

  if (count == 3)
    return 1;

  // left diagonal, start at (0,0)
  count = 0;
  r_i = 0;
  r_j = 0;
  for (int x = 0; x < 3; x++) {
    if (board.b[r_i][r_j] == board.b[i][j])
      count++;
    ++r_i;
    ++r_j;
  }

  return count == 3;
}

// check if computer can win in 1 move
pair<int, int> winning(Board &board, char c) {
  // check all row,column , diagonal to see if computer can win
  // if a cell contains c, do count++, if it is players char do count--

  int count = 0;
  int column;
  // row
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board.b[i][j] == c)
        count++;
      else if (board.b[i][j] != '.')
        count--;
      else if (board.b[i][j] == '.')
        column = j;
    }
    if (count == 2)
      return {i, column};
  }

  count = 0;
  int row;
  // column
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
      if (board.b[i][j] == c)
        count++;
      else if (board.b[i][j] != '.')
        count--;
      else if (board.b[i][j] == '.')
        row = i;
    }
    if (count == 2)
      return {row, j};
  }

  // left diagonal
  count = 0;
  row = 0;
  for (int i = 0; i < 3; i++) {
    if (board.b[i][i] == c)
      count++;
    else if (board.b[i][i] != '.')
      count--;
    else
      row = i;
  }

  if (count == 2)
    return {row, row};

  // right diagonal
  count = 0;
  row = 2;
  for (int i = 0; i < 3; i++) {
    if (board.b[i][i] == c)
      count++;
    else if (board.b[i][i] != '.')
      count--;
    else
      column = i;
  }

  // find row
  (column == 0) ? row = 2 : (column == 1) ? row = 1 : row = 0;
  if (count == 2)
    return {row, column};

  return {-1, -1}; // not winning right now
}

// computer turn
pair<int, int> smart_move(Board &board, int n, char c, char p) {
  if (n == 0)
    return {1, 1}; // pick center if going first
  // check if computer winning in one move, if yes , play that move
  pair<int, int> comp = winning(board, c);
  if (comp.first != -1) {
    return comp;
  }

  // check if player in winning in one move, if yes, mark at that position

  pair<int, int> pl = winning(board, p);
  if (pl.first != -1)
    return pl;

  // else we can play any move, prefer corners first

  pair<int, int> corner = (board.b[0][0] == '.')   ? pair<int, int>{0, 0}
                          : (board.b[0][2] == '.') ? pair<int, int>{0, 2}
                          : (board.b[2][0] == '.') ? pair<int, int>{2, 0}
                          : (board.b[2][2] == '.') ? pair<int, int>{2, 2}
                                                   : pair<int, int>{-1, -1};
  if (corner.first != -1)
    return corner;

  // no corners left, pick the first emmpty cell we can find

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board.b[i][j] == '.')
        return {i, j};
    }
  }
  return {-1, -1};
}

int main() {
  Board board;
  board.print_board();
  // choose any character but .

  cout << "Pick 0 (vs friend) or 1 (vs computer)" << '\n';
  int choice;
  cin >> choice;
  cout << "Enter 2 characters : " << '\n';
  char c1, c2;
  cin >> c1 >> c2;

  Player p1(c1);
  Player p2(c2);
  // we have 9 cells max
  /*
   * 1 2 3
   * 4 5 6
   * 7 8 9
   *
   * we are mapping i,j to numbers , like 1,1 to 5
   * formula i * 3 + j + 1 = number
   * i = (num-1) / 3
   * j = (num-1) % 3
   */

  int turn = 0; // 0 for player 1, 1 for player 2
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      // take a number as i/p from user from 1 to 9

      if (!choice) {
        int n;
        cin >> n;
        cout << '\n';
        // not a valid number
        if (n < 1 or n > 9) {
          cout << "Pick a number from 1 to 9" << '\n';
          --j; // give another chance
          continue;
        }
        // if number is valid, check if it has been already used
        if (dupe(n, board.b)) {
          cout << "Can't mark here" << '\n';
          j--;
          continue;
        }

        // Mark
        int x = (n - 1) / 3;
        int y = (n - 1) % 3;

        board.b[x][y] = (turn == 0) ? c1 : c2;
        board.print_board();

        // check if the game is won
        if (win_check(x, y, board)) {
          (turn == 0) ? cout << "Player1 won" : cout << "Player2 won" << '\n';
          goto done;
        }
      }
      cout << '\n';
      turn = !turn;
    }
  }
done:
  cout << "Thanks for playing" << '\n';
}
