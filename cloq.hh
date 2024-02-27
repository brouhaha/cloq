// cloq.hh
//
// Copyright 2024 Eric Smith
// SPDX-License-Identifier: GPL-3.0-only

#ifndef CLOQ_HH
#define CLOQ_HH

#include <string>

#include <QGraphicsView>
#include <QPoint>
#include <QString>
#include <QWidget>

#include "cloq_item.hh"


class Cloq: public QGraphicsView
  {
    Q_OBJECT

  public:
    Cloq(QString svg_fn,
	 QWidget* parent = nullptr);

    Cloq           (const Cloq& ) = delete;  // no copy constructor
    Cloq           (      Cloq& ) = delete;  // no move constructor
    Cloq& operator=(const Cloq& ) = delete;  // no copy assignment
    Cloq& operator=(      Cloq&&) = delete;  // no move assignment

    QSize sizeHint() const override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

  public slots:
    void update(unsigned hour, unsigned minute, unsigned second);
    void update_local_time();

  protected:
    QSize size_hint;

    void resizeEvent(QResizeEvent *event) override;

  private:
    double aspect_ratio;
    bool moving;
    bool resizing;
    QPoint mouse_start_widget_pos;

    QSvgRenderer* renderer;
    QGraphicsScene* scene;
    CloqItem* boundary;
    CloqItem* clock;
    CloqItem* hour_hand;
    CloqItem* minute_hand;
    CloqItem* second_hand;

  private:
    void mouse_resize_handler(QMouseEvent* event);
  };

#endif // CLOQ_HH
