// Include Statements
#include "Reflection.h"
#include <QtWidgets/QApplication>
// Include Statements

// Main functions
int main(int argc, char *argv[])
{
	// Initialize QT application
	QApplication a(argc, argv);

	// Initialize Reflection window
	Reflection w;

	// Show Reflection window
	w.show();
	return a.exec();
}
