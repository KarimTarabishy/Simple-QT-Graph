#include "GraphViewer.h"
#include "GraphController.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GraphController controller;
	GraphViewer w(&controller);
	controller.setGraphViewer(&w);
	
	w.show();
	return a.exec();
}
