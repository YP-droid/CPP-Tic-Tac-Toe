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

  if ((i + j) % 2 == 0) {
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
  }

  return count == 3;
}

int evaluate(Board &board, char c1, char c2) {
  int count1 = 0, count2 = 0;
  // row
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board.b[i][j] == c1) {
        count1++;
      }
      if (board.b[i][j] == c2) {
        count2++;
      }
    }
    if (count1 == 3)
      return 10;
    if (count2 == 3)
      return -10;
    count1 = 0, count2 = 0;
  }
  // column
  count1 = 0, count2 = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board.b[j][i] == c1) {
        count1++;
      }
      if (board.b[j][i] == c2) {
        count2++;
      }
    }
    if (count1 == 3)
      return 10;
    if (count2 == 3)
      return -10;
    count1 = 0;
    count2 = 0;
  }

  // left diagonal
  count1 = 0, count2 = 0;
  for (int i = 0; i < 3; i++) {
    if (board.b[i][i] == c1)
      count1++;
    if (board.b[i][i] == c2)
      count2++;
  }
  if (count1 == 3)
    return 10;
  if (count2 == 3)
    return -10;

  // right diagonal
  count1 = 0;
  count2 = 0;
  int row = 2;
  for (int i = 0; i < 3; i++) {
    if (board.b[row][i] == c1)
      count1++;
    if (board.b[row][i] == c2)
      count2++;
    row--;
  }
  if (count1 == 3)
    return 10;
  if (count2 == 3)
    return -10;

  return 0;
}

bool isfull(Board &board) {
  int full = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board.b[i][j] != '.') {
        full++;
      }
    }
  }
  return full == 9;
}

// computer turn
int minimax(Board &board, bool isMaximizing, char c1, char c2) {
  int score =
      evaluate(board, c1, c2); // +10 if X won, -10 if O won, 0 otherwise
  if (score != 0)
    return score; // someone won, stop here
  if (isfull(board))
    return 0;

  if (isMaximizing) {
    int best = INT_MIN;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board.b[i][j] == '.') {
          board.b[i][j] = c1;
          best = max(best, minimax(board, false, c1, c2));
          board.b[i][j] = '.';
        }
      }
    }
    return best;
  } else {
    int best = INT_MAX;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (board.b[i][j] == '.') {
          board.b[i][j] = c2;
          best = min(best, minimax(board, true, c1, c2));
          board.b[i][j] = '.';
        }
      }
    }
    return best;
  }
}

pair<int, int> bestmove(Board &board, char c, char p) {
  int val = INT_MIN;
  int row = 0, col = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board.b[i][j] == '.') {
        board.b[i][j] = c;
        int x = minimax(board, false, c, p);
        board.b[i][j] = '.';
        if (x > val) {
          val = x;
          row = i;
          col = j;
        }
      }
    }
  }
  return {row, col};
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

  bool won = 0; // just see if somebody won or not
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
          won = 1;
          (turn == 0) ? cout << "Player1 won" << '\n'
                      : cout << "Player2 won" << '\n';
          goto done;
          cout << '\n';
        }
        turn = !turn;
      } else {
        if (!turn) {
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

          turn = !turn;
          board.b[x][y] = c1;
          board.print_board();
          if (win_check(x, y, board)) {
            won = 1;
            cout << "Player1 won" << '\n';
            goto done;
            cout << '\n';
          }
          cout << '\n';
        } else {
          // comp
          auto [x, y] = bestmove(board, c2, c1);
          board.b[x][y] = c2;
          board.print_board();
          if (win_check(x, y, board)) {
            won = 1;
            cout << "computer won" << '\n';
            goto done;
            cout << '\n';
          }
          cout << '\n';
          turn = !turn;
        }
      }
    }
  }
done:
  if (!won)
    cout << "DRAW !" << '\n';
  cout << "Thanks for playing" << '\n';
}
