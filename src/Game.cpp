#include "Game.hpp"

void Game::_initTexts()
{
    m_texts["word"] = gf::Text { m_word, m_font, 50 };
    m_texts.at("word").setLetterSpacing(10.f);
    m_texts.at("word").setPosition(
        { (m_window.getSize().x - m_texts.at("word").getLocalBounds().getSize().x) / 2.f,
        (m_window.getSize().y - m_texts.at("word").getLocalBounds().getSize().y) * 0.7f }
    );
    m_texts.at("word").setColor(gf::Color::White);
    
    m_texts["information"] = gf::Text { "Press on a letter", m_font, 30 };
    m_texts.at("information").setPosition(
        { (m_window.getSize().x - m_texts.at("information").getLocalBounds().getSize().x) / 2.f, 
        m_texts.at("word").getPosition().y + m_texts.at("word").getCharacterSize() + m_texts.at("information").getCharacterSize() }
    );
    m_texts.at("information").setColor(gf::Color::White);
}

void Game::_initSprites()
{
    m_textures["blackboard"] = gf::Texture { "../assets/images/blackboard.jpg" };
    m_textures["hangman"] = gf::Texture { "../assets/images/hangman.png" };

    m_sprites["blackboard"].setTexture(m_textures.at("blackboard"));
    m_sprites["hangman"].setTexture(m_textures.at("hangman"), gf::RectF::fromPositionSize({ 1.f / 11.f * m_attemptsMax, 0.f }, { 1.f / 11.f, 1.f }));
    m_sprites.at("hangman").setPosition({ (m_window.getSize().x - m_sprites.at("hangman").getLocalBounds().getSize().x) / 2.f, 0.f });
}

Game::Game()
    : m_window { "GF Hangman", { 800, 600 } }, m_renderer { m_window },
      m_attemptsMax { 10 }, m_attempts { m_attemptsMax },
      m_font { "../assets/fonts/SF Atarian System.ttf" }
{
    std::ifstream ifs("../assets/data/words.data");
    std::getline(ifs, m_secretWord);

    m_word = std::string(m_secretWord.size(), '_');

	m_window.setResizable(false);
    m_renderer.clear(gf::Color::Black);

    this->_initTexts();
    this->_initSprites();
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
    else
    {
        m_attempts--;
        
        m_sprites["hangman"].setTexture(
            m_textures.at("hangman"), 
            gf::RectF::fromPositionSize({ 1.f / 11.f * m_attempts, 0.f }, { 1.f / 11.f, 1.f })
        );
    }
}

void Game::changeInformationText(const std::string &newText)
{
    if (m_texts.at("information").getString() != newText)
    {
        m_texts.at("information").setString(newText);
        m_texts.at("information").setPosition(
            { (m_window.getSize().x - m_texts.at("information").getLocalBounds().getSize().x) / 2, 
            m_texts.at("word").getPosition().y + m_texts.at("word").getCharacterSize() + m_texts.at("information").getCharacterSize() }
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
                        if ((keyName.size() == 1) && std::isalpha(keyName[0]))
                        {
                            if (m_usedLetters.find(keyName[0]) != std::string::npos)
                            {
                                this->changeInformationText("You already entered that letter!");
                            }
                            else
                            {
                                this->changeInformationText("You pressed on " + keyName);
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

    if (m_texts.at("word").getString() != m_word)
    {
        m_texts.at("word").setString(m_word);
    }
}

void Game::render()
{
    m_renderer.clear();

    for (auto &pair : m_sprites)
    {
        m_renderer.draw(pair.second);
    }

    for (auto &pair : m_texts)
    {
        m_renderer.draw(pair.second);
    }

    m_renderer.display();
}

void Game::renderGameOverScreen(const std::string &text)
{
    this->changeInformationText(text);

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
