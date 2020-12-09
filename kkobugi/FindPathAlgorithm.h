#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 5
#define DONTMOVE -1
#define WALL -2
#define CLOSED -3
#define UNDEF -1
#define INF 0
int MList[10][2];
static int YC = 0;
typedef struct vertex {
  int c;
  int r;
  int g;
} VERTEX;

typedef struct queue {
  VERTEX v;
  struct queue *next;
} QUEUE;

void astar(void);
void add_openlist(VERTEX);
int calc_heuristic(VERTEX v, int c, int r, int *gx);
void enqueue(VERTEX);
VERTEX dequeue(void);
int empty_queue(void);
void print_character(void);
void print_weights(void);
int findPath(int StartX, int StartY, int EndX, int Endy, int obstaclePoint[][2], int repeat);

QUEUE *Q;
VERTEX s, e;
int g[MAX][MAX];
int visit[MAX][MAX];
int pre[MAX][MAX];

int findPath(int StartX, int StartY, int EndX, int Endy, int obstaclePoint[][2], int repeat) {
  int obc, obr;
  int obcnt = 0;
  int i;

  Serial.print("시작좌표: (y, x)>>");
  s.c = StartY;
  s.r = StartX;
  Serial.print(s.c);
  Serial.println(s.r);
  Serial.print("종점좌표: (y, x)>>");
  e.c = EndX;
  e.r = Endy;
  Serial.print(e.c);
  Serial.println(e.r);
  s.c--;
  s.r--;// 시작지점이 가상 좌표는 1이나 배열의 첫번째 인덱스는 0이다
  e.c--;
  e.r--;

  Serial.print("장애물 개수 >>");

  obcnt = repeat; //잘못된 값이 전달되면 장애물 개수는 0으로 받는다
  Serial.println(obcnt);
  for (i = 0; i < obcnt; i++) {
    obc = obstaclePoint[i][1];
    obr = obstaclePoint[i][0];
    visit[obc - 1][obr - 1] = WALL;
  }//장애물 개수만큼 선택한 좌표에 배치


  astar();
  // A* 알고리즘 사용. 깊이 탐색은 모든 곳을 끝까지 탐색하기 때문에 오래 걸리고
  // 다익스트라와 유클리드는 재퀴함수를 활용하기 때문에
  // 넓은 탐색 면적에서 메모리 할당 값이 커져 지금의 보드에서 성능 저하를 발생신킨다

  //print_weights(); //이동경로의 가중치를 확인할 때 사용할 것
  Serial.print("\n\n");
  print_character();
  MList[YC + 1][0] = StartX;
  MList[YC + 1][1] = StartY;
  for (int i = YC + 1; i > 0 ; i--) {
    Serial.print("{");
    Serial.print(MList[i][1]);
    Serial.print(", ");
    Serial.print(MList[i][0]);
    Serial.print("} ");
  }
  MList[0][0] = e.c + 1;
  MList[0][1] = e.r + 1;
  Serial.print("{");
  Serial.print(MList[0][1]);
  Serial.print(", ");

  Serial.print(MList[0][0]);
  Serial.println("} ");
  return 0;
}

/*
  print_character()
  지나온 길을 역추적하여 최단경로로 이동한 좌표를 출력 및 저장
*/
void print_character(void) {
  int i, j, backtrack;

  // 역추적을 하기 위해 경로 탐색
  i = pre[e.c][e.r] / MAX;
  j = pre[e.c][e.r] % MAX;

  // 반복문으로 이전경로를 역 추적
  while (pre[i][j] != UNDEF) {
    backtrack = pre[i][j];

    MList[YC + 1][0] = i + 1;
    MList[YC + 1][1] = j + 1;
    YC++;
    g[i][j] = 7;

    i = backtrack / MAX;
    j = backtrack % MAX;
  }

  // MAX x MAX 의 미니맵을 출력:
  for (i = 0; i < MAX; i++) {
    for (j = 0; j < MAX; j++) {
      if (i == e.c && j == e.r) {
        Serial.print("E");  //E:종착점
      } else if (i == s.c && j == s.r) {
        Serial.print("S");  //S:시작 지점
      } else if (g[i][j] == 7) {
        Serial.print("+");  //이동한 경로
      } else if (visit[i][j] == -2) {
        Serial.print("x");  //장애물
      } else {
        Serial.print("-");  //이동하지 않은. 개척된 공간
      }
    }
    Serial.println("");
  }
}

