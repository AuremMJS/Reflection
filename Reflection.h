#pragma once

// Include Statements
#include <QtWidgets/QMainWindow>
#include "ui_Reflection.h"
// Include Statements

// Reflection window class
class Reflection : public QMainWindow
{
	Q_OBJECT

public:
	// Constructor
	Reflection(QWidget *parent = Q_NULLPTR);

private:
	// Reference to UI
	Ui::ReflectionClass ui;
};
