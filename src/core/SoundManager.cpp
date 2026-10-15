#include "core/SoundManager.h"

#include <QAudioOutput>
#include <QEventLoop>
#include <QFileInfo>
#include <QMediaDevices>
#include <QMediaPlayer>
#include <QTimer>

SoundManager::SoundManager(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_inputDevice(QMediaDevices::defaultAudioInput())
{
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.5f);

    connect(m_player, &QMediaPlayer::errorOccurred, this,
            [this](QMediaPlayer::Error, const QString &message) {
                emit playbackError(message);
            });

    connect(m_player, &QMediaPlayer::positionChanged,
            this, &SoundManager::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &SoundManager::durationChanged);
    connect(m_player, &QMediaPlayer::sourceChanged,
            this, &SoundManager::sourceChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this,
            [this](QMediaPlayer::PlaybackState state) {
                emit playbackStateChanged(state == QMediaPlayer::PlayingState);
            });
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this,
            [this](QMediaPlayer::MediaStatus status) {
                handleMediaStatusChanged(static_cast<int>(status));
            });
}

SoundManager::~SoundManager()
{
    m_player->stop();
}

qint64 SoundManager::position() const { return m_player->position(); }
QUrl SoundManager::source() const { return m_player->source(); }
QAudioDevice SoundManager::outputDevice() const { return m_audioOutput->device(); }
QAudioDevice SoundManager::inputDevice() const { return m_inputDevice; }
bool SoundManager::isRepeating() const { return m_isRepeating; }

qint64 SoundManager::audioDuration(const QString &filePath) const
{
    QMediaPlayer probe;
    QEventLoop loop;
    connect(&probe, &QMediaPlayer::durationChanged, &loop, &QEventLoop::quit);
    probe.setSource(QUrl::fromLocalFile(filePath));
    QTimer::singleShot(3000, &loop, &QEventLoop::quit);
    loop.exec();
    return probe.duration();
}

QAudioDevice SoundManager::findOutputDevice(const QByteArray &id)
{
    for (const auto &device : QMediaDevices::audioOutputs())
        if (device.id() == id) return device;
    return QMediaDevices::defaultAudioOutput();
}

QAudioDevice SoundManager::findInputDevice(const QByteArray &id)
{
    for (const auto &device : QMediaDevices::audioInputs())
        if (device.id() == id) return device;
    return QMediaDevices::defaultAudioInput();
}

void SoundManager::play()
{
    if (!m_player->source().isEmpty()) m_player->play();
}

void SoundManager::pause() { m_player->pause(); }
void SoundManager::stop() { m_player->stop(); }

void SoundManager::stopAndClear()
{
    m_restorePosition = -1;
    stop();
    m_player->setSource({});
}

void SoundManager::playFile(const QString &path)
{
    if (!QFileInfo(path).isFile()) return;

    emit sourceAccepted(path);
    const QUrl newSource = QUrl::fromLocalFile(path);
    const bool sameSource = m_player->source() == newSource;

    stop();
    m_restorePosition = -1;
    if (sameSource) m_player->setPosition(0);
    else m_player->setSource(newSource);
    play();
}

void SoundManager::loadFile(const QString &path, qint64 position)
{
    if (!QFileInfo(path).isFile()) return;

    emit sourceAccepted(path);
    stop();
    m_restorePosition = qMax<qint64>(0, position);
    m_player->setSource(QUrl::fromLocalFile(path));
}

void SoundManager::setPosition(qint64 position) { m_player->setPosition(position); }

void SoundManager::setVolume(int value)
{
    m_audioOutput->setVolume(qBound(0, value, 100) / 100.0f);
}

void SoundManager::setOutputDevice(const QAudioDevice &device)
{
    m_audioOutput->setDevice(device);
}

void SoundManager::setInputDevice(const QAudioDevice &device)
{
    m_inputDevice = device.isNull() ? QMediaDevices::defaultAudioInput() : device;
}

void SoundManager::setRepeating(bool repeating)
{
    if (m_isRepeating == repeating) return;
    m_isRepeating = repeating;
    emit repeatingChanged(m_isRepeating);
}

void SoundManager::toggleRepeating()
{
    setRepeating(!m_isRepeating);
}

void SoundManager::handleMediaStatusChanged(int status)
{
    const auto mediaStatus = static_cast<QMediaPlayer::MediaStatus>(status);
    if (mediaStatus == QMediaPlayer::LoadedMedia && m_restorePosition >= 0) {
        const qint64 restorePosition = m_restorePosition;
        m_restorePosition = -1;
        m_player->setPosition(restorePosition);
        return;
    }

    if (mediaStatus != QMediaPlayer::EndOfMedia) return;

    if (m_isRepeating && !m_player->source().isEmpty()) {
        m_player->setPosition(0);
        play();
        return;
    }

    emit mediaEnded();
}
