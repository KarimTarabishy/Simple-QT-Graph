#ifndef QGRAPH_H
#define QGRAPH_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include "GraphScene.h"
#include "InputDialog.h"

class GraphController;

class GraphViewer : public QMainWindow
{
	Q_OBJECT

public:
	GraphViewer(GraphController* controller, QWidget *parent = 0);
	~GraphViewer();

	void debug(QString str, bool append = false);
	void debug(QPointF str, bool append = false);
	static GraphViewer* get(){ return me; }
	QGraphicsView* getQGView(){ return view; };
	void getQGVPoints(QPointF& start,QPointF& end);
	GraphScene* getScene(){ return scene; };
	GraphController* getController(){ return controller; }
	void selectEdges(std::vector<int> edgeIds, std::vector<int> nodeIds);
	void unselectEdges(bool cleanUpOnly = false);
	void showWeightDialog();
	void showResult(const QString& str);

protected:
	virtual void resizeEvent(QResizeEvent * event) override;
	virtual void moveEvent(QMoveEvent * event) override;
	

signals:
	void entered();
	void left();

public slots:
	void requestUnselect();

private:
	QGraphicsView* view;
	GraphScene* scene;
	QLabel* label;

	GraphController* controller;
	QPushButton* prim, *kruskal;

	const int scrollSize;
	QPointF viewMin, viewMax;
	static GraphViewer* me;
	std::vector<int> lastSelectedEdgeIds, lastSelectedNodes;
	bool eventFilter(QObject *obj, QEvent *event) override;
	InputDialog inputDialog;


};

#endif // QGRAPH_H
