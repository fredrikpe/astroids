
#include <iostream> 
#include <cstdlib> 
#include <cmath>
#include <memory>
#include <algorithm>

#include "gameobjects.h"

using namespace std;

int GameObject::width;
int GameObject::height;

void GameObject::update() {
    if (isOutside()) {
        deleteMe = true;
    }
    goStraight();
}

bool GameObject::isOutside() const
{
    float x = sprite.getPosition().x;
    float y = sprite.getPosition().y;
    return x > width || x < 0 || y > height || y < 0;
}

void GameObject::goStraight()
{
	float angle = sprite.getRotation() / 180 * 3.14159265;
	float xspeed = velocity*sin(angle);
	float yspeed = -velocity*cos(angle);

	sprite.move(xspeed, yspeed);
}

void GameObject::rotate(float angle)
{
	sprite.rotate(angle);
}

bool GameObject::collision(GameObject& rhs) const 
{
    float x1 = sprite.getPosition().x;
    float y1 = sprite.getPosition().y;
    float x2 = rhs.getSprite().getPosition().x;
    float y2 = rhs.getSprite().getPosition().y;

    float distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    return distance < getRadius() + rhs.getRadius();
}

void GameObject::loadTexture(string filename, float scale)
{
    /* Loads an image and sets the sprite's texture */
	sf::Image image;
	if (!image.loadFromFile(filename)) {
		cout << "Failed to load " << filename << endl;
		return;
	}
	image.createMaskFromColor(sf::Color::White);
	texture.loadFromImage(image);
	sprite.setTexture(texture);

	int centerx = sprite.getTextureRect().width / 2;
	int centery = sprite.getTextureRect().height / 2;
	sprite.setOrigin(centerx, centery);

	sprite.setPosition(width/2, height/2);
	sprite.scale(scale, scale);
}

void Spaceship::update()
{
	//check keyboard input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		//rotate left
		rotate(-getRotationSpeed());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		//rotate right
		rotate(getRotationSpeed());
	}
	//go forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		goStraight();
    }
}

unique_ptr<GameObject> Spaceship::spawnObject()
{
    float angle = getSprite().getRotation() / 180 * 3.14159265;
    float x = getSprite().getPosition().x + 35*sin(angle);
    float y = getSprite().getPosition().y - 35*cos(angle);

    float rotation = getSprite().getRotation();
    return make_unique<Bullet>(rotation, x, y);
}

Spaceship::Spaceship()
{
	loadTexture("spaceship.png", 0.5);
    setRadius(20);
}

Bullet::Bullet(double rotation, float x, float y) {
    loadTexture("bullet.png", 0.05);
    getSprite().setPosition(x, y);
    getSprite().setRotation(rotation);
    setVelocity(7);
    setRadius(2);
}

unique_ptr<GameObject> Asteroid::spawnObject()
{
    int size = getSize() - 1;
    auto pos = getSprite().getPosition();
    return make_unique<Asteroid>(size, pos);
}

bool Asteroid::isOutside() const
{
    float x = getSprite().getPosition().x;
    float y = getSprite().getPosition().y;
    return x > width + 100 || x < -100 || y > 100 + height || y < -100;
}

Asteroid::Asteroid(int size, sf::Vector2f pos)
{
    // Tip: use a picture of your enemy to make the game more fun
    loadTexture("asteroid.png", 1 * 0.1 * (size + 2));
    if (size < 0) deleteMe = true;

    int rotation = rand() % 360;
    setSize(size);

    getSprite().setPosition(pos);
    getSprite().setRotation(rotation);
    setVelocity((3 - size)*3*0.3);
    setRadius((size + 2) * 8);
}

