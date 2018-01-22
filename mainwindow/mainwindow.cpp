#include "mainwindow.h"

MainWindow::MainWindow(TabletCanvas *canvas) :
    _canvas(canvas),
    _scrollArea(new QScrollArea),
    _initWinSize(0, 0, 800, 600),
    _vocalFoldsButton("vocal folds"),
    _otherTissueButton("other tissue"),
    _tracheaButton("free trachea"),
    _pathologyButton("pathology"),
    _instrumentButton("instrument"),
    _intubationButton("intubation"),
    _laryngoscopeButton("laryngoscope"),
    _fillButton("fill area"),
    _clearButton("clear"),
    _saveButton("save + next"),
    _vocalFoldsColor(QColor(  0,   0,   205, 255)), // mediumblue
    _otherTissueColor(QColor( 0,   255, 255, 255)), // cyan
    _tracheaColor(QColor(     169, 169, 169, 255)), // darkgray
    _pathologyColor(QColor(   139, 0,   139, 255)), // darkmagenta
    _instrumentColor(QColor(  139, 0,   0,   255)), // darkred
    _intubationColor(QColor(  255, 20,  147, 255)), // deeppink
    _laryngoscopeColor(QColor(144, 238, 144, 255))  // lightgreen
{
    grabKeyboard();

    // read training files to be segmented
    // TODO: change hard coded paths
    QDirIterator trainDirIt("./train", QDir::Files);
    QDirIterator annotDirIt("./trainannot", QDir::Files);

    QList<QUrl> annotImagesShort;

    while (annotDirIt.hasNext())
    {
        QUrl file = QUrl::fromLocalFile(annotDirIt.next());
        _annotImages << file;
        annotImagesShort << file.fileName();
    }

    while (trainDirIt.hasNext())
    {
        QUrl file = QUrl::fromLocalFile(trainDirIt.next());

        if (!annotImagesShort.contains(file.fileName()))
        {
            _todoImages << file;
        }
    }

    qDebug() << _annotImages;
    qDebug() << _todoImages;

    if (!_todoImages.empty())
    {
        _canvas->loadImage(_todoImages.first().toLocalFile());
    }
    else
        qDebug() << "no new images found in ./train!";

    this->setGeometry(_initWinSize);
    this->showMaximized();
    this->setMinimumSize(640, 480);

    // set button colors
    _vocalFoldsButton.setStyleSheet(
                "QPushButton { background-color: mediumblue; color : black; }");
    _otherTissueButton.setStyleSheet(
                "QPushButton { background-color: cyan; color : black; }");
    _tracheaButton.setStyleSheet(
                "QPushButton { background-color: darkgray; color : black; }");
    _pathologyButton.setStyleSheet(
                "QPushButton { background-color: darkmagenta; }");
    _instrumentButton.setStyleSheet(
                "QPushButton { background-color: darkred; }");
    _intubationButton.setStyleSheet(
                "QPushButton { background-color: deeppink; }");
    _laryngoscopeButton.setStyleSheet(
                "QPushButton { background-color: lightgreen; color : black; }");

    // set slider properties
    _opacSlider.setMinimum(0);
    _opacSlider.setMaximum(255);
    _opacSlider.setTickPosition(QSlider::NoTicks);
    _opacSlider.setValue(_canvas->getPainterOpacity());

    // add buttons to layout
    _buttonvboxlayout.addWidget(&_opacSlider);
    _buttonvboxlayout.addWidget(&_vocalFoldsButton);
    _buttonvboxlayout.addWidget(&_otherTissueButton);
    _buttonvboxlayout.addWidget(&_tracheaButton);
    _buttonvboxlayout.addWidget(&_pathologyButton);
    _buttonvboxlayout.addWidget(&_instrumentButton);
    _buttonvboxlayout.addWidget(&_intubationButton);
    _buttonvboxlayout.addWidget(&_laryngoscopeButton);
    _buttonvboxlayout.addWidget(&_fillButton);
    _buttonvboxlayout.addWidget(&_clearButton);
    _buttonvboxlayout.addWidget(&_saveButton);
    _buttonvboxlayout.setAlignment(&_opacSlider, Qt::AlignHCenter);

    // set touch pen area to main layout
    _scrollArea->setWidget(_canvas);
    _mainLayout.addWidget(_scrollArea);

    // compose layouts and add to QMainWindow
    _mainLayout.setStretch(0, 2);
    _mainLayout.addLayout(&_buttonvboxlayout);
    _centralWidget.setLayout(&_mainLayout);
    this->setCentralWidget(&_centralWidget);

    // set cursor properties
    _classifyCursor.setShape(Qt::PointingHandCursor);

    QObject::connect(&_opacSlider, &QSlider::valueChanged,
                     _canvas, &TabletCanvas::setPainterOpacity);

    // connect to OnClick signals of buttons
    QObject::connect(&_vocalFoldsButton, &QPushButton::clicked,
                     this, &MainWindow::onVocalFoldButtonClicked);
    QObject::connect(&_otherTissueButton, &QPushButton::clicked,
                     this, &MainWindow::onOtherTissueButtonClicked);
    QObject::connect(&_tracheaButton, &QPushButton::clicked,
                     this, &MainWindow::onTracheaButtonClicked);
    QObject::connect(&_pathologyButton, &QPushButton::clicked,
                     this, &MainWindow::onPathologyButtonClicked);
    QObject::connect(&_instrumentButton, &QPushButton::clicked,
                     this, &MainWindow::onInstrumentButtonClicked);
    QObject::connect(&_intubationButton, &QPushButton::clicked,
                     this, &MainWindow::onIntubationButtonClicked);
    QObject::connect(&_laryngoscopeButton, &QPushButton::clicked,
                     this, &MainWindow::onLaryngoscopeButtonClicked);

    QObject::connect(&_fillButton, &QPushButton::clicked,
                     _canvas, &TabletCanvas::onFillButtonClicked);
    QObject::connect(&_clearButton, &QPushButton::clicked,
                     this, &MainWindow::onClearButtonClicked);
    QObject::connect(&_saveButton, &QPushButton::clicked,
                     this, &MainWindow::onSaveButtonClicked);
}

