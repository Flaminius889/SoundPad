#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QAudioDevice>
#include <QtWidgets>
#include <QSettings>
#include <QUrl>
#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include "./ui_mainwindow.h"
#include "core/Sound.h"
#include "core/repository.h"
#include "ui/categoryAdd.h"
#include "ui/settingsdialog.h"
#include "core/Sound.h"
#include "models/TableModel.h"
#include "models/CategoryModel.h"
#include "core/contextManager.h"
#include "core/hotkeymanager.h"
#include "core/SoundManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    void setDefaultSettings();
    void loadStyles();
    QString loadStyle(const QString &path);

    void createModels();
    void loadSettings();
    void setupTableView();
    void setupTreeView();
    void setupSoundManager();
    void setupConnections();

    void addCategory(const QString &categoryName);

    Qt::SortOrder toggleSortOrder(int column);

    void refreshAllModels(bool refreshCategories = true);
    void updateCountLabel(const QString& category);
    void cleanupSoundsBeforeRemoval(const QVector<SoundT>& sounds);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void addSound(const QString &filePath, const QString &category);
    void launchMusic(const QString& musicName);
    void updateWindowTitleForSource(const QString &path);
    void playPreviousTrack();
    void playNextTrack();

    void setupDragDrop();
    void setupContextMenus();

    void onSoundsMoved(const QStringList& soundIds, const QString& sourceCategory,
                       const QString& targetCategory, int targetRow);
    void onSoundsMovedToCategory(const QStringList& soundIds, const QString& targetCategory);
    void onCategoryMoved(const QString& category, int fromRow, int toRow);
    void onSoundMovedWithinCategory(const QStringList& soundIds, int toRow);
    SoundT findSoundById(const QString& idStr);
    void removeSounds(const QStringList& soundIds);

    void handleAddCategory();
    void handleDeleteSounds(const QStringList &ids);
    void handleDeleteCategory(const QString &categoryName);
    void handleShowAllSounds();
    void handleChangeCategory();

    void handleRemoveHotkey(const QString &soundId);
    void handleAddHotkey(const QString &soundId);

    QString getCurrentTime();
    QString formatDuration(qint64 ms);
    qint64 getAudioDuration(const QString& filePath);
private slots:
    void on_actionAdd_new_sound_triggered();

    void on_muteBtn_clicked();
    void on_volumeControl_valueChanged(int value);

    void onCategorySelectionChanged();
    void onSelectionChanged();

    void onSoundDoubleClicked(const QModelIndex &index);

    void on_actionSettings_triggered();
public:
    Ui::MainWindow *ui;
    QSettings settings;
    QHash<int, Qt::SortOrder> sortOrders;
    int currentRow = -1;
    QString MuteBTNiconPath;

    QString m_sliderEnabledStyle;
    QString m_sliderDisabledStyle;

    ContextMenuManager *m_contextMenuManager;
    HotkeyManager *m_hotkeyManager;

    SoundManager *soundManager = nullptr;

    QAudioDevice selectedOutputDevice;
    QAudioDevice selectedInputDevice;

    TableModel *tableModel;
    CategoryModel *categoryModel;
};
#endif // MAINWINDOW_H
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioInput>
#include <QtWidgets>
#include <QSettings>
#include <QUrl>
#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include "./ui_mainwindow.h"
#include "core/Sound.h"
#include "core/repository.h"
#include "ui/categoryAdd.h"
#include "ui/settingsdialog.h"
#include "core/Sound.h"
#include "models/TableModel.h"
#include "models/CategoryModel.h"
#include "core/contextManager.h"
#include "core/hotkeymanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    void setDefaultSettings();
    void loadStyles();
    QString loadStyle(const QString &path);

    void createModels();
    void loadSettings();
    void setupTableView();
    void setupTreeView();
    void setupPlayer();
    void setupConnections();

    void addCategory(const QString &categoryName);

    Qt::SortOrder toggleSortOrder(int column);

    void refreshAllModels();
    void updateCountLabel(const QString& category);
    void cleanupSoundsBeforeRemoval(const QVector<SoundT>& sounds);

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

    void addSound(const QString &filePath, const QString &category);
    void launchMusic(const QString& musicName);
    void playPreviousTrack();
    void playNextTrack();

    void setupDragDrop();
    void setupContextMenus();

    void onSoundsMoved(const QStringList& soundIds, const QString& sourceCategory,
                       const QString& targetCategory, int targetRow);
    void onSoundsMovedToCategory(const QStringList& soundIds, const QString& targetCategory);
    void onCategoryMoved(const QString& category, int fromRow, int toRow);
    void onSoundMovedWithinCategory(const QStringList& soundIds, int toRow);
    SoundT findSoundById(const QString& idStr);
    void removeSounds(const QStringList& soundIds);

    QAudioDevice findOutputDevice(const QByteArray &id);
    QAudioDevice findInputDevice(const QByteArray &id);

    void handleAddCategory();
    void handleDeleteSounds(const QStringList &ids);
    void handleDeleteCategory(const QString &categoryName);
    void handleShowAllSounds();
    void handleChangeCategory();

    void handleRemoveHotkey(const QString &soundId);
    void handleAddHotkey(const QString &soundId);

    QString getCurrentTime();
    QString formatDuration(qint64 ms);
    qint64 getAudioDuration(const QString& filePath);
private slots:
    void on_actionAdd_new_sound_triggered();

    void on_muteBtn_clicked();
    void on_volumeControl_valueChanged(int value);

    void onCategorySelectionChanged();
    void onSelectionChanged();

    void onSoundDoubleClicked(const QModelIndex &index);

    void on_actionSettings_triggered();
public:
    Ui::MainWindow *ui;
    QSettings settings;
    qint64 m_restorePosition;

    QHash<int, Qt::SortOrder> sortOrders;
    int currentRow = -1;
    QString MuteBTNiconPath;

    QString m_sliderEnabledStyle;
    QString m_sliderDisabledStyle;

    ContextMenuManager *m_contextMenuManager;
    HotkeyManager *m_hotkeyManager;

    QMediaPlayer *player = nullptr;
    QAudioOutput *audioOutput = nullptr;
    QAudioInput *audioInput = nullptr;

    QAudioDevice selectedOutputDevice;
    QAudioDevice selectedInputDevice;

    TableModel *tableModel;
    CategoryModel *categoryModel;
};
#endif // MAINWINDOW_H
