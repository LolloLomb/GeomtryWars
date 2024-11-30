#include "Game.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <typeinfo>

#define PI_CONV 3.1415926 * 2 / 360;

std::vector<std::string> split(std::string str, const char* delimiter){
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0;
    size_t end = 0;

    while((end = str.find(delimiter, start)) != std::string::npos) {
        token = str.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;
    }
    tokens.push_back(str.substr(start, std::string::npos));
    return tokens;
}

Game::Game(const std::string& config){
    init(config);
}

void Game::init(const std::string& config){
    srand(time(NULL));

    std::fstream input(config);
    std::string buffer;
    
    if (!input){
        std::cout << "Errore nell'aprire il file" << std::endl;
    }

    // Window Info
    std::getline(input, buffer);
    std::vector<std::string> Info = split(buffer, " ");
    {
    size_t viewMode = std::stoi(Info[4]);
    m_window.create(sf::VideoMode(std::stoi(Info[1]), std::stoi(Info[2])), "SHAPES", viewMode ? 8 : 4);
    m_window.setFramerateLimit(std::stoi(Info[3]));
    }
    // Font Info
    std::getline(input, buffer);
    Info = split(buffer, " ");
    {
    if (!m_font.loadFromFile(Info[1])){
        std::cout << "Font non trovato" << std::endl;
    }  
    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(std::stoi(Info[2]));
    m_scoreText.setFillColor(sf::Color(std::stoi(Info[3]), std::stoi(Info[4]), std::stoi(Info[5])));
    }
    // Player Info
    std::getline(input, buffer);
    Info = split(buffer, " ");
    {
    m_playerConfig.SR = std::stoi(Info[1]);
    m_playerConfig.CR = std::stoi(Info[2]);
    m_playerConfig.S = std::stof(Info[3]);
    m_playerConfig.FR = std::stoi(Info[4]);
    m_playerConfig.FG = std::stoi(Info[5]);
    m_playerConfig.FB = std::stoi(Info[6]);
    m_playerConfig.OR = std::stoi(Info[7]);
    m_playerConfig.OG = std::stoi(Info[8]);
    m_playerConfig.OB = std::stoi(Info[9]);
    m_playerConfig.OT = std::stoi(Info[10]);
    m_playerConfig.V = std::stoi(Info[11]);
    }
    // Enemy Info
    std::getline(input, buffer);
    Info = split(buffer, " ");
    {
    m_enemyConfig.SR = std::stoi(Info[1]);
    m_enemyConfig.CR = std::stoi(Info[2]);
    m_enemyConfig.SMIN = std::stof(Info[3]);
    m_enemyConfig.SMAX = std::stof(Info[4]);
    m_enemyConfig.OR = std::stoi(Info[5]);
    m_enemyConfig.OG = std::stoi(Info[6]);
    m_enemyConfig.OB = std::stoi(Info[7]);
    m_enemyConfig.OT = std::stoi(Info[8]);
    m_enemyConfig.VMIN = std::stoi(Info[9]);
    m_enemyConfig.VMAX = std::stoi(Info[10]);
    m_enemyConfig.L = std::stoi(Info[11]);
    m_enemyConfig.SI = std::stoi(Info[12]);
    }
    // Bullet Info
    std::getline(input, buffer);
    Info = split(buffer, " ");
    {
    m_bulletConfig.SR = std::stoi(Info[1]);
    m_bulletConfig.CR = std::stoi(Info[2]);
    m_bulletConfig.S = std::stof(Info[3]);
    m_bulletConfig.FR = std::stoi(Info[4]);
    m_bulletConfig.FG = std::stoi(Info[5]);
    m_bulletConfig.FB = std::stoi(Info[6]);
    m_bulletConfig.OR = std::stoi(Info[7]);
    m_bulletConfig.OG = std::stoi(Info[8]);
    m_bulletConfig.OB = std::stoi(Info[9]);
    m_bulletConfig.OT = std::stoi(Info[10]);
    m_bulletConfig.V = std::stoi(Info[11]);
    m_bulletConfig.L = std::stoi(Info[12]);
    m_bulletConfig.SB = std::stoi(Info[13]);
    }

    m_scoreText.setPosition(0, 0);
    m_scoreText.setString(std::to_string(m_score));

    spawnPlayer();
}

void Game::run(){
    
    while (m_running){
        if (!m_paused){
            m_entities.update();
            sSpawner();
            sMovement();
            sLifeSpan();
            sCollision();
            sRender();
            m_currentFrame++;
        }
        sUserInput();
    }
}

void Game::setPaused(bool paused){
	m_paused = paused;
}

