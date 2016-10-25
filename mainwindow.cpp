#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    playState(false)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);
    songs = new QStringListModel(this);
    player->setPlaylist(playList);

    connect(ui->volume,SIGNAL(valueChanged(int)),player,SLOT(setVolume(int)));
    connect(ui->lastSong,SIGNAL(clicked()),playList,SLOT(previous()));
    connect(ui->nextSong,SIGNAL(clicked()),playList,SLOT(next()));
    connect(playList,SIGNAL(currentIndexChanged(int)),this,SLOT(currentSongChanged(int)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(on_durationChanged(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(on_positionChanged(qint64)));
    connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(on_progressSliderMoved(int)));

    connect(ui->close,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->minimize,SIGNAL(clicked()),this,SLOT(showMinimized()));

    player->setVolume(80);
    ui->volume->setValue(80);
    playList->setPlaybackMode(QMediaPlaylist::Loop);
    ui->centralWidget->setStyleSheet("border-image: url(:/background/picture/cat.jpg)");
    ui->playerList->setModel(songs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ReleaseCapture())
      SendMessage(HWND(this->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    event->ignore();
}

void MainWindow::on_playState_clicked()
{
    if(!playState)
    {
        if(!playList->isEmpty()){
            player->play();
        }
        ui->playState->setStyleSheet("border-image: url(:/icon/picture/5.png);");
        playState = true;
    }
    else
    {
        player->pause();
        ui->playState->setStyleSheet("border-image: url(:/icon/picture/6.png);");
        playState = false;
    }
}

void MainWindow::on_volume_sliderMoved(int position)
{
    player->setVolume(position);
}

void MainWindow::on_addSongs_clicked()
{
    QStringList list = QFileDialog::getOpenFileNames(this,QStringLiteral("选择你想播放的音乐"),QDir::homePath(),tr("Music Files (*.mp3 *mav)"));
    if(!list.empty()){
        foreach (QString fileName, list) {
            if(!fileName.isEmpty()){
                playList->addMedia(QUrl::fromLocalFile(fileName));
                QString name = fileName.split("/").last();
                name = name.split(".").first();
                songsStrings.append(name);
            }
        }
        songs->removeRows(0,songs->rowCount());
        songs->setStringList(songsStrings);
    }
    playList->setCurrentIndex(0);
}


void MainWindow::on_playerList_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    playList->setCurrentIndex(row);
    playState = false;
    on_playState_clicked();
}

void MainWindow::currentSongChanged(int index)
{
    ui->playerList->setCurrentIndex(songs->index(index));
    QString currentSong = songs->data(songs->index(index),Qt::DisplayRole).toString();
    ui->songName->setText(currentSong);
}

void MainWindow::on_removeSong_clicked()
{
    int row = ui->playerList->currentIndex().row();
    songs->removeRow(row);
    if(row == playList->currentIndex()){
        playList->next();
        on_playState_clicked();
    }
    playList->removeMedia(row);
}

void MainWindow::on_durationChanged(qint64 duration)
{
    ui->progressSlider->setRange(0,duration);
}

void MainWindow::on_positionChanged(qint64 position)
{
    ui->progressSlider->setValue(position);
    QTime playCurrentTime(0,position/60000,position/1000%60);
    ui->progress->setText(playCurrentTime.toString("mm:ss"));
}

void MainWindow::on_progressSliderMoved(int position)
{
    player->setPosition(position);
    QTime playCurrentTime(0,position/60000,position/1000%60);
    ui->progress->setText(playCurrentTime.toString("mm:ss"));
}
