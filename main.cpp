#include <iostream>
#include <Windows.h>
#include <fstream> // For file checking
#include <list>    // For playlist

#pragma comment(lib, "winmm.lib")

std::string current_music_file; // Stores the currently played music file

bool FileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

void PlayMusic(const char* filename, bool resume = false) {
    if (!FileExists(filename)) {
        std::cout << "Failed to play the audio. Please check the file name and try again." << std::endl;
        return;
    }

    if (resume) {
        if (PlaySoundA(current_music_file.c_str(), NULL, SND_FILENAME | SND_ASYNC) == 0) {
            std::cout << "Failed to resume the audio. Please try again." << std::endl;
            return;
        }
    }
    else {
        if (PlaySoundA(filename, NULL, SND_FILENAME | SND_ASYNC) == 0) {
            std::cout << "Failed to play the audio. Please check the file name and try again." << std::endl;
            return;
        }
        current_music_file = filename; // Store the currently played music file
    }
    std::cout << "Playing: " << current_music_file << std::endl;
}

void PauseMusic() {
    if (current_music_file.empty()) {
        std::cout << "No music file is currently playing. Please play a music file first." << std::endl;
        return;
    }

    if (PlaySoundA(NULL, NULL, SND_PURGE) == 0) {
        std::cout << "Failed to pause the audio. Please try again." << std::endl;
    }
    else {
        std::cout << "Paused: " << current_music_file << std::endl;
    }
}

void StopMusic() {
    if (current_music_file.empty()) {
        std::cout << "No music file is currently playing. Please play a music file first." << std::endl;
        return;
    }

    if (PlaySoundA(NULL, NULL, 0) == 0) {
        std::cout << "Failed to stop the audio. Please try again." << std::endl;
    }
    else {
        std::cout << "Stopped: " << current_music_file << std::endl;
    }
    current_music_file = ""; // Reset the currently played music file
}


class Playlist {
private:
    struct Song {
        std::string filename;
        Song* next;

        explicit Song(const std::string& name) : filename(name), next(nullptr) {}
    };

    Song* front_;
    Song* rear_;

public:
    Playlist() : front_(nullptr), rear_(nullptr) {}

    ~Playlist() {
        while (!IsEmpty()) {
            DequeueFront();
        }
    }

    bool IsEmpty() const {
        return front_ == nullptr;
    }

    void EnqueueFront(const std::string& filename) {

        if (!FileExists(filename)) {
            std::cout << "Failed to add file to the playlist. The file '" << filename << "' does not exist." << std::endl;
            return;
        }

        Song* new_song = new Song(filename);
        if (IsEmpty()) {
            front_ = rear_ = new_song;
        }
        else {
            new_song->next = front_;
            front_ = new_song;
        }

        std::cout << "Added to the front of the playlist." << std::endl;
    }

    void EnqueueRear(const std::string& filename) {

        if (!FileExists(filename)) {
            std::cout << "Failed to add file to the playlist. The file '" << filename << "' does not exist." << std::endl;
            return;
        }

        Song* new_song = new Song(filename);
        if (IsEmpty()) {
            front_ = rear_ = new_song;
        }
        else {
            rear_->next = new_song;
            rear_ = new_song;
        }

        std::cout << "Added to the rear of the playlist." << std::endl;
    }

    void DequeueFront() {
        if (IsEmpty()) {
            std::cout << "Playlist is empty. No song to remove." << std::endl;
            return;
        }

        Song* temp = front_;
        front_ = front_->next;
        if (front_ == nullptr) {
            rear_ = nullptr;
        }
        delete temp;

        std::cout << "Removed song from the front of the playlist." << std::endl;
    }

    void ShowPlaylist() const {
        if (IsEmpty()) {
            std::cout << "Playlist is empty." << std::endl;
            return;
        }

        std::cout << "Playlist:" << std::endl;
        Song* current = front_;
        while (current != nullptr) {
            std::cout << current->filename << std::endl;
            current = current->next;
        }
    }

    void PlayPlaylist() {
        if (IsEmpty()) {
            std::cout << "The playlist is empty. Please add songs to the playlist first." << std::endl;
            return;
        }


        Song* current = front_;
        while (current != nullptr) {
            std::cout << "Playing playlist..." << std::endl;
            std::cout << "Main menu will be available when the playlist ends." << std::endl;
            std::cout << "\nPlaying: " << current->filename << std::endl;
            if (PlaySoundA(current->filename.c_str(), NULL, SND_FILENAME | SND_SYNC) == 0) {
                std::cout << "Failed to play the audio. Please check the file name and try again." << std::endl;
                return;
            }
            system("cls");
            std::cout << "Finished: " << current->filename << std::endl;
            current = current->next;
        }
    }
};

int main() {
    std::cout << "Music Player" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "Info:" << std::endl;
    std::cout << "Programm only work on Windows" << std::endl;
    std::cout << "Music file format must be .wav" << std::endl;
    std::cout << "Program EXE file must be in a folder with music" << std::endl;
    std::cout << "--------------------" << std::endl;

    Playlist playlist;

    while (true) {
        std::cout << "1. Play Music file" << std::endl;
        std::cout << "2. Pause Music" << std::endl;
        std::cout << "3. Resume Music" << std::endl;
        std::cout << "4. Stop Music" << std::endl;
        std::cout << "5. Add Music to Front of Playlist" << std::endl;
        std::cout << "6. Add Music to Rear of Playlist" << std::endl;
        std::cout << "7. Remove Music from Playlist" << std::endl;
        std::cout << "8. Show Playlist" << std::endl;
        std::cout << "9. Play Playlist" << std::endl;
        std::cout << "10. Exit" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            system("cls");
            std::cout << "Example: filename.wav\n";
            std::cout << "Enter the filename of the music file: ";
            std::string filename;
            std::cin >> filename;
            PlayMusic(filename.c_str());
            break;
        }
        case 2:
            system("cls");
            PauseMusic();
            break;
        case 3:
            system("cls");
            if (!current_music_file.empty()) {
                PlayMusic(current_music_file.c_str(), true);
            }
            else {
                std::cout << "No music file to resume. Please play a music file first." << std::endl;
            }
            break;
        case 4:
            system("cls");
            StopMusic();
            break;
        case 5: {
            system("cls");
            std::cout << "Example: filename.wav\n";
            std::cout << "Enter the filename of the music file to add to the front of the playlist: ";
            std::string filename;
            std::cin >> filename;
            playlist.EnqueueFront(filename);
            break;
        }
        case 6: {
            system("cls");
            std::cout << "Example: filename.wav\n";
            std::cout << "Enter the filename of the music file to add to the rear of the playlist: ";
            std::string filename;
            std::cin >> filename;
            playlist.EnqueueRear(filename);
            break;
        }
        case 7:
            system("cls");
            playlist.DequeueFront();
            break;
        case 8:
            system("cls");
            playlist.ShowPlaylist();
            break;
        case 9:
            system("cls");
            playlist.PlayPlaylist();
            break;
        case 10:
            system("cls");
            std::cout << "Exiting the program." << std::endl;
            return 0;
        default:
            system("cls");
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

        std::cout << std::endl;
    }
}
