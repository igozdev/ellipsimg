#include <SFML/Graphics.hpp>

#define POINT_COUNT 70
#define FACTOR 40
#define RAND(min, max) (rand() % (max - min) + min)
#define RANDF(min, max) (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min)) + min)

sf::CircleShape CreateEllipse(const sf::Vector2f& position, const float& rotation, const float& radiusX, const float& radiusY, const sf::Color& fillColor)
{
	sf::CircleShape ellipse(1, POINT_COUNT);
	ellipse.setFillColor(fillColor);
	ellipse.scale(radiusX, radiusY);
	ellipse.setPosition(position);
	ellipse.setRotation(rotation);

	return ellipse;
}

sf::CircleShape RandomEllipse(const unsigned int& iteration, const unsigned int& width, const unsigned int& height)
{
	float distrf = iteration * 2.0f;
	if (distrf > height - FACTOR)
		distrf = height - FACTOR;

	float randomXRadius = RANDF(0.1f, height - distrf);
	float randomYRadius = RANDF(0.1f, height - distrf);
	float randomXPos = RANDF(-randomXRadius, width);
	float randomYPos = RANDF(-randomXRadius, height);
	float randomRotation = RANDF(0.0f, 360.0f);

	sf::Color randomColor = sf::Color(RAND(0, 256), RAND(0, 256), RAND(0, 256), RAND(0, 256));

	return CreateEllipse(sf::Vector2f(randomXPos, randomYPos), randomRotation, randomXRadius, randomYRadius, randomColor);
}

sf::Image Capture(const sf::RenderWindow& renderTarget)
{
	sf::Texture texture;
	texture.create(renderTarget.getSize().x, renderTarget.getSize().y);
	texture.update(renderTarget);

	return texture.copyToImage();
}

unsigned long long int GetError(const sf::Image& img1, const sf::Image& img2)
{
	unsigned long long int totalError = 0;
	for (unsigned int x = 0; x < img1.getSize().x; x++)
	{
		for (unsigned int y = 0; y < img1.getSize().y; y++)
		{
			sf::Color c1 = img1.getPixel(x, y);
			sf::Color c2 = img2.getPixel(x, y);
			totalError += 
				static_cast<unsigned long long int>(std::abs(c1.r - c2.r)) + 
				static_cast<unsigned long long int>(std::abs(c1.g - c2.g)) + 
				static_cast<unsigned long long int>(std::abs(c1.b - c2.b));
		}
	}
	return totalError;
}

int main(int argc, char** argv)
{
	sf::Image tImage;
	std::string imageName = "image.jpg";
	if (argc == 2)
		imageName = argv[1];
	if (!tImage.loadFromFile(imageName))
		return -1;
	printf("Loaded image from: %s\n\n", imageName.c_str());

	unsigned int windowWidth = tImage.getSize().x;
	unsigned int windowHeight = tImage.getSize().y;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "ellipsimg");

	sf::Image bgImage;
	bgImage.create(windowWidth, windowHeight, sf::Color::White);
	sf::Image bufImage;
	bufImage.create(windowWidth, windowHeight, sf::Color::White);

	unsigned long long int bestError = GetError(bufImage, tImage);
	unsigned int iteration = 0;

	bool drawAllEllipses = false;

	srand(time(0));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::C)
				{
					bufImage.saveToFile(imageName + "_i" + std::to_string(iteration) + ".png");
					printf("[Last image saved]\n");
				}
				if (event.key.code == sf::Keyboard::T)
				{
					drawAllEllipses = !drawAllEllipses;
					printf("[Displaying all ellipses toggled to %i]\n", drawAllEllipses);
				}
			}
		}

		sf::Texture bgTexture;
		bgTexture.loadFromImage(bgImage);
		sf::Sprite bgSprite;
		bgSprite.setTexture(bgTexture);

		for (;;)
		{
			window.clear(sf::Color::White);
			window.draw(bgSprite);

			sf::CircleShape ellipse = RandomEllipse(iteration, windowWidth, windowHeight);
			window.draw(ellipse);

			sf::Image curImage = Capture(window);

			if (drawAllEllipses)
				window.display();

			unsigned long long int curError = GetError(curImage, tImage);
			if (curError < bestError)
			{
				bestError = curError;
				bufImage = curImage;
				break;
			}
		}

		window.display();
		iteration++;
		printf("Iteration %i complete with error of %lld\n", iteration, bestError);
		bgImage = bufImage;
	}

	return 0;
}