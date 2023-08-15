#include "Game.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <random>

int RGen(int lower_bound, int upper_bound) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(lower_bound, upper_bound);
	return distribution(gen);
}

Game::Game(const std::string& config)
{
	init(config);
}

void Game::loadFromFile(std::string file)
{
	std::ifstream fin(file);
	std::vector<std::string> words;
	std::string data;

	while (getline(fin, data))
	{
		std::cout << data << std::endl;
		std::stringstream ss(data);

		std::string word;

		while (ss >> word)
		{
			words.push_back(word);
			//std::cout << word << std::endl;
		}

		if (words[0] == "Window")
		{
			//int W, H, FPS, F
			m_windowConfig.W = stoi(words[1]);
			m_windowConfig.H = stoi(words[2]);
			m_windowConfig.FPS = stoi(words[3]);
			m_windowConfig.F = stoi(words[4]);
		}

		else if (words[0] == "Font")
		{
			//std::string F; int FS, R, G, B;
			m_fontConfig.F = words[1];
			m_fontConfig.FS = stoi(words[2]);
			m_fontConfig.R = stoi(words[3]);
			m_fontConfig.G = stoi(words[4]);
			m_fontConfig.B = stoi(words[5]);
		}

		else if (words[0] == "Player")
		{
			//SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S
			m_playerConfig.SR = stoi(words[1]);
			m_playerConfig.CR = stoi(words[2]);
			m_playerConfig.S = stoi(words[3]);
			m_playerConfig.FR = stoi(words[4]);
			m_playerConfig.FG = stoi(words[5]);
			m_playerConfig.OB = stoi(words[6]);
			m_playerConfig.OR = stoi(words[7]);
			m_playerConfig.OG = stoi(words[8]);
			m_playerConfig.OB = stoi(words[9]);
			m_playerConfig.OT = stoi(words[10]);
			m_playerConfig.V = stoi(words[11]);
		}

		else if (words[0] == "Enemy")
		{
			//SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX;
			m_enemyConfig.SR = stoi(words[1]);
			m_enemyConfig.CR = stoi(words[2]);
			m_enemyConfig.SMIN = stoi(words[3]);
			m_enemyConfig.SMAX = stoi(words[4]);
			m_enemyConfig.OR = stoi(words[5]);
			m_enemyConfig.OG = stoi(words[6]);
			m_enemyConfig.OB = stoi(words[7]);
			m_enemyConfig.OT = stoi(words[8]);
			m_enemyConfig.VMIN = stoi(words[9]);
			m_enemyConfig.VMAX = stoi(words[10]);
			m_enemyConfig.L = stoi(words[11]);
			m_enemyConfig.SI = stoi(words[12]);
		}

		else if (words[0] == "Bullet")
		{
			//int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S;
			m_BulletConfig.SR = stoi(words[1]);
			m_BulletConfig.CR = stoi(words[2]);
			m_BulletConfig.S = stoi(words[3]);
			m_BulletConfig.FR = stoi(words[4]);
			m_BulletConfig.FG = stoi(words[5]);
			m_BulletConfig.FB = stoi(words[6]);
			m_BulletConfig.OR = stoi(words[7]);
			m_BulletConfig.OG = stoi(words[8]);
			m_BulletConfig.OB = stoi(words[9]);
			m_BulletConfig.OT = stoi(words[10]);
			m_BulletConfig.V = stoi(words[11]);
			m_BulletConfig.L = stoi(words[12]);
		}

		words.clear();

	}
}

void Game::init(const std::string& path)
{
	//todo read in config file
	loadFromFile(path);

	//set up defualt windows perameters
	if (!m_font.loadFromFile("NABILA.ttf"))
	{
		// error...
		std::cout << "failed to load font" << std::endl;
	}
	else
	{
		std::cout << "loaded font" << std::endl;
	}
	if (m_windowConfig.F == 0)
	{
		m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Assignmnet 2");
	}
	else
	{
		m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Assignmnet 2", sf::Style::Fullscreen);
	}

	m_window.setFramerateLimit(m_windowConfig.FPS);

	spawnPlayer();
}

void Game::run()
{
	//todo add pause function 
	//rendering should function while paused
	//movement input shoudlnt

	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
		}

		sUserInput();
		sRender();


		//may need to be moved when paused implimented
		m_currentFrame++;


	}
}

void Game::setPaused()
{
	m_paused = !m_paused;
}


