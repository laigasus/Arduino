char NE = 'X';
char NW = 'Z';
char SW = 'V';
char SEA = 'Y';
// 이동함수 구현 내용.
void L_O() {
  Serial.println("좌회전");
  maneuver(-200, 200, 485);
  maneuver(200, 200, 1075);
  //maneuver(200, 200, 905);
}
void F_L() {
  Serial.println("전진+좌");
  maneuver(-200, 200, 300);
  maneuver(200, 200, 1075);
  //maneuver(200, 200, 1250);
}

void F_O() {
  Serial.println("전진");
  //maneuver(200, 200, 905);
  maneuver(200, 200, 1075);
}

void F_R() {
  Serial.println("전진+우");
  maneuver(200, -200, 300);
  //maneuver(200, 200, 1250);
  maneuver(200, 200, 1075);
}

void R_O() {
  Serial.println("우회전");
  maneuver(200, -200, 485);
  //maneuver(200, 200, 905);
  maneuver(200, 200, 1075);
}

void moving() {

  char lookDirection = 'S';
  for (int i = YC + 1; i > 0; i--) {
    switch (lookDirection)
    {
      case 'N': {
          if ( (MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            L_O();
            lookDirection = 'W';
          } else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_L();
            lookDirection = NW;
          } else if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_O();
            lookDirection = 'N';
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_R();
            lookDirection = NE;
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            R_O();
            lookDirection = 'E';
          }
        }
        break;

      case 'X': {
          if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            L_O();
            lookDirection = NW;
          } else if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_L();
            lookDirection = 'N';
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_O();
            lookDirection = NE;
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            F_R();
            lookDirection = 'E';
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            R_O();
            lookDirection = SEA;
          }
        } break;

      case 'E': {
          if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            L_O();
            lookDirection = 'N';
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_L();
            lookDirection = NE;
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            F_O();
            lookDirection = 'E';
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_R();
            lookDirection = SEA;
          } else if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            R_O();
            lookDirection = 'S';
          }
        }
        break;

      case 'Y': {
          if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            L_O();
            lookDirection = NE;
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            F_L();
            lookDirection = 'E';
          } else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_O();
            lookDirection = SEA;
          } else if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_R();
            lookDirection = 'S';
          } else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            R_O();
            lookDirection = SW;
          }
        } break;
      case 'S': {
          if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            L_O();
            lookDirection = 'E';
          }
          else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_L();
            lookDirection = SEA;
          }
          else if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_O();
            lookDirection = 'S';
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_R();
            lookDirection = SW;
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            R_O();
            lookDirection = 'W';
          }
        }
        break;

      case 'V': {
          if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            L_O();
            lookDirection = SEA;
          }
          else if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_L();
            lookDirection = 'S';
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_O();
            lookDirection = SW;
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            F_R();
            lookDirection = 'W';
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            R_O();
            lookDirection = NW;
          }
        } break;

      case 'W': {
          if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            L_O();
            lookDirection = 'S';
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            F_L();
            lookDirection = SW;
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            F_O();
            lookDirection = 'W';
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_R();
            lookDirection = NW;
          }
          else if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            R_O();
            lookDirection = 'N';
          }
        } break;

      case 'Z': {
          if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) > 0) {
            L_O();
            lookDirection = SW;
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) == 0) {
            F_L();
            lookDirection = 'W';
          }
          else if ((MList[i - 1][1] - MList[i][1]) < 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_O();
            lookDirection = NW;
          }
          else if ((MList[i - 1][1] - MList[i][1]) == 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            F_R();
            lookDirection = 'N';
          }
          else if ((MList[i - 1][1] - MList[i][1]) > 0 && (MList[i - 1][0] - MList[i][0]) < 0) {
            R_O();
            lookDirection = NE;
          }
        } break;

      default:
        Serial.println("잘못된 경로값!");
    }
    Serial.print("이동후 현재 좌표 위치");
    Serial.print("x : ");
    Serial.print(MList[i][1]);
    Serial.print(" y : ");
    Serial.println(MList[i][0]);
    maneuver(0, 0, 100); //행동구분
  }

}
