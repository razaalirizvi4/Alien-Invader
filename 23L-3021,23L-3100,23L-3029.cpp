//23L-3021, 23L-3100, 23L-3029
#include<iostream>
#include<fstream>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

const int size = 5;
const float ENEMY_SPEED_INCREMENT = 100000.0f;
const float ENEMY_BULLET_SPEED_INCREMENT = 10.0f;
const int MAX_ENEMY_BULLETS = 10;
const int ENEMY_BULLET_SPEED = 20;
const int MAX_BULLETS = 2;
const int MAX_ENEMIES_ROWS = 5;  // Maximum number of rows of enemies
const int MAX_ENEMIES_COLS = 10; // Maximum number of columns of enemies
const float ENEMIES_WIDTH = 50.0f;
const float ENEMIES_HEIGHT = 50.0f;
const float ENEMIES_PADDING = 9.0f;
const float ENEMIES_SPEED = 900.0f;

struct Player
{
    sf::Sprite sprite;
    sf::Texture texture;
    int lives;
    bool gameOver;
    int score;
    float bulletCooldown = 0.5f; // Set the cooldown time in seconds
    float timeSinceLastShot = 0.0f;

    void initialize();
    void move(float offsetX, float offsetY, float deltaTime);
};

struct Enemies
{
    sf::Sprite sprite; // Replace sf::RectangleShape with sf::Sprite
    sf::Texture texture;
    sf::Vector2f velocity;
    bool isActive; // New member variable

    void initialize(sf::Vector2f position);
    void move(float offsetX, float offsetY);
};

struct Bullet
{
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool isActive; // New flag to indicate if the bullet is active

    void initialize(sf::Vector2f position);
    void move(float deltaTime);
};

struct EnemyBullet
{
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool isActive;

    void initialize(sf::Vector2f position);
    void move(float deltaTime);
};

// Yasir Logic
void handlePlayerMovement(Player& player, sf::Clock& clock);
void handleBulletShooting(Player& player, Bullet bullets[],
    int& bulletCount);
void handlePlayerEnemiesCollisions(
    Player& player, Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int enemiesCount);

void updateBullets(Bullet bullets[], int& bulletCount, float deltaTime);
void handleBulletEnemiesCollisions(
    Bullet bullets[MAX_BULLETS], int& bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS], Player& player,
    int& enemiesCount);

void updateEnemiesFormation(
    int& currentLevel, Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int& enemiesCount, float deltaTime, EnemyBullet enemyBullets[],
    int& enemyBulletCount);
void spawnEnemiesFormation(
    sf::RenderWindow& window,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int& enemiesCount);

void updateEnemyBullets(EnemyBullet enemyBullets[], int& enemyBulletCount,
    float deltaTime, int currentLevel);
void handlePlayerCollisionsWithEnemyBullets(Player& player,
    EnemyBullet enemyBullets[],
    int& enemyBulletCount);
void shootEnemyBullet(EnemyBullet enemyBullets[], int& enemyBulletCount,
    sf::Vector2f enemyPosition);
bool isEnemyInFront(Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int row, int col);
void tryEnemyShooting(
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS], int row, int col,
    EnemyBullet enemyBullets[], int& enemyBulletCount);
bool isAnyEnemyBulletActive(const EnemyBullet enemyBullets[],
    int enemyBulletCount);

void processGame(sf::RenderWindow& window, int& currentLevel, Player& player,
    Bullet bullets[MAX_BULLETS], int& bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int& enemiesCount, sf::Clock& clock,
    EnemyBullet enemyBullets[], int& enemyBulletCount);
void processEvents(sf::RenderWindow& window, Player& player,
    Bullet bullets[MAX_BULLETS], int& bulletCount,
    sf::Clock& clock);
void updateGame(int& currentLevel, Bullet bullets[MAX_BULLETS],
    int& bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    float deltaTime, Player& player, int enemiesCount,
    EnemyBullet enemyBullets[], int& enemyBulletCount);
void cleanupGame(sf::RenderWindow& window);

void drawPlayer(sf::RenderWindow& window, const Player& player);
void drawBullets(sf::RenderWindow& window,
    const Bullet bullets[MAX_BULLETS], int bulletCount);
void drawEnemies(
    sf::RenderWindow& window,
    const Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS]);
void drawEnemyBullets(sf::RenderWindow& window,
    const EnemyBullet enemyBullets[MAX_ENEMY_BULLETS],
    int enemyBulletCount);
void renderGameOver(sf::RenderWindow& window);
void renderScore(sf::RenderWindow& window, const Player& player,
    sf::Text text);
void renderGame(sf::RenderWindow& window, const Player& player,
    Bullet bullets[MAX_BULLETS], int bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    EnemyBullet enemyBullets[MAX_ENEMY_BULLETS],
    int enemyBulletCount);

