#include <stdio.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <GL/freeglut.h>


//region code
int INSIDE  = 0 ; //0000
int LEFT    = 1 ; //0001
int RIGHT   = 2 ; //0010
int BOTTOM  = 4 ; //0100
int TOP     = 8 ; //1000
int BOTTOM_LEFT  = 5 ; //0101
int BOTTOM_RIGHT = 6 ; //0110
int TOP_LEFT     = 9 ; //1001
int TOP_RIGHT    = 10 ; //1010


int minX, minY, maxX, maxY;
int X1, Y1, Y2, X2;
int marginX, marginY = 0 ;

int boxX1, boxX2, boxY1, boxY2;


bool isReject = false;
bool isAccept = false ;
bool clipPoint1 = false ;
bool clipPoint2 =false ;


int getCode(int x, int y){
    int code = INSIDE ;

    if (y < minY) code |= BOTTOM;
    else if (y > maxY) code |= TOP ;

    if ( x > maxX) code |= RIGHT ;
    else if (x < minX) code |= LEFT;

    return code;

}


void drawLine() {
    glLineWidth(5);

    if (isReject){
        glLineWidth(5);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
            glVertex2i(X1, Y1);
            glVertex2i(X2, Y2);
        glEnd();
        glFlush();
    }else{
        glColor3f(1.0, 1.0, 1.0);

        glBegin(GL_LINES);
            glVertex2i(boxX1, boxY1);
            glVertex2i(boxX2, boxY2);
        glEnd();
        glFlush();
    }

    if (clipPoint1){
        glLineWidth(5);
        glColor3d(1,0,0);
        glBegin(GL_LINES);
            glVertex2i(X1, Y1);
            glVertex2i(boxX1, boxY1); //intersection
        glEnd();
        glFlush();
    }
    if(clipPoint2){
        glLineWidth(5);
        glColor3d(1,0,0);

        glBegin(GL_LINES);
            glVertex2i(boxX2, boxY2); //intersection
            glVertex2i(X2, Y2);
        glEnd();
        glFlush();
    }

}

void addAxisUnt(){
    int a = minX;
    int b = minY;
    int c = maxX;
    int d = maxY;
    int posX = 0 ;
    int posY = 0 ;
    char temp ;

    //minX
    posX = minX + 30 ;
    posY = minY - 15;
    glColor3d(0.0, 0.0, 0.0);
    while(a > 0){
        int currentNum = a %10 ;
        temp = currentNum + 48;
        a /= 10 ;
        glRasterPos2d(posX, posY);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)(temp));

        //update pos
        posX -=7;
    }
    glFlush();

    //minY
    posX = minX - 15 ;
    posY = minY + 25;
    glColor3d(0.0, 0.0, 0.0);
    while(b > 0){
        int currentNum = b %10 ;
        b /= 10 ;
        temp = currentNum + 48;

        glRasterPos2d(posX, posY);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)(temp));
        //udpate pos
        posX -=7;
    }

    //maxY
    posX = minX - 15 ;
    posY = maxY - 20;
    glColor3d(0.0, 0.0, 0.0);
    while(c > 0){
        int currentNum = c %10 ;
        c /= 10 ;
        temp = currentNum + 48;

        glRasterPos2d(posX, posY);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)(temp));
        //update pos
        posX -=7;
    }



    //maxX
    posY = minY - 15;
    posX = maxX - 15 ;
    glColor3d(0.0, 0.0, 0.0);
    while(d > 0){
        int currentNum = d %10 ;
        d /= 10 ;
        temp = currentNum + 48;

        glRasterPos2d(posX, posY);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int)(temp));
        //udpate pos
        posX -=7;
    }
    glFlush();
}

