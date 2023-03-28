#include <stdio.h>
#include <GL/glut.h>
#define true 1;
#define false 0;
#define bool int;

double x, y; // To find intersection points

// Defining xmax, ymax and xmin, ymin for clipping rectangle.
int xmin = 50, xmax = 100, ymin = 50, ymax = 100;

// Define region codes
const int RIGHT = 8;  // Region Code - 1000
const int LEFT = 2;   // Region Code - 0010
const int TOP = 4;    // Region Code - 0100
const int BOTTOM = 1; // Region Code - 0001

int outcode0, outcode1, outcodeout, done, accept;

int computeoutcode(double x, double y)
{
    int code = 0;
    if (y > ymax) // Above the rectangle
        code |= TOP;
    else if (y < ymin) // Below the rectangle
        code |= BOTTOM;

    if (x > xmax) // TO the right of rectangle
        code |= RIGHT;
    else if (x < xmin) // To the left ogf rectangle
        code |= LEFT;

    return code;
}

void LineClip(double x0, double y0, double x1, double y1)
{
    int accept = false;
    int done = false;
    // Compute region codes for P1, P2
    outcode0 = computeoutcode(x0, y0);
    outcode1 = computeoutcode(x1, y1);
    do
    {
        // Check if both endpoint region code is 0, then line is completely inside
        if (!(outcode0 | outcode1))
        {
            accept = true;
            done = true;
        }
        // If both the endpoints are outside rectangle, in same region
        else if (outcode0 & outcode1)
        {
            done = true;
        }
        else
        {
            // At least one endpoint is outside the rectangle, pick it.
            outcodeout = outcode0 ? outcode0 : outcode1;
        }

        // Find intersection point using formulas
        // y=y1+m*(x-x1) and x=x1+(1/m) * (y-y1)
        if (outcodeout & TOP)
        {
            // Point is above the clip rectangle
            x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
            y = ymax;
        }
        else if (outcodeout & BOTTOM)
        {
            // Point is below the rectangle
            x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
            y = ymin;
        }
        else if (outcodeout & RIGHT)
        {
            // Point is to the right of rectangle
            y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
            x = xmax;
        }
        else
        {
            // Point is to the left of rectangle
            y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
            x = xmin;
        }

        // Now intersection point x, y is found. We replace point outside rectangle by intersection point
        if (outcodeout == outcode0)
        {
            x0 = x;
            y0 = y;
            outcode0 = computeoutcode(x0, y0);
        }
        else
        {
            x1 = x;
            y1 = y;
            outcode1 = computeoutcode(x1, y1);
        }

        if (accept)
        {
            glPushMatrix();            // Creates matrix 1 on the top
            glTranslatef(100, 100, 0); // Applies translation to matrix 1

            // Draw a clip window with translation <100, 100, 0>
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_LINE_LOOP);
            glVertex2i(50, 50);
            glVertex2i(100, 50);
            glVertex2i(100, 100);
            glVertex2i(50, 100);
            glEnd();

            glColor3f(1.0, 0.0, 1.0);
            glBegin(GL_LINES);
            glVertex2i(x0, y0);
            glVertex2i(x1, y1);
            glEnd();

            glPopMatrix(); // Dletes matrix 1
            glFlush();
        }
    } while (1);
}

void display()
{
    glClearColor(1, 1, 1, 1); // Set window background color
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0); // Set Red color for clip window
    glBegin(GL_LINE_LOOP);
    glVertex2i(50, 50);
    glVertex2i(100, 50);
    glVertex2i(100, 100);
    glVertex2i(50, 100);
    glEnd();
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(60, 20);
    glVertex2i(80, 120);
    glVertex2i(80, 20);
    glVertex2i(60, 120);
    glEnd();
    LineClip(60, 20, 80, 120);
    LineClip(80, 20, 60, 120);
    glFlush();
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 300, 0, 300);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Cohen Sutherland line clipping algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}