void runGame(sf::RenderWindow& window, int& currentLevel, Player& player,
    Bullet bullets[MAX_BULLETS], int& bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int& enemiesCount, sf::Clock& clock,
    EnemyBullet enemyBullets[MAX_ENEMY_BULLETS],
    int& enemyBulletCount);

// Yasir Logic
void Player::initialize()
{
    if (!texture.loadFromFile("les.png"))
        std::cout << "Failed to load player image!" << std::endl;

    sprite.setTexture(texture);
    sprite.setPosition(370, 500);
    sprite.scale(0.1f, 0.1f);
    sprite.setColor(sf::Color(0xFFFFFFFF));
    lives = 3;
    gameOver = false;
    score = 0;
}

void Player::move(float offsetX, float offsetY, float deltaTime)
{
    if (!gameOver) // Only move if the game is not over
    {
        float speed = 400.0f;
        sf::Vector2f newPosition = sprite.getPosition() +
            sf::Vector2f(offsetX * speed * deltaTime,
                offsetY * speed * deltaTime);
        if (newPosition.x >= 0 &&
            newPosition.x + sprite.getGlobalBounds().width <= 800 &&
            newPosition.y >= 0 &&
            newPosition.y + sprite.getGlobalBounds().height <= 600)
            sprite.move(offsetX * speed * deltaTime,
                offsetY * speed * deltaTime);
    }
}

void Enemies::initialize(sf::Vector2f position)
{
    if (!texture.loadFromFile("meow.png"))
        std::cout << "Failed to load player image!" << std::endl;

    sprite.setTexture(texture);
    sprite.scale(0.1f, 0.1f);
    sprite.setColor(sf::Color(0xFFFFFFFF));
    sprite.setPosition(position);
    isActive = true;
}

void Enemies::move(float offsetX, float offsetY)
{
    sprite.move(offsetX, offsetY);
}

void Bullet::initialize(sf::Vector2f position)
{
    shape.setSize(sf::Vector2f(5, 25));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(position);
    velocity = sf::Vector2f(0, -100000); // Adjust the speed as needed
    isActive = true;
}

void Bullet::move(float deltaTime) { shape.move(velocity * deltaTime); }

void EnemyBullet::initialize(sf::Vector2f position)
{
    shape.setSize(sf::Vector2f(5, 5));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(position);
    velocity =
        sf::Vector2f(0, ENEMY_BULLET_SPEED); // Adjust the speed as needed
    isActive = true;
}

void EnemyBullet::move(float deltaTime)
{
    shape.move(velocity * deltaTime);
}

void handlePlayerMovement(Player& player, sf::Clock& clock)
{
    float deltaTime = clock.restart().asSeconds();
    // Handle player movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        player.move(-1.0f, 0.0f, deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        player.move(1.0f, 0.0f, deltaTime);
}

void handleBulletShooting(Player& player, Bullet bullets[],
    int& bulletCount)
{
    // Shoot bullets on space key press
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
        bulletCount < MAX_BULLETS)
    {
        bullets[bulletCount].initialize(
            player.sprite.getPosition() +
            sf::Vector2f(player.sprite.getGlobalBounds().width / 2, 0));
        bulletCount++;
    }
}

void handlePlayerEnemiesCollisions(
    Player& player, Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int enemiesCount)
{
    for (int i = 0; i < MAX_ENEMIES_ROWS; ++i)
    {
        for (int j = 0; j < MAX_ENEMIES_COLS; ++j)
        {
            // Check if we have reached the actual count of enemies
            if (i * MAX_ENEMIES_COLS + j >= enemiesCount)
            {
                break;
            }

            if (player.sprite.getGlobalBounds().intersects(
                enemies[i][j].sprite.getGlobalBounds()))
            {
                player.lives = 0;
                player.score = 0;
                player.sprite.setPosition(370, 500);
                if (player.lives <= 0)
                {
                    player.gameOver = true;
                }
            }
        }
    }
}

