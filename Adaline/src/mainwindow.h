#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pixelgrid.h"
#include "settings.h"
#include "trainingitem.h"
#include "adaline.h"
#include "customplot.h"
#include "util.h"

#include <QGuiApplication>
#include <QScreen>
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
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include <fstream>
#include <ctime>
#include <cstdlib>
#include <utility>

#define MAX_TRAINING_EPOCHS 1000
#define PERCEPTRON_NUM 10

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void printData();
    void loadData(const QString& _path);

signals:
    void msg(const QString& _msg, bool _done = false);

private slots:
    void newGrid(int _width = 5, int _height = 5);
    void openFile();
    void saveFile();
    void clearScene();
    void startLearning();
    void openSettingsDialog();
    void checkInPerceptron();
    void finishedLearning(const QString& _msg, bool _done);
    void perceptronItemClicked(QTreeWidgetItem* _item, int column);

private:
    // GUI
    void init();
    void updateGUI();
    void createMenus();
    void createActions();
    void createShortcuts();
    void learn();

    // MainWindow GUI
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
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

    // Plots
    CustomPlot* plotWidget;

    // Dialog
    Settings* settings;
    SettingsData data;

    // Learning
    std::vector<std::pair<int, Adaline*>> perceptrons;
    std::vector<TrainingItem*> trainingItems, expectedItems;
    bool learned;
};

#endif // MAINWINDOW_H
