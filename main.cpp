#include <iostream>
#include <cstdio>
//glut header file
#include <GL/glut.h>
#include <time.h>
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

void initRendering() {
    glClearColor(1,1,1,1);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    /*glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); */
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

static void handleresize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt (rotLx, rotLy, 0.5f + rotLz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

static void draw_sphere(GLfloat x, GLfloat y, GLfloat z, GLdouble _radius, GLint _slices, GLint _stacks){
    glPushMatrix();
        glTranslated(x, y, z);
        glutSolidSphere(_radius, _slices, _stacks);
    glPopMatrix();

}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glColor3d(1,0,0);
    glRotatef(rotX,1.0,0.0,0.0); // Rotate on x
    glRotatef(rotY,0.0,1.0,0.0); // Rotate on y
    glRotatef(rotZ,0.0,0.0,1.0); // Rotate on z
    glTranslatef(X, Y, Z); 	// Translates the screen left or right,

    glTranslated(X, Y, Z);
    //six points up
    draw_sphere(0.0, 0.6, -6, 0.6, 50, 50);
    //six points down
    draw_sphere(0.0, -0.6, -6, 0.6, 50, 50);
    glPopMatrix();
    glutSwapBuffers();
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

    glutMainLoop();
    return 0;
}