MainWindow::~MainWindow()
{

}

void MainWindow::resetCursor()
{
    _centralWidget.setCursor(_defaultCursor);
}

void MainWindow::onVocalFoldButtonClicked()
{
    qDebug() << "onVocalFoldButtonClicked";
    _canvas->setColor(_vocalFoldsColor);
}

void MainWindow::onOtherTissueButtonClicked()
{
    qDebug() << "onOtherTissueButtonClicked";
    _canvas->setColor(_otherTissueColor);
}

void MainWindow::onTracheaButtonClicked()
{
    qDebug() << "onTracheaButtonClicked";
    _canvas->setColor(_tracheaColor);
}

void MainWindow::onPathologyButtonClicked()
{
    qDebug() << "onPathologyButtonClicked";
    _canvas->setColor(_pathologyColor);
}

void MainWindow::onInstrumentButtonClicked()
{
    qDebug() << "onInstrumentButtonClicked";
    _canvas->setColor(_instrumentColor);
}

void MainWindow::onIntubationButtonClicked()
{
    qDebug() << "onIntubationButtonClicked";
    _canvas->setColor(_intubationColor);
}

void MainWindow::onLaryngoscopeButtonClicked()
{
    qDebug() << "onLaryngoscopeButtonClicked";
    _canvas->setColor(_laryngoscopeColor);
}

void MainWindow::onClearButtonClicked()
{
    qDebug() << "onClearButtonClicked";
    _canvas->clearAnnotImage();
}

void MainWindow::onSaveButtonClicked()
{
    qDebug() << "onSaveButtonClicked";
    if (!_todoImages.empty())
    {
        QUrl origFile = _todoImages.first();
        _todoImages.pop_front();

        QString annotFile = "/home/max-heinrich/Hacking/build-vocal_fold_segmentation-Desktop_Qt_5_8_0_clang_64bit-Debug/trainannot/" + origFile.fileName();

        qDebug() << annotFile;

        _canvas->saveAnnotImage(annotFile);

        // load next image
        if (!_todoImages.empty())
        {
            _canvas->loadImage(_todoImages.first().toLocalFile());
        }
    }
    else
    {
        qDebug() << "no new images found in ./train!";
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        _canvas->zoomIn();
        break;
    case Qt::Key_Minus:
        _canvas->zoomOut();
        break;
    case Qt::Key_R:
        _canvas->resetZoom();
        break;
    case Qt::Key_X:
        _canvas->zoomIn();
        break;
    case Qt::Key_Y:
        _canvas->zoomOut();
        break;
    case Qt::Key_A:
        _opacSlider.setValue(_opacSlider.value() + 10);
        break;
    case Qt::Key_S:
        _opacSlider.setValue(_opacSlider.value() - 10);
        break;
    }
}
