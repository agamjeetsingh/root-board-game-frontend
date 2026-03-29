//
// Created by Agamjeet Singh on 29/03/2026.
//

#ifndef ROOT_BOARD_GAME_FRONTEND_TEXTURESHOLDER_H
#define ROOT_BOARD_GAME_FRONTEND_TEXTURESHOLDER_H
#include <functional>
#include <unordered_map>
#include <vector>

#include "SFML/Graphics/Texture.hpp"

template <typename Key = int>
class TexturesHolder {
public:
    explicit TexturesHolder(const std::vector<std::string>& paths, std::function<Key(int)> keyGen = [](int i) { return static_cast<Key>(i); }) {
        for (int i = 0; i < paths.size(); i++) {
            const auto& path = paths[i];
            sf::Texture texture;
            if (!texture.loadFromFile(path)) {
                throw std::runtime_error("Cannot load texture from file with path: " + path);
            }
            textures[keyGen(i)] = std::move(texture);
        }
    }

    const sf::Texture& getTexture(const Key& key) const {
        return textures.at(key);
    }

private:
    std::unordered_map<Key, sf::Texture> textures;
};

#endif //ROOT_BOARD_GAME_FRONTEND_TEXTURESHOLDER_H