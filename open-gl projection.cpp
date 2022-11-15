#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif
#include <iostream>
#include <vector>

#define pnt vector<float>
#define mtx vector<pnt>

using namespace std;

GLfloat theta=0;
void spin(){
    theta+=0.05;
    if(theta > 360) theta = 0;
    glutPostRedisplay();
}

mtx cube={{0.0, 0.2, 0.2,1},
          { 0.2, 0.2, 0.2,1},
          { 0.2,0.0, 0.2,1},
          {0.0,0.0, 0.2,1},
          {0.0, 0.2,0.0,1},
          { 0.2, 0.2,0.0,1},
          { 0.2,0.0,0.0,1},
          {0.0,0.0,0.0,1}
          };

int type;
float x,y,z,x0,y0,z0;
float n1,n2,n3;

void MyInit(char* s){
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(1000,1000);
  glutCreateWindow(s);
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  //gluOrtho2D (-500,500,-500,500);
  glClearColor(1.0,1.0,1.0,0);
  glColor3f(0,0,0);
  glEnable(GL_DEPTH_TEST);
}

mtx multiply(mtx a, mtx b){
    int n=a.size();
    int m=b[0].size();
    mtx c(n,pnt(m,0));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            for (int k=0;k<4;k++)
                c[i][j]+=a[i][k]*b[k][j];
    return c;
}

void face(mtx sqr){
    glBegin(GL_LINE_LOOP);
        for(int i=0;i<4;i++)
            glVertex3f(sqr[i][0]/sqr[i][3],sqr[i][1]/sqr[i][3],sqr[i][2]/sqr[i][3]);
    glEnd();
};

void drawcube(mtx c){
    face({c[0],c[1],c[2],c[3]});
    face({c[1],c[5],c[6],c[2]});
    face({c[0],c[4],c[5],c[1]});
    face({c[4],c[0],c[3],c[7]});
    face({c[7],c[6],c[2],c[3]});
    face({c[4],c[5],c[6],c[7]});
}

void perspective(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(30,1,0,0);
    glRotatef(theta,0,-1,0);
    glColor3f(0,0,0);
    glBegin(GL_POINTS);
        glVertex3f(x,y,z);
        glVertex3f(x0,y0,z0);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINES);
        glVertex3f(0,2,0);
        glVertex3f(0,-2,0);
        glVertex3f(2,0,0);
        glVertex3f(-2,0,0);
        glVertex3f(0,0,2);
        glVertex3f(0,0,-2);
    glEnd();

    drawcube(cube);
    glColor3f(1,0,0);

    float d0=n1*x0+n2*y0+n3*z0;
    float d1=n1*x +n2*y +n3*z;
    float d=d0-d1;

    // printf("%f %f %f\n",n1,n2,n3);

    // printf("%f %f %f\n",x,y,z);

    // printf("%f %f %f\n",x0,y0,z0);

    // printf("%f %f %f\n",d,d0,d1);

    mtx M={{d+x*n1,x*n2,x*n3,-d0*x},
           {y*n1,d+y*n2,y*n3,-d0*y},
           {z*n1,z*n2,d+z*n3,-d0*z},
           {n1,   n2,   n3,  -d1  }};
    // cout<<M;

    mtx cube2,t;
    for(auto x: cube){
        t=multiply(M,{{x[0]},{x[1]},{x[2]},{x[3]}});
        cube2.push_back({t[0][0],t[1][0],t[2][0],t[3][0]});
    }
    drawcube(cube2);
    // cout<<cube2;
    glutSwapBuffers();
}

void parallel(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(30,1,0,0);
    glRotatef(theta,0,-1,0);
    glColor3f(0,0,0);
    glBegin(GL_POINTS);
        glVertex3f(x,y,z);
        glVertex3f(x0,y0,z0);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINES);
        glVertex3f(0,2,0);
        glVertex3f(0,-2,0);
        glVertex3f(2,0,0);
        glVertex3f(-2,0,0);
        glVertex3f(0,0,2);
        glVertex3f(0,0,-2);
    glEnd();

    drawcube(cube);
    glColor3f(1,0,0);

    float d0=n1*x0+n2*y0+n3*z0;
    float d1=n1*x +n2*y +n3*z;
    float d=d1;

    // printf("%f %f %f\n",n1,n2,n3);

    // printf("%f %f %f\n",x,y,z);

    // printf("%f %f %f\n",x0,y0,z0);

    // printf("%f %f %f\n",d,d0,d1);

    mtx M={{d-x*n1,-x*n2,-x*n3,d0*x},
           {-y*n1,d-y*n2,-y*n3,d0*y},
           {-z*n1,-z*n2,d-z*n3,d0*z},
           {    0,    0,     0,d1  }};
    // cout<<M;

    mtx cube2,t;
    for(auto x: cube){
        t=multiply(M,{{x[0]},{x[1]},{x[2]},{x[3]}});
        cube2.push_back({t[0][0],t[1][0],t[2][0],t[3][0]});
    }
    drawcube(cube2);
    // cout<<cube2;
    glutSwapBuffers();
}

int main(int argc, char* argv[]){
    glutInit(&argc,argv);
    cout<<"Select Projection:\n1. Perspective Projection\n2. Parallel Projection\nEnter -> ";
    cin>>type;
    switch(type){
        case 1:
            cout<<"Enter COP (Center of projection) (x, y, z): ";
            cin>>x>>y>>z;
            cout<<"Enter a Point on plane of projection (x, y, z): ";
            cin>>x0>>y0>>z0;
            cout<<"Enter normal vector to the plane of projection (n1, n2, n3): ";
            cin>>n1>>n2>>n3;

            //x=-0.6,y=-0.5,z=-0.7;
            //x0=-0.2,y0=-0.3,z0=-0.2;
            //n1=3,n2=4,n3=5;

            MyInit("Perspective Projection");
            glutDisplayFunc(perspective);
            break;
        case 2:
            cout<<"Enter vector in the direction of projection (x, y, z): ";
            cin>>x>>y>>z;
            cout<<"Enter a Point on plane of projection (x, y, z): ";
            cin>>x0>>y0>>z0;
            cout<<"Enter normal vector to the plane of projection (n1, n2, n3): ";
            cin>>n1>>n2>>n3;

            x=1,y=0.8,z=1.2;
            x0=-0.2,y0=-0.3,z0=-0.2;
            n1=3,n2=4,n3=5;

            MyInit("Parallel Projection");
            glutDisplayFunc(parallel);
            break;
        default:
            break;
    }
    glutIdleFunc(spin);
    glutMainLoop();
    return 0;
}
