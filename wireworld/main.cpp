#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Circuit.h"
using namespace sf;

int main() {

	/* SETUP */

	RenderWindow window(VideoMode(800, 600), "wireworld");
	window.setMouseCursorVisible(false);


	/* RESOURCES */
	Font ProductSans;
	if (not ProductSans.loadFromFile("resources/fonts/ProductSans-Regular.ttf")) std::cout << "Error loading font \"Product Sans\"" << std::endl;

	Text title;
	title.setFont(ProductSans);
	title.setString("WIREWORLD");


	/* OBJECTS */

	CircleShape circle(5,4);

	View view(Vector2f(0,0),Vector2f(800,600));	// Center, Size
	
	Circuit circuit;

	Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window), view);







	while (window.isOpen()) {



		/* MANAGE INPUT */
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					circuit.updateCell(mousePosition);
				}
			}
		}



		/* UPDATE */
		mousePosition = window.mapPixelToCoords(Mouse::getPosition(window), view);
		circle.setPosition(mousePosition);


		/* PRINT */
		window.setView(view);

		window.clear(Color::Black);

		window.draw(title);
		window.draw(circle);

		window.display();


	}
	return 0;
}