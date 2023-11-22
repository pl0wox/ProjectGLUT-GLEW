#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>

GLuint cloudVBO;  // for clouds using vbo
float cloud1X = -0.8; // cloud 1 x coor
float cloud2X = -0.5; // cloud 2 x coor
float cloud3X = 0.5;  // cloud 3 x coor
float cloud4X = 0.3;  // cloud 4 x coor
float cloud5X = 0.4;  // cloud 5 x coor


const GLfloat cloudVertices[] = {
    -0.1, 0.1, 0.0,
    0.0, 0.2, 0.0,
    0.2, 0.2, 0.0,
    0.3, 0.1, 0.0,
    0.2, 0.0, 0.0,
    0.0, 0.0, 0.0
};


void createCloudVBO() {
    GLfloat cloudVertices[] = {
        -0.1, 0.1, 0.0,
        0.0, 0.2, 0.0,
        0.2, 0.2, 0.0,
        0.3, 0.1, 0.0,
        0.2, 0.0, 0.0,
        0.0, 0.0, 0.0
    };

    glGenBuffers(1, &cloudVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cloudVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cloudVertices), cloudVertices, GL_STATIC_DRAW);
}

void iceKingMountain() {
    glBegin(GL_TRIANGLES); // ice kings castle
    glColor3ub(72, 202, 228);
    glVertex2f(-0.45, -1); // left
    glVertex2f(0.0, 0.5);  // tip
    glVertex2f(0.45, -1);  // right
    glEnd();

    glBegin(GL_POLYGON); // ice kings castle snow tip
    glColor3ub(255, 255, 255);
    glVertex2f(0.06, 0.3);
    glVertex2f(0.0, 0.5);
    glVertex2f(-0.06, 0.3);
    glEnd();

    glBegin(GL_TRIANGLES); // ice kings castle snow tip
    glColor3ub(255, 255, 255);
    glVertex2f(0.05, 0.30);
    glVertex2f(0, 0.25);
    glVertex2f(-0.05, 0.30);
    glEnd();

    glPointSize(20.0);
    glBegin(GL_POINTS); // castles eyes
    glColor3ub(0, 119, 182);
    glVertex2f(-0.05, 0.0);
    glVertex2f(0.05, 0.0);
    glEnd();

    glBegin(GL_TRIANGLES); // ice kings castle mouth
    glColor3ub(0, 119, 182);
    glVertex2f(-0.05, -0.3);
    glVertex2f(0.0, -0.07);
    glVertex2f(0.05, -0.3);
    glEnd();
}

void clouds(float x, float y, float size) { // cloud func for varying size
    glPushMatrix();
    glTranslatef(x, y, 0.0);  // Translate to the specified position

    glBindBuffer(GL_ARRAY_BUFFER, cloudVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    GLfloat scaledCloudVertices[18];

    for (int i = 0; i < 18; i += 3) {
        scaledCloudVertices[i] = cloudVertices[i] * size;
        scaledCloudVertices[i + 1] = cloudVertices[i + 1] * size;
        scaledCloudVertices[i + 2] = cloudVertices[i + 2];
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(scaledCloudVertices), scaledCloudVertices);

    glColor3ub(255, 255, 255);
    glDrawArrays(GL_POLYGON, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();
}


void updateClouds(int value) {
    cloud1X += 0.002; // Adjust the movement speed as needed
    cloud2X += 0.001;
    cloud3X += 0.002;
    cloud4X += 0.001;
    cloud5X += 0.001;

    // Reset clouds' position when they go beyond the right edge
    if (cloud1X > 1.2) {
        cloud1X = -1.2;
    }

    if (cloud2X > 1.2) {
        cloud2X = -1.2;
    }

    if (cloud3X > 1.2) {
        cloud3X = -1.2;
    }

    if (cloud4X > 1.2) {
        cloud4X = -1.2;
    }

    if (cloud5X > 1.2) {
        cloud5X = -1.2;
    }

    glutPostRedisplay(); // Trigger a redisplay
    glutTimerFunc(16, updateClouds, 0); // 60 frames per second (1000 ms / 60)
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    iceKingMountain(); // display iceking mountain

    clouds(cloud1X, 0.7, 1.0);
    clouds(cloud2X, 0.4, 0.6);
    clouds(cloud3X, 0.6, 0.5);		// display the cloud with different x, y coordinates and sizes
    clouds(cloud4X, 0.3, 0.3);
    clouds(cloud5X, 0.8, 0.7);

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void exit(unsigned char key, int x, int y) {
    if (key == 27) { // 27 is the ASCII code for the ESC key
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("JulJethProj");
    glutFullScreen();

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glClearColor(202.0 / 255.0, 240.0 / 255.0, 248.0 / 255.0, 1.0); // set background color

    glutKeyboardFunc(exit); // exit the program using `ESC` key

    createCloudVBO();  // Create the VBO for clouds

    glutTimerFunc(0, updateClouds, 0); // for the animated clouds

    glutMainLoop();

    return 0;
}
