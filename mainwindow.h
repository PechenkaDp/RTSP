#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startServer();
    void selectFile();
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::MainWindow *ui;
    QProcess *ffmpegProcess;
};

#endif // MAINWINDOW_H
