#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class QGraphicsItem;
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
	void OnPathBtnClicked();
	void OnUpDownBtnClicked();
	void OnLeftRightBtnClicked();
	void OnTurnLeftBtnClicked();
	void OnTurnRightBtnClicked();

	void SetMatrix();

	void ZoomIn(int v);
	void ZoomOut(int v);

protected:
	void keyPressEvent(QKeyEvent* e) override;
	void keyReleaseEvent(QKeyEvent* e) override;

private:
	void InitConn();
	void InitScene();

private:
	Ui::MainWindow* ui;

};

#endif // MAINWINDOW_H
