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

#include <QtWidgets>
#include <math.h>

#include "tabletcanvas.h"

//! [0]
TabletCanvas::TabletCanvas(QWidget *parent) :
    QWidget(parent),
    m_painterOpacity(255),
    m_lineWidthValuator(PressureValuator),
    m_color(QColor(0,0,0,0)),
    m_brush(m_color),
    m_pen(m_brush, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin),
    m_deviceDown(false),
    m_fillButtonClicked(false),
    m_zoom(1.0)
{

}

void TabletCanvas::initImage()
{
    QImage newImage = QImage(m_origImage.width(), m_origImage.height(),
                             QImage::Format_RGBA8888);
    newImage.fill(QColor(0,0,0,0));

    QPainter painter(&newImage);
    if (!m_annotImage.isNull())
    {
        painter.drawImage(0, 0, m_annotImage);
    }

    painter.end();
    m_annotImage = newImage;
}
//! [0]

//! [1]
bool TabletCanvas::saveImage(const QString &file)
{
    return m_annotImage.save(file);
}
//! [1]

//! [2]
bool TabletCanvas::loadImage(const QString &file)
{
    bool success = m_origImage.load(file);
    this->setGeometry(0, 0,
                      m_origImage.width() * m_zoom,
                      m_origImage.height() * m_zoom);

    if (success) {
        m_annotImage = QImage();
        initImage();
        update();
        return true;
    }

    qDebug() << "Could not load original image!";
    return false;
}
//! [2]

//! [3]
void TabletCanvas::tabletEvent(QTabletEvent *event)
{
    switch (event->type())
    {
    case QEvent::TabletPress:
        if (!m_deviceDown) {
            m_deviceDown = true;
            lastPoint.pos = event->posF() / m_zoom;
            lastPoint.rotation = event->rotation();
        }
        break;
    case QEvent::TabletMove:
        if (event->device() == QTabletEvent::RotationStylus)
            updateCursor(event);
        if (m_deviceDown) {
            QPainter painter(&m_annotImage);
            updateBrush(event, painter);
            paintImage(painter, event);
            lastPoint.pos = event->posF() / m_zoom;
            lastPoint.rotation = event->rotation();
        }
        emit tabletCursorPos(event->pos() / m_zoom);
        break;
    case QEvent::TabletRelease:
        if (m_deviceDown && event->buttons() == Qt::NoButton)
            m_deviceDown = false;
        break;
    default:
        break;
    }
    event->accept();
    update();
}
//! [3]

//! [4]
void TabletCanvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setOpacity(1.0);
    painter.scale(m_zoom, m_zoom);
    painter.drawImage(0, 0, m_origImage);
    painter.setOpacity(m_painterOpacity/255.0);
    painter.drawImage(0, 0, m_annotImage);
}
//! [4]

//! [5]
void TabletCanvas::paintImage(QPainter &painter, QTabletEvent *event)
{
//    painter.setRenderHint(QPainter::Antialiasing);

    switch (event->device()) {
    //! [6]
        //! [6]
    case QTabletEvent::Puck:
    case QTabletEvent::FourDMouse:
    {
        const QString error(tr("This input device is not supported by the example."));
#ifndef QT_NO_STATUSTIP
        QStatusTipEvent status(error);
        QApplication::sendEvent(this, &status);
#else
        qWarning() << error;
#endif
    }
        break;
    default:
    {
        const QString error(tr("Unknown tablet device - treating as stylus"));
#ifndef QT_NO_STATUSTIP
        QStatusTipEvent status(error);
        QApplication::sendEvent(this, &status);
#else
        qWarning() << error;
#endif
    }
        // FALL-THROUGH
    case QTabletEvent::Stylus:
        if (m_fillButtonClicked)
        {
            fillTool(event->pos() / m_zoom, m_color);
            m_fillButtonClicked = false;
            break;
        }

        painter.setPen(m_pen);
        painter.drawLine(lastPoint.pos,
                         event->posF() / m_zoom);
        break;
    }
}
//! [5]

//! [7]
void TabletCanvas::updateBrush(const QTabletEvent *event,
                               QPainter &painter)
{
    int hue, saturation, value, alpha;
    m_color.getHsv(&hue, &saturation, &value, &alpha);
    //! [7] //! [8]

    //! [9] //! [10]
    switch (m_lineWidthValuator) {
    case PressureValuator:
        m_pen.setWidthF(event->pressure() * 40 * event->pressure() + 1);
        break;
    default:
        m_pen.setWidthF(1);
    }

    //! [10] //! [11]
    if (event->pointerType() == QTabletEvent::Eraser) {
        painter.setCompositionMode (QPainter::CompositionMode_SourceOver);
        m_brush.setColor(QColor(0, 0, 0, 0));
        m_pen.setColor(QColor(0, 0, 0, 0));
        painter.setCompositionMode (QPainter::CompositionMode_Source);
        m_pen.setWidthF(event->pressure() * 40 * event->pressure() + 1);
    } else {
        m_brush.setColor(m_color);
        m_pen.setColor(m_color);
    }
}
//! [11]

