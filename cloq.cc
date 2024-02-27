// Copyright 2022-2024 Eric Smith
// SPDX-License-Identifier: GPL-3.0-only

#include <iostream>
#include <format>

#include <string>

#include <QGraphicsScene>
#include <QRegion>
#include <QResizeEvent>
#include <QSvgRenderer>
#include <QTime>
#include <QTransform>

#include "cloq.hh"


Cloq::Cloq(QString svg_fn,
	   QWidget* parent):
  QGraphicsView(parent),
  size_hint(200, 200),
  aspect_ratio(1.0),
  moving(false),
  resizing(false)
{
  renderer = new QSvgRenderer(svg_fn, this);
  scene = new QGraphicsScene(-100.0, -100.0, 200.0, 200.0, this);
  setContentsMargins(0, 0, 0, 0);
  setStyleSheet("border: 0px");

  setWindowFlag(Qt::FramelessWindowHint);

  // Eliminate drop shadow for frameless window, because the drop
  // shadow is always rectangular, even when the window is not.
  setAttribute(Qt::WA_TranslucentBackground, true);

  size_hint = QSize(200, 200);
  setMinimumSize(size_hint / 2);

  boundary    = new CloqItem("clock",       renderer, scene);
  boundary->setBoundingRegionGranularity(1.0);

  clock       = new CloqItem("clock",       renderer, scene);
  clock->setPos(-100.0, -100.0);


  hour_hand   = new CloqItem("hour_hand",   renderer, scene);
  hour_hand->setPos(-4.0, -50.0);
  hour_hand->setTransformOriginPoint(4.0, 50.0);

  minute_hand = new CloqItem("minute_hand", renderer, scene);
  minute_hand->setPos(-2.5, -70.0);
  minute_hand->setTransformOriginPoint(2.5, 70.0);

  second_hand = new CloqItem("second_hand", renderer, scene);
  second_hand->setPos(-7.0, -80.0);
  second_hand->setTransformOriginPoint(7.0, 80.0);

  std::cout << std::format("second hand pos({}, {})\n", second_hand->x(), second_hand->y());
  QRectF shbr = second_hand->boundingRect();
  std::cout << std::format("second hand bounding rect({}, {}, {}, {})\n",
			   shbr.left(), shbr.top(), shbr.right(), shbr.bottom());

  setScene(scene);
}


QSize Cloq::sizeHint() const
{
  return size_hint;
}


void Cloq::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton)
  {
    moving = true;
  }
  else if (event->button() == Qt::MiddleButton)
  {
    resizing = true;
  }

  if (moving || resizing)
  {
    mouse_start_widget_pos = QWidget::pos();
    event->accept();
  }
  else
  {
    QGraphicsView::mousePressEvent(event);
  }
}

void Cloq::mouseReleaseEvent(QMouseEvent* event)
{
  if (moving)
  {
    moving = false;
    event->accept();
  }
  else if (resizing)
  {
    resizing = false;
    event->accept();
  }
  else
  {
    QGraphicsView::mouseReleaseEvent(event);
  }
}

void Cloq::mouseMoveEvent(QMouseEvent* event)
{
  QPoint rel = event->globalPosition().toPoint() - mouse_start_widget_pos;
  if (moving)
  {
    move(mouse_start_widget_pos + rel);
    event->accept();
  }
  else if (resizing)
  {
    mouse_resize_handler(event);
    event->accept();
  }
  else
  {
    QGraphicsView::mouseMoveEvent(event);
  }
}

void Cloq::mouse_resize_handler(QMouseEvent* event)
{
  QPoint point = event->globalPosition().toPoint();
  QSize new_size (point.x() - x(), point.y() - y());

  if (new_size.width() < minimumWidth())
  {
    new_size.setWidth(minimumWidth());
  }

  if (new_size.height() < minimumHeight())
  {
    new_size.setHeight(minimumHeight());
  }

  QSize new_size_w = new_size;
  QSize new_size_h = new_size;

  new_size_w.setHeight(new_size.width() / aspect_ratio);
  new_size_h.setWidth(new_size.height() * aspect_ratio);

  new_size = (new_size_w.width() > new_size_h.width()) ? new_size_w : new_size_h;

  resize(new_size);
}

void Cloq::resizeEvent(QResizeEvent *event)
{
  QGraphicsView::resizeEvent(event);
  fitInView(sceneRect());
  QRegion region = boundary->boundingRegion(transform());
  setMask(region);
}


void Cloq::update(unsigned hour, unsigned minute, unsigned second)
{
  hour_hand->setRotation((hour % 12) * 30.0 + minute * 0.5 + second * (0.5 / 60.0));
  minute_hand->setRotation(minute * 6.0 + second * 0.1);
  second_hand->setRotation(second * 6.0);
}

void Cloq::update_local_time()
{
  QTime t = QTime::currentTime();
  update(t.hour() % 12, t.minute(), t.second());
}

