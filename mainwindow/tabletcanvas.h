/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TABLETCANVAS_H
#define TABLETCANVAS_H

#include <functional>

#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QPoint>
#include <QAbstractScrollArea>
#include <QTabletEvent>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QPoint>
#include <QtConcurrent/QtConcurrent>
#include <QQueue>
#include <QKeyEvent>

#include "qimagecl.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QString;
QT_END_NAMESPACE

//! [0]
class TabletCanvas : public QWidget
{
    Q_OBJECT

public:
    enum Valuator { PressureValuator, TangentialPressureValuator,
                    TiltValuator, VTiltValuator, HTiltValuator, NoValuator };
    Q_ENUM(Valuator)

    TabletCanvas(QWidget *parent = nullptr);

    bool saveImage(const QString &file);
    bool loadImage(const QString &file);

    void setAlphaChannelValuator(Valuator type)
        { m_alphaChannelValuator = type; }

    void setColorSaturationValuator(Valuator type)
        { m_colorSaturationValuator = type; }

    void setLineWidthType(Valuator type)
        { m_lineWidthValuator = type; }

    void setColor(const QColor &c)
        { if (c.isValid()) m_color = c; }

    QColor color() const
        { return m_color; }

    void setTabletCursor(QTabletEvent *event)
        { updateCursor(event); }

    int maximum(int a, int b)
        { return a > b ? a : b; }

    bool isInProximity;

    void fillTool(const QPoint &, const QColor &);
    void clearAnnotImage();

    int getPainterOpacity();

protected:
    void tabletEvent(QTabletEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    int m_painterOpacity;

signals:
    void tabletCursorPos(const QPoint &);

public slots:
    void setPainterOpacity(const int &);
    void onFillButtonClicked();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void saveAnnotImage(const QString &);

private:
    void initImage();
    void paintImage(QPainter &painter, QTabletEvent *event);
    Qt::BrushStyle brushPattern(qreal value);
    void updateBrush(const QTabletEvent *event, QPainter &);
    void updateCursor(const QTabletEvent *event);

    Valuator m_alphaChannelValuator;
    Valuator m_colorSaturationValuator;
    Valuator m_lineWidthValuator;
    QColor m_color;
    QImage m_origImage;
    QImage m_annotImage;
    QBrush m_brush;
    QPen m_pen;
    bool m_deviceDown;
    bool m_fillButtonClicked;
    qreal m_zoom;

    QImageCL qimagecl;

    struct Point {
        QPointF pos;
        qreal rotation;
    } lastPoint;
};
//! [0]

#endif
