<<<<<<< HEAD
#include "core/hotkeymanager.h"

#include <QCoreApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

HotkeyManager::HotkeyManager(QObject *parent)
    : QObject(parent)
{
    qApp->installNativeEventFilter(this);
}

HotkeyManager::~HotkeyManager()
{
    qApp->removeNativeEventFilter(this);

    const auto ids = hotkeys.keys();
    for (int id : ids)
        unregisterHotkey(id);
}

bool HotkeyManager::keySequenceToNative(const QKeySequence &key, quint32 &modifiers, quint32 &vk)
{
#ifdef Q_OS_WIN
    if (key.isEmpty())
        return false;

    const QKeyCombination combo = key[0];

    const Qt::KeyboardModifiers qtMods = combo.keyboardModifiers();
    const Qt::Key qtKey = combo.key();

    modifiers = 0;
    if (qtMods & Qt::ShiftModifier)   modifiers |= MOD_SHIFT;
    if (qtMods & Qt::ControlModifier) modifiers |= MOD_CONTROL;
    if (qtMods & Qt::AltModifier)     modifiers |= MOD_ALT;
    if (qtMods & Qt::MetaModifier)    modifiers |= MOD_WIN;

    modifiers |= MOD_NOREPEAT;

    if (qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z)
    {
        vk = 'A' + (qtKey - Qt::Key_A);
        return true;
    }
    if (qtKey >= Qt::Key_0 && qtKey <= Qt::Key_9)
    {
        vk = '0' + (qtKey - Qt::Key_0);
        return true;
    }
    if (qtKey >= Qt::Key_F1 && qtKey <= Qt::Key_F24)
    {
        vk = VK_F1 + (qtKey - Qt::Key_F1);
        return true;
    }

    switch (qtKey)
    {
    case Qt::Key_Escape:    vk = VK_ESCAPE;  return true;
    case Qt::Key_Tab:       vk = VK_TAB;     return true;
    case Qt::Key_Space:     vk = VK_SPACE;   return true;
    case Qt::Key_Return:
    case Qt::Key_Enter:     vk = VK_RETURN;  return true;
    case Qt::Key_Backspace: vk = VK_BACK;    return true;
    case Qt::Key_Delete:    vk = VK_DELETE;  return true;
    case Qt::Key_Insert:    vk = VK_INSERT;  return true;
    case Qt::Key_Home:      vk = VK_HOME;    return true;
    case Qt::Key_End:       vk = VK_END;     return true;
    case Qt::Key_PageUp:    vk = VK_PRIOR;   return true;
    case Qt::Key_PageDown:  vk = VK_NEXT;    return true;
    case Qt::Key_Left:      vk = VK_LEFT;    return true;
    case Qt::Key_Right:     vk = VK_RIGHT;   return true;
    case Qt::Key_Up:        vk = VK_UP;      return true;
    case Qt::Key_Down:      vk = VK_DOWN;    return true;
    default:
        return false;
    }
#else
    Q_UNUSED(key);
    Q_UNUSED(modifiers);
    Q_UNUSED(vk);
    return false;
#endif
}

bool HotkeyManager::registerHotkey(int id, const QKeySequence &key)
{
#ifdef Q_OS_WIN
    quint32 modifiers = 0;
    quint32 vk = 0;

    if (!keySequenceToNative(key, modifiers, vk))
        return false;

    unregisterHotkey(id);

    if (!RegisterHotKey(nullptr, id, modifiers, vk))
        return false;

    hotkeys.insert(id, key);
    return true;
#else
    Q_UNUSED(id);
    Q_UNUSED(key);
    return false;
#endif
}

void HotkeyManager::unregisterHotkey(int id)
{
#ifdef Q_OS_WIN
    if (!hotkeys.contains(id))
        return;

    UnregisterHotKey(nullptr, id);
    hotkeys.remove(id);
#else
    Q_UNUSED(id);
#endif
}

bool HotkeyManager::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    Q_UNUSED(result);

#ifdef Q_OS_WIN
    if (eventType != "windows_generic_MSG")
        return false;

    const MSG *msg = static_cast<const MSG *>(message);

    if (msg->message == WM_HOTKEY)
    {
        const int id = static_cast<int>(msg->wParam);

        if (hotkeys.contains(id))
        {
            emit hotkeyPressed(id);
            return true;
        }
    }
#else
    Q_UNUSED(eventType);
    Q_UNUSED(message);
#endif

    return false;
