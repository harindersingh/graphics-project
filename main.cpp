#include <iostream>
#include <cstdio>
//glut header file
#include <GL/glut.h>
#include <time.h>
#include <cmath>
using namespace std;
GLfloat X = 0.0f; // Translate screen to x direction (left or right)
GLfloat Y = 0.0f; // Translate screen to y direction (up or down)
GLfloat Z = 0.0f; // Translate screen to z direction (zoom in or out)
GLfloat rotX = 0.0f; // Rotate screen on x axis
GLfloat rotY = 0.0f; // Rotate screen on y axis
GLfloat rotZ = 0.0f; // Rotate screen on z axis
GLfloat rotLx = 0.0f; // Translate screen by using the glulookAt function (left or right)
GLfloat rotLy = 0.0f; // Translate screen by using the glulookAt function (up or down)
GLfloat rotLz = 0.0f; // Translate screen by using the glulookAt function (zoom in or out)
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 2.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 70.0f };

void initRendering() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,1,1,1);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

void handleKeyPress(unsigned char key, int x, int y){
    //(x, y) are the current mouse coordinates
    switch(key){
        case 27: // escape key
            exit(0);
            case 'x': // Rotates screen on x axis
        rotX -= 0.5f;
        break;
        case 'X': // Opposite way
        rotX += 0.5f;
        break;
        case 'y': // Rotates screen on y axis
        rotY -= 0.5f;
        break;
        case 'Y': // Opposite way
        rotY += 0.5f;
        break;
        case 'z': // Rotates screen on z axis
        rotZ -= 0.5f;
        break;
        case 'Z': // Opposite way
        rotZ += 0.5f;
        break;
        case 'o': // Default, resets the translations vies from starting view
        case 'O':
            X = Y = 0.0f;
            Z = 0.0f;
            rotX = 0.0f;
            rotY = 0.0f;
            rotZ = 0.0f;
            rotLx = 0.0f;
            rotLy = 0.0f;
            rotLz = 0.0f;
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            gluLookAt(rotLx, rotLy, 0.5f + rotLz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
            break;

    }
    glutPostRedisplay();
}

// called on special key pressed
void specialKey(int key, int x, int y) {
// The keys below are using the gluLookAt() function for navigation
// Check which key is pressed
    switch(key) {
        case GLUT_KEY_LEFT : // Rotate on x axis
        X -= 0.1f;
        break;
        case GLUT_KEY_RIGHT : // Rotate on x axis (opposite)
        X += 0.1f;
        break;
        case GLUT_KEY_UP : // Rotate on y axis
        Y += 0.1f;
        break;
        case GLUT_KEY_DOWN : // Rotate on y axis (opposite)
        Y -= 0.1f;
        break;
        case GLUT_KEY_PAGE_UP: // Rotate on z axis
        Z -= 0.1f;
        break;
        case GLUT_KEY_PAGE_DOWN:// Rotate on z axis (opposite)
        Z += 0.1f;
        break;
    }
    glutPostRedisplay(); // Redraw the scene
}


void handleresize(int width, int height){
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt (rotLx, rotLy, 0.5f + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void draw_sphere(GLfloat x, GLfloat y, GLfloat z, GLdouble _radius, GLint _slices, GLint _stacks){
    glPushMatrix();
        glTranslated(x, y, z);
        glutSolidSphere(_radius, _slices, _stacks);
    glPopMatrix();
}

void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
    float vx = x2-x1;
    float vy = y2-y1;
    float vz = z2-z1;

    //handle the degenerate case of z1 == z2 with an approximation
    if(vz == 0)
        vz = .0001;

    float v = sqrt( vx*vx + vy*vy + vz*vz );
    float ax = 57.2957795*acos( vz/v );
    if ( vz < 0.0 )
        ax = -ax;
    float rx = -vy*vz;
    float ry = vx*vz;
    glPushMatrix();

    //draw the cylinder body
    glTranslatef( x1,y1,z1 );
    glRotatef(ax, rx, ry, 0.0);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, v, subdivisions, 50);

    //draw the first cap
    gluQuadricOrientation(quadric,GLU_INSIDE);
    gluDisk( quadric, 0.0, radius, subdivisions, 1);
    glTranslatef( 0,0,v );

    //draw the second cap
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluDisk( quadric, 0.0, radius, subdivisions, 1);
    glPopMatrix();
}
void renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
    //the same quadric can be re-used for drawing many cylinders
    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
    gluDeleteQuadric(quadric);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glColor4f(0.9, 0.9, 0.0, 0.1);
    glRotatef(rotX,1.0,0.0,0.0); // Rotate on x
    glRotatef(rotY,0.0,1.0,0.0); // Rotate on y
    glRotatef(rotZ,0.0,0.0,1.0); // Rotate on z
    glTranslatef(X, Y, Z); 	// Translates the screen left or right,
    //six points up
    draw_sphere(0.0, 0.6, -3, 0.6, 50, 50);
    //six points down
    draw_sphere(0.0, -0.6, -3, 0.6, 50, 50);

    renderCylinder_convenient(0.0, 0.6, -3, 0.0, -0.6, -3, 0.6, 50);

    //body is drawn ... moving on to the legs
    //minion's left leg
    renderCylinder_convenient(-0.3, -1.05, -3, -0.3, -1.4, -3, 0.09, 50);
    draw_sphere(-0.3, -1.4, -3, 0.09, 50, 50);
    //minion's right leg
    renderCylinder_convenient(0.3, -1.05, -3, 0.3, -1.4, -3, 0.09, 50);
    draw_sphere(0.3, -1.4, -3, 0.09, 50, 50);

    //minion's left arm

    //minion's right arm
    glPopMatrix();

    glutSwapBuffers();
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitWindowSize(720,540);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GRAPHICS PROJECT");
    initRendering();
    glutReshapeFunc(handleresize);
    glutKeyboardFunc(handleKeyPress);
    glutSpecialFunc(specialKey); // set window's to specialKey callback
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