//! [12]
void TabletCanvas::updateCursor(const QTabletEvent *event)
{
    QCursor cursor;
    if (event->type() != QEvent::TabletLeaveProximity) {
        if (event->pointerType() == QTabletEvent::Eraser) {
            cursor = QCursor(QPixmap(":/images/cursor-eraser.png"), 3, 28);
        } else {
            switch (event->device()) {
            case QTabletEvent::Stylus:
                cursor = QCursor(QPixmap(":/images/cursor-pencil.png"), 0, 0);
                break;
            default:
                break;
            }
        }
    }
    setCursor(cursor);
}
//! [12]

void TabletCanvas::resizeEvent(QResizeEvent *)
{
}

void TabletCanvas::clearAnnotImage()
{
    m_annotImage.fill(QColor(0, 0, 0, 0));
    update();
}

void TabletCanvas::fillTool(const QPoint &p, const QColor &fillColor)
{
    qDebug() << "fillTool";

    // fills the annot image from point p with region growing
    QList<QPoint> todoList;
    todoList << p;
    QColor startColor(m_annotImage.pixelColor(p));

    if (m_annotImage.pixelColor(p) == fillColor)
        return;

    // creating function with closures
    // [ capture-list ] ( params ) -> ret { body }
    // Capture list can be passed as follows
    // [a,&b] where a is captured by copy and b is captured by reference.
    // [this] captures the current object (*this) by reference
    // [&] captures all automatic variables used in the body of the lambda
    //     by reference and current object by reference if exists
    // [=] captures all automatic variables used in the body of the lambda
    //     by copy and current object by reference if exists
    // [] captures nothing

    // recursive filling of pixels (segfaults due to small stack size)
    std::function<void(const QPoint &)> find
            = [&](const QPoint &q)->auto
    {
        if (q.x() < 0 || q.x() >= m_annotImage.width()
                || q.y() < 0 || q.y() >= m_annotImage.height())
            return;
        if (m_annotImage.pixelColor(q) != startColor)
            return;

        m_annotImage.setPixelColor(q, fillColor);

        find(QPoint(q.x(), q.y()-1));
        find(QPoint(q.x()-1, q.y()));
        find(QPoint(q.x()+1, q.y()));
        find(QPoint(q.x(), q.y()+1));
    };
//    find(p);

    // concurrent filling of pixels
    std::function<void(const QPoint &)> fill
            = [&](const QPoint &p)->auto
    {
        m_annotImage.setPixelColor(p, fillColor);
    };
//    QtConcurrent::blockingMap(todoList, fill);

    QQueue<QPoint> queue;
    queue.enqueue(p);

    while (!queue.isEmpty())
    {
        QPoint q = queue.dequeue();

        if (q.x() < 0 || q.x() >= m_annotImage.width()
                || q.y() < 0 || q.y() >= m_annotImage.height())
            continue;

        if (m_annotImage.pixelColor(q) == startColor)
        {
            m_annotImage.setPixelColor(q, fillColor);

            queue.enqueue(QPoint(q.x(), q.y()-1));
            queue.enqueue(QPoint(q.x()-1, q.y()));
            queue.enqueue(QPoint(q.x()+1, q.y()));
            queue.enqueue(QPoint(q.x(), q.y()+1));
        }
    }

    update();
}

int TabletCanvas::getPainterOpacity()
{
    return m_painterOpacity;
}

void TabletCanvas::setPainterOpacity(const int &newOpacity)
{
    m_painterOpacity = newOpacity;
    update();
}

void TabletCanvas::onFillButtonClicked()
{
    m_fillButtonClicked = !m_fillButtonClicked;
}

void TabletCanvas::zoomIn()
{
    m_zoom += 0.1;
    this->setGeometry(0, 0,
                      m_origImage.width() * m_zoom,
                      m_origImage.height() * m_zoom);
    update();
}

void TabletCanvas::zoomOut()
{
    m_zoom -= 0.1;
    this->setGeometry(0, 0,
                      m_origImage.width() * m_zoom,
                      m_origImage.height() * m_zoom);
    update();
}

void TabletCanvas::resetZoom()
{
    m_zoom = 1.0;
    this->setGeometry(0, 0,
                      m_origImage.width() * m_zoom,
                      m_origImage.height() * m_zoom);
    update();
}

void TabletCanvas::saveAnnotImage(const QString &fileName)
{
    // test if segmentation is complete
    // TODO (with opencl)

    // transform colors to single channel grayscale
    QImage gray(m_annotImage.width(), m_annotImage.height(),
                m_annotImage.format());

    if (qimagecl.mapColors(m_annotImage, gray) == true)
    {
        // this can be done without conversion
        // just change gray to single channel image
        // and change OpenCL kernel accordingly
        gray.convertToFormat(QImage::Format_Grayscale8).save(fileName);
        m_origImage = QImage();
        update();
    }
}
