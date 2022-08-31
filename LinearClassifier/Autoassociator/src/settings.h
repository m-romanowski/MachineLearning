#ifndef SETTINGS_H
#define SETTINGS_H

#include "clickablelabel.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QPixmap>
#include <QColorDialog>
#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#define DEFAULT_SETTINGS_FILE_NAME "settings.ini"
#define DEFAULT_TRAINING_DATA_DIR "training_data"

struct SettingsData
{
    int penWeight = 10;
    QColor penColor = Qt::black;
    bool blockResizing = true;
    int noiseProbability = 10;
    QString trainingDataDir = QDir::currentPath() + "/" + DEFAULT_TRAINING_DATA_DIR;

    inline bool operator ==(const SettingsData& _rhs) const
    {
        return penWeight == _rhs.penWeight && penColor == _rhs.penColor
            && blockResizing == _rhs.blockResizing && noiseProbability == _rhs.noiseProbability
            && trainingDataDir == _rhs.trainingDataDir;
    }

    inline bool operator !=(const SettingsData& _rhs) const
    {
        return penWeight != _rhs.penWeight || penColor != _rhs.penColor
            || blockResizing != _rhs.blockResizing || noiseProbability != _rhs.noiseProbability
            || trainingDataDir != _rhs.trainingDataDir;
    }
};

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget* _parent = nullptr);
    ~Settings();

    void readSettingsFile();

signals:
    void newSettings(SettingsData _data);

private slots:
    void penWeightChanged(int _value);
    void blockResizingStateChanged(bool _state);
    void colorPicker();
    void noiseProbabilityChanged(int _value);
    void openDirPath();

private:
    void initGUI();
    void saveSettings();

    // Widgets
    QVBoxLayout* mainLayout;

    QGroupBox* mainOptGroup;
    QGridLayout* mainOptGroupLayout;
    QLabel* blockResizingLabel;
    QCheckBox* blockResizingCheckBox;
    QLabel* trainingDataDirLabel;
    QLineEdit* trainDirPath;
    QPushButton* trainDirChoosePath;
    QLabel* noiseProbabilityLabel;
    QSpinBox* noiseProbabilityBox;

    QGroupBox* editorGroup;
    QGridLayout* editorGroupLayout;
    QLabel* penWeightLabel;
    QSpinBox* penWeightBox;
    QLabel* penColorLabel;
    ClickableLabel* penColorPreview;

    SettingsData data, prevDataState;
};

#endif // SETTINGS_H