void Game::spawnPlayer(){
    auto entity = m_entities.addEntity("player");
    entity->cTransform = std::make_shared<CTransform>(  Vec2(m_window.getSize().x/2, m_window.getSize().y/2), 
                                                        Vec2(m_playerConfig.S, m_playerConfig.S),
                                                        0.0f);
    // Diamo una forma e un colore, un outline e una thickness
    entity->cShape = std::make_shared<CShape>(  m_playerConfig.SR, 
                                                m_playerConfig.V, 
                                                sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
                                                sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), 
                                                m_playerConfig.OT);
    // Aggiungiamo la possibilità di comandarlo con i tasti
    entity->cInput = std::make_shared<CInput>();
    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
    m_player = entity;
}

void Game::spawnEnemy(){
    auto entity = m_entities.addEntity("enemy");
    
    entity->cShape =        std::make_shared<CShape>(m_enemyConfig.SR,    
                                                    rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN) + m_enemyConfig.VMIN, 
                                                    sf::Color(0, 0, 0),
                                                    sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), 
                                                    m_enemyConfig.OT);

    float a = static_cast<float>(rand()) / RAND_MAX;
    float b = static_cast<float>(rand()) / RAND_MAX;
    float velx = (m_enemyConfig.SMIN + static_cast<float>(rand()) / RAND_MAX * (m_enemyConfig.SMAX - m_enemyConfig.SMIN)) * (a > 0.5 ? 1 : -1);
    float vely = (m_enemyConfig.SMIN + static_cast<float>(rand()) / RAND_MAX * (m_enemyConfig.SMAX - m_enemyConfig.SMIN)) * (b > 0.5 ? 1 : -1);
    Vec2 startingPoint( rand() % (m_window.getSize().x - 2 * m_enemyConfig.SR) + m_enemyConfig.SR,
                        rand() % (m_window.getSize().y - 2 * m_enemyConfig.SR) + m_enemyConfig.SR);

    while(m_player->cTransform->pos.dist(startingPoint) < 300){
        startingPoint=Vec2(rand() % (m_window.getSize().x - 2 * m_enemyConfig.SR) + m_enemyConfig.SR,
                            rand() % (m_window.getSize().y - 2 * m_enemyConfig.SR) + m_enemyConfig.SR);
    }

    entity->cTransform = std::make_shared<CTransform>(startingPoint, Vec2(velx, vely), 0.0f);
    
    entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
    entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
    entity->cScore = std::make_shared<CScore>(100);
    m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity){
    size_t smallEnemiesQuantity = entity->cShape->circle.getPointCount();

    for (size_t i = 0; i < smallEnemiesQuantity; i++){
        auto smallEntity = m_entities.addEntity("small_entity");
        smallEntity->cShape = std::make_shared<CShape>( entity->cShape->circle.getRadius() * 0.5f,
                                                        entity->cShape->circle.getPointCount(),
                                                        entity->cShape->circle.getFillColor(),
                                                        sf::Color(255, 0, 0),
                                                        entity->cShape->circle.getOutlineThickness());
        smallEntity->cLifespan = std::make_shared<CLifespan>(  entity->cLifespan->total);
        smallEntity->cCollision = std::make_shared<CCollision>( entity->cCollision->radius * 0.5f);
        smallEntity->cScore = std::make_shared<CScore>( entity->cScore->score * 2);                                                    
        

        Vec2 parentStartingPoint = entity->cTransform->pos;
        float angle = i * (360 / smallEnemiesQuantity) * PI_CONV;
        Vec2 smallStartingPoint(parentStartingPoint.x + entity->cShape->circle.getRadius() * cos(angle),
                                parentStartingPoint.y + entity->cShape->circle.getRadius() * sin(angle));
        Vec2 velocity = smallStartingPoint - parentStartingPoint;
        velocity /= velocity.length();

        smallEntity->cTransform = std::make_shared<CTransform>(smallStartingPoint, velocity, 0.0f);
    }
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos){
    auto e = m_entities.addEntity("bullet");
    e->cShape = std::make_shared<CShape>(   m_bulletConfig.SR, 
                                            m_bulletConfig.V, 
                                            sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
                                            sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
                                            m_bulletConfig.OT);

    Vec2 direction = mousePos - m_player->cTransform->pos;
    float norm = direction.length();

    e->cTransform = std::make_shared<CTransform>(   m_player->cTransform->pos,
                                                    (direction / norm) * m_bulletConfig.S, 0.0f);

    e->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
    e->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2& mousePos){
    auto e = m_entities.addEntity("special");
    int radius = m_bulletConfig.SR * 2;

    e->cShape = std::make_shared<CShape>(   radius, 
                                            3, 
                                            sf::Color::Black,
                                            sf::Color::Yellow,
                                            m_bulletConfig.OT);

    Vec2 direction = mousePos - m_player->cTransform->pos;
    float norm = direction.length();

    e->cTransform = std::make_shared<CTransform>(   m_player->cTransform->pos,
                                                    (direction / norm) * m_bulletConfig.S / 2, 0.0f);

    e->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L * 2);
    e->cCollision = std::make_shared<CCollision>(radius);
}

