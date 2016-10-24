#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "result_table.h"

#include <QMainWindow>
#include <QtWidgets>
#include <QtXml/QDomDocument>

struct TestCase
{
    int Number;

    QString Name;
    QString Text;

    QString LabelPath;
    QString ResultPath;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

protected:
    void closeEvent(QCloseEvent *event);
    void RunTest(const TestCase& test_case);

public slots:
    void ChooseFolder();
    void OnRunTests();
    void About();
    void OnFinish();


private:
    void CreateActions();
    void CreateMenus();
    void CreateToolBars();
    void CreateStatusBar();

    void ReadSettings();
    void SaveSettings();

    /// comparing files
    bool Compare(const QString &file1, const QString &file2);

    void LoadTestCases(QString path);

private:
    QWidget *centralWidget;
    QAction *actAbout;
    QAction *actExit;
    QAction *actRun;
    QAction *actOpenDir;
    QMenu *menuFile;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QVBoxLayout *layoutVertical;
    QHBoxLayout *layoutHorizontal;
    QLabel *labelTestFolder;
    QLineEdit *editTestFolder;
    CResultTable *tableResults;

    int ResultColumn;

    QStandardItemModel modelResults;

    QVector<TestCase> TestCases;

    QMutex mutex;
};

#endif // MAINWINDOW_H
