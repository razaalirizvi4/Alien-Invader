#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

bool collision(const sf::Sprite& sprite, const sf::Sprite& otherSprite)
{
	return sprite.getGlobalBounds().intersects(otherSprite.getGlobalBounds());
}
void runGame() {
    int score = 0;
    int level = 0;
    const int size = 5;
    int lives = 0;

    sf::SoundBuffer buffer;
    buffer.loadFromFile("guns.wav");

    sf::Music backg;
    backg.openFromFile("bgs.wav");

    sf::Font font;
    font.loadFromFile("ARCADECLASSIC.ttf");
    sf::Text sc;
    sc.setFont(font);
    sc.setPosition(100.0, 0.0);
    sc.setCharacterSize(24);

    sf::Text sco;
    sco.setFont(font);
    sco.setString("SCORE ");
    sco.setCharacterSize(24);

    sf::Text lvl;
    lvl.setFont(font);
    lvl.setString("LEVEL ");
    lvl.setPosition(0.0, 24.0);
    lvl.setCharacterSize(24);

    sf::Text life;
    life.setFont(font);
    life.setString("LIVES ");
    life.setPosition(0.0, 48.0);
    life.setCharacterSize(24);

    sf::Text lv;
    lv.setFont(font);
    lv.setPosition(70.0, 48.0);
    lv.setCharacterSize(24);

    sf::Text levl;
    levl.setFont(font);
    levl.setPosition(70.0, 24.0);
    levl.setCharacterSize(24);

    sf::Sound gun;
    gun.setBuffer(buffer);
    sf::RenderWindow window(sf::VideoMode(1388, 720), "DA GAME TO GAME ALL GAMES", sf::Style::Close | sf::Style::Titlebar);
    sf::Sprite player;
    sf::Sprite bullet;
    sf::Sprite enemies[size];
    sf::Sprite background;

    sf::Texture playerTexture;
    sf::Texture bulletTexture;
    sf::Texture enemyTexture;
    sf::Texture back;

    playerTexture.loadFromFile("les.png");
    bulletTexture.loadFromFile("bullet.png");
    enemyTexture.loadFromFile("meow.png");
    back.loadFromFile("back.jpg");

    player.setTexture(playerTexture);
    bullet.setTexture(bulletTexture);
    background.setTexture(back);

    for (int i = 0; i < size; ++i)
    {
        enemies[i].setTexture(enemyTexture);
        enemies[i].setColor(sf::Color(255, 255, 255, 255));
        enemies[i].setScale(0.2f, 0.2f);
        enemies[i].setPosition(static_cast<float>(rand() % (1200 - 10) + 10), 0.0f);
    }

    player.setScale(0.1f, 0.1f);
    player.setPosition(650.0f, 550.0f);
    background.setScale(2.5f, 2.5f);
    bullet.setScale(0.2f, 0.1f);

    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            if (evnt.type == sf::Event::Closed)
                window.close();
        }

        for (int i = 0; i < size; ++i)
        {
            enemies[i].move(0.0f, 0.05f);

            if (enemies[i].getPosition().y > static_cast<float>(window.getSize().y))
            {
                enemies[i].setPosition(static_cast<float>(rand() % (1200 - 10) + 10), 0.0f);
            }

            if (collision(player, enemies[i]))
            {
                enemies[i].setPosition(2000, 2000);
                lives++;
                lv.setString(std::to_string(3 - lives));
                if (lives == 3) {
                    std::cout << "GAME OVER";
                    window.close();
                }

            }

            if (collision(enemies[i], bullet))
            {
                enemies[i].setPosition(-100, -10
                ); // Move the enemy out of the window
                score += 10;
                sc.setString(std::to_string(score));

            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            player.move(2.0f, 0.0f);
            if (player.getPosition().x < 0.0f) {
                player.setPosition(0.0f, player.getPosition().y);
            }
            else if (player.getPosition().x > 1329.0f) {
                player.setPosition(1329.0f, player.getPosition().y);
            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            player.move(-2.0f, 0.0f);
            if (player.getPosition().x < 0.0f) {
                player.setPosition(0.0f, player.getPosition().y);
            }
            else if (player.getPosition().x > 1329.0f) {
                player.setPosition(1329.0f, player.getPosition().y);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
        {
            sf::Vector2f playerPos = player.getPosition();
            bullet.setPosition(playerPos.x, playerPos.y);
            bullet.move(0.0f, -10.0f);
            gun.play();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            window.close();
        }

        window.clear();
        window.draw(background);
        window.draw(player);
        window.draw(bullet);

        for (int i = 0; i < size; ++i)
        {
            if (enemies[i].getPosition().y >= 0) // Only draw enemies that are on screen
            {
                window.draw(enemies[i]);
                if (score <= 100) {
                    enemies[i].move(0.0f, 0.5f);
                    level = 1;
                }
                else if (score <= 200 && score > 100) {
                    enemies[i].move(0.0f, 0.8f);
                    level = 2;
                }
                else if (score <= 300 && score > 200) {
                    enemies[i].move(0.0f, 1.1f);
                    level = 3;
                }
                else if (score <= 400 && score > 300) {
                    enemies[i].move(0.0f, 1.4f);
                    level = 4;
                }
                else {
                    enemies[i].move(0.0f, 1.7f);
                    level = 5;
                }
            }
        }
        levl.setString(std::to_string(level));
        bullet.move(0.0f, -15.0f);
        window.draw(life);
        window.draw(lv);
        window.draw(levl);
        window.draw(lvl);
        window.draw(sco);
        window.draw(sc);
        window.display();
    }

}

int main() {
	sf::RenderWindow menu(sf::VideoMode(1366, 720), "Menu", sf::Style::Close);
	sf::RectangleShape option1(sf::Vector2f(500.0, 35.0));
	sf::RectangleShape option2(sf::Vector2f(500.0, 35.0));
	sf::RectangleShape option3(sf::Vector2f(500.0, 35.0));
    sf::RectangleShape men(sf::Vector2f(1366.0, 720.0));

    sf::Texture me;
    me.loadFromFile("menubg.jpg");
    men.setTexture(&me);

	option1.setOrigin(sf::Vector2f(250.0, 17.5));
	option2.setOrigin(sf::Vector2f(250.0, 17.5));
	option3.setOrigin(sf::Vector2f(250.0, 17.5));

	option1.setPosition(683.0, 260.0);
	option2.setPosition(683.0, 360.0);
	option3.setPosition(683.0, 460.0);

    option1.setFillColor(sf::Color::Green);
    option2.setFillColor(sf::Color::Red);
    option3.setFillColor(sf::Color::Magenta);
    
    sf::Font font;
    font.loadFromFile("ARCADECLASSIC.ttf");
    sf::Text op1;
    op1.setFont(font);
    op1.setPosition(615.0, 245.0);
    op1.setCharacterSize(24);
    op1.setString("ORIGINAL GAME");

    sf::Text op2;
    op2.setFont(font);
    op2.setPosition(635.0, 345.0);
    op2.setCharacterSize(24);
    op2.setString("BLITZ MODE");

    sf::Text op3;
    op3.setFont(font);
    op3.setPosition(635.0, 445.0);
    op3.setCharacterSize(24);
    op3.setString("CLOSE GAME");

	while (menu.isOpen()) {
        bool rungame = false;
		sf::Event event;
		while (menu.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				menu.close();
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(menu);
			sf::FloatRect optionsIn = option1.getGlobalBounds();
            sf::FloatRect optionsIn2 = option2.getGlobalBounds();
            sf::FloatRect optionsIn3 = option3.getGlobalBounds();
			if (optionsIn.contains((float)mousePos.x, (float)mousePos.y)) {
				rungame = true;
			}
            if (optionsIn2.contains((float)mousePos.x, (float)mousePos.y)) {
                rungame = true;
            }
            if (optionsIn3.contains((float)mousePos.x, (float)mousePos.y)) {
                menu.close();
            }
		}
		menu.clear();
        menu.draw(men);
		menu.draw(option1);
		menu.draw(option2);
		menu.draw(option3);
        menu.draw(op1);
        menu.draw(op2);
        menu.draw(op3);
        
		menu.display();
        if (rungame == true) {
            runGame();
        }
	}
}
