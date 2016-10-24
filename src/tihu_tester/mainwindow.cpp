#include "mainwindow.h"

#include <QtConcurrent\QtConcurrent>
#include <QDirIterator>
#include <QMessageBox>
#include <QtWidgets>
#include <QAction>

#include "../tihu/tihu.h"

#define TIHU        "Tihu"
#define APP_NAME    "Tihu Tester"

bool CompareTestCase( const TestCase& t1, const TestCase& t2 )
{
    return ( t1.ResultPath < t2.ResultPath );
}


TIHU_CALLBACK_RETURN call_back(TIHU_CALLBACK_MESSAGE message, int l_param, int w_param, void* data)
{
    Q_UNUSED(message);
    Q_UNUSED(l_param);
    Q_UNUSED(w_param);
    Q_UNUSED(data);

    return TIHU_DATA_PROCESSED;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    centralWidget = new QWidget(this);
    statusBar = new QStatusBar(this);
    layoutVertical = new QVBoxLayout(centralWidget);
    layoutHorizontal = new QHBoxLayout(centralWidget);
    labelTestFolder = new QLabel(centralWidget);
    editTestFolder = new QLineEdit(centralWidget);
    tableResults = new CResultTable(centralWidget);
    

    labelTestFolder->setText("Tests Folder: ");

    layoutVertical->setSpacing(6);
    layoutHorizontal->setSpacing(6);

    layoutVertical->addLayout(layoutHorizontal);
    layoutVertical->addWidget(tableResults);

    setStatusBar(statusBar);
    setCentralWidget(centralWidget);

    CreateActions();
    CreateMenus();
    CreateToolBars();
    CreateStatusBar();

    ReadSettings();

    tableResults->setModel(&modelResults);

    modelResults.setHorizontalHeaderItem(0, new QStandardItem("Test Name"));
    modelResults.setHorizontalHeaderItem(1, new QStandardItem("Result"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    SaveSettings();
    event->accept();
}

void MainWindow::CreateActions()
{
    actExit = new QAction(tr("E&xit"), this);
    actExit->setShortcut(QKeySequence::Quit);
    actExit->setStatusTip(tr("Exit the application"));
    connect(actExit, SIGNAL(triggered()), this, SLOT(close()));

    actAbout = new QAction(tr("&About"), this);
    actAbout->setStatusTip(tr("Show the application's About box"));
    connect(actAbout, SIGNAL(triggered()), this, SLOT(About()));

    actRun = new QAction(tr("&Run Tests"), this);
    actRun->setShortcut(QKeySequence("F5"));
    actRun->setIcon(QIcon(":/images/run.png"));
    actRun->setStatusTip(tr("Run all test cases"));
    connect(actRun, SIGNAL(triggered()), this, SLOT(OnRunTests()));

    actOpenDir = new QAction(tr("Open "), this);
    actOpenDir->setStatusTip(tr("Run all test cases"));
    actOpenDir->setIcon(QIcon(":/images/open.png"));
    connect(actOpenDir, SIGNAL(triggered()), this, SLOT(ChooseFolder()));
}

void MainWindow::CreateMenus()
{
    menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actOpenDir);
    menuFile->addAction(actRun);
    menuFile->addSeparator();
    menuFile->addAction(actExit);

    menuBar()->addSeparator();

    menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->addAction(actAbout);
}

void MainWindow::CreateToolBars()
{
    mainToolBar = addToolBar(tr("Main"));
    mainToolBar->addAction(actRun);
    mainToolBar->addSeparator();

    mainToolBar->addWidget(labelTestFolder);
    mainToolBar->addWidget(editTestFolder);
    mainToolBar->addAction(actOpenDir);
}

void MainWindow::CreateStatusBar()
{
    statusBar->showMessage(tr("Ready"));
}

void MainWindow::ReadSettings()
{
    QSettings settings(TIHU, APP_NAME);
    QString test_folder = settings.value("test_folder").toString();
    QByteArray geometry = settings.value("geometry").toByteArray();

    LoadTestCases(test_folder);
    restoreGeometry(geometry);
}

void MainWindow::SaveSettings()
{
    QSettings settings(TIHU, APP_NAME);
    settings.setValue("test_folder", editTestFolder->text());
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::About()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("Tihu Tester Application.\n"));
}

void MainWindow::ChooseFolder()
{
    QString dir = QFileDialog::getExistingDirectory (this, APP_NAME);

    if(!dir.isEmpty())
    {
        LoadTestCases(dir);
    }
}

