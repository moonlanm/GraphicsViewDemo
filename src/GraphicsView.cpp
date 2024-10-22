#include "GraphicsView.h"
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QDebug>
GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)
{
	setMouseTracking(true);
}

void GraphicsView::wheelEvent(QWheelEvent* e)
{
	if (e->modifiers() & Qt::ControlModifier) {
		if (e->angleDelta().y() > 0)
			emit ZoomIn(6);
		else
			emit ZoomOut(6);
		centerOn(e->position());
		e->accept();
	}
}

void GraphicsView::mousePressEvent(QMouseEvent* e)
{
	if (e->button() != Qt::LeftButton)
	{
		return;
	}

	auto localPos = e->localPos().toPoint();
	m_topLeftPos = mapToScene(localPos);

	QRectF rect(mapToScene(localPos), mapToScene(localPos + QPoint(1,1)));
	switch (m_type)
	{
	case DrawType::Unknown:
		return;
	case DrawType::Circle:
		m_curItem = scene()->addEllipse(rect, QPen(Qt::blue), QBrush(Qt::blue));
		break;
	case DrawType::Rect:
		m_curItem = scene()->addRect(rect, QPen(Qt::blue), QBrush(Qt::blue));
		break;
	case DrawType::Line:
		m_curItem = scene()->addLine(QLine(rect.topLeft().toPoint(), rect.bottomRight().toPoint()));
		break;
	case DrawType::Path:
		m_curItem = scene()->addPath(QPainterPath(rect.topLeft()));
		break;
	default:
		break;
	}
}

void GraphicsView::mouseMoveEvent(QMouseEvent* e)
{
	if (!m_curItem)
	{
		return;
	}
	auto localPos = e->localPos().toPoint();
	QRectF rect(m_topLeftPos, mapToScene(localPos));
	switch (m_type)
	{
	case DrawType::Unknown:
		break;
	case DrawType::Circle:
		((QGraphicsEllipseItem*)m_curItem)->setRect(rect);
		break;
	case DrawType::Rect:
		((QGraphicsRectItem*)m_curItem)->setRect(rect);
		break;
	case DrawType::Line:
		((QGraphicsLineItem*)m_curItem)->setLine(QLine(rect.topLeft().toPoint(), rect.bottomRight().toPoint()));
		break;
	case DrawType::Path:
	{
		auto p = ((QGraphicsPathItem*)m_curItem)->path();
		if (p.currentPosition() == QPoint(0, 0))
		{
			p.moveTo(mapToScene(localPos));
		}
		p.lineTo(mapToScene(localPos));
		((QGraphicsPathItem*)m_curItem)->setPath(p);
		break;
	}
	default:
		break;
	}
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* e)
{
	if (!m_curItem)
	{
		return;
	}
	m_curItem = nullptr;
	m_type = DrawType::Unknown;
}

void GraphicsView::MoveStatusChange(bool change)
{
	setDragMode(!change ? QGraphicsView::RubberBandDrag : QGraphicsView::ScrollHandDrag);
}