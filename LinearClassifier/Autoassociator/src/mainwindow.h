#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsviewarea.h"
#include "newviewdialog.h"
#include "settings.h"
#include "perceptron.h"
#include "bitmaplc.h"

#include <QGuiApplication>
#include <QScreen>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include <cstdlib>
#include <ctime>
#include <vector>

#define MAX_LOOP_IT 500

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void msg(const QString& _msg, bool _finished = false);

private slots:
    void updateMousePos(QPoint _pos);
    void newImage();
    void openFile();
    void saveFile();
    void zoomIn();
    void zoomOut();
    void addRandomNoise();
    void removeNoise();
    void clearViews();
    void openSettingsDialog();
    void saveNewSettings(const SettingsData& _data);
    void startLearning();
    void finishedLearning(const QString& _msg, bool _finished = false);

private:
    void initGUI();
    void updateGUI(const QImage& _img);
    void createMenus();
    void createActions();
    void updateGraphicsViewSettings(int _penWeight, const QColor& _penColor, bool _resizeMode,
        GraphicsView* _view);
    void updateZoomActions();
    void learn();

    // Widgets
    QWidget* centralWidget;
    QWidget* scrollWidget;
    QScrollArea* scrollArea;
    QHBoxLayout* mainLayout;

    QStatusBar* statusBar;
    QWidget* statusPermWidget;
    QHBoxLayout* statusPermLayout;
    QLabel* statusLabel, *infoLabel;

    // Menu
    QMenuBar* menuBar;

    QMenu* fileMenu;
    QAction* newImageAction;
    QAction* openFileAction;
    QAction* saveFileAction;

    QMenu* editMenu;
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* learnAction;
    QAction* addRandomNoiseAction;
    QAction* removeNoiseAction;
    QAction* clearViewsAction;

    QMenu* settingsMenu;
    QAction* openSettingsAction;

    // View
    GraphicsViewArea* inView;
    GraphicsViewArea* outView;

    bool viewEdited, tryNext;
    std::vector<BitmapLC*> linearClassifiers;
    std::vector<TrainingItem*> trainingItems;

    // Setting
    SettingsData data;
};


#endif // MAINWINDOW_H