void MainWindow::LoadTestCases(QString test_path)
{
    TestCases.clear();
    modelResults.removeRows( 0, modelResults.rowCount() );

    editTestFolder->setText(test_path);

    int case_no = 0;
    QDirIterator it(test_path, QDirIterator::NoIteratorFlags);
    
    while (it.hasNext())
    {
        QString dir_path = it.next();
        QDir dir(dir_path);

        if(dir.dirName() == "." ||
           dir.dirName() == "..")
            continue;

        QString test_path   = dir_path + QDir::separator() + "text.txt";
        QString label_path  = dir_path + QDir::separator() + "text.lbl";
        QString result_path = dir_path + QDir::separator() + "run.lbl";

        QFile test_file(test_path);

        test_file.open(QIODevice::ReadOnly);

        if(!test_file.isOpen())
            continue;

        QTextStream stream(&test_file);
        stream.setCodec("UTF-8");

        QString test_name = stream.readLine().remove("#Test Name: ");

        /*QString test_res1 = */stream.readLine(); /// option 1 not_used
        /*QString test_res2 = */stream.readLine(); /// option 2 not_used
        /*QString test_res3 = */stream.readLine(); /// option 3 not_used
        /*QString test_res4 = */stream.readLine(); /// option 4 not_used
        /*QString test_res4 = */stream.readLine(); /// option 5 not_used

        QString test_text = stream.readAll().trimmed();

        test_text = test_text.replace("\r\n", "\n");
        test_text = test_text.trimmed();

        TestCase test_case;

        test_case.Number     = case_no++;
        test_case.Name       = test_name;
        test_case.Text       = test_text;
        test_case.LabelPath  = label_path;
        test_case.ResultPath = result_path;

        TestCases.append(test_case);
    }

    qSort( TestCases.begin(), TestCases.end(), CompareTestCase );

    QVectorIterator<TestCase> iter(TestCases);
    QBrush gray_brush(QColor(0xEE, 0xEE, 0xEE));
    QStandardItem *item = 0;
    int row_no = 0;
    
    while(iter.hasNext())
    {
        TestCase test_case = iter.next();
        row_no = test_case.Number;

        /// test name
        item = new QStandardItem(test_case.Name);
        modelResults.setItem(row_no, 0, item);

        /// result
        item = new QStandardItem("NA");
        item->setBackground(gray_brush);
        modelResults.setItem(row_no, 1, item);
        ResultColumn = 1;
    }
}

void MainWindow::OnFinish()
{
    
}

void MainWindow::OnRunTests()
{
    // Create a progress dialog.
    QProgressDialog dialog;
    dialog.setLabelText("Running Tests...");
    dialog.setCancelButtonText(tr("&Cancel"));
    dialog.setRange(0, TestCases.size());
    dialog.setWindowTitle(tr("Running Tests..."));
    dialog.show();

    // Create a QFutureWatcher and connect signals and slots.
    QFutureWatcher<void> futureWatcher;
    QObject::connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));
    QObject::connect(&dialog, SIGNAL(canceled()), &futureWatcher, SLOT(cancel()));
    QObject::connect(&futureWatcher, SIGNAL(progressRangeChanged(int,int)), &dialog, SLOT(setRange(int,int)));
    QObject::connect(&futureWatcher, SIGNAL(progressValueChanged(int)), &dialog, SLOT(setValue(int)));

    for(auto test_case : TestCases)
    {
        RunTest(test_case);
    }

    // Start the computation.
    QFuture<void> future = QtConcurrent::map(TestCases, [this] (TestCase const& test_case) { RunTest(test_case); });

    futureWatcher.setFuture(future);
    
    // Display the dialog and start the event loop.
    dialog.exec();
}

void MainWindow::RunTest(const TestCase& test_case)
{
    QMutexLocker locker(&mutex);

    bool initialized = tihu_Init();
    if (!initialized)
    {
        QMessageBox::critical(this, APP_NAME, "Loading Tihu failed.");
        return;
    }

    tihu_LoadVoice(TIHU_VOICE_MBROLA_MALE);
    tihu_SetCallback(call_back, this);

    int row_no = test_case.Number;

    /// run test
    tihu_Speak(test_case.Text.toUtf8().data());

    tihu_Dump(test_case.ResultPath.toStdString().c_str());

    bool status = Compare(test_case.LabelPath, test_case.ResultPath);

    QStandardItem *item = modelResults.item(row_no, ResultColumn);

    if (status)
    {
        item->setText("Passed");
        item->setBackground( QColor(0, 255, 0) );
    }
    else
    {
        item->setText("Failed");
        item->setBackground( QColor(255, 0, 0) );
    }

    tihu_Close();

    Q_EMIT modelResults.dataChanged(modelResults.index(row_no, 2), modelResults.index(row_no, 6));
}

bool MainWindow::Compare(const QString &file1, const QString &file2)
{
    QFile f1(file1);
    if(!f1.open(QIODevice::ReadOnly))
        return false;

    QFile f2(file2);
    if(!f2.open(QIODevice::ReadOnly))
        return false;

    QString text1 = f1.readAll();
    QString text2 = f2.readAll();

    text1.remove(' ');
    text2.remove(' ');

    text1 = text1.trimmed();
    text2 = text2.trimmed();

    return text1 == text2;
}

