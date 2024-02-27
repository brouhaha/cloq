// Copyright 2022-2024 Eric Smith
// SPDX-License-Identifier: GPL-3.0-only

#include <QGraphicsScene>
#include <QRectF>
#include <QString>
#include <QSvgRenderer>
#include <QTransform>

#include "cloq_item.hh"

CloqItem::CloqItem(std::string element_id,
		   QSvgRenderer* renderer,
		   QGraphicsScene* scene,
		   bool visible,
		   QGraphicsItem* parent):
  QGraphicsSvgItem(parent)
{
  QString q_element_id = QString::fromStdString(element_id);
  if (! renderer->elementExists(q_element_id))
  {
    throw std::runtime_error("No SVG element \"" + element_id + "\"\n");
  }
  setSharedRenderer(renderer);
  setElementId(q_element_id);

  QRectF bounds = renderer->boundsOnElement(q_element_id);
  setPos(bounds.x(), bounds.y());
  setTransformOriginPoint(-bounds.x(), -bounds.y());

  setVisible(visible);

  scene->addItem(this);
}
