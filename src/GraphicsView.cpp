#include "GraphicsView.h"
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QMimeData>
GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)
{
	setMouseTracking(true);
	setAcceptDrops(true);
}

void GraphicsView::wheelEvent(QWheelEvent* e)
{
	if (Qt::ControlModifier) {
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
	// 在移动画布的时候，禁止选中图元，和移动图元
	if (dragMode() == QGraphicsView::ScrollHandDrag)
	{
		auto items = this->items();
		for (auto& item : items)
		{
			item->setFlag(QGraphicsItem::ItemIsMovable, false);
			item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		}
	}
	else
	{
		auto items = this->items();
		for (auto& item : items)
		{
			item->setFlag(QGraphicsItem::ItemIsMovable, true);
			item->setFlag(QGraphicsItem::ItemIsSelectable, true);
		}
	}

	if (e->button() != Qt::LeftButton)
	{
		return QGraphicsView::mousePressEvent(e);
	}

	auto localPos = e->localPos().toPoint();
	m_topLeftPos = mapToScene(localPos);

	QRectF rect(mapToScene(localPos), mapToScene(localPos + QPoint(1, 1)));
	switch (m_type)
	{
	case DrawType::Unknown:
		return QGraphicsView::mousePressEvent(e);
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
	m_curItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* e)
{
	if (!m_curItem)
	{
		return QGraphicsView::mouseMoveEvent(e);
	}

	auto localPos = e->localPos().toPoint();
	QRectF rect(m_topLeftPos, mapToScene(localPos));
	switch (m_type)
	{
	case DrawType::Unknown:
		return QGraphicsView::mouseMoveEvent(e);
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
		return QGraphicsView::mouseReleaseEvent(e);
	}
	m_curItem = nullptr;
	m_type = DrawType::Unknown;
	return QGraphicsView::mouseReleaseEvent(e);
}

void GraphicsView::dragMoveEvent(QDragMoveEvent* event)
{
	event->accept();
}

void GraphicsView::dragEnterEvent(QDragEnterEvent* e)
{
	if (e->mimeData()->hasFormat("text/uri-list"))
	{
		e->acceptProposedAction();
	}
	e->accept();
}

void GraphicsView::dropEvent(QDropEvent* e)
{
	auto urls = e->mimeData()->urls();
	if (urls.empty())
	{
		return;
	}
	QPixmap map(urls[0].toLocalFile());

	auto item = new QGraphicsPixmapItem(map);
	item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	item->setPos(mapToScene(e->pos()));
	scene()->addItem(item);

	auto center = item->boundingRect().center();

	auto centerCircle = new QGraphicsEllipseItem(QRect(center.x(), center.y(), 100, 100), item);
	centerCircle->setPen(QPen(Qt::green));
	centerCircle->setBrush(QBrush(Qt::green));
	scene()->addItem(centerCircle);
}

void GraphicsView::MoveStatusChange(bool change)
{
	setDragMode(!change ? QGraphicsView::RubberBandDrag : QGraphicsView::ScrollHandDrag);
}