/*
  print_weights()
  각 지점의 가중치를 표식
*/
void print_weights(void) {
  int i, j;

  for (i = 0; i < MAX; i++) {
    for (j = 0; j < MAX; j++) {
      Serial.print(g[i][j]);
    }

    Serial.println("");
  }
}

/*
  empty_queue()
  큐가 비어있으면 null로 반환
*/
int empty_queue(void) {
  return Q == NULL;
}

/*
  dequeue()
  처음 저장 한 포인트를 반환
*/
VERTEX dequeue(void) {
  QUEUE *f = Q;
  VERTEX v = {0, 0, 0};

  if (f != NULL) {
    Q = f->next;

    v.c = f->v.c;
    v.r = f->v.r;
    v.g = f->v.g;

    free(f);

    return v;
  }

  return v;
}

/*
  enqueue()
  순차적으로 최적의 경로를 할당하여 현재의 노드와 다음 노드를 이어줌
*/
void enqueue(VERTEX v) {
  QUEUE *f = Q;
  QUEUE *newq = (QUEUE *)malloc(sizeof(QUEUE));
  VERTEX temp;
  int cnt = 0;
  int key;

  newq->next = NULL;
  newq->v = v;

  if (f == NULL) {
    Q = newq;
    return;
  }

  // 경로를 순차적으로 이동시키기 위해 정렬함
  while (f->next != NULL) {
    key = g[v.c][v.r];

    if (key < g[f->v.c][f->v.r]) {
      temp = f->v;
      f->v = v;
      v = temp;
    }

    f = f->next;
  }

  newq->v = v;
  f->next = newq;
}

/*
  calc_heuristic()
  이동 가능한 경로들 중 가중치 기준으로 가장 최적의 경로를 산출
  A* 탐색은{g(n)+h(n)}가 최솟값을 가지는 노드들을 확장
  g(n): 초기 상태에서 현재 노드까지의 정확한 경로 비용
  h(n): 목표 도달 비용을 절대 넘지 않는 용인된 함수
*/
int calc_heuristic(VERTEX v, int c, int r, int *gx) {
  int result;

  // h(x) 값 계산
  result = ((abs(e.c - c) + abs(e.r - r)) * 10);

  // g(x) 값 계산
  *gx = v.g;

  if (abs(v.c - c) == abs(v.r - r)) {
    *gx = *gx + 14;
  } else {
    *gx = *gx + 10;
  }

  return result + *gx;
}

/*
  add_openlist()
  대기열에 노드 인접 지점을 추가해주는 함수
*/
void add_openlist(VERTEX v) {
  VERTEX temp;
  int cnt = 0;
  int i, j, w, gx;

  // 가장 가까운 점에 있는 vertex 확인
  for (i = v.c - 1; i <= v.c + 1; i++) {
    // 지도에서 벗어난 경우
    if (i < 0 || i >= MAX) continue;

    for (j = v.r - 1; j <= v.r + 1; j++) {
      //지도에서 벗어나거나 현재 지점과 같은 경우
      if (j < 0 || j >= MAX || (i == v.c && j == v.r) ||
          visit[i][j] <= DONTMOVE)
        continue;

      // 해당 위치의 가중치 계산
      w = calc_heuristic(v, i, j, &gx);

      // 계산 된 가중치가 이전보다 작으면 대체
      if (w < g[i][j] || g[i][j] == INF) {
        g[i][j] = w;

        pre[i][j] = (v.c * MAX) + v.r;

        // 종착점에 다다르면
        if (e.c == i && e.c == j) {
          Q = NULL;
          return;
        }
      }

      temp.c = i;
      temp.r = j;
      temp.g = gx;

      enqueue(temp);
    }
  }
}

/*
  astar()
  다익스트라보다 효율적이며 8방향의 최단 경로를 파악할 수 있음
*/
void astar(void) {
  VERTEX v;

  g[s.c][s.r] = 0;        // 시작점에 가중치를 초기화
  pre[s.c][s.r] = UNDEF;  // 시작점에는 이전 루트가 없음
  s.g = 0;                // 이동횟수

  v = s;  // 시작 지점을 현재 지점으로 변환

  // 열린 목록에 인접 정점을 추가
  add_openlist(v);

  while (!empty_queue()) {
    // 닫힌 목록에 현재 정점을 추가
    visit[v.c][v.r] = CLOSED;

    // 현재 정점 갱신
    v = dequeue();

    // 인접 정점을 추가
    add_openlist(v);
  }
}
