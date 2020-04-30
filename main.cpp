// Playlist

#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <limits>
#include <list>
#include <fstream>

class Song {
        friend std::ostream &operator<<(std::ostream &os, const Song &song);
        std::string name;
        std::string artist;
        int rating;
    public:
        Song() = default;
        Song(std::string song_name, std::string artist, int rating) : name{song_name}, artist{artist}, rating{rating} {}
        std::string get_name() const { return name; }
        std::string get_artist() const { return artist; }
        int get_rating() const { return rating; }
        bool operator<(const Song &rhs) const { return this->name < rhs.name; }
        bool operator==(const Song &rhs) const { return this->name == rhs.name; }
};

std::ostream &operator<<(std::ostream &os, const Song &song) {
    os << std::setw(20) << std::left << song.name << 
          std::setw(30) << std::left << song.artist << 
          std::setw(2) << std::left << song.rating;
    return os;
}

void display_menu() {
    std::cout << "\nF - Play first song" << std::endl;
    std::cout << "\nN - Play next song" << std::endl;
    std::cout << "\nP - Play previous song" << std::endl;
    std::cout << "\nA - Add and play a new song at current location" << std::endl;
    std::cout << "\nL - List the current playlist" << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "Enter a selection (Q to quit): ";
}

void play_cur_song(const Song &song) {
    std::cout << "Playing: " << std::endl;
    std::cout << song << std::endl;
}

void display_playlist(const std::list<Song> &playlist, const Song &cur_song) {
    for (const Song &song: playlist)
        std::cout << song << std::endl;
    std::cout << "Current song: " << std::endl;
    std::cout << cur_song << std::endl;
}

int main() {
    std::ifstream in_file;
    in_file.open("songs.txt");
    if (!in_file) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    
    std::string song_name, artist;
    int rating;
    std::list<Song> playlist;
    
    while (in_file >> song_name >> artist >> rating) {
        playlist.emplace_back(song_name, artist, rating);
    }
    in_file.close();

    auto cur_song = playlist.begin();
    display_playlist(playlist, *cur_song);

    char selection {};
    do
    {
        display_menu();
        std::cin >> selection;
        selection = std::toupper(selection);
        switch(selection) {
        case 'F':
            std::cout << "Playing first song" << std::endl;
            cur_song = playlist.begin();
            play_cur_song(*cur_song);
            break;
        case 'N':
            std::cout << "Playing next song" << std::endl;
            cur_song++;
            if (cur_song == playlist.end()) {
                std::cout << "Returning to the start of playlist" << std::endl;
                cur_song = playlist.begin();
            }
            play_cur_song(*cur_song);
            break;
        case 'P':
            std::cout << "Playing previous song" << std::endl;
            if (cur_song == playlist.begin()) {
                std::cout << "Returning to the end of playlist" << std::endl;
                cur_song = playlist.end();
            }
            cur_song--;
            play_cur_song(*cur_song);
            break;
        case 'A':
            {
            std::ofstream out_file {"songs.txt", std::ios::app}; 
            if (!out_file) {
                std::cerr << "Error writing to file" << std::endl;
                return 1;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Adding and playing new song" << std::endl;
            std::string song_name, artist;
            int rating; 
            std::cout << "Enter song name: ";
            std::getline(std::cin, song_name);
            std::cout << "Enter artist: ";
            std::getline(std::cin, artist);
            std::cout << "Enter rating: ";
            std::cin >> rating;
            out_file << song_name << " " << artist << " " << rating << std::endl;
            playlist.insert(cur_song, Song{song_name, artist, rating});
            cur_song--;
            play_cur_song(*cur_song);
            break;
            }
        case 'L':
            std::cout << std::endl;
            display_playlist(playlist, *cur_song);
            break;
        case 'Q':
            std::cout << "Quitting..." << std::endl;
            break;
        default:
            std::cout << "Unknown choice, please try again." << std::endl; 
        }
    } while (selection != 'Q');
    

    std::cout << "\nThanks for listening!" << std::endl;
    return 0;
}