void handleBulletEnemiesCollisions(
    Bullet bullets[MAX_BULLETS], int& bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS], Player& player,
    int& enemiesCount)
{
    static const sf::Time collisionCooldown =
        sf::milliseconds(200); // Adjust as needed
    static sf::Clock collisionClock;

    for (int i = 0; i < bulletCount; ++i)
    {
        for (int row = 0; row < MAX_ENEMIES_ROWS; ++row)
        {
            for (int col = 0; col < MAX_ENEMIES_COLS; ++col)
            {
                // Check if we have reached the actual count of enemies
                if (row * MAX_ENEMIES_COLS + col >= enemiesCount)
                    break;
                if (bullets[i].isActive && enemies[row][col].isActive)
                {
                    if (bullets[i].shape.getGlobalBounds().intersects(
                        enemies[row][col]
                        .sprite.getGlobalBounds()))
                    {
                        if (collisionClock.getElapsedTime() >
                            collisionCooldown)
                        {

                            enemies[row][col].isActive = false;
                            bullets[i] = bullets[bulletCount - 1];
                            bulletCount--;
                            bullets[i].isActive = false;

                            // Increase the player's score
                            player.score += 10;
                            
                           
                            

                            // Reset the cooldown timer
                            collisionClock.restart();

                            // Decrement counters to recheck the current
                            // position
                            i--;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void updateEnemiesFormation(
    int& currentLevel, Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int& enemiesCount, float deltaTime, EnemyBullet enemyBullets[],
    int& enemyBulletCount)
{
    static bool moveRight = true;

    // Adjust enemy speed based on the current level
    float enemySpeed =
        ENEMIES_SPEED +
        static_cast<float> ((currentLevel - 1)) * ENEMY_SPEED_INCREMENT;

    for (int i = 0; i < MAX_ENEMIES_ROWS; ++i)
    {
        for (int j = 0; j < MAX_ENEMIES_COLS; ++j)
        {
            // Check if we have reached the actual count of enemies
            if (i * MAX_ENEMIES_COLS + j >= enemiesCount)
            {
                break;
            }

            sf::Vector2f enemyPosition =
                enemies[i][j].sprite.getPosition();
            if (moveRight)
            {
                // Update the position of the enemy
                enemies[i][j].move(enemySpeed * deltaTime, 0.0f);

                // Check if the enemy has hit the right wall
                if (enemyPosition.x + ENEMIES_WIDTH >= 800)
                {
                    moveRight = false;
                    for (int row = 0; row < MAX_ENEMIES_ROWS; ++row)
                    {
                        for (int col = 0; col < MAX_ENEMIES_COLS; ++col)
                        {
                            enemies[row][col].move(0.0f,
                                ENEMIES_HEIGHT);
                        }
                    }
                    break;
                }
            }
            else
            {
                // Update the position of the enemy
                enemies[i][j].move(-enemySpeed * deltaTime, 0.0f);

                // Check if the enemy has hit the left wall
                if (enemyPosition.x <= 0)
                {
                    moveRight = true;
                    for (int row = 0; row < MAX_ENEMIES_ROWS; ++row)
                    {
                        for (int col = 0; col < MAX_ENEMIES_COLS; ++col)
                        {
                            enemies[row][col].move(0.0f,
                                ENEMIES_HEIGHT);
                        }
                    }
                    break;
                }
            }
            tryEnemyShooting(enemies, i, j, enemyBullets, enemyBulletCount);
        }
    }
}

void spawnEnemiesFormation(
    sf::RenderWindow& window,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int& enemiesCount)
{
    window.clear();
    for (int i = 0; i < MAX_ENEMIES_ROWS; ++i)
    {
        for (int j = 0; j < MAX_ENEMIES_COLS; ++j)
        {
            enemies[i][j].initialize(sf::Vector2f(
                static_cast<float> (40 +
                    static_cast<float> (j) *
                    (ENEMIES_WIDTH + ENEMIES_PADDING)),
                static_cast<float> (
                    40 + static_cast<float> (i) *
                    (ENEMIES_HEIGHT + ENEMIES_PADDING))));
            // Resetting the sprite properties
            // Assuming texture is a member of the Enemies struct
            enemies[i][j].sprite.setTexture(enemies[i][j].texture);
        }
    }
    enemiesCount = MAX_ENEMIES_ROWS * MAX_ENEMIES_COLS;
}

// Add this function to the bottom of updateBullets:
void updateBullets(Bullet bullets[], int& bulletCount, float deltaTime)
{
    // Update bullets
    for (int i = 0; i < bulletCount; ++i)
    {
        if (bullets[i].isActive)
        { // Check if the bullet is active before updating
            bullets[i].shape.move(bullets[i].velocity * deltaTime);
        }
    }
    // Remove bullets that are out of bounds
    for (int i = 0; i < bulletCount; ++i)
        if (bullets[i].shape.getPosition().y < 0)
        {
            bullets[i] = bullets[bulletCount - 1];
            bulletCount--;
            i--;
        }
}

void tryEnemyShooting(
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS], int row, int col,
    EnemyBullet enemyBullets[], int& enemyBulletCount)
{
    // Check if the enemy has a clear line of sight to shoot
    if (enemies[row][col].isActive &&
        !isEnemyInFront(enemies, row, col))
    {
        // Random chance for the enemy to shoot
        if (rand() % 100 < 1.0) // Adjust the percentage as needed
        {
            shootEnemyBullet(enemyBullets, enemyBulletCount,
                enemies[row][col].sprite.getPosition());
        }
    }
}

bool isEnemyInFront(Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int row, int col)
{
    // Check if there's an active enemy in the row in front of the given enemy
    int enemiesCount = MAX_ENEMIES_COLS * MAX_ENEMIES_ROWS;
    for (int indexRow = row + 1; indexRow < MAX_ENEMIES_ROWS; ++indexRow)
    {
        // Check if we have reached the actual count of enemies
        if (indexRow * MAX_ENEMIES_COLS + col >= enemiesCount)
        {
            break;
        }

        if (enemies[indexRow][col].isActive)
        {
            return true; // There is an enemy in front
        }
    }
    return false;
}

bool isAnyEnemyBulletActive(const EnemyBullet enemyBullets[],
    int enemyBulletCount)
{
    for (int i = 0; i < enemyBulletCount; ++i)
    {
        if (enemyBullets[i].isActive)
        {
            return true; // At least one enemy bullet is active
        }
    }
    return false; // No enemy bullets are currently active
}

void shootEnemyBullet(EnemyBullet enemyBullets[], int& enemyBulletCount,
    sf::Vector2f position)
{
    // Check if there is space for a new enemy bullet and no active bullets
    if (enemyBulletCount < MAX_ENEMY_BULLETS &&
        !isAnyEnemyBulletActive(enemyBullets, enemyBulletCount))
    {
        // Find an inactive bullet slot
        for (int i = 0; i < MAX_ENEMY_BULLETS; ++i)
        {
            if (!enemyBullets[i].isActive)
            {
                // Initialize the bullet and mark it as active
                enemyBullets[i].initialize(position);
                enemyBullets[i].isActive = true;
                enemyBulletCount++;
                break;
            }
        }
    }
}

void updateEnemyBullets(EnemyBullet enemyBullets[], int& enemyBulletCount,
    float deltaTime, int currentLevel)
{
    float enemyBulletSpeed =
        ENEMY_BULLET_SPEED + static_cast<float> ((currentLevel - 1)) *
        ENEMY_BULLET_SPEED_INCREMENT;

    for (int i = 0; i < enemyBulletCount; ++i)
    {
        if (enemyBullets[i].isActive)
        {
            enemyBullets[i].move(enemyBulletSpeed * deltaTime);

            // Check if the enemy bullet has gone off-screen
            if (enemyBullets[i].shape.getPosition().y > 600)
            {
                // Deactivate the enemy bullet
                enemyBullets[i].isActive = false;
                enemyBulletCount--;
            }
        }
    }
}

void handlePlayerCollisionsWithEnemyBullets(Player& player,
    EnemyBullet enemyBullets[],
    int& enemyBulletCount)
{
    for (int i = 0; i < enemyBulletCount; ++i)
    {
        if (enemyBullets[i].isActive &&
            player.sprite.getGlobalBounds().intersects(
                enemyBullets[i].shape.getGlobalBounds()))
        {
            // Handle player hit by enemy bullet
            // For example, decrease player lives, reset player position, etc.
            // Here, we'll just deactivate the bullet
            player.lives--;
            // Check for game over
            if (player.lives <= 0)
            {
                player.gameOver = true;
            }
            enemyBullets[i].isActive = false;
            
        }
    }
}
using namespace std;
void sethighscore(const Player& player)
{
    ifstream infile;
    ofstream outfile;
    infile.open("highscore normal.txt");
    outfile.open("highscore normal.txt");
    int highscore = 0;
    infile >> highscore;
    if (player.score > highscore) {

        outfile << player.score;
    }
    else {
        outfile << highscore;
    }
    infile.close();
    outfile.close();
}

void renderGame(sf::RenderWindow& window, const Player& player,
    Bullet bullets[MAX_BULLETS], int bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    EnemyBullet enemyBullets[MAX_ENEMY_BULLETS],
    int enemyBulletCount)
{
    window.clear();

    sf::Font font;
    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
        std::cout << "Failed to load font!" << std::endl;

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);

    sf::Text win;
    win.setFont(font);
    win.setCharacterSize(20);
    win.setFillColor(sf::Color::White);

    if (!player.gameOver)
    {
        drawPlayer(window, player);
        drawBullets(window, bullets, bulletCount);
        drawEnemies(window, enemies);
        drawEnemyBullets(window, enemyBullets,
            enemyBulletCount); // Draw enemy bullets
    }
    else
    {
        renderGameOver(window);
    }

    renderScore(window, player, scoreText);
    if (player.score == 500) {
        sf::RenderWindow in(sf::VideoMode(800, 600), "YOU WIN", sf::Style::Close);
        sf::Text win;
        win.setFont(font);
        win.setCharacterSize(20);
        win.setOrigin(40, 40);
        win.setPosition(400, 300);
        win.setString("YOU WIN!");
        while (in.isOpen()) {
            sf::Event winner;
            while (in.pollEvent(winner)) {
                if (winner.type == sf::Event::Closed) {
                    in.close();
                    window.close();
                }
            }
            in.draw(win);
            in.display();
        }
    }

    window.display();
}

void drawPlayer(sf::RenderWindow& window, const Player& player)
{
    window.draw(player.sprite);
}

void drawBullets(sf::RenderWindow& window,
    const Bullet bullets[MAX_BULLETS], int bulletCount)
{
    for (int i = 0; i < bulletCount; ++i)
    {
        if (bullets[i].isActive)
        { // Check if the bullet is active before drawing
            window.draw(bullets[i].shape);
        }
    }
}

void drawEnemies(
    sf::RenderWindow& window,
    const Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS])
{
    for (int i = 0; i < MAX_ENEMIES_ROWS; ++i)
    {
        for (int j = 0; j < MAX_ENEMIES_COLS; ++j)
        {
            // Draw the sprite if the enemy is active
            if (enemies[i][j].isActive)
            {
                window.draw(enemies[i][j].sprite);
            }
        }
    }
}

void drawEnemyBullets(sf::RenderWindow& window,
    const EnemyBullet enemyBullets[MAX_ENEMY_BULLETS],
    int enemyBulletCount)
{
    for (int i = 0; i < enemyBulletCount; ++i)
    {
        if (enemyBullets[i].isActive)
        { // Check if the enemy bullet is active before drawing
            window.draw(enemyBullets[i].shape);
        }
    }
}

void renderGameOver(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
        std::cout << "Failed to load font!" << std::endl;

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(20);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(340, 300);

    window.draw(gameOverText);
}


void renderScore(sf::RenderWindow& window, const Player& player,
    sf::Text text)
{

    if (!player.gameOver)
    {

        text.setString("Score: " + std::to_string(player.score));
        text.setPosition(10, 10);
        window.draw(text);
        sethighscore(player);
    }

}

void processGame(sf::RenderWindow& window, int& currentLevel, Player& player,
    Bullet bullets[MAX_BULLETS], int& bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int& enemiesCount, sf::Clock& clock,
    EnemyBullet enemyBullets[], int& enemyBulletCount)
{
    processEvents(window, player, bullets, bulletCount, clock);
    updateGame(currentLevel, bullets, bulletCount, enemies,
        clock.restart().asSeconds(), player, enemiesCount,
        enemyBullets, enemyBulletCount);
    renderGame(window, player, bullets, bulletCount, enemies, enemyBullets,
        enemyBulletCount);
}

void processEvents(sf::RenderWindow& window, Player& player,
    Bullet bullets[MAX_BULLETS], int& bulletCount,
    sf::Clock& clock)
{
    sf::Event event;
    while (window.pollEvent(event))
        if (event.type == sf::Event::Closed)
            window.close();

    handlePlayerMovement(player, clock);

    handleBulletShooting(player, bullets, bulletCount);
}

void updateGame(int& currentLevel, Bullet bullets[MAX_BULLETS],
    int& bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    float deltaTime, Player& player, int enemiesCount,
    EnemyBullet enemyBullets[], int& enemyBulletCount)
{
    updateBullets(bullets, bulletCount, deltaTime);
    updateEnemiesFormation(currentLevel, enemies, enemiesCount, deltaTime,
        enemyBullets, enemyBulletCount);
    handlePlayerEnemiesCollisions(player, enemies,
        enemiesCount); // Fixed function name
    handleBulletEnemiesCollisions(bullets, bulletCount, enemies, player,
        enemiesCount);
    updateEnemyBullets(enemyBullets, enemyBulletCount, deltaTime,
        currentLevel);
    handlePlayerCollisionsWithEnemyBullets(player, enemyBullets,
        enemyBulletCount);
}

void cleanupGame(sf::RenderWindow& window) { window.close(); }

void runGame(sf::RenderWindow& window, int& currentLevel, Player& player,
    Bullet bullets[MAX_BULLETS], int& bulletCount,
    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS],
    int& enemiesCount, sf::Clock& clock,
    EnemyBullet enemyBullets[MAX_ENEMY_BULLETS],
    int& enemyBulletCount)
{
    spawnEnemiesFormation(window, enemies, enemiesCount);
    while (window.isOpen())
    {
        processGame(window, currentLevel, player, bullets, bulletCount,
            enemies, enemiesCount, clock, enemyBullets,
            enemyBulletCount);
        if (player.gameOver)
        {
            sf::sleep(sf::seconds(3));
            window.close();
        }
    }
    cleanupGame(window);
}

void runOG() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    int currentLevel = 1;

    Player player;
    player.initialize();

    Bullet bullets[MAX_BULLETS];
    int bulletCount = 1;

    Enemies enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_COLS];
    int enemiesCount = 49;

    EnemyBullet enemyBullets[MAX_ENEMY_BULLETS];
    int enemyBulletCount = 0;

    sf::Clock clock; // Declare the clock instance

    runGame(window, currentLevel, player, bullets, bulletCount, enemies,
        enemiesCount, clock, enemyBullets, enemyBulletCount);
}
bool collision(const sf::Sprite& sprite, const sf::Sprite& otherSprite)
{
    return sprite.getGlobalBounds().intersects(otherSprite.getGlobalBounds());
}
using namespace std;
void runGame() {
    sf::Font font;
    font.loadFromFile("ARCADECLASSIC.ttf");
    fstream inFile("highscore.txt", ios::in | ios::out);
    int highscore = 0;
    inFile >> highscore;
    sf::Text i;
    i.setFont(font);
    i.setString("PRESS    A    TO MOVE LEFT AND    D    TO MOVE RIGHT     PRESS   ENTER   TO SHOOT \nPRESS ENTER KEY TO CONTINUE ");
    i.setCharacterSize(24);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

        int score = 0;
        int level = 0;
        const int size = 5;
        int lives = 0;

        sf::SoundBuffer buffer;
        buffer.loadFromFile("guns.wav");

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
        sf::RenderWindow window(sf::VideoMode(1388, 720), "DA GAME TO GAME ALL GAMES", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
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
        ofstream write("highscore.txt");
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
                lv.setString(std::to_string(3 - lives));
                if (collision(player, enemies[i]))
                {
                    enemies[i].setPosition(2000, 2000);
                    lives++;
                    
                    if (lives == 3) {
                        sf::RenderWindow over(sf::VideoMode(800, 600), "GAME OVER", sf::Style::Close);
                        sf::Text text;
                        text.setFont(font);
                        text.setString("GAME OVER!");
                        text.setPosition(400, 300);
                        text.setOrigin(48.0, 48.0);
                        text.setCharacterSize(24);
                        text.setFillColor(sf::Color::Red);
                        while (over.isOpen()) {
                            sf::Event overed;
                            while (over.pollEvent(overed)) {
                                if (overed.type == sf::Event::Closed) {
                                    over.close();
                                    window.close();
                                }
                            }
                            over.draw(text);
                            over.display();
                        }

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
            window.draw(bullet);
            window.draw(player);


            for (int i = 0; i < size; ++i)
            {
                if (enemies[i].getPosition().y >= 0)
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
        if (score > highscore) {
            inFile << score;
            write << score;
        }
        else {
            write << highscore;
        }
        inFile.close();
        write.close();
    }

}

using namespace sf;
void menu()
{
    void modeselect();
    RenderWindow window(sf::VideoMode(800, 600), "Menu", sf::Style::Default);
    SoundBuffer buffer;
    buffer.loadFromFile("sound.wav");
    Sound sound;
    sound.setBuffer(buffer);

    SoundBuffer buffer2;
    buffer2.loadFromFile("gun.wav");
    Sound sound2;
    sound2.setBuffer(buffer2);
    RectangleShape background;
    background.setSize(sf::Vector2f(800, 600));
    Texture Texture;
    Texture.loadFromFile("back.jpg");
    background.setTexture(&Texture);
    Font font;

    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
    {
        // error...
    }

    Text text2;
    Text text;


    // select the font
    text.setFont(font);
    // set the string to display
    text.setString("\n\n\n\t\t\tUp to the battleground");

    // set the character size
    text.setCharacterSize(54); // in pixels, not points!

    // set the color
    text.setFillColor(Color::Green);

    // set the text style
    text.setStyle(Text::Bold);
    text2.setFont(font);
    // set the string to display
    text2.setString("\n\n\n\n\n\t\t\t\t\t\tDown to the end");

    // set the character size
    text2.setCharacterSize(54); // in pixels, not points!

    // set the color
    text2.setFillColor(Color::Red);

    // set the text style
    text2.setStyle(Text::Bold);

    sound2.play();
    sound2.setLoop(true);

    // inside the main loop, between window.clear() and window.display()
    while (window.isOpen())
    {
        sf::Event event;
        window.setTitle("SFML window");


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (sf::Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Down)
                {

                    window.close();
                }
                else if (event.key.code == Keyboard::Up)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    modeselect();


                }

            }


        }

        window.draw(background);
        window.draw(text2);
        window.draw(text);

        window.display();
    }
}
void displayhighscorenormalmode()
{
    RenderWindow window(sf::VideoMode(800, 600), "Normal_mode highscore", sf::Style::Default);
    ifstream infile;
    infile.open("highscore normal.txt");
    int score = 0;
    infile >> score;
    SoundBuffer buffer;
    buffer.loadFromFile("sound.wav");
    Sound sound;
    sound.setBuffer(buffer);
    SoundBuffer buffer2;
    buffer2.loadFromFile("gun.wav");
    Sound sound2;
    sound2.setBuffer(buffer2);
    Text text;
    Text text2;
    Text text3;
    Font font;
    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
    {
        // error...
    }
    // select the font
    text.setFont(font);
    // set the string to display
    text.setString("\n\n\t\t\t\t\tHIGHSCORE TO BEAT");

    // set the character size
    text.setCharacterSize(54); // in pixels, not points!

    // set the color
    text.setFillColor(Color::Green);

    // set the text style
    text.setStyle(Text::Bold);
    text2.setFont(font);
    // set the string to display
    text2.setString("\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + to_string(score));
    // set the character size
    text2.setCharacterSize(32); // in pixels, not points!

    // set the color
    text2.setFillColor(Color::Red);

    // set the text style
    text2.setStyle(Text::Bold);
    text3.setFont(font);
    // set the string to display
    text3.setString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tPress Space to continue");
    // set the character size
    text3.setCharacterSize(32); // in pixels, not points!

    // set the color
    text3.setFillColor(Color::Red);

    // set the text style
    text3.setStyle(Text::Bold);
    sound2.play();
    sound2.setLoop(true);
    while (window.isOpen())
    {
        sf::Event event;
        window.setTitle("Initialise window");


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    runOG();
                    menu();
                }
                else if (event.key.code == Keyboard::Escape)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    menu();
                }


            }

        }
        window.clear(Color::Black);
        window.draw(text);
        window.draw(text2);
        window.draw(text3);
        window.display();

    }
}
void displayhighscoreblitzmode()
{
    RenderWindow window(sf::VideoMode(800, 600), "blitz_mode highscore", sf::Style::Default);
    ifstream infile;
    infile.open("highscore.txt");
    int score = 0;
    infile >> score;
    SoundBuffer buffer;
    buffer.loadFromFile("sound.wav");
    Sound sound;
    sound.setBuffer(buffer);
    SoundBuffer buffer2;
    buffer2.loadFromFile("gun.wav");
    Sound sound2;
    sound2.setBuffer(buffer2);
    Text text;
    Text text2;
    Text text3;
    Font font;
    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
    {
        // error...
    }
    // select the font
    text.setFont(font);
    // set the string to display
    text.setString("\n\n\t\t\t\t\tHIGHSCORE TO BEAT");

    // set the character size
    text.setCharacterSize(54); // in pixels, not points!

    // set the color
    text.setFillColor(Color::Green);

    // set the text style
    text.setStyle(Text::Bold);
    text2.setFont(font);
    // set the string to display
    text2.setString("\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + to_string(score));
    // set the character size
    text2.setCharacterSize(32); // in pixels, not points!

    // set the color
    text2.setFillColor(Color::Red);

    // set the text style
    text2.setStyle(Text::Bold);
    text3.setFont(font);
    // set the string to display
    text3.setString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\tPress Space to continue");
    // set the character size
    text3.setCharacterSize(32); // in pixels, not points!

    // set the color
    text3.setFillColor(Color::Red);

    // set the text style
    text3.setStyle(Text::Bold);
    sound2.play();
    sound2.setLoop(true);
    while (window.isOpen())
    {
        sf::Event event;
        window.setTitle("Initialise window");


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Space)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    runGame();
                    menu();
                }
                else if (event.key.code == Keyboard::Escape)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    menu();
                }


            }

        }
        window.clear(Color::Black);
        window.draw(text);
        window.draw(text2);
        window.draw(text3);
        window.display();

    }
}
void normalinstructions()
{
    void modeselect();
    RenderWindow window(sf::VideoMode(800, 600), "Normal instructions", sf::Style::Default);
    SoundBuffer buffer;
    buffer.loadFromFile("sound.wav");
    Sound sound;
    sound.setBuffer(buffer);
    SoundBuffer buffer2;
    buffer2.loadFromFile("gun.wav");
    Sound sound2;
    sound2.setBuffer(buffer2);
    Text text;
    Text text2;
    Font font;
    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
    {
        // error...
    }
    // select the font
    text.setFont(font);
    // set the string to display
    text.setString("\n\n\t\t\t\t\tINSTRUCTIONS");

    // set the character size
    text.setCharacterSize(54); // in pixels, not points!

    // set the color
    text.setFillColor(Color::Green);

    // set the text style
    text.setStyle(Text::Bold);
    text2.setFont(font);
    // set the string to display
    text2.setString("\n\n\n\n\n\n\n1 You have to destroy the army of aliens who are\ndescending towards you\n2 Press and hold LEFT to move left and RIGHT to move right\n3 Use LEFT MOUSE BUTTON to shoot\n4 Use escape to go back to modeselect\n5 Enter to advance\n\t\t\t\t\tBEST OF LUCK");

    // set the character size
    text2.setCharacterSize(32); // in pixels, not points!

    // set the color
    text2.setFillColor(Color::Red);

    // set the text style
    text2.setStyle(Text::Bold);
    sound2.play();
    sound2.setLoop(true);
    while (window.isOpen())
    {
        sf::Event event;
        window.setTitle("Initialise window");


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    modeselect();
                }
                else if (event.key.code == Keyboard::Enter)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    displayhighscorenormalmode();
                }

            }

        }
        window.clear(Color::Black);
        window.draw(text);
        window.draw(text2);
        window.display();

    }
}
void blitzinstructions()
{
    void modeselect();
    RenderWindow window(sf::VideoMode(800, 600), "Instructions", sf::Style::Default);
    SoundBuffer buffer;
    buffer.loadFromFile("sound.wav");
    Sound sound;
    sound.setBuffer(buffer);
    SoundBuffer buffer2;
    buffer2.loadFromFile("gun.wav");
    Sound sound2;
    sound2.setBuffer(buffer2);
    Text text;
    Text text2;
    Font font;
    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
    {
        // error...
    }
    // select the font
    text.setFont(font);
    // set the string to display
    text.setString("\n\n\t\t\t\t\tINSTRUCTIONS");

    // set the character size
    text.setCharacterSize(54); // in pixels, not points!

    // set the color
    text.setFillColor(Color::Green);

    // set the text style
    text.setStyle(Text::Bold);
    text2.setFont(font);
    // set the string to display
    text2.setString("\n\n\n\n\n\n\n1 Aliens coming at you\nTHINK FAST\n2 Press and hold A to move left and D to move right\n3 Use Enter to shoot\n4 Use escape to go back to modeselect\n5 Enter to advance\n\t\t\t\t\tBEST OF LUCK");

    // set the character size
    text2.setCharacterSize(32); // in pixels, not points!

    // set the color
    text2.setFillColor(Color::Red);

    // set the text style
    text2.setStyle(Text::Bold);
    sound2.play();
    sound2.setLoop(true);
    while (window.isOpen())
    {
        sf::Event event;
        window.setTitle("Initialise window");


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    modeselect();
                }
                else if (event.key.code == Keyboard::Enter)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    displayhighscoreblitzmode();
                }

            }

        }
        window.clear(Color::Black);
        window.draw(text);
        window.draw(text2);
        window.display();

    }
}