void Game::sLifeSpan(){
    for (auto e : m_entities.getEntities()){
        if (!e->cLifespan){continue;}

        if (e->cLifespan->remaining <= 0){
            e->destroy();
        }

        if (e->cLifespan->remaining > 0){
            --e->cLifespan->remaining;
            if (e->isActive()){
                float alphaMultiplier = static_cast<float>(e->cLifespan->remaining)/e->cLifespan->total * 255;
                e->cShape->circle.setOutlineColor(sf::Color(    e->cShape->circle.getOutlineColor().r,
                                                                e->cShape->circle.getOutlineColor().g,
                                                                e->cShape->circle.getOutlineColor().b,
                                                                alphaMultiplier));
            }
            if (e->tag() == "special" || e ->tag() == "bullet"){
                float alphaMultiplier = static_cast<float>(e->cLifespan->remaining)/e->cLifespan->total * 255;
                e->cShape->circle.setFillColor(sf::Color(       e->cShape->circle.getFillColor().r,
                                                                e->cShape->circle.getFillColor().g,
                                                                e->cShape->circle.getFillColor().b,
                                                                alphaMultiplier));
            }
        }
    }
}

void Game::sMovement() {
    if (m_player->cInput->up && m_player->cTransform->pos.y - m_player->cCollision->radius > 0){
        m_player->cTransform->pos.y -= m_player->cTransform->velocity.y;
    }
    if (m_player->cInput->down && m_player->cTransform->pos.y + m_player->cCollision->radius < m_window.getSize().y){
        m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
    }
    if (m_player->cInput->right && m_player->cTransform->pos.x + m_player->cCollision->radius < m_window.getSize().x){
        m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
    }
    if (m_player->cInput->left && m_player->cTransform->pos.x - m_player->cCollision->radius > 0){
        m_player->cTransform->pos.x -= m_player->cTransform->velocity.x;
    }

    for (auto e : m_entities.getEntities()){
            e->cTransform->angle += 2.0f;
            e->cShape->circle.setRotation(e->cTransform->angle);
            if (e->tag() != "player"){
                e->cTransform->pos += e->cTransform->velocity;
            }
    }
}

void Game::sCollision() {
    // Collisione tra giocatore e nemico
    for (auto e : m_entities.getEntities("player")){
        for (auto enemy : m_entities.getEntities("enemy")){
            float distance = e->cTransform->pos.dist(Vec2(enemy->cTransform->pos));
            if (distance <= m_player->cCollision->radius + enemy->cCollision->radius){
                if (e->isActive()){
                    m_score = 0;
                    m_scoreText.setString(std::to_string(m_score));

                    enemy->destroy();
                    e->destroy();
                    spawnPlayer();
                }
            }
        }
    }
    // Collisione dei nemici con i bordi
    for (auto e : m_entities.getEntities()){
        if (e->tag() == "enemy" || e->tag() == "small_entity"){
            if (e->cTransform->pos.x + e->cCollision->radius > m_window.getSize().x){
                e->cTransform->velocity.x *= -1;
            }
            else if (e->cTransform->pos.x - e->cCollision->radius < 0){
                e->cTransform->velocity.x *= -1;
            }
            if (e->cTransform->pos.y + e->cCollision->radius > m_window.getSize().y){
                e->cTransform->velocity.y *= -1;
            }
            else if (e->cTransform->pos.y - e->cCollision->radius < 0){
                e->cTransform->velocity.y *= -1;
            }
        }
    }
    // Collisione dei proiettili con i nemici o i loro figli
    for (auto bullet : m_entities.getEntities("bullet")){
        for (auto enemy : m_entities.getEntities()){
            if (enemy->tag() == "enemy" || enemy->tag() == "small_entity"){
                float distance = bullet->cTransform->pos.dist(enemy->cTransform->pos);
                if (distance <= bullet->cCollision->radius + enemy->cCollision->radius){
                    m_score += enemy->cScore->score;
                    m_scoreText.setString(std::to_string(m_score));
                    if (enemy->tag() == "enemy")
                        spawnSmallEnemies(enemy);
                    enemy->destroy();
                    bullet->destroy();
                    break;
                }
            }
        }
    }
    // Collisione speciale-nemico
    for (auto special : m_entities.getEntities("special")){
        for (auto e : m_entities.getEntities()){
            if (e->tag() == "enemy" || e->tag() == "small_entity"){
                float distance = special->cTransform->pos.dist(e->cTransform->pos);
                if (distance <= special->cCollision->radius + e->cCollision->radius){
                    m_score += e->cScore->score;
                    m_scoreText.setString(std::to_string(m_score));
                    if (e->tag() == "enemy")
                        spawnSmallEnemies(e);
                    e->destroy();
                }
            }
        }
    }
    // Collisione speciale-bordo
    for (auto special : m_entities.getEntities("special")){
        if( special->cTransform->pos.x - special->cCollision->radius < 0 || 
            special->cTransform->pos.x + special->cCollision->radius > m_window.getSize().x){

            special->cTransform->velocity.x *= -1;
                
        }
        if( special->cTransform->pos.y - special->cCollision->radius < 0 ||
            special->cTransform->pos.y + special->cCollision->radius > m_window.getSize().y){

            special->cTransform->velocity.y *= -1;
        }
    }


    // Collisione con piccolo nemico
    for (auto e : m_entities.getEntities("small_entity")){
        float distance = e->cTransform->pos.dist(m_player->cTransform->pos);
        if (distance <= m_player->cCollision->radius + e->cCollision->radius){
            m_score /= 2.0f;
            m_scoreText.setString(std::to_string(m_score));
            e->destroy();
            m_player->destroy();
            spawnPlayer();
        }
    }
}