=======
#include "core/hotkeymanager.h"

#include <QCoreApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

HotkeyManager::HotkeyManager(QObject *parent)
    : QObject(parent)
{
    qApp->installNativeEventFilter(this);
}

HotkeyManager::~HotkeyManager()
{
    qApp->removeNativeEventFilter(this);

    const auto ids = hotkeys.keys();
    for (int id : ids)
        unregisterHotkey(id);
}

bool HotkeyManager::keySequenceToNative(const QKeySequence &key, quint32 &modifiers, quint32 &vk)
{
#ifdef Q_OS_WIN
    if (key.isEmpty())
        return false;

    const QKeyCombination combo = key[0];

    const Qt::KeyboardModifiers qtMods = combo.keyboardModifiers();
    const Qt::Key qtKey = combo.key();

    modifiers = 0;
    if (qtMods & Qt::ShiftModifier)   modifiers |= MOD_SHIFT;
    if (qtMods & Qt::ControlModifier) modifiers |= MOD_CONTROL;
    if (qtMods & Qt::AltModifier)     modifiers |= MOD_ALT;
    if (qtMods & Qt::MetaModifier)    modifiers |= MOD_WIN;

    modifiers |= MOD_NOREPEAT;

    if (qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z)
    {
        vk = 'A' + (qtKey - Qt::Key_A);
        return true;
    }
    if (qtKey >= Qt::Key_0 && qtKey <= Qt::Key_9)
    {
        vk = '0' + (qtKey - Qt::Key_0);
        return true;
    }
    if (qtKey >= Qt::Key_F1 && qtKey <= Qt::Key_F24)
    {
        vk = VK_F1 + (qtKey - Qt::Key_F1);
        return true;
    }

    switch (qtKey)
    {
    case Qt::Key_Escape:    vk = VK_ESCAPE;  return true;
    case Qt::Key_Tab:       vk = VK_TAB;     return true;
    case Qt::Key_Space:     vk = VK_SPACE;   return true;
    case Qt::Key_Return:
    case Qt::Key_Enter:     vk = VK_RETURN;  return true;
    case Qt::Key_Backspace: vk = VK_BACK;    return true;
    case Qt::Key_Delete:    vk = VK_DELETE;  return true;
    case Qt::Key_Insert:    vk = VK_INSERT;  return true;
    case Qt::Key_Home:      vk = VK_HOME;    return true;
    case Qt::Key_End:       vk = VK_END;     return true;
    case Qt::Key_PageUp:    vk = VK_PRIOR;   return true;
    case Qt::Key_PageDown:  vk = VK_NEXT;    return true;
    case Qt::Key_Left:      vk = VK_LEFT;    return true;
    case Qt::Key_Right:     vk = VK_RIGHT;   return true;
    case Qt::Key_Up:        vk = VK_UP;      return true;
    case Qt::Key_Down:      vk = VK_DOWN;    return true;
    default:
        return false;
    }
#else
    Q_UNUSED(key);
    Q_UNUSED(modifiers);
    Q_UNUSED(vk);
    return false;
#endif
}

bool HotkeyManager::registerHotkey(int id, const QKeySequence &key)
{
#ifdef Q_OS_WIN
    quint32 modifiers = 0;
    quint32 vk = 0;

    if (!keySequenceToNative(key, modifiers, vk))
        return false;

    unregisterHotkey(id);

    if (!RegisterHotKey(nullptr, id, modifiers, vk))
        return false;

    hotkeys.insert(id, key);
    return true;
#else
    Q_UNUSED(id);
    Q_UNUSED(key);
    return false;
#endif
}

void HotkeyManager::unregisterHotkey(int id)
{
#ifdef Q_OS_WIN
    if (!hotkeys.contains(id))
        return;

    UnregisterHotKey(nullptr, id);
    hotkeys.remove(id);
#else
    Q_UNUSED(id);
#endif
}

bool HotkeyManager::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    Q_UNUSED(result);

#ifdef Q_OS_WIN
    if (eventType != "windows_generic_MSG")
        return false;

    const MSG *msg = static_cast<const MSG *>(message);

    if (msg->message == WM_HOTKEY)
    {
        const int id = static_cast<int>(msg->wParam);

        if (hotkeys.contains(id))
        {
            emit hotkeyPressed(id);
            return true;
        }
    }
#else
    Q_UNUSED(eventType);
    Q_UNUSED(message);
#endif

    return false;
>>>>>>> origin/main
}