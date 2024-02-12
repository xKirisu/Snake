#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

#define BlockSize 64
#define WindowSizeX 832
#define WindowSizeY 896
#define UISize WindowSizeY - WindowSizeX

enum Direction {
    up, down, left, right
};
std::string LoadHighScore(std::string path);
void SaveToFile(std::string, std::string);
int WinMain()
{
    std::string highscore = LoadHighScore("save.dat");
    int highscore_int = std::stoi(highscore);

    bool isPlay = true;
    sf::Mouse mouse;
    sf::RenderWindow window(sf::VideoMode(WindowSizeX, WindowSizeY), "Snake");

    //Random
    std::srand(time(NULL));
    uint8_t randX = std::rand() % 13;
    uint8_t randY = std::rand() % 13;

    //UI Panel
    sf::RectangleShape uibakground;
    uibakground.setFillColor(sf::Color::Cyan);
    uibakground.setSize(sf::Vector2f(WindowSizeX, UISize));
    uibakground.setPosition(0, WindowSizeY-BlockSize);

    //Text
    sf::Font font;
    font.loadFromFile("font/Roboto-Black.ttf");

    sf::Text text_score_value, text_score, text_highscore, text_highscore_value, text_gameover, new_record;

        text_gameover.setFont(font);
        text_gameover.setString("Game Over");
        text_gameover.setCharacterSize(96);
        text_gameover.setPosition(WindowSizeX / 4 - BlockSize/3, WindowSizeY / 3);
        
        text_score_value.setFont(font);
        text_score_value.setCharacterSize(42);
        text_score_value.setPosition(WindowSizeX - BlockSize*1.85, WindowSizeY - BlockSize + 2);

        text_score.setFont(font);
        text_score.setString("Score:");
        text_score.setPosition(WindowSizeX - (BlockSize * 3.9), WindowSizeY - BlockSize);
        text_score.setCharacterSize(42);

        text_highscore.setFont(font);
        text_highscore.setString("Highscore:");
        text_highscore.setCharacterSize(42);
        text_highscore.setPosition(BlockSize * 1.2, WindowSizeY - BlockSize + 2);

        text_highscore_value.setFont(font);
        text_highscore_value.setCharacterSize(42);
        text_highscore_value.setPosition(BlockSize * 4.55, WindowSizeY - BlockSize + 4);
        text_highscore_value.setString(highscore);

        new_record.setFont(font);
        new_record.setCharacterSize(32);
        new_record.setPosition(WindowSizeX / 2 - BlockSize, WindowSizeY / 3 + 96);
        new_record.setString("new record");

    //Head
    sf::RectangleShape head;
    head.setFillColor(sf::Color::Yellow);
    head.setPosition(448, 448);
    head.setSize(sf::Vector2f(BlockSize, BlockSize));
    Direction direction = down;

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

    //Score values
    text_score_value.setString(std::to_string(tail_counter - 3));

    sf::Clock time;
    float getTime = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)    if (direction != down)   direction = up;
                if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)  if (direction != up)     direction = down;
                if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)  if (direction != right)  direction = left;
                if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) if (direction != left)   direction = right;
            }
        }

        getTime = time.getElapsedTime().asSeconds();
       
        window.clear();

        if (getTime > 0.5f&&isPlay) {

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
            //Check if head go out of borders
            sf::Vector2f checkhead = head.getPosition();
            if (checkhead.x < 0 || checkhead.y < 0 || checkhead.x > WindowSizeX - BlockSize || checkhead.y > WindowSizeY - (BlockSize+UISize)) {
                isPlay = false;
            }

            //Check if tail collided head
            for (sf::Vector2f tailpos : tail) {
                if (head.getGlobalBounds().contains(tailpos)) {
                    isPlay = false;
                }
            }

            //Ckeck if food iseatable
            if (head.getGlobalBounds().contains(food.getPosition())) {

                tail_counter++;
                tail.push_back(head.getPosition());

                randX = std::rand() % 13;
                randY = std::rand() % 13;
                food.setPosition(randX * BlockSize, randY * BlockSize);

                //Food spawn controller
                bool checkagain = true;
                while (checkagain) {
                    checkagain = false;
                    for (int i = 0; i < tail_counter; i++) {
                        if (food.getGlobalBounds().contains(sf::Vector2f(tail[i].x+BlockSize/2, tail[i].y+BlockSize/2))) {
                            randX = std::rand() % 13;
                            randY = std::rand() % 13;
                            food.setPosition(randX * BlockSize, randY * BlockSize);
                            checkagain = true; // Ckeck again
                            break;
                        }
                    }
                }
                
                text_score_value.setString(std::to_string(tail_counter - 3));
            }

            tail[last_id] = last_position;

            if (--last_id < 0)
                last_id = tail_counter-1;


            time.restart();

        }

        //Draw tail
        for (int i = 0; i < tail_counter; i++) {
            tail_draw.setPosition(tail[i]);
            window.draw(tail_draw);
        }

        //Drawfood
        window.draw(food);
        

        //Drawhead
        window.draw(head);

        if (!isPlay) {
            window.draw(text_gameover);

            if (highscore_int < tail_counter - 3) {
                window.draw(new_record);
                SaveToFile("save.dat", std::to_string(tail_counter - 3));
            }
        }

        //DrawUI
        window.draw(uibakground);
        window.draw(text_highscore);
        window.draw(text_highscore_value);
        window.draw(text_score);
        window.draw(text_score_value);

        window.display();
        
    }

    return 0;
}
std::string LoadHighScore(std::string path) {
    std::ifstream file;
    std::string value;
    try {
        file.open(path);
        if(file >> value)
            return value;
    }
    catch (const std::exception& e) {
        return e.what();
    }
}
void SaveToFile(std::string path, std::string content) {
    std::ofstream file;

    try {
        file.open(path);
        file << content;
    }
    catch (const std::exception& e) {
        std::cout<<e.what();
    }
}