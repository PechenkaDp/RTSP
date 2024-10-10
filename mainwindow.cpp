#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ffmpegProcess(new QProcess(this))
{
    ui->setupUi(this);
    connect(ui->startServerButton, &QPushButton::clicked, this, &MainWindow::startServer);
    connect(ui->selectFileButton, &QPushButton::clicked, this, &MainWindow::selectFile);
    connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::handleProcessFinished);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (ffmpegProcess->state() == QProcess::Running) {
        ffmpegProcess->kill();
    }
}

void MainWindow::startServer()
{
    QString videoPath = ui->videoPathLineEdit->text();
    qDebug() << "Путь к видеофайлу:" << videoPath;
    if (videoPath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, укажите путь к видеофайлу.");
        return;
    }

    QStringList arguments;
    arguments << "-re"
              << "-r" << "60"
              << "-i" << videoPath
              << "-c:v" << "libx264"
              << "-b:v" << "500k"
              << "-f" << "mpegts"
              << QString("udp://%1:%2").arg("239.255.0.1", "1234");

    ffmpegProcess->start("C:\\Users\\dimap\\Downloads\\ffmpeg-2024-10-10-git-0f5592cfc7-essentials_build\\ffmpeg-2024-10-10-git-0f5592cfc7-essentials_build\\bin\\ffmpeg.exe", arguments);

    if (!ffmpegProcess->waitForStarted()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось запустить FFmpeg: " + ffmpegProcess->errorString());
    } else {
        connect(ffmpegProcess, &QProcess::readyReadStandardError, this, [this]() {
            qDebug() << "FFmpeg Error:" << ffmpegProcess->readAllStandardError();
        });

        QMessageBox::information(this, "Успех", QString("Трансляция запущена на адресе: udp://%1:%2").arg("239.255.0.1", "1234"));
    }
}

void MainWindow::selectFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Выберите видеофайл"), "", tr("Video Files (*.mp4 *.mkv *.avi *.mov)"));
    if (!fileName.isEmpty()) {
        ui->videoPathLineEdit->setText(fileName);
    }
}

void MainWindow::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        QMessageBox::critical(this, "Ошибка", "FFmpeg завершился с ошибкой. Код выхода: " + QString::number(exitCode));
    } else {
        QMessageBox::information(this, "Завершено", "Трансляция завершена успешно.");
    }
}
