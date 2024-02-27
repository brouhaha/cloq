// Copyright 2022-2024 Eric Smith
// SPDX-License-Identifier: GPL-3.0-only

#ifndef CLOQ_ITEM_HH
#define CLOQ_ITEM_HH

#include <string>

#include <QGraphicsSvgItem>

class CloqItem: public QGraphicsSvgItem
{
public:
  CloqItem(std::string element_id,
	   QSvgRenderer* renderer,
	   QGraphicsScene* scene,
	   bool visible = true,
	   QGraphicsItem* parent = nullptr);
};

#endif // CLOQ_ITEM_HH
