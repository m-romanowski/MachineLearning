#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      viewEdited(false),
      tryNext(false)
{
    // Main window settings.
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    // Read settings from the file if exists.
    auto* settings = new Settings;
    connect(settings, &Settings::newSettings, this, &MainWindow::saveNewSettings);
    settings->readSettingsFile();
    delete settings;

    // Initialize GUI.
    initGUI();
}

MainWindow::~MainWindow()
{
    delete inView;
    delete outView;

    delete fileMenu;
    delete newImageAction;
    delete openFileAction;
    delete saveFileAction;
    delete editMenu;
    delete zoomInAction;
    delete zoomOutAction;
    delete addRandomNoiseAction;
    delete clearViewsAction;
    delete settingsMenu;
    delete openSettingsAction;
    delete menuBar;

    delete infoLabel;
    delete statusLabel;
    delete statusBar;
    delete mainLayout;
    delete centralWidget;
}

void MainWindow::initGUI()
{
    // Main layout.
    centralWidget = new QWidget;

    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(5);
    centralWidget->setLayout(mainLayout);

    // Views
    inView = new GraphicsViewArea;
    inView->setVisible(false);
    inView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(inView);

    outView = new GraphicsViewArea;
    outView->setVisible(false);
    outView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(outView);

    // Menu bar
    menuBar = new QMenuBar;
    this->setMenuBar(menuBar);
    createMenus();
    createActions();

    // Status bar.
    statusBar = new QStatusBar;
    statusLabel = new QLabel;
    statusBar->addPermanentWidget(statusLabel);
    infoLabel = new QLabel;
    statusBar->addWidget(infoLabel);

    connect(this, &MainWindow::msg, this, &MainWindow::finishedLearning, Qt::QueuedConnection);

    this->setStatusBar(statusBar);
    this->setCentralWidget(centralWidget);
}

void MainWindow::updateGUI(const QImage& _img)
{
    inView->setView(_img);
    inView->setVisible(true);
    updateGraphicsViewSettings(data.penWeight, data.penColor, data.blockResizing, inView->view());

    outView->setView(inView->view()->imageSize());
    outView->setVisible(true);
    outView->view()->setEnabled(false);
    updateGraphicsViewSettings(data.penWeight, data.penColor, data.blockResizing, outView->view());

    connect(inView->view(), &GraphicsView::mouseMoved, this, &MainWindow::updateMousePos);

    saveFileAction->setEnabled(true);
    zoomInAction->setEnabled(true);
    zoomOutAction->setEnabled(true);
    clearViewsAction->setEnabled(true);
    addRandomNoiseAction->setEnabled(true);
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("File"));
    menuBar->addMenu(fileMenu);

    editMenu = new QMenu(tr("Edit"));
    menuBar->addMenu(editMenu);

    settingsMenu = new QMenu(tr("Settings"));
    menuBar->addMenu(settingsMenu);
}

void MainWindow::createActions()
{
    newImageAction = new QAction(tr("New image"));
    newImageAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(newImageAction, &QAction::triggered, this, &MainWindow::newImage);
    fileMenu->addAction(newImageAction);

    openFileAction = new QAction(tr("Open file"));
    openFileAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openFileAction);

    saveFileAction = new QAction(tr("Save as"));
    saveFileAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    saveFileAction->setEnabled(false);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveFileAction);

    zoomInAction = new QAction(tr("Zoom in"));
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    zoomInAction->setEnabled(false);
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    editMenu->addAction(zoomInAction);

    zoomOutAction = new QAction(tr("Zoom out"));
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    zoomOutAction->setEnabled(false);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    editMenu->addAction(zoomOutAction);

    learnAction = new QAction(tr("Start learning"));
    learnAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    learnAction->setEnabled(true);
    connect(learnAction, &QAction::triggered, this, &MainWindow::startLearning);
    editMenu->addAction(learnAction);

    addRandomNoiseAction = new QAction(tr("Add noise"));
    addRandomNoiseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    addRandomNoiseAction->setEnabled(false);
    connect(addRandomNoiseAction, &QAction::triggered, this, &MainWindow::addRandomNoise);
    editMenu->addAction(addRandomNoiseAction);

    removeNoiseAction = new QAction(tr("Remove noise"));
    removeNoiseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    removeNoiseAction->setEnabled(false);
    connect(removeNoiseAction, &QAction::triggered, this, &MainWindow::removeNoise);
    editMenu->addAction(removeNoiseAction);

    clearViewsAction = new QAction(tr("Clear image"));
    clearViewsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    clearViewsAction->setEnabled(false);
    connect(clearViewsAction, &QAction::triggered, this, &MainWindow::clearViews);
    editMenu->addAction(clearViewsAction);

    openSettingsAction = new QAction(tr("Open settings"));
    connect(openSettingsAction, &QAction::triggered, this, &MainWindow::openSettingsDialog);
    settingsMenu->addAction(openSettingsAction);
}

void MainWindow::updateMousePos(QPoint _pos)
{
    statusLabel->setText("Cursor position: (" + QString::number(_pos.x()) + ", " + QString::number(_pos.y())
        + ")");
}

void MainWindow::updateGraphicsViewSettings(int _penWeight, const QColor& _penColor, bool _resizeMode,
    GraphicsView* _view)
{
    if(!_view)
        return;

    _view->setPenColor(_penColor);
    _view->setPenWeight(_penWeight);
    _view->setResizeState(_resizeMode);
}

