#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include "includes.hpp"

class Game
{
private:
	std::string _getRandomWord();
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
	/* Resources */

	gf::Window m_window;
	gf::RenderWindow m_renderer;

	std::map<std::string, gf::Texture> m_textures;
	std::map<std::string, gf::Sprite> m_sprites;

	gf::Font m_font;
	std::map<std::string, gf::Text> m_texts;

	/* Game logic variables */

	std::string m_usedLetters;
	std::string m_secretWord;
	std::string m_word;

    const int m_attemptsMax;
    int m_attempts;
};

#endif // SRC_GAME_HPP
