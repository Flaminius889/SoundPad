#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QAudioDevice>
#include <QUrl>
#include <QMediaPlayer>

class QAudioOutput;

class SoundManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRepeating READ isRepeating WRITE setRepeating NOTIFY repeatingChanged)

public:
    explicit SoundManager(QObject *parent = nullptr);
    ~SoundManager() override;

    qint64 position() const;
    QUrl source() const;
    QAudioDevice outputDevice() const;
    QAudioDevice inputDevice() const;
    bool isRepeating() const;
    qint64 audioDuration(const QString &filePath) const;

    static QAudioDevice findOutputDevice(const QByteArray &id);
    static QAudioDevice findInputDevice(const QByteArray &id);

public slots:
    void play();
    void pause();
    void stop();
    void stopAndClear();
    void playFile(const QString &path);
    void loadFile(const QString &path, qint64 position = 0);
    void setPosition(qint64 position);
    void setVolume(int value);
    void setOutputDevice(const QAudioDevice &device);
    void setInputDevice(const QAudioDevice &device);
    void setRepeating(bool repeating);
    void toggleRepeating();

signals:
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void playbackStateChanged(bool playing);
    void mediaEnded();
    void sourceAccepted(const QString &path);
    void sourceChanged(const QUrl &source);
    void repeatingChanged(bool repeating);
    void playbackError(const QString &message);

private:
    void handleMediaStatusChanged(int status);

    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QAudioDevice m_inputDevice;
    qint64 m_restorePosition = -1;
    bool m_isRepeating = false;
};

#endif // SOUNDMANAGER_H
