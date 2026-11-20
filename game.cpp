#include <bits/stdc++.h>
using namespace std;

class board {
public:
  vector<vector<char>> b;
  board() : b(3, vector<char>(3, '.')) {}
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

class player {};
int main() {
  board b;
  b.print_board();
  Player p1('O');
  Player p2('X');

  // we have 9 cells max
  for (int i = 0; i < 9; i++) {
  }
}
