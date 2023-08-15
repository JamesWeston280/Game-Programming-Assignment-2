#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>

struct WindowConfig { int W, H, FPS, F; };
struct FontConfig { std::string F; int FS, R, G, B; };
struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow	m_window;		// window to draw to
	EntityManager		m_entities;		// vector of entities to maintain
	sf::Font			m_font;			// font
	sf::Text			m_text;			// the score text
	WindowConfig		m_windowConfig;
	FontConfig			m_fontConfig;
	PlayerConfig		m_playerConfig;
	EnemyConfig			m_enemyConfig;
	BulletConfig		m_BulletConfig;

	int					m_score = 0;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;
	bool				m_paused = false; //whether or not to update game logic
	bool				m_running = true;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& path); // initialise gamestate with config file path
	void setPaused();					// paused game
	void loadFromFile(std::string file);

	void sMovement();					//system: entity position / movment update
	void sUserInput();
	void sLifespan();
	void sRender();
	void sEnemySpawner();
	void sCollision();
	void setScore();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:

	Game(const std::string& config); 

	void run();

};