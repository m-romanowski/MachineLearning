#include "mainwindow.h"

#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      outputLabel(nullptr),
      resultOutput(nullptr),
      grid(nullptr),
      plotWidget(nullptr),
      settings(new Settings(this)),
      learned(false)
{
    // Main window settings.
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    // Initialize MainWindow GUI.
    init();
    newGrid();

    connect(settings, &Settings::newSettings, this, [=](SettingsData _data) {
        data = std::move(_data);
    });

    data = settings->getSettingsData();
}

MainWindow::~MainWindow()
{
    // Clear data.
    if(!trainingItems.empty())
        for(auto& item : trainingItems)
            delete item;

    if(!perceptrons.empty())
        perceptrons.clear();

    // Settings dialog handler.
    delete settings;

    // Menus, Actions.
    delete startLearningAction;
    delete clearSceneAction;
    delete saveFileAction;
    delete openFileAction;
    delete openSettingsDialogAction;
    delete openMenu;
    delete editMenu;
    delete learnMenu;
    delete settingsMenu;
    delete menuBar;

    // Window GUI
    delete outputLabel;
    delete resultOutput;
    delete sideLayout;
    delete grid;
    delete plotWidget;
    delete layout;
    delete mainLayout;
}

void MainWindow::init()
{
    // Central layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout;
    layout = new QHBoxLayout;
    mainLayout->addLayout(layout);
    sideLayout = new QVBoxLayout;

    // Menu bar
    menuBar = new QMenuBar(this);
    createMenus();
    createActions();
    createShortcuts();
    this->setMenuBar(menuBar);

    // Status bar
    statusBar = new QStatusBar(this);
    statusBarLabel = new QLabel;
    statusBar->addPermanentWidget(statusBarLabel);
    this->setStatusBar(statusBar);

    connect(this, &MainWindow::msg, this, &MainWindow::finishedLearning, Qt::QueuedConnection);

    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);
}

void MainWindow::updateGUI()
{    
    if(outputLabel != nullptr || resultOutput != nullptr)
        return;

    // Side layout
    outputLabel = new QLabel(tr("Perceptrons output"));
    sideLayout->addWidget(outputLabel);

    resultOutput = new QTreeWidget;
    resultOutput->setColumnCount(2);
    resultOutput->setColumnWidth(0, 50);

    QStringList columnHeaders;
    columnHeaders << "ID" << "Value";
    resultOutput->setHeaderLabels(columnHeaders);

    resultOutput->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    resultOutput->setMaximumWidth(180);
    sideLayout->addWidget(resultOutput);

    connect(resultOutput, &QTreeWidget::itemClicked, this, &MainWindow::perceptronItemClicked);

    layout->addLayout(sideLayout);
}

void MainWindow::createMenus()
{
    openMenu = new QMenu(tr("Open"));
    menuBar->addMenu(openMenu);

    learnMenu = new QMenu(tr("Learn"));
    menuBar->addMenu(learnMenu);

    editMenu = new QMenu(tr("Edit"));
    menuBar->addMenu(editMenu);

    settingsMenu = new QMenu(tr("Settings"));
    menuBar->addMenu(settingsMenu);
}

void MainWindow::createActions()
{
    newGridLayout = new QAction(tr("New pixel layout"), this);
    connect(newGridLayout, &QAction::triggered, [this] {
        newGrid();
    });
    openMenu->addAction(newGridLayout);

    openFileAction = new QAction(tr("Open file"), this);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);
    openMenu->addAction(openFileAction);

    saveFileAction = new QAction(tr("Save"), this);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
    openMenu->addAction(saveFileAction);

    clearSceneAction = new QAction(tr("Clear scene"), this);
    connect(clearSceneAction, &QAction::triggered, this, &MainWindow::clearScene);
    clearSceneAction->setEnabled(false);
    editMenu->addAction(clearSceneAction);

    startLearningAction = new QAction(tr("Start learning"), this);
    connect(startLearningAction, &QAction::triggered, this, &MainWindow::startLearning);
    learnMenu->addAction(startLearningAction);

    openSettingsDialogAction = new QAction(tr("Open settings"), this);
    connect(openSettingsDialogAction, &QAction::triggered, this, &MainWindow::openSettingsDialog);
    settingsMenu->addAction(openSettingsDialogAction);
}

