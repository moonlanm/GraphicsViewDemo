#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private slots:

	void OnCircleBtnClicked();
	void OnRectBtnClicked();
	void OnLineBtnClicked();
	void OnImgBtnClicked();
	void OnUpDownBtnClicked();
	void OnLeftRightBtnClicked();
	void OnTurnLeftBtnClicked();
	void OnTurnRightBtnClicked();

	void SetMatrix();

protected:
	void wheelEvent(QWheelEvent*) override;
	bool eventFilter(QObject* o, QEvent* e) override;

private:
	void InitConn();
	void InitScene();

private:
	Ui::MainWindow* ui;
	bool m_upDown = false;
	bool m_leftRight = false;
};

#endif // MAINWINDOW_H
