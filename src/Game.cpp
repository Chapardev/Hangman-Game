#include "Game.hpp"

Game::Game()
    : m_window { "GF Hangman", { 800, 600 } }, m_renderer { m_window },
      m_secretWord { "GANGE" }, m_attemptsMax { 10 },
      m_word(m_secretWord.size(), '_'), m_attempts { m_attemptsMax },
      m_blackboardTexture { "../assets/images/blackboard.jpg" },
      m_blackboardSprite { m_blackboardTexture },
      m_font { "../assets/fonts/SF Atarian System.ttf" },
      m_attemptsText { "Attempts: " + std::to_string(m_attempts), m_font, 24 },
      m_wordText { m_word, m_font, 50 },
      m_underscoresText { m_wordText },
      m_informationText { "Press on a letter", m_font, 30 }
{
	m_window.setResizable(false);
    m_renderer.clear(gf::Color::Black);

    m_attemptsText.setPosition( { 15.f, m_attemptsText.getCharacterSize() + 10.f } );
    m_attemptsText.setColor(gf::Color::White);

    m_wordText.setLetterSpacing(10.f);
    m_wordText.setPosition(
        { (m_window.getSize().x - m_wordText.getLocalBounds().getSize().x) / 2.f,
        (m_window.getSize().y - m_wordText.getLocalBounds().getSize().y) * 0.7f }
    );
    m_wordText.setColor(gf::Color::White);

    m_underscoresText.setLetterSpacing(m_wordText.getLetterSpacing());
    m_underscoresText.setPosition( { m_wordText.getPosition().x + 3.f, m_wordText.getPosition().y } );
    m_underscoresText.setColor(gf::Color::White);
    
    m_informationText.setPosition(
        { (m_window.getSize().x - m_informationText.getLocalBounds().getSize().x) / 2.f, 
        m_wordText.getPosition().y + m_wordText.getCharacterSize() + m_informationText.getCharacterSize() }
    );
    m_informationText.setColor(gf::Color::White);
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

void Game::changeInformationText(const std::string &newText)
{
    if (m_informationText.getString() != newText)
    {
        m_informationText.setString(newText);
        m_informationText.setPosition(
            { (m_window.getSize().x - m_informationText.getLocalBounds().getSize().x) / 2, 
            m_wordText.getPosition().y + m_wordText.getCharacterSize() + m_informationText.getCharacterSize() }
        );
    }
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
                                this->changeInformationText("You already entered that letter!");
                            }
                            else
                            {
                                this->changeInformationText("Press on a letter");
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

    m_renderer.draw(m_blackboardSprite);

    m_renderer.draw(m_attemptsText);
    m_renderer.draw(m_wordText);
    m_renderer.draw(m_underscoresText);
    m_renderer.draw(m_informationText);

    m_renderer.display();
}

void Game::renderGameOverScreen(const std::string &text)
{
    m_informationText.setString(text);
    m_informationText.setPosition(
        { (m_window.getSize().x - m_informationText.getLocalBounds().getSize().x) / 2, 
        m_wordText.getPosition().y + m_wordText.getCharacterSize() + m_informationText.getCharacterSize() }
    );

    gf::Clock clock;
    while (clock.getElapsedTime() < gf::seconds(1.f)) 
    {
        this->render();
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
        this->renderGameOverScreen("You won!");
    }
    else if (m_attempts == 0)
    {
        this->renderGameOverScreen("You lost!");
    }
}
