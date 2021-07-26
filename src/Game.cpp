#include "Game.hpp"

Game::Game()
    : m_window { "GF Hangman", { 800, 600 } }, m_renderer { m_window },
      m_secretWord { "GANGE" }, m_attemptsMax { 10 },
      m_word(m_secretWord.size(), '_'), m_attempts { m_attemptsMax },
      m_font { "../assets/fonts/SF Atarian System.ttf" },
      m_wordText { m_word, m_font, 50 },
      m_attemptsText { "Attempts: " + std::to_string(m_attempts), m_font, 24 }
      // m_informationText { "Press on a letter" }
{
    // TODO: Publish this first version on Github
    // TODO: Updates information text ("Enter a letter", "You already entered that letter!", "You won", "You lost")

	m_window.setResizable(false);
    m_renderer.clear(gf::Color::Black);

    m_wordText.setLetterSpacing(10.f);
    m_wordText.setPosition((m_window.getSize() - m_wordText.getLocalBounds().getSize()) / 2.f);
    m_wordText.setColor(gf::Color::White);
    
    m_attemptsText.setPosition({ 10.f, m_attemptsText.getCharacterSize() + 5.f });
    m_attemptsText.setColor(gf::Color::White);
}

void Game::checkLetter(char letter)
{
    if (m_secretWord.find(letter) != std::string::npos)
    {
        for (int i { 0 }; m_secretWord.find(letter, i) != std::string::npos; i++)
        {
            m_word[m_secretWord.find(letter, i)] = letter;
        }
    }
    
    m_attempts--;
    m_attemptsText.setString("Attempts: " + std::to_string(m_attempts));
}

void Game::pollEvents()
{
    gf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case gf::EventType::Closed:
                m_window.close();
                break;

            case gf::EventType::KeyPressed:
                switch (event.key.keycode)
                {
                    case gf::Keycode::Escape:
                        m_window.close();
                        break;

                    default:
                    {
                        const std::string keyName { gf::Keyboard::getKeycodeName(event.key.keycode) };
                        if ((std::strlen(keyName.c_str()) == 1) && std::isalpha(keyName[0]))
                        {
                            if (m_usedLetters.find(keyName[0]) != std::string::npos)
                            {
                                std::cout << "You already entered that letter!\n";
                            }
                            else
                            {
                                this->checkLetter(keyName[0]);
                                m_usedLetters += keyName[0];
                            }
                        }
                        break;
                    }
                }
                break;

            default:
                break;
        }
    }
}

void Game::update()
{
    this->pollEvents();

    if (m_wordText.getString() != m_word)
    {
        m_wordText.setString(m_word);
    }
}

void Game::render()
{
    m_renderer.clear();

    m_renderer.draw(m_attemptsText);
    m_renderer.draw(m_wordText);

    m_renderer.display();
}

void Game::renderGameOverScreen(const std::string &text)
{
    m_wordText.setString(text);
    m_wordText.setPosition((m_window.getSize() - m_wordText.getLocalBounds().getSize()) / 2.f);

    gf::Clock clock;
    while (clock.getElapsedTime() < gf::seconds(1.5f)) 
    {
        m_renderer.clear();
        m_renderer.draw(m_wordText);
        m_renderer.display();
    }

    m_window.close();
}

void Game::run()
{
    while (m_window.isOpen() && (m_word != m_secretWord) && (m_attempts > 0))
    {
        this->update();

        this->render();
    }

    if (m_word == m_secretWord)
    {
        this->renderGameOverScreen("You won");
    }
    else if (m_attempts == 0)
    {
        this->renderGameOverScreen("You lost");
    }
}