void MainWindow::createShortcuts()
{
    printDataSC = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(printDataSC, &QShortcut::activated, this, &MainWindow::printData);
}

void MainWindow::newGrid(int _width, int _height)
{
    if(grid != nullptr)
        return;

    grid = new PixelGrid(_width, _height);
    connect(grid, &PixelGrid::gridUpdated, this, &MainWindow::checkInPerceptron);
    layout->addLayout(grid);

    clearSceneAction->setEnabled(true);
    newGridLayout->setEnabled(false);
    updateGUI();
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());

    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        int id = 0;
        while(!in.atEnd())
        {
            QString line = in.readLine();
            QStringList list = line.split(",");

            std::vector<double> weights;
            for(auto& item : list)
            {
                if(item != "")
                    weights.push_back(item.toDouble());
            }

            auto* perceptron = new Adaline;
            perceptron->setWeights(weights);
            perceptrons.emplace_back(id, perceptron);

            id++;
        }

        if(id != 0)
            learned = true;
    }
}

void MainWindow::saveFile()
{
    if(perceptrons.empty())
        return;

    QString filename = QFileDialog::getSaveFileName(this, tr("Save result"), QDir::homePath(),
        tr("Perceptron Files (*.per)"));

    if(!filename.endsWith(".per"))
        filename.append(".per");

    QFile file(filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        for(auto& perceptron : perceptrons)
            out << QString::fromStdString(perceptron.second->toString(perceptron.second->weights())) << "\n";
    }

    file.close();
}

void MainWindow::clearScene()
{
    grid->clearGrid();
}

void MainWindow::loadData(const QString& _path)
{
    // Read data.
    QDir dir(_path);
    QStringList images = dir.entryList(QStringList() << "*.jpg" << "*.jpeg" << "*.png", QDir::Files);

    if(images.empty())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Information"));
        msgBox.setText(tr("Directory is empty"));
        msgBox.exec();

        return;
    }

    for(auto& image : images)
    {
        QImage img;
        img.load(_path + "/" + image);

        auto* item = new TrainingItem;
        item->toBits(&img);
        item->setLabel(image.split(".")[0].toInt());
        trainingItems.push_back(item);
    }
}

void MainWindow::startLearning()
{
    // Disable draw widget until the app learns.
    centralWidget->setEnabled(false);
    startLearningAction->setEnabled(false);
    openFileAction->setEnabled(false);
    saveFileAction->setEnabled(false);

    perceptrons.clear();
    trainingItems.clear();
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::learn);
}

void MainWindow::finishedLearning(const QString& _msg, bool _done)
{
    statusBarLabel->setText(_msg);

    if(_done)
    {
        centralWidget->setEnabled(true);
        startLearningAction->setEnabled(true);
        openFileAction->setEnabled(true);
        saveFileAction->setEnabled(true);

        learned = true;

        for(auto& perceptron : perceptrons)
        {
            auto* item = new QTreeWidgetItem;
            item->setText(0, QString::number(perceptron.first));
            item->setText(1, QString::number(0));
            resultOutput->addTopLevelItem(item);
        }
    }
}

