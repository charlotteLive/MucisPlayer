#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QString>
#include <QMouseEvent>
#include <Windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *);

private slots:
    void on_playState_clicked();

    void on_volume_sliderMoved(int position);

    void on_addSongs_clicked();

    void on_playerList_doubleClicked(const QModelIndex &index);

    void currentSongChanged(int);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playList;
    bool playState;
};

#endif // MAINWINDOW_H
