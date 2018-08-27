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
#ifndef TIHUCONSOLE_H
#define TIHUCONSOLE_H

#include <QtConcurrent/QtConcurrent>
#include <QtWidgets/QMainWindow>
#include <QAudioOutput>
#include <QMenu>

#include "ui_tihu_console.h"

#include "../tihu/tihu.h"


class TihuConsole : public QMainWindow
{
	Q_OBJECT

public:
    TihuConsole(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~TihuConsole();

    void WriteAudioBuffer(char* buffer, int length);
    void ReportMessage(char *message, int length);

private:
    void WriteSettings();
    void ReadSettings();
    void closeEvent(QCloseEvent *event);

    bool LoadTihu(const QString& path);

    void Speak(const QString& text);

private slots:
	void onLoad();
    void onUnload();
	void onSpeak();
    void onAutoTagger();
    void onNormalize();
    void onStop();
    void onOpenFile();
    void onVoiceChange(QAction *action);
    void onFinishSpeaking();

signals:
    void AppendMessage(const QString &message);
    void SpeakingFinished();

private:
    Ui::TihuConsoleClass ui;

    TIHU_PROC_INIT procInit;
    TIHU_PROC_CLOSE procClose;
    TIHU_PROC_SPEAK procSpeak;
    TIHU_PROC_TAGGER procTagger;
    TIHU_PROC_STOP procStop;
    TIHU_PROC_SET_PARAM procSetParam;
    TIHU_PROC_GET_PARAM procGetParam;
    TIHU_PROC_SET_CALLBACK procSetCallback;
    TIHU_PROC_LOAD_VOICE procLoadVoice;

    QFuture<void> SpeakFuture;

    QAudioDeviceInfo m_device;
    QAudioFormat m_format;
    QAudioOutput *m_audioOutput;
    QIODevice *m_output; // not owned
    QFile* m_rawAudio;

    QActionGroup *actionGroup;
};

#endif // TIHUCONSOL_H
