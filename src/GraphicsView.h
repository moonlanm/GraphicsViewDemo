#pragma once
#include <QGraphicsView>


enum class DrawType
{
	Unknown,
	Circle,
	Rect,
	Line,
	Path
};
class QGraphicsItem;
class GraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit GraphicsView(QWidget* parent = nullptr);

signals:
	void ZoomOut(int v);
	void ZoomIn(int v);

public slots:
	void MoveStatusChange(bool isMove);

protected:
	void wheelEvent(QWheelEvent*) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void dragMoveEvent(QDragMoveEvent* event) override;
	void dragEnterEvent(QDragEnterEvent* e) override;
	void dropEvent(QDropEvent* e) override;

private:
	friend class MainWindow;
	DrawType m_type = DrawType::Unknown;
	QGraphicsItem* m_curItem = nullptr;
	QPointF m_topLeftPos;
};