void modeselect()
{
    RenderWindow window(sf::VideoMode(800, 600), "ModeSelect", sf::Style::Default);
    SoundBuffer buffer;
    buffer.loadFromFile("sound.wav");
    Sound sound;
    sound.setBuffer(buffer);
    SoundBuffer buffer2;
    buffer2.loadFromFile("gun.wav");
    Sound sound2;
    sound2.setBuffer(buffer2);
    Text text;
    Text text2;
    Text text3;
    Font font;
    if (!font.loadFromFile("ARCADECLASSIC.ttf"))
    {
        // error...
    }
    // select the font
    text.setFont(font);
    // set the string to display
    text.setString("\n\n\n\n\n\tLeft to Normal");

    // set the character size
    text.setCharacterSize(45); // in pixels, not points!

    // set the color
    text.setFillColor(Color::Blue);
    text2.setFont(font);
    // set the string to display
    text2.setString("\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tRight to Blitz");

    // set the character size
    text2.setCharacterSize(45); // in pixels, not points!

    // set the color
    text2.setFillColor(Color::Red);

    // set the text style
    text2.setStyle(Text::Bold);
    text3.setFont(font);
    // set the string to display
    text3.setString("\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tEscape to Main Menu");

    // set the character size
    text3.setCharacterSize(45); // in pixels, not points!

    // set the color
    text3.setFillColor(Color::White);

    // set the text style
    text3.setStyle(Text::Bold);
    sound2.play();
    sound2.setLoop(true);
    while (window.isOpen())
    {
        sf::Event event;
        window.setTitle("Initialise window");
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    menu();
                }
                else if (event.key.code == Keyboard::Left)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    normalinstructions();
                }
                else if (event.key.code == Keyboard::Right)
                {
                    sound.play();
                    window.close();
                    sound2.stop();
                    blitzinstructions();
                }
            }
        }
        window.clear(Color::Black);
        window.draw(text);
        window.draw(text2);
        window.draw(text3);
        window.display();
    }
}
int main()
{
    menu();
    return 0;
}