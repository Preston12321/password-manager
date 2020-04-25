#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QObject>

class AudioController : public QObject {
    Q_OBJECT
public:
    explicit AudioController(QObject *parent = nullptr);

    bool isPlaying();

public slots:
    void beginPlayback();
    void stopPlayback();
    void changePlaylist(const int &playlist);

private:
    QMediaPlayer *player;
    QList<QMediaPlaylist *> playlists;
    int currentPlaylist;
};

#endif  // AUDIOMANAGER_H
