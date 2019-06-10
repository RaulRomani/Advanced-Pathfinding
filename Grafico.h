#ifndef H_GRAFICO
#define H_GRAFICO
#include <SFML/Graphics.hpp>
#include "edge.h"
#include "vector2.h"
#include "triangle.h"
#include "delaunay.h"
#include "MapGraph.h"

class Grafico {

  typedef CGraph<Location, double>::E E;
  typedef CGraph<Location, double>::N N;
  typedef CGraph<N, E>::Edge Edge;
public:

  void delay(int secs) {
    for (int i = (time(NULL) + secs); time(NULL) != i; time(NULL));
  }

  void graficar(CGraph<Location, double>& graph,
                     std::list<Location>& pathList) {


    // Make shortest path lines
    std::vector<std::array<sf::Vertex, 2> > shortestPathLines;
    auto end = pathList.end();
    --end;
    for (auto it = pathList.begin(); it != end; ++it) {
//        auto next = ++it;
        Location u = *it;
        Location v = *(++it);
        shortestPathLines.push_back({
          {
            sf::Vertex(sf::Vector2f(u.getX() + 2, u.getY() + 2), sf::Color::Yellow),
            sf::Vertex(sf::Vector2f(v.getX() + 2, v.getY() + 2), sf::Color::Yellow)
          }
        });
        --it;
    }




    //FUENTE BASE
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return;
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(12); // in pixels, not points!
    text.setColor(sf::Color::Yellow);
    text.setCharacterSize(12); // in pixels, not points!
    text.setColor(sf::Color::Green);
    //  text.setStyle(sf::Text::Bold);

    std::vector<sf::Text> vertexLabels;
        //Make squares
    std::vector<sf::RectangleShape*> squares;

    // Make the lines  "EDGES"
    std::vector<std::array<sf::Vertex, 2> > lines;

    for (auto it = graph.adjList.begin(); it != graph.adjList.end(); ++it) {

      N u = it->first;
      //squares
      sf::RectangleShape *square = new sf::RectangleShape(sf::Vector2f(4, 4));
      square->setPosition(u.getX(), u.getY());
      squares.push_back(square);

      //vertex labels
      text.setString("(" + to_string(int(u.getX())) + ", " + to_string(int(u.getY())) + ")");
      text.setPosition(u.getX() - 20, u.getY() + 5);
      vertexLabels.push_back(text);

      //lines "edges"
      list<Edge*> neighbors = graph.getOutGoingEdges(it->first);
      for (Edge* neighbor : neighbors) {
        N v = neighbor->nodes[1]->data; // nodes[1] == dest

        lines.push_back({
          {
            sf::Vertex(sf::Vector2f(u.getX() + 2, u.getY() + 2), sf::Color::White),
            sf::Vertex(sf::Vector2f(v.getX() + 2, v.getY() + 2), sf::Color::White)
          }
        });
      }
    }


    std::cout<<shortestPathLines.size()<<std::endl;

    // SFML window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 32;
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Shortest path visualization", sf::Style::Default, settings);
    sf::Clock clock;
    sf::Time deltaTime;
    deltaTime = clock.restart();
      sf::Event event;

    auto sPathIt = shortestPathLines.begin();

    while (window.isOpen()) {

      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
          window.close();
        // Escape pressed : exit

        // key pressed
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
          window.close();
      }

      window.clear();



      // Draw the squares
      for (auto s = squares.begin(); s != squares.end(); s++) {
        window.draw(**s);
      }

      //		 Draw the lines
      for (auto l = lines.begin(); l != lines.end(); l++) {
        window.draw((*l).data(), 2, sf::Lines);
      }
//      //		 Draw the shortest path lines
      for (auto l = shortestPathLines.begin(); l != shortestPathLines.end(); l++) {
        window.draw((*l).data(), 2, sf::Lines);
      }
//
//      // Draw the vertex labels
      for (auto l = vertexLabels.begin(); l != vertexLabels.end(); l++) {
        window.draw(*l);
      }

      window.draw((*sPathIt).data(), 2, sf::Lines);

      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && sPathIt != shortestPathLines.end() - 1)
        //          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sPathIt != end(shortestPathLines))
      {
        std::cout << "the Right key was pressed" << std::endl;
        ++sPathIt;
        delay(1);
      }
      //      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right && sPathIt != end(shortestPathLines))
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sPathIt != shortestPathLines.begin()) {
        std::cout << "the Left key was pressed" << std::endl;
        --sPathIt;
        delay(1);
      }

      //      }


      //    for (auto l = begin(shortestPathLines); l != end(shortestPathLines); l++)
      //        window.draw((*l).data(), 2, sf::Lines);

      //		 Draw the shortest path lines
      //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
      //      for (auto l = begin(shortestPathLines); l != end(shortestPathLines); l++)
      //        window.draw((*l).data(), 2, sf::Lines);
      //    }



      //    auto sPathIt = begin(shortestPathLines);
      //    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sPathIt != end(shortestPathLines))
      //    {
      //        // left key is pressed: move our character
      //        ++sPathIt;
      //        window.draw((*sPathIt).data(), 2, sf::Lines);
      //    }


      window.display();
    }


  }
};

#endif
