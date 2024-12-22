#include <SFML/Graphics.hpp>
#include <stdexcept>
using namespace sf;

namespace Textures {
    Texture redBallTexture;
    Texture yellowBallTexture;
    Texture backgroundTexture;
    Texture gameBackgroundTexture;
    
    Font rechargeFont;

    void loadTextures() {
        if (!redBallTexture.loadFromFile("resources\\redBall.png")) {
            throw std::runtime_error("Failed to load red ball texture");
        }
        if (!yellowBallTexture.loadFromFile("resources\\yellowBall.png")) {
            throw std::runtime_error("Failed to load yellow ball texture");
        }
        if (!gameBackgroundTexture.loadFromFile("resources\\greyBackground.jpg")) {
            throw std::runtime_error("Failed to load grey background texture");
        }
        if (!rechargeFont.loadFromFile("resources\\rechargeFont.otf")) {
            throw std::runtime_error("Failed to load recharge font");
        }
    }
}