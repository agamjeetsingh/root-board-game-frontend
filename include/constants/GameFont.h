//
// Created by Agamjeet Singh on 19/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_GAMEFONT_H
#define ROOT_BOARD_GAME_FRONTEND_GAMEFONT_H

#include <string>

enum class GameFont {
    TITLE,
    BODY
};

inline std::string getPath(GameFont font) {
    switch (font) {
        case GameFont::TITLE: return "../assets/Luminari-Regular.ttf";
        case GameFont::BODY: return "../assets/Baskerville_Ten_Pro_Regular.otf";
        default: throw std::runtime_error("GameFont not recognised");
    }
}

#endif //ROOT_BOARD_GAME_FRONTEND_GAMEFONT_H