#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QString>
#include <QMouseEvent>
#include <Windows.h>
#include <QStringList>
#include <QStringListModel>

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

    void on_removeSong_clicked();

    void on_durationChanged(qint64 );

    void on_positionChanged(qint64);

    void on_progressSliderMoved(int);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playList;
    QStringListModel *songs;
    QStringList songsStrings;
    bool playState;
};

#endif // MAINWINDOW_H