void Game::sMovement()
{

	//impliment player movement

	for (auto& e : m_entities.getEntities())
	{
		m_player->cTransform->velocity = { 0,0 };

	}

	//player bounds collisions
	//x axis
	if (m_player->cTransform->pos.x + m_player->cCollision->radius >= m_window.getSize().x)
	{
		m_player->cInput->right = false;
	}
	if (m_player->cTransform->pos.x - m_player->cCollision->radius <= 0)
	{
		m_player->cInput->left = false;
	}

	//y axis
	if (m_player->cTransform->pos.y + m_player->cCollision->radius >= m_window.getSize().y)
	{
		m_player->cInput->down = false;
	}
	if (m_player->cTransform->pos.y - m_player->cCollision->radius <= 0)
	{
		m_player->cInput->up = false;
	}

	//player input 
	if (m_player->cInput->up)
	{
		m_player->cTransform->velocity.y = -m_playerConfig.S;
	}
	if (m_player->cInput->right)
	{
		m_player->cTransform->velocity.x = m_playerConfig.S;
	}
	if (m_player->cInput->left)
	{
		m_player->cTransform->velocity.x = -m_playerConfig.S;
	}
	if (m_player->cInput->down)
	{
		m_player->cTransform->velocity.y = m_playerConfig.S;
	}

	//all entities add velocity and out of bounds collisions
	for (auto& e : m_entities.getEntities())
	{
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;

		//out of bounds collisions
		//x axis
		if (e->cCollision != NULL)
		{
			if (e->cTransform->pos.x + e->cCollision->radius >= m_window.getSize().x)
			{
				e->cTransform->velocity.x *= -1;
			}
			if (e->cTransform->pos.x - e->cCollision->radius <= 0)
			{
				e->cTransform->velocity.x *= -1;
			}
			//y axis
			if (e->cTransform->pos.y + e->cCollision->radius >= m_window.getSize().y)
			{
				e->cTransform->velocity.y *= -1;
			}
			if (e->cTransform->pos.y - e->cCollision->radius <= 0)
			{
				e->cTransform->velocity.y *= -1;
			}
		}
	}


}

void Game::sUserInput()
{

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		//triggers event when window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		//this event is triggered when key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W key pressed" << std::endl;
				m_player->cInput->up = true;
				break;

			case sf::Keyboard::D:
				std::cout << "W key pressed" << std::endl;
				m_player->cInput->right = true;
				break;

			case sf::Keyboard::A:
				std::cout << "W key pressed" << std::endl;
				m_player->cInput->left = true;
				break;

			case sf::Keyboard::S:
				std::cout << "W key pressed" << std::endl;
				m_player->cInput->down = true;
				break;

			case sf::Keyboard::Escape:
				std::cout << "esc key pressed" << std::endl;
				setPaused();
				break;
			}
		}

		//this event is triggered when key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W key released" << std::endl;
				m_player->cInput->up = false;
				break;

			case sf::Keyboard::D:
				std::cout << "W key released" << std::endl;
				m_player->cInput->right = false;
				break;

			case sf::Keyboard::A:
				std::cout << "W key released" << std::endl;
				m_player->cInput->left = false;
				break;

			case sf::Keyboard::S:
				std::cout << "W key released" << std::endl;
				m_player->cInput->down = false;
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (!m_paused)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "Left mouse button clicked" << std::endl;

					//call spawn bullet
					spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
				}

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << "right mouse button clicked" << std::endl;

					//call spawn bullet
				}
			}
		}
	}
}

void Game::sLifespan()
{

	for (auto& e : m_entities.getEntities())
	{
		if (e->cLifespan != nullptr && e->cShape != nullptr)
		{
			sf::Color fill = e->cShape->circle.getFillColor();
			sf::Color outline = e->cShape->circle.getOutlineColor();

			float r = fill.r;
			float g = fill.g;
			float b = fill.b;
			float a = fill.a;

			float oR = outline.r;
			float oG = outline.g;
			float oB = outline.b;
			float oA = outline.a;

			float decrementA = 255.0f / e->cLifespan->total;
			a -= decrementA;

			if (a <= 0) { a = 0; }

			sf::Color newColor = sf::Color(r, g, b, a);
			sf::Color newOutline = sf::Color(oR, oG, oB, a);

			e->cShape->circle.setFillColor(newColor);
			e->cShape->circle.setOutlineColor(newOutline);

			e->cLifespan->remaining--;

			if (e->cLifespan->remaining <= 0)
			{
				e->destroy();
			}
		}
	}
}

void Game::setScore()
{
	sf::Text text("", m_font, m_fontConfig.FS);
	text.setFillColor(sf::Color(m_fontConfig.R, m_fontConfig.G, m_fontConfig.B));
	std::string printScore = "Score: " + std::to_string(m_score);
	text.setString(printScore);
	text.setPosition(20, 20);
	m_window.draw(text);
}

void Game::sRender()
{

	m_window.clear();


	for (auto& e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		m_window.draw(e->cShape->circle);
	}

	setScore();

	m_window.display();

}

