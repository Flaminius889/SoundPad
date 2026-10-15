<<<<<<< HEAD
#pragma once

#include <QObject>
#include <QKeySequence>
#include <QHash>
#include <QAbstractNativeEventFilter>

// Manages global (system-wide) hotkeys on Windows via RegisterHotKey/UnregisterHotKey.
//
// Flow:
//   User presses Ctrl+5
//     -> Windows delivers a WM_HOTKEY message (wParam = hotkey id)
//     -> nativeEventFilter() intercepts it
//     -> emits hotkeyPressed(id)
//     -> MainWindow reacts (launchMusic())
class HotkeyManager : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit HotkeyManager(QObject *parent = nullptr);
    ~HotkeyManager() override;

    bool registerHotkey(int id, const QKeySequence &key);
    void unregisterHotkey(int id);

    const QHash<int, QKeySequence> &registeredHotkeys() const { return hotkeys; }

signals:
    void hotkeyPressed(int id);

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

private:
    static bool keySequenceToNative(const QKeySequence &key, quint32 &modifiers, quint32 &vk);

    QHash<int, QKeySequence> hotkeys;
=======
#pragma once

#include <QObject>
#include <QKeySequence>
#include <QHash>
#include <QAbstractNativeEventFilter>

// Manages global (system-wide) hotkeys on Windows via RegisterHotKey/UnregisterHotKey.
//
// Flow:
//   User presses Ctrl+5
//     -> Windows delivers a WM_HOTKEY message (wParam = hotkey id)
//     -> nativeEventFilter() intercepts it
//     -> emits hotkeyPressed(id)
//     -> MainWindow reacts (launchMusic())
class HotkeyManager : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit HotkeyManager(QObject *parent = nullptr);
    ~HotkeyManager() override;

    bool registerHotkey(int id, const QKeySequence &key);
    void unregisterHotkey(int id);

    const QHash<int, QKeySequence> &registeredHotkeys() const { return hotkeys; }

signals:
    void hotkeyPressed(int id);

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

private:
    static bool keySequenceToNative(const QKeySequence &key, quint32 &modifiers, quint32 &vk);

    QHash<int, QKeySequence> hotkeys;
>>>>>>> origin/main
};