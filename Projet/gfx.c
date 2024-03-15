#include "GfxLib.h"

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Exemple FreeGLUT");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
