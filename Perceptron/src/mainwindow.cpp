#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      outputLabel(nullptr),
      resultOutput(nullptr),
      grid(nullptr),
      settings(new Settings(this)),
      learned(false)
{
    srand(static_cast<unsigned>(time(nullptr)));

    // Default window settings.
    this->setMinimumSize(600, 600);

    // Initialize MainWindow GUI.
    init();
    newGrid();

    connect(settings, &Settings::newSettings, this, [=](SettingsData _data) {
        data = _data;
    });

    data = settings->getSettingsData();
}

MainWindow::~MainWindow()
{
    // Clear data.
    if(items.size() > 0)
        for(auto& item : items)
            delete item;

    if(perceptrons.size() > 0)
        perceptrons.clear();

    // Settings dialog handler.
    if(settings != nullptr)
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
    if(outputLabel != nullptr)
        delete outputLabel;

    if(resultOutput != nullptr)
        delete resultOutput;

    delete sideLayout;

    if(grid != nullptr)
        delete grid;

    delete layout;
}

void MainWindow::init()
{
    // Central layout
    centralWidget = new QWidget(this);
    layout = new QHBoxLayout;
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

    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);
}

void MainWindow::loadRestGUI()
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

    type = SceneType::GRID;
    grid = new PixelGrid(_width, _height);
    connect(grid, &PixelGrid::gridUpdated, this, &MainWindow::checkInPerceptron);
    layout->addLayout(grid);

    clearSceneAction->setEnabled(true);
    newGridLayout->setEnabled(false);
    loadRestGUI();
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

            std::vector<float> weights;
            for(auto& item : list)
            {
                if(item != "")
                    weights.push_back(item.toFloat());
            }

            Perceptron* perceptron = new Perceptron;
            perceptron->setWeights(weights);
            perceptrons.push_back(std::make_pair(id, perceptron));

            id++;
        }

        if(id != 0)
            learned = true;
    }
}

void MainWindow::saveFile()
{
    if(perceptrons.size() <= 0)
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
            out << QString::fromStdString(perceptron.second->toString()) << "\n";
    }

    file.close();
}

void MainWindow::clearScene()
{
    if(type == SceneType::GRID)
    {
        grid->clearGrid();

        if(resultOutput != nullptr)
            resultOutput->clear();
    }
    else
        throw std::invalid_argument("Unexpected scene type");
}

void MainWindow::loadData(const QString& _path, const QString& _sep)
{
    // Read data.
    QDir dir(_path);
    QStringList images = dir.entryList(QStringList() << "*.jpg" << "*.jpeg" << "*.png", QDir::Files);

    if(images.size() <= 0)
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

        TrainingItem* item = new TrainingItem;
        item->toBits(&img);
        QStringList filename = image.split(_sep);
        item->setLabel(filename[0].toInt());

        items.push_back(item);
    }
}

bool MainWindow::checkPerceptronsStatus()
{
    for(auto& perceptron : perceptrons)
    {
        if(!perceptron.second->currentStatus())
            return false;
    }

    return true;
}

void MainWindow::setPerceptronsStatus(bool _status)
{
    for(auto& perceptron : perceptrons)
        perceptron.second->setStatus(_status);
}

void MainWindow::startLearning()
{
    // Disable draw widget until the app learns.
    centralWidget->setEnabled(false);
    statusBarLabel->setText(tr("Learning... Please wait."));

    // Enable button if we are after the training.
    saveFileAction->setEnabled(true);

    perceptrons.clear();
    items.clear();

    // Load images and create a new vector of bits arrays.
    loadData(data.trainingDataDir, "_");

    // Generate random noises to each loaded image.
    if(data.enableRandom)
    {
        for(size_t i = 0; i < items.size(); ++i)
        {
            std::vector<bool*> newItem;
            newItem = Util::randomNoise(items[i]->getData(),
                static_cast<size_t>(items[i]->getSize()), static_cast<size_t>(data.noiseItemCount),
                static_cast<unsigned int >(data.noiseProbability));

            if(newItem.size() > 0)
            {
                for(size_t j = 0; j < newItem.size(); ++j)
                {
                    TrainingItem* trainingItem = new TrainingItem;
                    trainingItem->setData(newItem[j]);
                    trainingItem->setLabel(items[i]->getLabel());
                    trainingItem->setSize(items[i]->getSize());

                    items.push_back(trainingItem);
                }
            }
        }
    }

    // Pre-computing
    for(unsigned int i = 0; i < PERCEPTRON_NUM; i++)
        perceptrons.push_back(std::make_pair(i, new Perceptron));

    // Learning
    for(int i = 0; i < MAX_IT; i++)
    {
        // Random item
        int idx = rand() % static_cast<int>(items.size());

        while(!checkPerceptronsStatus())
        {
            for(unsigned int j = 0; j < perceptrons.size(); j++)
            {
                /* trainSPLA(input, expected) / trainPLA(input, expected)
                 * input: bool* (array of image bitmap, 1 - black, 0 - white)
                 * expected: 1 / -1
                 */
                perceptrons[j].second->trainPLA(items[static_cast<unsigned long>(idx)]->getData(),
                    static_cast<int>(j) == items[static_cast<unsigned long>(idx)]->getLabel() ? 1 : -1);
            }
        }

        setPerceptronsStatus(false);
    }

    // TMP: tests
    // TODO: clickable draw area and check result from for each perceptron.
    /*for(auto& perceptron : perceptrons)
        qDebug() << perceptron->check(items[8]->getData());*/

    // Enable "draw" widget after learning.
    statusBarLabel->setText("Done. Waiting for input");
    centralWidget->setEnabled(true);
    learned = true;
}

void MainWindow::checkInPerceptron()
{
    resultOutput->clear();

    bool** grid2D = grid->getGrid();
    bool* grid1D = new bool[grid->getWidth() * grid->getHeight()];

    // Convert 2d array to 1d.
    for(int i = 0; i < grid->getWidth(); i++)
    {
        for(int j = 0; j < grid->getHeight(); j++)
            grid1D[i * grid->getHeight() + j] = grid2D[i][j];
    }

    // Check "image" for each perceptron.
    for(auto& perceptron : perceptrons)
    {
        // Get perceptron value and update QTreeWidget.
        int value = perceptron.second->check(grid1D);

        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, QString::number(perceptron.first));
        item->setText(1, QString::number(value));
        resultOutput->addTopLevelItem(item);

        if(value == 1)
        {
            item->setBackground(0, QBrush(QColor(63, 191, 127)));
            item->setBackground(1, QBrush(QColor(63, 191, 127)));
        }
    }
}

void MainWindow::openSettingsDialog()
{
    settings->show();
}

void MainWindow::printData()
{
#ifdef QT_DEBUG
    for(auto& item : items)
    {
        qDebug() << *item;
    }
#endif
}
