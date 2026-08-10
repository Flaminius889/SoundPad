# SoundPad

Настільний застосунок для керування та відтворення звукових ефектів.

## Можливості

- Відтворення аудіофайлів
- Категорії
- Підтримка Drag & Drop
- Сортування
- Гарячі клавіші
- Вибір пристрою виводу/введення звуку
- Збереження налаштувань застосунку
- Власні іконки категорій

## Скріншоти

![adding_sounds](adding_sounds.gif)
![multimedia_btn](multimedia_btn.gif)
![deleting](deleting.gif)
![drag_drop](drag_drop.gif)
![add-use_hotkey](add-use_hotkey.gif)

## Технології

- C++17
- Qt 6
- Архітектура Model/View
- QMediaPlayer
- QAudioOutput
- QSettings
- Drag & Drop
- Inno Setup

## Встановлення

1.  Завантажте інсталятор.
2.  Запустіть `SoundPad Installer.exe`.
3.  Запустіть **SoundPad**.

## Збірка

Вимоги:

- Qt 6.x
- CMake
- MSVC 2022

```bash
git clone https://github.com/Flaminius889/Simple-SoundPad
cd SoundPad
mkdir build
cd build
cmake ..
cmake --build .
```

## Структура проєкту

build/
include/
resources/
src/
ui/

## Плани на майбутнє

- Іконки для категорій
- Додати гарячу клавішу для мультимедійних кнопок у налаштуваннях

## Ліцензія

MIT