void performAlgorithm(){
    int code1 = getCode(X1, Y1);
    int code2 = getCode(X2, Y2);

    bool firstRun = true ;
    if(code1 == 0){
        boxX1 = X1;
        boxY1 = Y1;
    }else if(code2 == 0){
        boxY2 = Y2;
        boxX2 = X2;
    }
    while(true){
        if ( code1 == 0 && code2 == 0 ){
            if(firstRun) {
                //trivially accept
                isAccept = true;
                boxX1 = X1;
                boxX2 = X2;
                boxY2 = Y2;
                boxY1 = Y1;
            } ;
            break;
        }else if( (code1 & code2) != 0 ){
            //trivially reject
            isReject = true ;
            boxX1 = X1;
            boxX2 = X2;
            boxY2 = Y2;
            boxY1 = Y1;
            break;
        }else{
            int testCode  ;
            float dx = X2 - X1 ;
            float dy = Y2 - Y1 ;
            float x, y ;

            if( code1 != 0){
                testCode = code1;
                boxX1 = (int)x ;
                boxY1 = (int)y ;
            }else if( code2 != 0){
                boxX2 = (int)x;
                boxY2 = (int)y ;
                testCode = code2 ;
            }

            if( testCode & TOP){
                x = X1 + (dx * (maxY - Y1) / dy);
                y = maxY;
            }else if (testCode & BOTTOM){
                x = X1 + (dx * (minY - Y1) / dy);
                y = minY;
            }else if ( testCode & RIGHT){
                y = Y1 + (dy * (maxX - X1) / dx);
                x = maxX ;
            }else if ( testCode & LEFT){
                y = Y1 + (dy * (minX - X1) / dx);
                x = minX ;
            }

            if ( testCode == code1){
                clipPoint1 = true ;
                boxX1 = (int)x ;
                boxY1 = (int)y ;
                code1 = getCode(boxX1, boxY1);
            }else {
                clipPoint2 = true ;
                boxX2 = (int)x;
                boxY2 = (int)y ;
                code2 = getCode(boxX2, boxY2);
            }
        }
        if(firstRun){
            firstRun = false ;
        }
    }
    drawLine();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    //area of interest
    glColor3f(0.0, 1.0, (64/255));
    glRecti(minX, minY, maxX, maxY);
    addAxisUnt();

    performAlgorithm();
    glFlush();
}


void initOpenGL(int argc, char ** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize((int) (maxX - minX + marginX ), (int) (maxY - minY + marginY));
    glutInitWindowPosition(150, 100);
    glutCreateWindow("Cohen Sutherland Algorithm");

    glLoadIdentity();
    gluOrtho2D((minX - marginX), (maxX + marginX), (minY - marginY), (maxY + marginY));
    glColor3f(0.0f, 0.0f, 0.0f);
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glutDisplayFunc(display);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);
    glutMainLoop();
}


void inputViewPortMin(){
    printf("%s", "Enter Minimum window coordinates(x y):");
    scanf("%d", &minX);
    scanf("%d", &minY);
}

void inputViewPortMax(){
    printf("%s","Enter Maximum window coordinates(x y):" );
    scanf("%d", &maxX);
    scanf("%d", &maxY);
}

void inputPoint1(){
    printf("%s", "Enter coordinates of first point(x y):" );
    scanf("%d", &X1);
    scanf("%d", &Y1);
}

void inputPoint2(){
    printf("%s", "Enter coordinates of second point(x y):");
    scanf("%d", &X2);
    scanf("%d", &Y2);
}

void inttialInput(){
    inputViewPortMin();
    inputViewPortMax();
    inputPoint1();
    inputPoint2();
}

void showPromptForAdjustingInput(int argc, char **argv){
    bool isContinue = true; ;
    int input =  -1;
    while(isContinue){
        printf("\n");
        printf("To adjust the parameter, enter corresponding number: \n");
        printf("1. Minimum window coordinates(x y).\n");
        printf("2. Maximum window coordinates(x y).\n");
        printf("3. Coordinates of first point(x y).\n");
        printf("4. Coordinates of second point(x y).\n");
        printf("0. Exit.\n");

        scanf("%d", &input);
        switch(input){
            case 1: inputViewPortMin(); break;
            case 2: inputViewPortMax(); break;
            case 3: inputPoint1(); break;
            case 4: inputPoint2(); break;
            case 0: isContinue = false ; continue;
            default: continue;
        }
        initOpenGL(argc, argv);
    }
}


int main(int argc, char **argv) {
    inttialInput();
    if(minX > maxX && minY > maxY){
        int temp = minX;
        minX = maxX ;
        maxX = temp;

        temp = minY;
        minY = maxY;
        maxY = temp;
    }
    marginX = (int)((maxX - minX)/ 4);
    marginY = (int)((maxY - minY)/ 4);

    initOpenGL(argc, argv);

    showPromptForAdjustingInput(argc, argv);
}