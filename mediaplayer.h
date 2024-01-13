#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QImage>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <QString>
#include <QUrl>
#include <QList>
#include <QMediaMetaData>
class mediaplayer : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(int varName READ varName WRITE setVarName NOTIFY varNameChanged FINAL)
    //Q_PROPERTY(QString currentTime READ currentTime WRITE setcurrentTime NOTIFY currentTimeChanged FINAL)

    //Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)
    //Q_PROPERTY(QString year READ year WRITE setYear NOTIFY yearChanged FINAL)
    //create a property for the playstate of the QmediaPlayer
    Q_PROPERTY(QMediaPlayer::PlaybackState state READ state NOTIFY stateChanged FINAL)
    //void checkAlbumTitleMetadata();
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged FINAL)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged FINAL)
    Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged FINAL)
    Q_PROPERTY(QImage albumArt READ albumArt WRITE setAlbumArt NOTIFY albumArtChanged FINAL)
    //Q_PROPERTY(int bitrate READ bitrate WRITE setBitrate NOTIFY bitrateChanged FINAL)
    Q_PROPERTY(QString albumArtist READ albumArtist WRITE setAlbumArtist NOTIFY albumArtistChanged FINAL)
    Q_PROPERTY(QString composer READ composer WRITE setComposer NOTIFY composerChanged FINAL)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged FINAL)
    Q_PROPERTY(qint64 duration READ duration WRITE setDuration NOTIFY durationChanged FINAL)


public: // Getters
    mediaplayer(QObject *parent = nullptr);

    //Media Controls
    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void playList(const QUrl &url);
    int varName() const;//geter

    QString currentTime() const;
    QString title() const;
    QString artist() const;
    QString album() const;
    QString genre() const;
    QString albumArtist() const;
    QString composer() const;

    //QString year() const;
    QMediaPlayer::PlaybackState state() const;

    QImage albumArt() const;
    int bitrate() const;
    QString availableKeys();
    //progress bar functions for the media player
    qint64 position() const;
    qint64 duration() const;
    qint64 trackTime() const;


public slots: //Setters
    void setDuration(qint64 newDuration);
    void setPosition(qint64 newPosition);
    void setComposer(const QString &newComposer);
    void setAlbumArtist(const QString &newAlbumArtist);
    void setTitle(const QString &newTitle);
    void setBitrate(int newBitrate);
    void setAlbumArt(const QImage &newAlbumArt);
    //void setVarName(int varName);//setter
    void setArtist(const QString &newArtist);
    void setAlbum(const QString &newAlbum);
    void setGenre(const QString &newGenre);

    //void setYear(const QString &newYear);
    void setMediaInfo();

signals: //notification signals

    void varNameChanged(int varName);//singal

    //void currentTimeChanged(QString currentTime);

    void titleChanged(QString title);

    void artistChanged(QString artist);

    void albumChanged(QString album);
    void albumArtChanged(QImage albumArt);
    void genreChanged(QString genre);
    void albumArtistChanged(QString albumArtist);

    void composerChanged(QString composer);

    void stateChanged();



    void bitrateChanged(int bitrate);



    void positionChanged(qint64 position);

    void durationChanged(qint64 duration);




private: // private properties for internal use
    QMediaPlayer *m_player;
    QString m_title;
    QString m_artist;
    QString m_album;
    QString m_genre;
    QString m_albumArtist;
    QString m_composer;
    qint64 m_position;
    QImage m_albumArt;
    int m_varName;
    //QString m_currentTime;
    //QTimer *m_timer;

    //QString m_year;
    QMediaPlayer::PlaybackState m_state;

    int m_bitrate;
    qint64 m_duration;
    QList<QUrl> m_playList;

};

#endif // MEDIAPLAYER_H
