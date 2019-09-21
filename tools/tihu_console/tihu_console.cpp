/*******************************************************************************
* This file is part of Tihu.
*
* Tihu is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Tihu is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Tihu.  If not, see <http://www.gnu.org/licenses/>.
*
* Contributor(s):
*    Mostafa Sedaghat Joo (mostafa.sedaghat@gmail.com)
*
*******************************************************************************/
#include "tihu_console.h"

#include <QFileDialog>
#include <QSettings>
#include <QLibrary>
#include <QMessageBox>
#include <QAudioOutput>
#include <QTextStream>
#include <QCloseEvent>
#include <QDir>

#define APP_NAME "tihu-with-hazm"


TIHU_CALLBACK_RETURN callback(TIHU_CALLBACK_MESSAGE Message, long lParam, long wParam, void* pData) {
    TihuConsole* consol = static_cast<TihuConsole*>(pData);

    switch(Message) {
    case TIHU_WAVE_BUFFER: {
        consol->WriteAudioBuffer((char*)lParam, (int)wParam);
    }break;

    case TIHU_TEXT_TAGS:
    case TIHU_TEXT_MESSAGE:{
        consol->ReportMessage((char*)lParam, wParam);
    }break;

    default:{
    } break;
    }

    return TIHU_DATA_PROCESSED;
}

TihuConsole::TihuConsole(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , m_device(QAudioDeviceInfo::defaultInputDevice()) {
    procInit = nullptr;
    procClose = nullptr;
    procTag = nullptr;
    procSpeak = nullptr;
    procStop = nullptr;
    procSetParam = nullptr;
    procGetParam = nullptr;
    procCallback = nullptr;
    procLastError = nullptr;
    procErrorString = nullptr;
    m_audioOutput = nullptr;
    m_output = nullptr;
    m_rawAudio = nullptr;

    ui.setupUi(this);

    ///ui.txtInput->setAlignment(Qt::AlignRight);
    ///ui.txtMessages->setAlignment(Qt::AlignRight);
    ///

    ui.cbVoice->addItem("Mbrola Male", TIHU_VOICE_MBROLA_MALE);
    ui.cbVoice->addItem("Mbrola Female", TIHU_VOICE_MBROLA_FEMALE);
    ui.cbVoice->addItem("Espeak Male", TIHU_VOICE_ESPEAK_MALE);
    ui.cbVoice->addItem("Espeak Female", TIHU_VOICE_ESPEAK_FEMALE);
    ui.cbVoice->setCurrentIndex(0);

    connect(ui.btnLoad, SIGNAL(clicked()), this, SLOT(onLoad()));
    connect(ui.btnUnload, SIGNAL(clicked()), this, SLOT(onUnload()));
    connect(ui.btnTag, SIGNAL(clicked()), this, SLOT(onTag()));
    connect(ui.btnSpeak, SIGNAL(clicked()), this, SLOT(onSpeak()));
    connect(ui.btnStop, SIGNAL(clicked()), this, SLOT(onStop()));
    connect(ui.btnOpenFile, SIGNAL(clicked()), this, SLOT(onOpenFile()));
    connect(this, SIGNAL(AppendMessage(const QString &)), ui.txtMessages, SLOT(appendPlainText(const QString &)));


    ReadSettings();
    onUnload();

    QSettings settings("Tihu", APP_NAME);
    QString path = settings.value("library_path").toString();

    if(!path.isEmpty())
        LoadTihu(path);
}

TihuConsole::~TihuConsole() {
    onStop();
    onUnload();
}

void TihuConsole::onLoad() {
    QString path = QFileDialog::getOpenFileName(this, tr("Open Tihu Library"), "", "*.so;;*");

    if(path.isEmpty())
        return;

    if(!LoadTihu(path)) {
        QMessageBox::critical(this, "Tihu", "Can not load tihu.");
        return;
    }
}

bool TihuConsole::LoadTihu(const QString& library) {
    onUnload();

    /// set currect directory to load required dlls.
    /// It's necessary  for windows
    QDir dir(library);
    dir.cdUp();
    QDir::setCurrent( dir.absolutePath() );

    QLibrary lib(library);

    if(!lib.load()) {
        qErrnoWarning("Tihu: load library failed. %s", lib.errorString().toStdString().c_str());
        return false;
    }

    procInit = (TIHU_PROC_INIT)lib.resolve("tihu_Init");
    procClose = (TIHU_PROC_CLOSE)lib.resolve("tihu_Close");
    procTag = (TIHU_PROC_TAG)lib.resolve("tihu_Tag");
    procSpeak = (TIHU_PROC_SPEAK)lib.resolve("tihu_Speak");
    procStop = (TIHU_PROC_STOP)lib.resolve("tihu_Stop");
    procSetParam = (TIHU_PROC_SET_PARAM)lib.resolve("tihu_SetParam");
    procGetParam = (TIHU_PROC_GET_PARAM)lib.resolve("tihu_GetParam");
    procCallback = (TIHU_PROC_CALLBACK)lib.resolve("tihu_Callback");
    procLastError = (TIHU_PROC_LAST_ERROR)lib.resolve("tihu_LastError");
    procErrorString = (TIHU_PROC_ERROR_STRING)lib.resolve("tihu_ErrorString");


    if( !procInit           ||
        !procClose          ||
        !procTag            ||
        !procSpeak          ||
        !procStop           ||
        !procSetParam       ||
        !procGetParam       ||
        !procCallback       ||
        !procLastError      ||
        !procErrorString) {
        lib.unload();
        return false;
    }

    bool initialized = procInit();

    if(!initialized) {
        qErrnoWarning("Tihu: load failed: %s.\n", procErrorString(procLastError()));

        onUnload();
        return false;
    }

    QSettings settings("Tihu", APP_NAME);
    settings.setValue("library_path", library);

    procCallback(callback, this);

    ///------------------------------------
    ui.btnLoad->setEnabled(false);
    ui.btnUnload->setEnabled(true);
    ui.btnSpeak->setEnabled(true);

    int frequency = -1;
    procGetParam(TIHU_PARAM_FREQUENCY, frequency);

    m_format.setSampleRate(frequency);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(m_device);
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        //m_format = info.nearestFormat(m_format);
    }

    m_audioOutput = new QAudioOutput(m_format, this);
    m_output = m_audioOutput->start();
    if (!m_output) {
        QMessageBox::critical(this, "Tihu", "Can not initialize audio output");
    }
    m_rawAudio = new QFile("/tmp/tihu_audio.raw");
    m_rawAudio->open(QFile::WriteOnly);

    ui.splitter->setStretchFactor(0, 1);
    ui.splitter->setStretchFactor(1, 0);

    return true;
}