void Game::sUserInput() {

    sf::Event event;
    static bool lockBullet;
    static bool lockSpecial;
    while(m_window.pollEvent(event)){
        if (event.type == sf::Event::Closed){
            m_running = false;
            break;
        }
        if (event.type == sf::Event::MouseButtonPressed){
            if (event.mouseButton.button == sf::Mouse::Left && lockBullet != true){
                lockBullet = true;
                m_player->cInput->shoot = true;
            }
            if (event.mouseButton.button == sf::Mouse::Right && lockSpecial != true){
                lockSpecial = true;
                m_player->cInput->special = true;
            }
        }
        if (event.type == sf::Event::MouseButtonReleased){
            if (event.mouseButton.button == sf::Mouse::Left && lockBullet == true){
                lockBullet = false;
                m_player->cInput->shoot = false;
            }
            if (event.mouseButton.button == sf::Mouse::Right && lockSpecial == true){
                lockSpecial = false;
                m_player->cInput->special = false;
            }
        }
        if (event.type == sf::Event::KeyPressed){
            if (event.key.code == sf::Keyboard::W){
                m_player->cInput->up = true;
            }
            if (event.key.code == sf::Keyboard::A){
                m_player->cInput->left = true;
            }
            if (event.key.code == sf::Keyboard::S){
                m_player->cInput->down = true;
            }
            if (event.key.code == sf::Keyboard::D){
                m_player->cInput->right = true;
            }
            if (event.key.code == sf::Keyboard::P){
                m_paused = !m_paused;
            }
        }
        if (event.type == sf::Event::KeyReleased){
            if (event.key.code == sf::Keyboard::W){
                m_player->cInput->up = false;
            }
            if (event.key.code == sf::Keyboard::A){
                m_player->cInput->left = false;
            }
            if (event.key.code == sf::Keyboard::S){
                m_player->cInput->down = false;
            }
            if (event.key.code == sf::Keyboard::D){
                m_player->cInput->right = false;
            }
        }
    }
}

void Game::sRender() {
    m_window.clear();
    for(auto e : m_entities.getEntities()){
        if (e->cShape){
            e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
            m_window.draw(e->cShape->circle);
        }
    }
    m_window.draw(m_scoreText);
    m_window.display();
}

void Game::sSpawner() {
    // Spawn dei nemici
    if (m_lastEnemySpawnTime + m_enemyConfig.SI < m_currentFrame)
        spawnEnemy();
    // Spawn dei bullet
    if (m_player->cInput->shoot == true){
        Vec2 mouse(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
        spawnBullet(m_player, mouse);
        m_player->cInput->shoot = false;
    }
    // Spawn dell'arma speciale
    if (m_player->cInput->special == true && m_currentFrame - m_lastSpecialSpawnTime >= m_specialCooldown){
        Vec2 mouse(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
        spawnSpecialWeapon(m_player, mouse);
        m_player->cInput->special = false;
        m_lastSpecialSpawnTime = m_currentFrame;
    }
}