void MainWindow::learn()
{
    // Prepare data
    emit msg("Loading data...");
    loadData(data.trainingDataDir);

    // Generate random noises for each loaded image.
    emit msg("Generating random data...");
    if(data.enableRandom)
    {
        for(size_t i = 0; i < trainingItems.size(); ++i)
        {
            std::vector<double*> newItem;
            newItem = Util::randomNoise(trainingItems[i]->data(),
                static_cast<size_t>(trainingItems[i]->size()), static_cast<size_t>(data.noiseItemCount),
                static_cast<unsigned int>(data.noiseProbability));

            if(!newItem.empty())
            {
                for(size_t j = 0; j < newItem.size(); ++j)
                {
                    auto* trainingItem = new TrainingItem;
                    trainingItem->setData(newItem[j], trainingItems[i]->size());
                    trainingItem->setLabel(trainingItems[i]->label());

                    trainingItems.push_back(trainingItem);
                }
            }
        }
    }

    // DFT
    emit msg("Generating DFT...");
    for(auto& item : trainingItems)
    {
        std::vector<std::complex<double>> itemDFT = Util::DFT(Util::toComplex(item->data(),
            static_cast<size_t>(item->size())));

        auto* newArr = new double[static_cast<unsigned>(item->size() * 2)];
        for(int i = 0; i < item->size() * 2; i++)
        {
            if(i < item->size())
                newArr[i] = item->data()[i];

            if(i >= item->size())
                newArr[i] = Util::DFTRealValues(itemDFT)[i];
        }

        item->setData(newArr, item->size() * 2);
    }

    // Learning
    emit msg("Learning...");

    clock_t beginTime = clock();

    for(unsigned int i = 0; i < PERCEPTRON_NUM; i++)
        perceptrons.emplace_back(i, new Adaline);

    for(size_t i = 0; i < perceptrons.size(); i++)
        perceptrons[i].second->learn(trainingItems, i, MAX_TRAINING_EPOCHS);

    emit msg("Done, training time: " + QString::number(static_cast<double>(float(clock() - beginTime) / CLOCKS_PER_SEC))
        + "s.", true);
}

void MainWindow::checkInPerceptron()
{
    if(!learned)
        return;

    bool** grid2D = grid->grid();
    auto* grid1D = new double[static_cast<unsigned>(grid->gridWidth() * grid->gridHeight())];

    // Convert 2d array to 1d.
    for(int i = 0; i < grid->gridWidth(); i++)
    {
        for(int j = 0; j < grid->gridHeight(); j++)
            grid1D[i * grid->gridHeight() + j] = static_cast<double>(grid2D[i][j]);
    }

    // Check "image" for each perceptron.
    for(auto& perceptron : perceptrons)
    {
        // Get perceptron value and update QTreeWidget.
        int value = perceptron.second->predict(grid1D);
        QTreeWidgetItem* item = resultOutput->topLevelItem(perceptron.first);

        if(value == 1)
        {
            item->setText(1, QString::number(1));
            item->setBackground(0, QBrush(QColor(63, 191, 127)));
            item->setBackground(1, QBrush(QColor(63, 191, 127)));
        }
        else
        {
            item->setText(1, QString::number(0));
            item->setBackground(0, QBrush(Qt::white));
            item->setBackground(1, QBrush(Qt::white));
        }
    }
}

void MainWindow::perceptronItemClicked(QTreeWidgetItem* _item, int _column)
{
    Q_UNUSED(_item);
    Q_UNUSED(_column);

    delete plotWidget;

    auto idx = static_cast<size_t>(resultOutput->indexOfTopLevelItem(resultOutput->currentItem()));

    int size = perceptrons[idx].second->errors().size();
    QVector<double> keys(size), values(size);

    for(int i = 0; i < size; i++)
    {
        keys[i] = static_cast<double>(i);
        values[i] = perceptrons[idx].second->errors()[i];
    }

    auto max = *std::max(perceptrons[idx].second->errors().begin(),
        perceptrons[idx].second->errors().end());
    auto min = *std::min(perceptrons[idx].second->errors().begin(),
        perceptrons[idx].second->errors().end());

    plotWidget = new CustomPlot;
    plotWidget->addGraph();
    plotWidget->graph(0)->setData(keys, values);
    plotWidget->xAxis->setLabel("epoch");
    plotWidget->yAxis->setLabel("error");
    plotWidget->xAxis->setRange(0, size);
    plotWidget->yAxis->setRange(max, min);
    plotWidget->replot();

    mainLayout->addWidget(plotWidget);
}

void MainWindow::openSettingsDialog()
{
    settings->show();
}

void MainWindow::printData()
{
#ifdef QT_DEBUG
    for(auto& item : trainingItems)
    {
        qDebug() << *item;
    }
#endif
}
