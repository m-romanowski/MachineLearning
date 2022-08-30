#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pixelgrid.h"
#include "settings.h"
#include "trainingitem.h"
#include "perceptron.h"
#include "util.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QDir>
#include <QTreeWidget>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QShortcut>
#include <QMessageBox>
#include <QList>
#include <QDebug>

#include <fstream>
#include <ctime>
#include <cstdlib>
#include <utility>

#define MAX_IT 100000
#define PERCEPTRON_NUM 10

enum SceneType
{
    GRID,
    IMAGE // Not supported
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void printData();
    void loadData(const QString& _path, const QString& _sep = " ");

private slots:
    void newGrid(int _width = 7, int _height = 5);
    void openFile();
    void saveFile();
    void clearScene();
    void startLearning();
    void openSettingsDialog();
    void checkInPerceptron();

private:
    // GUI
    void init();
    void loadRestGUI();
    void createMenus();
    void createActions();
    void createShortcuts();

    bool checkPerceptronsStatus();
    void setPerceptronsStatus(bool _status);

    // MainWindow GUI
    QWidget* centralWidget;
    QHBoxLayout* layout;
    QVBoxLayout* sideLayout;
    QLabel* outputLabel;
    QTreeWidget* resultOutput;

    // Menu bar
    QMenuBar* menuBar;

    QMenu* openMenu;
    QAction* newGridLayout;
    QAction* openFileAction;
    QAction* saveFileAction;

    QMenu* editMenu;
    QAction* clearSceneAction;

    QMenu* learnMenu;
    QAction* startLearningAction;

    QMenu* settingsMenu;
    QAction* openSettingsDialogAction;

    // Shortcuts
    QShortcut* printDataSC;

    // Status bar
    QStatusBar* statusBar;
    QLabel* statusBarLabel;

    // Scene
    PixelGrid* grid;
    SceneType type;

    // Dialog
    Settings* settings;
    SettingsData data;

    // Learning
    std::vector<std::pair<int, Perceptron*>> perceptrons;
    std::vector<TrainingItem*> items;
    bool learned;
};

#endif // MAINWINDOW_H
