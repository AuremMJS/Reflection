#pragma once
// Include Statements
#include <QGLWidget>
// Include Statements

// Class OpenGLWidget to render the OpenGL Elements into the window
class OpenGLWidget : public QGLWidget
{
	Q_OBJECT

public:

	// Constructor
	OpenGLWidget(QWidget *parent);

	// Destructor
	~OpenGLWidget();

protected:

	// OpenGL functions
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	// Function to draw walls
	void DrawWalls();

	// Function to draw a ball
	void DrawBall();

	// Function to animate the ball 
	void AnimateBall(bool);

	// Function to draw the mirror
	void DrawMirror();

	// Function to draw a sphere
	void DrawSphere(double r, int lats, int longs, double xoffset, double yoffset, double zoffset);

	// Function to capture mirror image
	void CaptureMirrorImage(int, int);
};