void TihuConsole::closeEvent(QCloseEvent *event) {
    WriteSettings();
    event->accept();
}

void TihuConsole::onUnload() {
    if(procClose) {
        procClose();
    }

    if(m_audioOutput) {
        delete m_audioOutput;
        m_audioOutput = 0;
    }

    if (m_rawAudio) {
        m_rawAudio->close();
        m_rawAudio= 0;
    }

    ui.btnLoad->setEnabled(true);
    ui.btnUnload->setEnabled(false);
    ui.btnSpeak->setEnabled(false);
    ui.btnStop->setEnabled(false);
}

void TihuConsole::onStop() {
    if(procStop)
        procStop();

    if(SpeakFuture.isRunning())
        SpeakFuture.waitForFinished();

    ui.btnStop->setEnabled(false);
}

void TihuConsole::onSpeak() {
    if (m_rawAudio) {
        m_rawAudio ->seek(0);
    }

    ui.btnSpeak->setEnabled(false);
    ui.btnStop->setEnabled(true);
    ui.txtMessages->clear();

    QString text = ui.txtInput->toPlainText();

    SpeakFuture = QtConcurrent::run(this, &TihuConsole::Speak, text);

    WriteSettings();
}

void TihuConsole::onNormalize() {
    AppendMessage("Sorry. Not implemented yet!");
}

void TihuConsole::onTag() {
    ui.txtMessages->clear();

    QString text = ui.txtInput->toPlainText();
    std::string str = text.toStdString();
    procTag(str.c_str());

    WriteSettings();
}

void TihuConsole::Speak(const QString& text) {
    connect(this, SIGNAL(SpeakingFinished()), this, SLOT(onFinishSpeaking()));

    std::string str = text.toStdString();
    TIHU_VOICE voice = (TIHU_VOICE)ui.cbVoice->currentData().toInt();

    procSpeak(str.c_str(), voice);

    Q_EMIT SpeakingFinished();
}

void TihuConsole::ReadSettings() {
    QSettings settings("Tihu", APP_NAME);
    settings.beginGroup("TihuConsole");
    QString text = settings.value("text").toString();
    QByteArray geo_console = settings.value("geo_console").toByteArray();
    QByteArray geo_splitter = settings.value("geo_splitter").toByteArray();
    settings.endGroup();

    ui.txtInput->setPlainText(text);
    ui.splitter->restoreState(geo_splitter);
    restoreGeometry(geo_console);
}

void TihuConsole::WriteSettings() {
    QSettings settings("Tihu", APP_NAME);

    QString text = ui.txtInput->toPlainText();
    text.truncate(1024*4);

    settings.beginGroup("TihuConsole");
    settings.setValue("text", text);
    settings.setValue("geo_console", saveGeometry());
    settings.setValue("geo_splitter", ui.splitter->saveState());
    settings.endGroup();
}

void TihuConsole::onOpenFile() {
    const QString DEFAULT_DIR_KEY("default_dir_text_file");

    QSettings settings("Tihu", APP_NAME);

    QString path = QFileDialog::getOpenFileName(this, tr("Open Text File"),
        settings.value(DEFAULT_DIR_KEY).toString(), "*.txt");

    if(path.isEmpty())
        return;

    QDir dir;
    settings.setValue(DEFAULT_DIR_KEY, dir.absoluteFilePath(path));

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QString text;

    /// read first line
    QString line = file.readLine();
    if(line.startsWith("#Test Name: ")) {
        file.readLine();
        file.readLine();
        file.readLine();
        file.readLine();
        text = file.readAll();
    }
    else
    {
        file.seek(0);
        text = file.readAll();
    }

    ui.txtInput->setPlainText(text);
}

void TihuConsole::WriteAudioBuffer(char* buffer, int length) {
    if(!m_output) {
        qWarning() << "no output io.";
        return;
    }

    if (m_rawAudio) {
        m_rawAudio->write(buffer, length);
    }

    qint64 data_remaining = length; // assign correct value here

    while (data_remaining) {
        qint64 bytes_written = m_output->write(buffer, data_remaining);
        data_remaining -= bytes_written;
        buffer += bytes_written;
    }
}

void TihuConsole::ReportMessage(char* message, int length) {
    QString qmessage = QString::fromUtf8(message, length);

    emit AppendMessage(qmessage);
}

void TihuConsole::onFinishSpeaking() {
    ui.btnSpeak->setEnabled(true);
    ui.btnStop->setEnabled(false);
}

