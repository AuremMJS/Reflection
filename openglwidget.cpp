// Include Statements
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <qelapsedtimer.h>
#include <QGLWidget>
#include "openglwidget.h"
#include <math.h>
// Include Statements

// Define PI value
#define M_PI 3.14 

// Bool to specify whether mirror image has to be captured for the current frame
bool requireMirrorCapture = true;

// Ball position to animate the ball
float ball_position_x = -2.0f;

// Pixels representing the mirror image
GLubyte *outpixels = NULL;

// Width and height of the widget
unsigned int Width, Height;

// Constructor
OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QGLWidget(parent)
{

}

// Destructor
OpenGLWidget::~OpenGLWidget()
{

}

// Function to draw walls
void OpenGLWidget::DrawWalls()
{
	glPushMatrix();
	glBegin(GL_QUADS);
	// Floor
	glColor3f(0.58, 0.29, 0.0);
	glVertex3f(-4, -2, 1);
	glVertex3f(4, -2, 1);
	glVertex3f(4, -2, -5);
	glVertex3f(-4, -2, -5);
	
	// Ceiling
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(-4, 3, 1);
	glVertex3f(4, 3, 1);
	glVertex3f(4, 3, -5);
	glVertex3f(-4, 3, -5);
	
	// Walls

	glColor3f(0.5, 0.5, 0.0);
	glVertex3f(-4, -2, -5);
	glVertex3f(4, -2, -5);
	glVertex3f(4, 3, -5);
	glVertex3f(-4, 3, -5);

	glColor3f(0.5, 0.0, 0.0);
	glVertex3f(-4, -2, 1);
	glVertex3f(4, -2, 1);
	glVertex3f(4, 3, 1);
	glVertex3f(-4, 3, 1);

	glColor3f(0.0, 0.0, 0.5);
	glVertex3f(4, 3, -5);
	glVertex3f(4, -2, -5);
	glVertex3f(4, -2, 1);
	glVertex3f(4, 3, 1);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-4, 3, -5);
	glVertex3f(-4, -2, -5);
	glVertex3f(-4, -2, 1);
	glVertex3f(-4, 3, 1);

	glEnd();

	glPopMatrix();
}

// Function to Draw Sphere
void OpenGLWidget::DrawSphere(double r, int lats, int longs, double xoffset, double yoffset, double zoffset)
{
	glPushMatrix();
	glBegin(GL_QUAD_STRIP);
	glLoadIdentity();
	glColor3f(0.8, 0.5, 0.5);

	int i, j;

	// Loop through latitudes and longitudes
	for (i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);
		z1 -= 0;
		z0 -= 0;
		for (j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			// Draw the vertices
			glNormal3f(x * zr0 * r, y * zr0 * r, z0 * r);
			glVertex3f(x * zr0 *r, y * zr0 * r, z0 *r);
			glNormal3f(x * zr1 * r, y * zr1 * r, z1 * r);
			glVertex3f(x * zr1 * r, y * zr1 * r, z1 * r);
		}
	}
	glEnd();
	glPopMatrix();
}

// Function to Draw Ball
void OpenGLWidget::DrawBall()
{
	DrawSphere(1, 36, 18, 1, 3.0, 1);
}

// Initialize OpenGL
void OpenGLWidget::initializeGL()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0, 0.0, 1.0, 1.0);
}

// Function to capture mirror image
void OpenGLWidget::CaptureMirrorImage(int windowWidth, int windowHeight) {

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Read the back buffer and store it in outpixels
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, outpixels);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

// Resize OpenGL
void OpenGLWidget::resizeGL(int w, int h)
{
	Width = w;
	Height = h;
	glViewport(0, 0, w, h);

	// set projection matrix to be glOrtho based on zoom & window size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// retrieve the scale factor
	float size = 5.0f;

	// compute the aspect ratio of the widget
	float aspectRatio = (float)w / (float)h;
	gluPerspective(90.0, aspectRatio, 2, 500);

	const int numberOfPixels = Width * Height * 3;

	// Reinitialize the outpixels based on new size
	outpixels = new GLubyte[numberOfPixels];
}

// Function to animate ball
void OpenGLWidget::AnimateBall(bool isMirror)
{
	requireMirrorCapture = false;
	if (ball_position_x < 3.0 && !isMirror)
	{
		//Translate based on current ball position
		ball_position_x += 0.1f;

		// Mirror image has to be updated
		requireMirrorCapture = true;
	}
	
	// Translate the ball
	glTranslatef(ball_position_x, 0.0, -3);
	glScalef(0.5f, 0.5f, 0.5f);
	glPushMatrix();
	DrawBall();
	//Pop Matrix so that the translation is not affected to the walls
	glPopMatrix();
	glPopMatrix();
}

