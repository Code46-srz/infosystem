#include "mediaplayer.h"
#include "QDateTime"
// add debug
#include <QDebug>
#include <QStringList>

mediaplayer::mediaplayer(QObject *parent)
    : QObject{parent},
    m_varName(10)
{
    //m_timer = new QTimer(this);
    ///m_timer->setInterval(500);
    //m_timer->setSingleShot(true);

    QAudioOutput *audio = new QAudioOutput(this);
    m_player = new QMediaPlayer(this);
    m_player->setSource(QUrl::fromLocalFile("C:\\sample_music\\Ketsa - Firstly.mp3"));

   // connect( m_timer, &QTimer::timeout, this, &mediaplayer::timerTime);
    connect(m_player, &QMediaPlayer::positionChanged, this, &mediaplayer::setPosition);
    //connect(m_player, &QMediaPlayer::durationChanged, this, &mediaplayer::setDuration);//add it to set info
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &mediaplayer::setMediaInfo);
    m_player->setAudioOutput(audio);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &mediaplayer::state);

    //we will be connecting m_timer object and use the timeout signal and connect it to our
    // our class we created and use the timerTime function within this class
    // eg. m_timer -> timouet singal   singal is then recieved by mediaplayer timerTime slot
    // the timerTime slot
    setMediaInfo(); // but now we know :)
    /*timerTime();*/ //we will call the function to start the timer when the program runs
}



void mediaplayer::play() {
    m_player->play();
    emit stateChanged();
}

void mediaplayer::stop() {
    m_player->stop();
    emit stateChanged();
}

void mediaplayer::pause() {
    m_player->pause();
    emit stateChanged();
}

void mediaplayer::playList(const QUrl &url) //todo
{
    m_playList.append(url);
}



int mediaplayer::varName() const
{
    return m_varName;
}


void mediaplayer::setMediaInfo()
{
    QMediaMetaData mediaMetaData = m_player->metaData();
     // mediaMetaData.isEmpty();
    //qDebug() << test;
    QString title = mediaMetaData.stringValue(QMediaMetaData::Title);
    QString artist = mediaMetaData.value(QMediaMetaData::Author).toString();
    QString album = mediaMetaData.value(QMediaMetaData::AlbumTitle).toString();
    QString genre = mediaMetaData.value(QMediaMetaData::Genre).toString();
    QImage albumArt = mediaMetaData.value(QMediaMetaData::CoverArtImage).value<QImage>();
    QString albumArtist = mediaMetaData.value(QMediaMetaData::AlbumArtist).toString();
    QString composer = mediaMetaData.value(QMediaMetaData::Composer).toString();
    int bitrate = mediaMetaData.value(QMediaMetaData::AudioBitRate).toInt();
    qint64 duration = mediaMetaData.value(QMediaMetaData::Duration).toLongLong();
    //QString year = mediaMetaData.value(QMediaMetaData::Year).toString();
    //qDebug() << "this is a debug test for title";
    setComposer(composer);
    setTitle(title);
    setAlbumArtist(albumArtist);
    setArtist(artist);
    setAlbum(album);
    setGenre(genre);
    setAlbumArt(albumArt);
    setBitrate(bitrate);
    setDuration(duration);

    //setYear(year);
}
QString mediaplayer::availableKeys()
{
    //QStringList infos;
    QString metadatalist = m_player->metaData().stringValue(QMediaMetaData::Title);

    return metadatalist;
        //int list_size = metadatalist.size();
    //    QString metadata_key;
    //    QVariant var_data;

    //    for (int indx = 0; indx < list_size; indx++)
    //    {
    //        metadata_key  = metadatalist.at(indx);
    //        var_data      = m_player->metaData(metadata_key);

    //        infos << QString(metadata_key + QString(" : %1").arg(var_data.toString()));
    //    }
}

QString mediaplayer::title() const
{
    return m_title;
}

void mediaplayer::setTitle(const QString &newTitle)
{

    if (m_title == newTitle)
        return;
    m_title = newTitle;
    emit titleChanged(m_title);
}

QString mediaplayer::artist() const
{
    return m_artist;
}

void mediaplayer::setArtist(const QString &newArtist)
{
    if (m_artist == newArtist)
        return;
    m_artist = newArtist;
    emit artistChanged(m_artist);
}

QString mediaplayer::album() const
{
    return m_album;
}

void mediaplayer::setAlbum(const QString &newAlbum)
{
    if (m_album == newAlbum)
        return;
    m_album = newAlbum;
    emit albumChanged(m_album);
}

QString mediaplayer::genre() const
{
    return m_genre;
}

void mediaplayer::setGenre(const QString &newGenre)
{
    if (m_genre == newGenre)
        return;
    m_genre = newGenre;
    emit genreChanged(m_genre);
}


QMediaPlayer::PlaybackState mediaplayer::state() const
{
    return  m_player->playbackState();
}


//void mediaplayer::setYear(const QString &newYear)
//{
//    if (m_year == newYear)
//        return;
//    m_year = newYear;
//    emit yearChanged(m_year);
//}

QImage mediaplayer::albumArt() const
{
    return m_albumArt;
}

void mediaplayer::setAlbumArt(const QImage &newAlbumArt)
{
    if (m_albumArt == newAlbumArt)
        return;
    m_albumArt = newAlbumArt;
    emit albumArtChanged(m_albumArt);
}

int mediaplayer::bitrate() const
{
    return m_bitrate;
}

void mediaplayer::setBitrate(int newBitrate)
{
    if (m_bitrate == newBitrate)
        return;
    m_bitrate = newBitrate;
    emit bitrateChanged(m_bitrate);
}

QString mediaplayer::albumArtist() const
{
    return m_albumArtist;
}

void mediaplayer::setAlbumArtist(const QString &newAlbumArtist)
{
    if (m_albumArtist == newAlbumArtist)
        return;
    m_albumArtist = newAlbumArtist;
    emit albumArtistChanged(m_albumArtist);
}

QString mediaplayer::composer() const
{
    return m_composer;
}



void mediaplayer::setComposer(const QString &newComposer)
{
    if (m_composer == newComposer)
        return;
    m_composer = newComposer;
    emit composerChanged(m_composer);
}

qint64 mediaplayer::position() const //how is qint64 displayed
{
    //i want to return the current position of the song

    return m_position;
}

void mediaplayer::setPosition(const qint64 newPosition)
{

    if (m_position == newPosition)
        return;
    m_position = newPosition;
    emit positionChanged(m_position);
}

qint64 mediaplayer::duration() const
{
    return m_duration;
}

void mediaplayer::setDuration(const qint64 newDuration)
{
    if (m_duration == newDuration)
        return;
    m_duration = newDuration;
    emit durationChanged(m_duration);
}

