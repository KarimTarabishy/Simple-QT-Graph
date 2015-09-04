#include "InputDialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QIntValidator>

InputDialog::InputDialog(QWidget* parent)
	:QDialog(parent, Qt::FramelessWindowHint)
{
	QVBoxLayout* mainLayout = new QVBoxLayout();
	QHBoxLayout* hl = new QHBoxLayout();
	QLabel* label = new QLabel("Enter edge weight(1->999):");
	le = new QLineEdit();
	label->setBuddy(le);
	push = new QPushButton("Set");

	QIntValidator* validator = new QIntValidator(1, 999, this);
	le->setValidator(validator);
	hl->addWidget(label);
	hl->addWidget(le);
	mainLayout->addLayout(hl);

	setLayout(mainLayout);
	setFixedHeight(sizeHint().height());

	connect(le, &QLineEdit::returnPressed, this, &InputDialog::onReturn);
	setWindowOpacity(0.5);

}

InputDialog::~InputDialog()
{
}

void InputDialog::reject()
{
}

void InputDialog::onReturn()
{
	
	int weight = le->text().toInt();
	emit weightEntered(weight);
	accept();
	
}