#include "BackgroundMusic.h"
#include <iostream>

#ifndef BACKGROUNDMUSIC_H
#define BACKGROUNDMUSIC_H

#include <string>
#include <SFML/Audio.hpp>

class BackgroundMusic {
public:
    BackgroundMusic(const std::string& filePath);
    ~BackgroundMusic() = default; // 使用默认析构函数

    void play();
    void pause();
    void stop();
    void setVolume(float volume);
    void setLoop(bool loop);
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;

private:
    sf::Music music;
    std::string filePath;
    void handleError(const std::string& errorMessage) const;
};

#endif // BACKGROUNDMUSIC_H

BackgroundMusic::BackgroundMusic(const std::string& filePath) : filePath(filePath) {
    if (!music.openFromFile(filePath)) {
        handleError("Failed to load music file: " + filePath);
    }
}

void BackgroundMusic::play() {
    if (music.getStatus() != sf::SoundSource::Playing) {
        music.play();
    }
}

void BackgroundMusic::pause() {
    if (music.getStatus() == sf::SoundSource::Playing) {
        music.pause();
    }
}

void BackgroundMusic::stop() {
    if (music.getStatus() != sf::SoundSource::Stopped) {
        music.stop();
    }
}

void BackgroundMusic::setVolume(float volume) {
    if (volume < 0.0f || volume > 100.0f) {
        handleError("Volume must be between 0 and 100.");
        return;
    }
    music.setVolume(volume);
}

void BackgroundMusic::setLoop(bool loop) {
    music.setLoop(loop);
}

bool BackgroundMusic::isPlaying() const {
    return music.getStatus() == sf::SoundSource::Playing;
}

bool BackgroundMusic::isPaused() const {
    return music.getStatus() == sf::SoundSource::Paused;
}

bool BackgroundMusic::isStopped() const {
    return music.getStatus() == sf::SoundSource::Stopped;
}

void BackgroundMusic::handleError(const std::string& errorMessage) const {
    std::cerr << "Error: " << errorMessage << std::endl;
}