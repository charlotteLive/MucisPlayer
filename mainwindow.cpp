#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    playState(false)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);
    player->setPlaylist(playList);

    connect(ui->volume,SIGNAL(valueChanged(int)),player,SLOT(setVolume(int)));
    connect(ui->lastSong,SIGNAL(clicked()),playList,SLOT(previous()));
    connect(ui->nextSong,SIGNAL(clicked()),playList,SLOT(next()));
    connect(playList,SIGNAL(currentIndexChanged(int)),this,SLOT(currentSongChanged(int)));

    connect(ui->close,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->minimize,SIGNAL(clicked()),this,SLOT(showMinimized()));

    player->setVolume(80);
    ui->volume->setValue(80);
    playList->setPlaybackMode(QMediaPlaylist::Loop);
    ui->centralWidget->setStyleSheet("border-image: url(:/background/picture/cat.jpg)");
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
        ui->playState->setText(QStringLiteral("暂停"));
        playState = true;
    }
    else
    {
        player->pause();
        ui->playState->setText(QStringLiteral("播放"));
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
                ui->playerList->addItem(name);
            }
        }
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
    ui->playerList->setCurrentRow(index);
    QString currentSong = ui->playerList->currentItem()->text();
    ui->songName->setText(currentSong);
}
