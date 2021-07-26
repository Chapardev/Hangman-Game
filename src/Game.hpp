#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include "includes.hpp"

class Game
{
private:
	void _initTexts();
	void _initSprites();

public:
	Game();

	void checkLetter(char letter);
	void changeInformationText(const std::string &newText);

	void pollEvents();
	void update();

	void render();
	void renderGameOverScreen(const std::string &text);

	void run();

private:
	// Resources
	gf::Window m_window;
	gf::RenderWindow m_renderer;

	// Game logic variables
    const int m_attemptsMax;
	std::string m_secretWord;

	std::string m_usedLetters;
	std::string m_word;
    int m_attempts;

	std::map<std::string, gf::Texture> m_textures;
	std::map<std::string, gf::Sprite> m_sprites;

	gf::Font m_font;
	std::map<std::string, gf::Text> m_texts;
};

#endif // SRC_GAME_HPP
