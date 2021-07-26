#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include "includes.hpp"

class Game
{
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
	const std::string m_secretWord;
    const int m_attemptsMax;

	std::string m_usedLetters;
	std::string m_word;
    int m_attempts;

	gf::Texture m_blackboardTexture;
	gf::Sprite m_blackboardSprite;

	gf::Font m_font;
	gf::Text m_attemptsText;
	gf::Text m_wordText;
	gf::Text m_underscoresText;
	gf::Text m_informationText;
};

#endif // SRC_GAME_HPP
