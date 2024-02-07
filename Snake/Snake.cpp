#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <algorithm> 
#define BlockSize 64
#define UISize 8

enum Direction {
    up, down, left, right
};
std::string LoadHighScore(std::string path);
int WinMain()
{
    std::string highscore = LoadHighScore("save.dat");
    int hightscore_int;
    hightscore_int = std::stoi(highscore);


    sf::RenderWindow window(sf::VideoMode(832, 840), "Snake");

    //Random
    std::srand(time(NULL));
    uint8_t randX = std::rand() % 13;
    uint8_t randY = std::rand() % 13;

    //Text
    sf::Font font;
    if (font.loadFromFile("font/Roboto_Bold.ttf")) {
        return 1;
    }
    sf::Text score_value_text;
    sf::Text hightscore_value_text;
    sf::Text score_text;
    sf::Text hisghscore_text;

    score_value_text.setFont(font);
    hightscore_value_text.setFont(font);
    score_text.setFont(font);
    hisghscore_text.setFont(font);

    //Head
    sf::RectangleShape head;
    head.setFillColor(sf::Color::Yellow);
    Direction direction;
    head.setPosition(448, 448);
    head.setSize(sf::Vector2f(BlockSize, BlockSize));

    //Food
    sf::CircleShape food(BlockSize/2);
    food.setFillColor(sf::Color(162, 64, 32));
    food.setPosition(randX * BlockSize, randY * BlockSize);

    //Tail
    int tail_counter = 3;
    int last_id = 2;
    sf::RectangleShape tail_draw;
    tail_draw.setSize(sf::Vector2f(BlockSize, BlockSize));
    tail_draw.setFillColor(sf::Color::Green);

    std::vector<sf::Vector2f> tail{ head.getPosition(),head.getPosition(), head.getPosition() };



    for (sf::Uint8 i = 0; i < 3; i++) {
        tail[i].y += -BlockSize-BlockSize*i;
    }




    sf::Clock time;
    float getTime;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) direction = up;
                if (event.key.code == sf::Keyboard::S) direction = down;
                if (event.key.code == sf::Keyboard::A) direction = left;
                if (event.key.code == sf::Keyboard::D) direction = right;
            }
        }

        getTime = time.getElapsedTime().asSeconds();
       
        window.clear();

        if (getTime > 0.5f) {

            sf::Vector2f last_position = head.getPosition();

            switch (direction) {
            case up:
                head.move(0, -BlockSize);
                break;
            case down:
                head.move(0, BlockSize);
                break;
            case left:
                head.move(-BlockSize, 0);
                break;
            case right:
                head.move(BlockSize, 0);
                break;
            }

            if (head.getGlobalBounds().contains(food.getPosition())) {
                tail_counter++;
                tail.push_back(head.getPosition());
                randX = std::rand() % 13;
                randY = std::rand() % 13;
                food.setPosition(randX * BlockSize, randY * BlockSize);
            }

            tail[last_id] = last_position;

            if (--last_id < 0)
                last_id = tail_counter-1;


            time.restart();

        }

        window.draw(food);

        for (int i = 0; i < tail_counter; i++) {
            tail_draw.setPosition(tail[i]);
            window.draw(tail_draw);
        }

        window.draw(head);
        window.display();
        
    }

    return 0;
}
std::string LoadHighScore(std::string path) {
    std::ifstream save_file;
    try {
        save_file.open(path);
    }
    catch (const std::exception& e) {
        return e.what();
    }
}