// PainGL Function
void OpenGLWidget::paintGL()
{
	float aspectRatio = 8.0 / 5;
	GLfloat lightpostion[] = { 0.0,0,5.0,1.0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_TEXTURE_2D);

	// Check whether mirror image has to be captured
	if (requireMirrorCapture)
	{
		// set model view matrix to the center of the mirror
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.5, -5.99,  //position
			0, 0, 100.0,  //where we are looking
			0.0, 1.0, 0.0); //up vector

		// set projection matrix to be glOrtho based on zoom & window size
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// compute the aspect ratio of the widget
		gluPerspective(90.0, aspectRatio, 1.0, 500);
		glMatrixMode(GL_MODELVIEW);

		// Set the light position
		glLightfv(GL_LIGHT0,
			GL_POSITION,
			lightpostion);

		glPushMatrix();

		// Animate the ball
		AnimateBall(true);

		// Draw the walls
		DrawWalls();

		glFlush();

		// Capture the mirror image
		CaptureMirrorImage(Width, Height);

		// Clear the screen to re-render
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// Set the projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// retrieve the scale factor
		aspectRatio = (float)Width / (float)Height;
		
		// compute the aspect ratio of the widget
		gluPerspective(90.0, aspectRatio, 1, 500);
		
		// Set the light position
		lightpostion[2] = -5.0;

		glLightfv(GL_LIGHT0,
			GL_POSITION,
			lightpostion);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the view matrix
	gluLookAt(0.0, 0, 1,  //position
		0, 0, -100.0,  //where we are looking
		0.0, 1.0, 0.0); //up vector

	glPushMatrix();
	glPushMatrix();
	
	// Draw the walls
	DrawWalls();

	// Animate the ball
	AnimateBall(false);

	// Draw the mirror
	DrawMirror();

	//Pop Matrix so that the translation is not affected to the walls
	glPopMatrix();
	glPopMatrix();

	// Update if new mirror image is captured
	if (requireMirrorCapture)
		update();
}

// Function to draw mirror
void OpenGLWidget::DrawMirror() {

	// Check whether mirror image is not empty
	if (outpixels != NULL)
	{
		
		glEnable(GL_TEXTURE_2D);

		// Initialize a texture
		unsigned int texture;
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		// Set the pixels from mirror image in the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, outpixels);

		// Set the texture parameters
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Width, Height, GL_RGB, GL_UNSIGNED_BYTE, outpixels);

		// Draw the mirror with texture coords specified
		glBegin(GL_QUADS);
		glTexCoord2f(0.8125, 0.2);
		glVertex3f(-2.5, -1, -4.99);
		glTexCoord2f(0.1875, 0.2);
		glVertex3f(2.5, -1, -4.99);
		glTexCoord2f(0.1875, 0.8);
		glVertex3f(2.5, 2, -4.99);
		glTexCoord2f(0.8125, 0.8);
		glVertex3f(-2.5, 2, -4.99);
		glPopMatrix();
		glEnd();
		
		// Delete the texture
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &texture);
		glDisable(GL_TEXTURE_2D);

		// Draw the wooden frame around mirror
		glBegin(GL_QUADS);
		glColor3f(0.3, 0.12, 0.15);
		glVertex3f(-2.6, -1.1, -4.5);
		glVertex3f(-2.5, -1.1, -4.99);
		glVertex3f(-2.5, 2.1, -4.99);
		glVertex3f(-2.6, 2.1, -4.5);

		glColor3f(0.3, 0.12, 0.0);
		glVertex3f(-2.6, 2.1, -4.5);
		glVertex3f(-2.5, 2, -4.99);
		glVertex3f(2.5, 2, -4.99);
		glVertex3f(2.6, 2.1, -4.5);


		glColor3f(0.5, 0.3, 0.2);
		glVertex3f(2.6, -1.1, -4.5);
		glVertex3f(2.5, -1.1, -4.99);
		glVertex3f(2.5, 2.1, -4.99);
		glVertex3f(2.6, 2.1, -4.5);


		glColor3f(0.5, 0.25, 0.2);
		glVertex3f(-2.6, -1.1, -4.5);
		glVertex3f(-2.5, -1, -4.99);
		glVertex3f(2.5, -1, -4.99);
		glVertex3f(2.6, -1.1, -4.5);
		glEnd();
	}
}
