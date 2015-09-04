#include "GraphViewer.h"
#include "GraphController.h"
#include <QBoxLayout>
#include <QEvent>
#include <QResizeEvent>
#include <QApplication>
#include <QStyle>
GraphViewer* GraphViewer::me = nullptr;
GraphViewer::GraphViewer(GraphController* controller, QWidget *parent)
	: QMainWindow(parent), controller(controller),
	scrollSize(qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent) + 5)
{
	me = this;
	
	controller->setGraphViewer(this);
	QWidget* widget = new QWidget();

	QVBoxLayout* layout = new QVBoxLayout();
	scene = new GraphScene();
	view = new QGraphicsView(scene);
	view->setRenderHints(QPainter::Antialiasing
		| QPainter::TextAntialiasing);
	label = new QLabel("hello there");
	prim = new QPushButton("Prim");
	QHBoxLayout* buttons = new QHBoxLayout();
	QPushButton* clearGraph = new QPushButton("Clear Graph");
	QPushButton* clearSelection = new QPushButton("Clear Selection");
	kruskal = new QPushButton("Kruskal");
	buttons->addWidget(prim);
	buttons->addWidget(kruskal);

	layout->addWidget(view);
	layout->addWidget(label);
	layout->addLayout(buttons);
	layout->addWidget(clearGraph);
	layout->addWidget(clearSelection);

	widget->setLayout(layout);
	setCentralWidget(widget);
	setMinimumSize(600, 600);
	connect(this, &GraphViewer::entered, scene, &GraphScene::entered);
	connect(this, &GraphViewer::left, scene, &GraphScene::left);
	connect(scene, &GraphScene::addGEdge, controller, &GraphController::edgeAdded);
	connect(scene, &GraphScene::addGNode, controller, &GraphController::nodeAdded);
	connect(prim, &QPushButton::clicked, controller, &GraphController::primRequest);
	connect(kruskal, &QPushButton::clicked, controller, &GraphController::kruskalRequest);
	connect(clearGraph, &QPushButton::clicked, controller, &GraphController::resetRequest);
	connect(clearGraph, &QPushButton::clicked, scene, &GraphScene::resetRequest);
	connect(clearSelection, &QPushButton::clicked, this, &GraphViewer::requestUnselect);
	connect(&inputDialog, &InputDialog::weightEntered, scene, &GraphScene::weightEntered);


	view->setMouseTracking(true);
	view->installEventFilter(this);

	label->setAlignment(Qt::AlignCenter);

}

GraphViewer::~GraphViewer()
{
	if (controller)
	{
		controller->setGraphViewer(nullptr);
	}
}

void GraphViewer::debug(QString str, bool append)
{
	QString before = append ? label->text() : "";
	label->setText(before+str);
}

void GraphViewer::debug(QPointF p, bool append)
{
	debug((std::to_string(p.x()) + ", " + std::to_string(p.y())).c_str(), append);
}

void GraphViewer::getQGVPoints(QPointF& start, QPointF& end)
{
	start = viewMin;
	end = viewMax;
}

void GraphViewer::selectEdges(std::vector<int> edgeIds, std::vector<int> nodes)
{
	unselectEdges();
	
	for (int id : nodes)
	{
		ViewNode::get(id)->setInPath(true);
	}
	for (int id : edgeIds)
	{
		ViewEdge::get(id)->enforce(true);
	}
	lastSelectedEdgeIds = edgeIds;
	lastSelectedNodes = nodes;

}

void GraphViewer::unselectEdges(bool cleanUpOnly)
{
	
	if (!cleanUpOnly)
	{
		for (int id : lastSelectedEdgeIds)
		{
			ViewEdge::get(id)->enforce(false);
		}

		for (int id : lastSelectedNodes)
		{
			ViewNode::get(id)->setInPath(false);
		}
	}
	lastSelectedNodes.clear();
	lastSelectedEdgeIds.clear();
}

void GraphViewer::showWeightDialog()
{
	inputDialog.exec();
}

void GraphViewer::showResult(const QString& str)
{
	label->setText(str);
}

void GraphViewer::resizeEvent(QResizeEvent* event)
{
	
}

void GraphViewer::moveEvent(QMoveEvent* event)
{
	viewMin = view->mapToGlobal(QPoint(0, 0));
	viewMax = view->mapToGlobal(QPoint(view->width(), view->height()));
}

bool GraphViewer::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == view)
	{
		if (event->type() == QEvent::Resize)
		{
			QResizeEvent* eventt = static_cast<QResizeEvent*>(event);
			auto current = scene->sceneRect();
			QRectF newrect(0, 0, eventt->size().width() - scrollSize,
				eventt->size().height() - scrollSize);
			scene->setSceneRect(0, 0, std::max(current.width(), newrect.width()), std::max(current.height(), newrect.height()));
			viewMin = view->mapToGlobal(QPoint(0, 0));
			viewMax = view->mapToGlobal(QPoint(eventt->size().width(), eventt->size().height()));
		}
		else if (event->type() == QEvent::Enter)
		{
			emit entered();
		}
		else if (event->type() == QEvent::Leave)
		{
			emit left();
		}
		
	
		

	}

	return QObject::eventFilter(obj, event);;
}

void GraphViewer::requestUnselect()
{
	unselectEdges();
}