void MainWindow::newImage()
{
    auto* newViewDialog = new NewViewDialog;

    if(newViewDialog->exec() == QDialog::Accepted)
    {
        QSize size = newViewDialog->getSize();
        QImage newImage = QImage(size, QImage::Format_RGB32);

        updateGUI(newImage);
        tryNext = false;
    }

    delete newViewDialog;
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath());

    if(!filename.isEmpty() && (filename.endsWith(".png") || filename.endsWith(".jpg")
        || filename.endsWith(".jpeg")))
    {
        QImage newImage;
        newImage.load(filename);

        // Convert image to (1 - black, 0 - white color) bitmap.
        // We need remove random color noises (e.g. smooth, gray shades).
        bool* bitmap = GraphicsView::bitmap(&newImage);
        GraphicsView::toBitmapImage(&newImage, bitmap);

        updateGUI(newImage);
        tryNext = false;
    }
}

void MainWindow::saveFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save as"), QDir::currentPath(),
        tr("All files (*.*);;JPEG (*.jpg *.jpeg);;PNG (*.png)"));

    if(filename.isEmpty())
        return;

    inView->view()->image().save(filename);
}

void MainWindow::updateZoomActions()
{
    zoomInAction->setEnabled(inView->view()->scaleFactor() >= 3.0);
    zoomOutAction->setEnabled(inView->view()->scaleFactor() <= 1.0);
    inView->updateScrollArea();
    outView->updateScrollArea();
}

void MainWindow::zoomIn()
{
    inView->view()->zoomIn();
    outView->view()->zoomIn();

    statusLabel->setText("Zoom: " + QString::number(inView->view()->scaleFactor() * 100) + "%");
    updateZoomActions();
}

void MainWindow::zoomOut()
{
    inView->view()->zoomOut();
    outView->view()->zoomOut();

    statusLabel->setText("Zoom: " + QString::number(inView->view()->scaleFactor() * 100) + "%");
    updateZoomActions();
}

void MainWindow::addRandomNoise()
{
    if(!inView)
        return;

    GraphicsView::randomNoise(inView->view(), data.noiseProbability);
    viewEdited = true;
}

void MainWindow::startLearning()
{
    learnAction->setEnabled(false);
    removeNoiseAction->setEnabled(false);
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::learn);
}

void MainWindow::finishedLearning(const QString& _msg, bool _finished)
{
    infoLabel->setText(_msg);

    if(_finished)
    {
        learnAction->setEnabled(true);
        removeNoiseAction->setEnabled(true);
    }
}

void MainWindow::learn()
{
    emit msg("Loading data...");

    // Prepare data
    clock_t beginTime = clock();

    // Read data.
    QDir dir(data.trainingDataDir);
    QStringList images = dir.entryList(QStringList() << "*.jpg" << "*.jpeg" << "*.png", QDir::Files);

    if(images.empty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setText(tr("Directory is empty"));
        msgBox.exec();

        return;
    }

    // Prepare data.
    for(auto& image : images)
    {
        QImage img;
        img.load(data.trainingDataDir + "/" + image);

        for(int i = 0 ; i < img.width() * img.height(); i++)
            linearClassifiers.push_back(new BitmapLC(static_cast<unsigned>(i), img.width(), img.height(), 2));

        trainingItems.push_back(new TrainingItem(TrainingItem::toBits(&img),
            static_cast<unsigned int>(img.width() * img.height())));
    }

    auto dataLoadingEndTime = static_cast<double>(float(clock() - beginTime) / CLOCKS_PER_SEC);
    emit msg("Data loaded. Learning...");

    // Start learning.
    beginTime = clock();

    srand(static_cast<unsigned>(time(nullptr)));
    for(int i = 0; i < MAX_LOOP_IT; i++)
    {
        // Random item
        int idx = rand() % static_cast<int>(trainingItems.size());

        for(auto& classifier : linearClassifiers)
            classifier->learn(trainingItems[static_cast<unsigned>(idx)]);
    }

    emit msg("Data loaded in: " + QString::number(dataLoadingEndTime)
        + "s. Training time: " + QString::number(static_cast<double>(float(clock() - beginTime) / CLOCKS_PER_SEC)) + "s.", true);
}

void MainWindow::removeNoise()
{
    if(!tryNext) tryNext = true;
    else inView->view()->updateImage(outView->view()->bitmap());

    bool* result = new bool[inView->view()->image1DSize()];

    for(unsigned int i = 0; i < inView->view()->image1DSize(); i++)
        result[i] = linearClassifiers[i]->check(inView->view()->bitmap(), inView->view()->image1DSize());

    outView->view()->updateImage(result);
}

void MainWindow::clearViews()
{
    if(inView && outView)
    {
        GraphicsView::clearImage(inView->view()->imagePtr());
        GraphicsView::clearImage(outView->view()->imagePtr());

        inView->view()->updateView();
        outView->view()->updateView();
    }
}

void MainWindow::openSettingsDialog()
{
    auto* settings = new Settings;
    connect(settings, &Settings::newSettings, this, &MainWindow::saveNewSettings);
    settings->exec();

    delete settings;
}

void MainWindow::saveNewSettings(const SettingsData& _data)
{
    data = _data;

    if(inView && outView)
    {
        updateGraphicsViewSettings(data.penWeight, data.penColor, data.blockResizing, inView->view());
        updateGraphicsViewSettings(data.penWeight, data.penColor, data.blockResizing, outView->view());
    }
}
