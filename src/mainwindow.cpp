#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QtMath>
#include <QWheelEvent>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);
	InitScene();
	InitConn();
	installEventFilter(this);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::OnCircleBtnClicked()
{
	auto circle = new QGraphicsEllipseItem();
	circle->setRect(0, 0, 10, 10);
	circle->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	ui->graphicsView->scene()->addItem(circle);
}

void MainWindow::OnRectBtnClicked()
{
	auto rect = new QGraphicsRectItem();
	rect->setRect(0, 0, 10, 10);
	rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	ui->graphicsView->scene()->addItem(rect);
}

void MainWindow::OnLineBtnClicked()
{
	auto line = new QGraphicsLineItem();
	line->setLine(QLine(0, 0, 10, 10));
	line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	ui->graphicsView->scene()->addItem(line);
}

void MainWindow::OnImgBtnClicked()
{
	auto filePath = QFileDialog::getOpenFileName(this, QStringLiteral("ÇëÑ¡ÔñÍ¼Æ¬"), ".", QString("image (*.png;*.jpg;*.jpeg)"));
	QPixmap map(filePath);
	if (map.isNull())
	{
		return;
	}

	auto item = new QGraphicsPixmapItem(map);
	item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	item->setPos(0, 0);
	ui->graphicsView->scene()->addItem(item);

	auto center = item->boundingRect().center();

	auto centerCircle = new QGraphicsEllipseItem(QRect(center.x(), center.y(), 100, 100), item);
	centerCircle->setPen(QPen(Qt::green));
	centerCircle->setBrush(QBrush(Qt::green));
	ui->graphicsView->scene()->addItem(centerCircle);
}

void MainWindow::OnUpDownBtnClicked()
{
	auto items = ui->graphicsView->scene()->selectedItems();
	for (auto& item : items)
	{
		auto rect = item->boundingRect();
		auto center = rect.center();


		QTransform matrix(item->matrix());
		matrix.translate(center.x(), center.y());
		matrix.setMatrix(matrix.m11(), matrix.m12(), matrix.m13(),
			matrix.m21(), -matrix.m22(), matrix.m23(),
			matrix.m31(), matrix.m32(), matrix.m33());
		matrix.translate(-center.x(), -center.y());
		item->setTransform(matrix);
	}
}

void MainWindow::OnLeftRightBtnClicked()
{
	auto items = ui->graphicsView->scene()->selectedItems();
	for (auto& item : items)
	{
		auto rect = item->boundingRect();
		auto center = rect.center();
		QTransform matrix(item->matrix());
		matrix.translate(center.x(), center.y());
		matrix.setMatrix(-matrix.m11(), matrix.m12(), matrix.m13(),
			matrix.m21(), matrix.m22(), matrix.m23(),
			matrix.m31(), matrix.m32(), matrix.m33());
		matrix.translate(-center.x(), -center.y());
		item->setTransform(matrix);
	}
}

void MainWindow::OnTurnLeftBtnClicked()
{
	auto items = ui->graphicsView->scene()->selectedItems();
	for (auto& item : items)
	{
		auto rect = item->boundingRect();
		auto center = rect.center();
		QTransform matrix(item->matrix());
		matrix.translate(center.x(), center.y());
		matrix.rotate(-90);
		matrix.translate(-center.x(), -center.y());
		item->setTransform(matrix);
	}
}

void MainWindow::OnTurnRightBtnClicked()
{
	auto items = ui->graphicsView->scene()->selectedItems();
	for (auto& item : items)
	{
		auto rect = item->boundingRect();
		auto center = rect.center();
		QTransform matrix(item->matrix());
		matrix.translate(center.x(), center.y());
		matrix.rotate(90);
		matrix.translate(-center.x(), -center.y());
		item->setTransform(matrix);
	}
}

void MainWindow::SetMatrix()
{
	qreal scale = qPow(qreal(2), (ui->scaleSlider->value() - 250) / qreal(50));

	QTransform matrix;
	matrix.scale(scale, scale);
	matrix.rotate(ui->rotationSlider->value());
	ui->graphicsView->setTransform(matrix);
}

void MainWindow::wheelEvent(QWheelEvent* e)
{
	if (e->modifiers() & Qt::ControlModifier) {
		if (e->angleDelta().y() > 0)
			ui->scaleSlider->setValue(ui->scaleSlider->value() + 6);
		else
			ui->scaleSlider->setValue(ui->scaleSlider->value() - 6);
		ui->graphicsView->centerOn(e->position());
		e->accept();
	}
}

bool MainWindow::eventFilter(QObject* o, QEvent* e)
{
	if (o == ui->graphicsView)
	{
		return true;
	}
	return QMainWindow::eventFilter(o, e);
}

void MainWindow::InitConn()
{
	connect(ui->circleButton, &QPushButton::clicked, this, &MainWindow::OnCircleBtnClicked);
	connect(ui->rectButton, &QPushButton::clicked, this, &MainWindow::OnRectBtnClicked);
	connect(ui->lineButton, &QPushButton::clicked, this, &MainWindow::OnLineBtnClicked);
	connect(ui->imgButton, &QPushButton::clicked, this, &MainWindow::OnImgBtnClicked);
	connect(ui->upDownBtn, &QPushButton::clicked, this, &MainWindow::OnUpDownBtnClicked);
	connect(ui->leftRightBtn, &QPushButton::clicked, this, &MainWindow::OnLeftRightBtnClicked);
	connect(ui->turnLeftBtn, &QPushButton::clicked, this, &MainWindow::OnTurnLeftBtnClicked);
	connect(ui->turnRightBtn, &QPushButton::clicked, this, &MainWindow::OnTurnRightBtnClicked);

	connect(ui->scaleSlider, &QSlider::valueChanged, this, &MainWindow::SetMatrix);
	connect(ui->rotationSlider, &QSlider::valueChanged, this, &MainWindow::SetMatrix);
}

void MainWindow::InitScene()
{
	auto scene = new QGraphicsScene(ui->graphicsView);
	auto center = ui->graphicsView->mapToScene(0, 0);
	scene->addEllipse(QRect(center.x(), center.y(), 100, 100), QPen(Qt::red), QBrush(Qt::red));
	ui->graphicsView->setScene(scene);
}

