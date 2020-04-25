#include "audiocontroller.h"

#include <QDir>
#include <QRandomGenerator>

AudioController::AudioController(QObject *parent) : QObject(parent) {
    player = new QMediaPlayer(this);
    player->setVolume(50);

    playlists = QList<QMediaPlaylist *>();
    currentPlaylist = 0;

    QDir dir("./resources/audio/feedbackScale");
    qDebug() << dir.entryList();

    for (int i = 1; i <= 5; i++) {
        if (!dir.cd(QString::number(i))) continue;
        qDebug() << "Searching directory:" << dir.path();

        auto playlist = new QMediaPlaylist(this);
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

        auto entries =
            dir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
        qDebug() << dir.entryList();

        for (auto entry : entries) {
            QString path = dir.absolutePath().append("/").append(entry);
            qDebug() << "Found audio feedback file:" << path;
            playlist->addMedia(QMediaContent(QUrl::fromLocalFile(path)));
        }

        playlist->setCurrentIndex(0);
        playlists.append(playlist);

        dir.cdUp();
    }

    player->setPlaylist(playlists.at(currentPlaylist));
}

bool AudioController::isPlaying() {
    return (player->state() == QMediaPlayer::PlayingState);
}

void AudioController::beginPlayback() {
    player->play();
    qDebug() << "Audio playback begun";
}

void AudioController::stopPlayback() { player->stop(); }

void AudioController::changePlaylist(const int &playlist) {
    if (playlist == currentPlaylist || playlist >= playlists.size() ||
        playlist < 0) {
        return;
    }
    currentPlaylist = playlist;

    auto selected = playlists.at(playlist);

    int high = selected->mediaCount();

    if (high != 0) {
        int index = QRandomGenerator::global()->bounded(high);
        selected->setCurrentIndex(index);
    }

    bool play = isPlaying();
    if (play) stopPlayback();

    player->setPlaylist(selected);

    if (high == 0) return;

    if (play) beginPlayback();
}
