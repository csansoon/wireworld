#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Circuit.h"
#include <SFML/System/Time.hpp>
using namespace sf;

//COLORS:
Color	backgroundColor	= Color::Color(35, 49, 66, 255);
Color		cableColor	= Color::Color(250, 207, 90, 255);
Color		headColor	= Color::Color(79, 157, 166, 255);
Color		tailColor	= Color::Color(255, 89, 89, 255);

int pixelSize = 64;



Vector2i positionInMap(Vector2i position, RenderWindow& window,View& view) {
	Vector2f mousePosition = window.mapPixelToCoords(Vector2i(position), view);
	Vector2f correctMousePosition = mousePosition;
	if (correctMousePosition.x < 0) correctMousePosition.x -= pixelSize;
	if (correctMousePosition.y < 0) correctMousePosition.y -= pixelSize;
	Vector2i mouseCell = Vector2i(correctMousePosition);
	mouseCell /= pixelSize;
	return mouseCell;
}


int main() {

	/* SETUP */

	RenderWindow window(VideoMode(800, 600), "wireworld");
	//window.setMouseCursorVisible(false);


	/* RESOURCES */
	Font ProductSans;
	if (not ProductSans.loadFromFile("resources/fonts/ProductSans-Regular.ttf")) std::cout << "Error loading font \"Product Sans\"" << std::endl;

	Text title;
	title.setFont(ProductSans);
	title.setString("Patty es la mejor");


	/* OBJECTS */

	Clock timer;

	RectangleShape chunk(Vector2f(100 * pixelSize,100 * pixelSize));
	RectangleShape pixel(Vector2f(1 * pixelSize, 1 * pixelSize));

	RectangleShape cursor(Vector2f(1 * pixelSize, 1 * pixelSize));
	cursor.setOutlineThickness(20);
	cursor.setFillColor(Color::Color(0, 0, 0, 0));

	View view(Vector2f(0,0),Vector2f(800,600));	// Center, Size

	Circuit circuit;
	if (not circuit.load("test.circuit")) std::cout << "[ERROR LOADING test.circuit FILE]" << std::endl;
	else std::cout << "[CIRCUIT LOADED FROM test.circuit]" << std::endl;

	Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window), view);
	Vector2f lastMousePosition = mousePosition;
	Vector2i mouseCell;


	int mode = 1;
	bool playing = true;
	bool dragging = false;
	bool Pressed = false;
	while (window.isOpen()) {



		/* MANAGE INPUT */
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Resized) {
				view.setSize(Vector2f(window.getSize()));
			}

			if (event.type == Event::Closed) {
				if (not circuit.save("test.circuit")) std::cout << "[ERROR SAVING test.circuit FILE]" << std::endl;
				else std::cout << "[CIRCUIT SAVED AS test.circuit]" << std::endl;
				window.close();
			}

			if (event.type == Event::MouseWheelMoved) {
				if (event.mouseWheel.delta > 0) view.zoom(0.9);
				else view.zoom(1.1);
			}

			if (event.type == Event::MouseButtonPressed) {
				if (not dragging and event.mouseButton.button == Mouse::Right) {
					dragging = true;
					lastMousePosition = mousePosition;
				}
				//if (event.mouseButton.button == Mouse::Left) {
				//	circuit.paint(mouseCell, mode);
				//	Pressed = true;
				//}
			}

			if (Mouse::isButtonPressed(Mouse::Left)) {
				circuit.paint(mouseCell, mode);
				Pressed = true;
			}


			if (event.type == Event::MouseButtonReleased) {
				if (dragging and event.mouseButton.button == Mouse::Right) {
					dragging = false;
				}
			}

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Space) {
					if (playing) { std::cout << "[PAUSE]" << std::endl; playing = false; }
					else { std::cout << "[NOW PLAYING]" << std::endl; playing = true; }
					//circuit.update();
				}
				if (event.key.code == Keyboard::Num0) { mode = 0; cursor.setOutlineColor(Color::Color(0, 0, 0, 127)); }
				if (event.key.code == Keyboard::Num1) { mode = 1; cursor.setOutlineColor(cableColor - Color::Color(0, 0, 0, 128)); }
				if (event.key.code == Keyboard::Num2) { mode = 2; cursor.setOutlineColor(headColor - Color::Color(0, 0, 0, 128)); }
				if (event.key.code == Keyboard::Num3) { mode = 3; cursor.setOutlineColor(tailColor - Color::Color(0, 0, 0, 128)); }
			}
		}



		/* UPDATE */
		Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(), view);
		mouseCell = positionInMap(Mouse::getPosition(window), window, view);

		cursor.setPosition(Vector2f(mouseCell * pixelSize));


		if (dragging) {
			view.move(-(mousePosition - lastMousePosition));
		}
		if (playing) {
			if (timer.getElapsedTime().asMilliseconds() >= 50) {
				circuit.update();
				timer.restart();
			}
		}
		//circle.setPosition(mousePosition);


		/* PRINT */
		window.setView(view);

		window.clear(backgroundColor);


		std::vector<Chunk> visibleChunks = circuit.visibleChunks(positionInMap(Vector2i(0, 0), window, view), positionInMap(Vector2i(window.getSize()), window, view));
		for (int i = 0; i < visibleChunks.size(); ++i) {
			//Draw active visible chunk
			chunk.setPosition(Vector2f(visibleChunks[i].position * pixelSize));
			if (visibleChunks[i].position.x/100 % 2 == visibleChunks[i].position.y/100 % 2) chunk.setFillColor(Color::Color(255,255,255,16));
			else chunk.setFillColor(Color::Color(255, 255, 255, 32));
			window.draw(chunk);

			//Draw active visible pixels
			for (auto it = visibleChunks[i].activeCells.begin(); it != visibleChunks[i].activeCells.end(); ++it) {
				pixel.setPosition(Vector2f(it->first * pixelSize, it->second * pixelSize));
				switch (circuit.state(Vector2i(it->first, it->second))) {
					case 0:
						break;
					case 1:
						pixel.setFillColor(cableColor);
						break;
					case 2:
						pixel.setFillColor(headColor);
						break;
					case 3:
						pixel.setFillColor(tailColor);
						break;
				}
				window.draw(pixel);
			}
		}

		if (Pressed) {
			std::cout << visibleChunks.size() << " visible chunks." << std::endl << std::endl; 
			Pressed = false;
		}


		window.draw(title);
		window.draw(cursor);

		window.display();


	}
	return 0;
}