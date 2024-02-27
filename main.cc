// main.cc
// Copyright 2024 Eric Smith
// SPDX-License-Identifier: GPL-3.0-only

#include <QApplication>
#include <QObject>
#include <QTimer>

#include "cloq.hh"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  Cloq* cloq = new Cloq("cloq.svg");

  QTimer* timer = new QTimer();
  QObject::connect(timer, &QTimer::timeout,
		   cloq,  &Cloq::update_local_time);
  timer->start(1000);

  cloq->show();

  return app.exec();
}