void Game::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SI)
	{
		spawnEnemy();
	}
}

void Game::sCollision()
{
	for (auto b : m_entities.getEntities("bullet"))
	{

		for (auto e : m_entities.getEntities("enemy"))
		{
			if ((e->cTransform->pos.distanceFrom(b->cTransform->pos)) <= (e->cCollision->radius + b->cCollision->radius))
			{
				//std::cout << e->cTransform->pos.distanceFrom(b->cTransform->pos) << std::endl;
				spawnSmallEnemies(e);
				m_score += 100;
				b->destroy();
			}
		}
	}

	for (auto e : m_entities.getEntities("enemy"))
	{
		for (auto p : m_entities.getEntities("player"))
		{
			if ((e->cTransform->pos.distanceFrom(p->cTransform->pos)) <= (e->cCollision->radius + p->cCollision->radius))
			{
				float mx = m_window.getSize().x / 2.0f;
				float my = m_window.getSize().y / 2.0f;

				p->cTransform->pos = Vec2(mx, my);
			}
		}
	}

	for (auto b : m_entities.getEntities("bullet"))
	{

		for (auto le : m_entities.getEntities("lilenemy"))
		{
			if ((le->cTransform->pos.distanceFrom(b->cTransform->pos)) <= (le->cCollision->radius + b->cCollision->radius))
			{
				//std::cout << e->cTransform->pos.distanceFrom(b->cTransform->pos) << std::endl;
				m_score += 200;
				b->destroy();
				le->destroy();
			}
		}
	}


}

void Game::spawnPlayer()
{

	//returns shared pointer 
	auto entity = m_entities.addEntity("player");

	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;

	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.0f);

	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V,
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), //fill
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), //outline
		m_playerConfig.OT);

	entity->cInput = std::make_shared<CInput>();

	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	m_player = entity;

}

void Game::spawnEnemy()
{

	//returns shared pointer 
	auto entity = m_entities.addEntity("enemy");

	float ex = RGen(30, m_window.getSize().x - 30); 
	float ey = RGen(30, m_window.getSize().y - 30); 

	float vx = RGen(m_enemyConfig.SMIN, m_enemyConfig.SMAX);
	float vy = RGen(m_enemyConfig.SMIN, m_enemyConfig.SMAX);

	int randVert = RGen(m_enemyConfig.VMIN, m_enemyConfig.VMAX);

	sf::Color randColor = sf::Color(RGen(0,255), RGen(0, 255), RGen(0, 255));

	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(vx, vy), 0.0f);

	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, //shape radius
		randVert, //random verticies
		randColor, //fill color
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), //outline color
		m_enemyConfig.OT);

	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);

	m_lastEnemySpawnTime = m_currentFrame;
}


void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	entity->cTransform->velocity = Vec2(0,0);

	int numSides = entity->cShape->circle.getPointCount();

	float angleFromCenter = 360.0f / numSides;

	float currentAngle = angleFromCenter;

	for (int times = 0; times < numSides; ++times)
	{
		auto lilEntity = m_entities.addEntity("lilenemy");

		float  radians = currentAngle * (3.1415f / 180.0f);

		lilEntity->cTransform = std::make_shared<CTransform>(entity->cTransform->pos,
			Vec2(entity->cTransform->pos.x * cos(radians),
			entity->cTransform->pos.y * sin(radians)).normalise()
			, 0);


		lilEntity->cShape = std::make_shared<CShape>(entity->cShape->circle.getRadius() / 2,
			numSides, 
			entity->cShape->circle.getFillColor(),
			entity->cShape->circle.getOutlineColor(),
			entity->cShape->circle.getOutlineThickness());

		lilEntity->cCollision = std::make_shared<CCollision>(entity->cShape->circle.getRadius() / 2);

		lilEntity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);

		currentAngle += angleFromCenter;
	}

	entity->destroy();
}


void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	auto bullet = m_entities.addEntity("bullet");

	//calculate direction
	Vec2 dir = mousePos - m_player->cTransform->pos;
	dir = dir.normalise();

	bullet->cTransform = std::make_shared<CTransform>(m_player->cTransform->pos, 
		dir*m_BulletConfig.S,
		0);

	bullet->cShape = std::make_shared<CShape>(m_BulletConfig.SR,
		m_BulletConfig.V,
		sf::Color(m_BulletConfig.FR, m_BulletConfig.FG, m_BulletConfig.FB, 255),
		sf::Color(m_BulletConfig.OR, m_BulletConfig.OG , m_BulletConfig.OB, 255),
		m_BulletConfig.OT);

	bullet->cCollision = std::make_shared<CCollision>(m_BulletConfig.CR);

	bullet->cLifespan = std::make_shared<CLifespan>(m_BulletConfig.L);

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}
