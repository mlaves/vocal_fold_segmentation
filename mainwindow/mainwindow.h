#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>
#include <QTabletEvent>
#include <QUrl>
#include <QList>
#include <QDirIterator>
#include "tabletcanvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(TabletCanvas *canvas);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *) override;

private:
    TabletCanvas *_canvas;
    QScrollArea *_scrollArea;
    QWidget _centralWidget;
    QCursor _defaultCursor;
    QCursor _classifyCursor;

    QList<QUrl> _todoImages;
    QList<QUrl> _annotImages;

    QRect _initWinSize;
    QHBoxLayout _mainLayout;
    QVBoxLayout _buttonvboxlayout;

    // opacity slider
    QSlider _opacSlider;

    // label buttons
    QPushButton _vocalFoldsButton;
    QPushButton _otherTissueButton;
    QPushButton _tracheaButton;
    QPushButton _pathologyButton;
    QPushButton _instrumentButton;
    QPushButton _intubationButton;
    QPushButton _laryngoscopeButton;
    QPushButton _fillButton;
    QPushButton _clearButton;
    QPushButton _saveButton;

    // label colors
    QColor _vocalFoldsColor;
    QColor _otherTissueColor;
    QColor _tracheaColor;
    QColor _pathologyColor;
    QColor _instrumentColor;
    QColor _intubationColor;
    QColor _laryngoscopeColor;

public slots:

private slots:
    void resetCursor();
    void onVocalFoldButtonClicked();
    void onOtherTissueButtonClicked();
    void onTracheaButtonClicked();
    void onPathologyButtonClicked();
    void onInstrumentButtonClicked();
    void onIntubationButtonClicked();
    void onLaryngoscopeButtonClicked();
    void onClearButtonClicked();
    void onSaveButtonClicked();

signals:

};

#endif // MAINWINDOW_H
