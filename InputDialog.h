#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class InputDialog :public QDialog
{
	Q_OBJECT

public:
	InputDialog(QWidget* parent = 0);
	~InputDialog();

	void reject() override;

private slots:
	void onReturn();

signals:
	void weightEntered(int weight);

private:
	QLineEdit* le;
	QPushButton* push;
	int weight;
};
#endif // INPUTDIALOG_H
