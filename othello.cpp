#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define all(x) (x).begin(),(x).end()
/*大事なこと
  自分のコマの色は白(@)
  白の値は1
  */
//定数を決める
//無限
int inf = 2147483647;
//マスの状態
const int EMPTY = -1;  //空きマス
const int WHITE = 1; // 白石
const int BLACK = 0;  // 黒石
const int board_size = 8;//盤面のサイズ
//向きを表す
const vector<int> dy = {-1,-1,0,1,1,-1,0,-1};
const vector<int> dx = {0,1,1,1,0,-1,-1,-1};
//マスの評価を計算する為の配列
vector<vector<int>> weight={
  {30, -12, 0, -1, -1, 0, -12, 30},
  {-12, -15, -3, -3, -3, -3, -15, -12},
  {0, -3, 0, -1, -1, 0, -3, 0},
  {-1, -3, -1, -1, -1, -1, -3, -1},
  {-1, -3, -1, -1, -1, -1, -3, -1},
  {0, -3, 0, -1, -1, 0, -3, 0},
  {-12, -15, -3, -3, -3, -3, -15, -12},
  {30, -12, 0, -1, -1, 0, -12, 30}
};
//盤面
//vector<vector<int>> board(board_size,vector<int>(board_size,0));
//マスに置くことができるかどうかを判定する変数
bool enable = false;
//値を受け取り、ボードの初期化を行う関数
void reset_board(vector<vector<int>> &board){
  string t;
  cin >> t;
  if (t=="PLACE")enable=true;
  else return;
  rep(i,board_size){
    string s;
    cin >> s;
    rep(j,board_size){
      if (s.at(j) == '#'){
        board[i][j] = BLACK;
      }else if (s.at(j) == '@'){
        board[i][j] = WHITE;
      }else if (s.at(j) == '.'){
        board[i][j] = EMPTY;
      }
    }
  }
}
//test用　盤面を表示する関数
void show_board(vector<vector<int>> &board){
  rep(i,board_size){
    rep(j,board_size){
      switch(board[i][j]){
        case WHITE:
          cout << '@';
          break;
        case BLACK:
          cout << '#';
          break;
        case EMPTY:
          cout << '.';
          break;
        default:
          break;
      }
    }
    cout << endl;
  }
}
/*
   どこかのマスに置けることはわかっているが、どのマスに置けるかどうかが
   まだわからないので探索する必要がある。どのように値を保管するかが問題(探索自体はそれほど問題では無い)
   */
//指定された方向に、ひっくり返すことができるかどうか確認する関数
bool check_d(int y,int x,int d,vector<vector<int>> &board){
  int flag = 0;
  while(true){
    x += dx[d];
    y += dy[d];

    //盤面の外に出たら終了する
    if (x < 0 || y < 0 || x > board_size -1|| y > board_size -1){
      return false;
    }
    //指定したますが空だった場合終了する
    if(board[y][x] == EMPTY){
      return false;
    }
    //相手のコマがあったらフラグを立てる
    if(board[y][x] ==  BLACK){
      flag=true;
      continue;
    }
    if (flag) break;
    return false;
  }
  return true;
}
// check_dでどの方向に置くことができるかどうかがわかるので、ひっくり返すことができるかどうかを判別する関数
bool can_put(int y,int x,vector<vector<int>> &board){
  rep(d,board_size){
    if(check_d(y,x,d,board)) return true;
  }
  return false;
}
//実際にマスを裏返す関数
void trun_over(int y, int x, int d,vector<vector<int>> &board){
  while(true){
    x+=dx[d];
    y+=dy[d];
    //自分のコマがあるまで探索を続ける
    if (board[y][x]==WHITE) break;
    //自分のコマじゃなかった場合裏返す。
    board[y][x]=WHITE;
  }
}
//実際にマスをおく関数
bool put(int y,int x,vector<vector<int>> &board){
  bool flag=false;
  //空白でなかったら終了
  if(board[y][x] != EMPTY) return 0;

  //全方向に裏返す
  rep(d,board_size){
    if(check_d(y,x,d,board)){
      trun_over(y,x,d,board);
      flag=true;
    }
  }
  if (flag){
    board[y][x] = WHITE;
    return true;
  }
  return false;
}
//1手先を読んで手を打つ関数を作成する。

//白から見た現在の盤面の評価値を計算する関数
int  evaluate(vector<vector<int>> &board){
  int sum = 0;
  rep(i,board_size){
    rep(j,board_size){
      if(board[i][j]==WHITE){
        sum+=weight[i][j];
      }
    }
  }
  return sum;
}
//上の関数で計算した値を元に、次に打つ手を考える関数
//main関数
int main(){
  int ansx = 1;
  int ansy = 1;
  while(true){
    vector<vector<int>> field(board_size,vector<int>(board_size,0));
    //値を受け取りボードの初期化を行う。
    reset_board(field);
    //TがPLACEじゃなかった場合以下の処理は行わない
    if(enable){
      int max_eval = -100100;
      rep(i,board_size){
        rep(j,board_size){
          vector<vector<int>> copy_field = field;
          if(can_put(i,j,copy_field)&&copy_field[i][j]==EMPTY){
            put(i,j,copy_field);
            int eval = evaluate(copy_field);
            if(eval>max_eval){
              ansx=j+1;
              ansy=i+1;
              max_eval=eval;
            }
          }
        }
      }
    }
    vector<vector<int>> copy_field = field;
    //put(ansy-1,ansx-1,copy_field);
    cout<<ansx<<" "<<ansy<<endl;
    //show_board(copy_field);
  }
}
