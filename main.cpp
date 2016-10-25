#include <iostream>
#include <sstream>
#include <iomanip>
#include <SFML/Graphics.hpp>
#include <stack>
#include <vector>
#include <unistd.h>
#define tailleCase 48
#define tailleMur 10


typedef struct Case Case;
typedef std::vector<std::vector<Case> >  Labyrinthe;
struct Case
{
    bool isVisite;
    int border[2];
};

int choisirCloison(Labyrinthe laby, std::pair<int, int> p) {
    int l = p.first;
    int c = p.second;

    std::vector<int> a;
    
    if (l != 0 && !laby[l-1][c].isVisite)
        a.push_back(0);
    if (c != laby[0].size()-1 && !laby[l][c+1].isVisite)
        a.push_back(1);
    if (l != laby.size()-1 && !laby[l+1][c].isVisite)
        a.push_back(2);
    if (c != 0 && !laby[l][c-1].isVisite)
        a.push_back(3);
    
    int size = a.size();
    if (size == 0)
        return -1;

    return a[rand()%size];
}


void casserCloison(std::stack<std::pair<int, int> > *p, Labyrinthe *laby) {
    int x = p->top().first;
    int y = p->top().second;
    if(x >= 0 && y >= 0 && x < laby->size() && y < laby[0].size())
    {
        (*laby)[x][y].isVisite = 1;
        int b = choisirCloison(*laby, {x, y});
        if(b == 0)
        {
            (*laby)[x-1][y].border[1] = 0;
            x = x-1;
        }
        else if(b == 1)
        {
            (*laby)[x][y].border[0] = 0;
            y = y+1;
        }
        else if(b == 2) 
        {
            (*laby)[x][y].border[1] = 0;
            x = x+1;
        }
        else if(b == 3)
        {
            (*laby)[x][y-1].border[0] = 0;
            y = y-1;
        }
        if  (b != -1) {
            p->push({x, y});
        }
        else {
            p->pop();
        }
    }
    else{
        p->pop();
    }
}

Labyrinthe creerLabyrinthe(int m, int n) {
    // Initialisation Labyrinthe
    Case caseDefault = {0, {1, 1}};
    std::vector<std::vector<Case> > laby;
    std::vector<Case> line;
    int i, j;
    for(i = 0; i < m; i++)
    {
        line.clear();
        for(j = 0; j < n; j++)
        {
            line.push_back(caseDefault);
        }
        laby.push_back(line);
    }

    std::stack<std::pair<int, int> > p;
    p.push({m-1, n-1});
    srand(time(NULL));

    while(!p.empty()) 
    {
        casserCloison(&p, &laby);
    }

    return laby;
}

std::vector<sf::RectangleShape> getLabyrintheShape(Labyrinthe laby)
{
    std::vector<sf::RectangleShape> labyrintheShape;
    int i, j;
    for(i = 0; i < laby.size(); i++)
    {
        for(j = 0; j < laby[0].size(); j++)
        {
            if(laby[i][j].border[0])
            {
                sf::RectangleShape rectangle(sf::Vector2f(tailleMur, tailleCase+2*tailleMur));
                rectangle.setPosition(tailleCase*(j+1) + tailleMur*(j+1), tailleCase*i + tailleMur*(i) + 28);
                labyrintheShape.push_back(rectangle);
            }
            if(laby[i][j].border[1])
            {
                sf::RectangleShape rectangle(sf::Vector2f(tailleCase+2*tailleMur, tailleMur));
                rectangle.setPosition(tailleCase*j + tailleMur*j, tailleCase*(i+1) + tailleMur*(i+1) + 28);
                labyrintheShape.push_back(rectangle);
            }
        }
    }

    return labyrintheShape;
}



int main()
{
    int m = 12;
    int n = 12;
    sf::RenderWindow window(sf::VideoMode(n*tailleCase+(n+1)*tailleMur, m*tailleCase+(m+1)*tailleMur + 28), "Le Labyrinthe");

    // Génération du labyrinthe parfait
    Labyrinthe laby = creerLabyrinthe(m, n);

    // Génération des bordures du labyrinthe
    std::vector<sf::RectangleShape> labyrintheShape = getLabyrintheShape(laby);
    sf::RectangleShape rectangle1(sf::Vector2f(tailleMur, m*(tailleCase+2*tailleMur)));
    sf::RectangleShape rectangle2(sf::Vector2f(m*(tailleCase+2*tailleMur), tailleMur));
    rectangle1.setPosition(0, 28);
    rectangle2.setPosition(0, 28);
    labyrintheShape.push_back(rectangle1);
    labyrintheShape.push_back(rectangle2);

    // Personnage
    sf::Texture texture;
    if (!texture.loadFromFile("sprite.png"))
    {
        std::cout << "Erreur pour charger la texture\n";
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setPosition(sf::Vector2f(16, 16+28));

    // Texte (chronometre)
    sf::Font font;
    if (!font.loadFromFile("KGInimitableOriginal.ttf"))
    {
        std::cout << "Erreur chargement font\n";
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setString("0 second");

    // Carré de depart et d'arrivé
    sf::RectangleShape depart(sf::Vector2f(tailleCase, tailleCase));
    depart.setPosition(tailleMur, tailleMur+28);
    depart.setFillColor(sf::Color(207, 0, 15));
    sf::FloatRect departBoundingBox = depart.getGlobalBounds();
    sf::RectangleShape arrive(sf::Vector2f(tailleCase, tailleCase));
    arrive.setPosition((n-1)*tailleCase+(n)*tailleMur, (m-1)*tailleCase+(m)*tailleMur + 28);
    arrive.setFillColor(sf::Color(38, 166, 91));
    sf::FloatRect arriveBoundingBox = arrive.getGlobalBounds();


    bool right = false, left = false, up = false, down = false;
    int move_x = 0, move_y = 0;
    int i = 0;

    sf::Clock clock;
    bool started = false, collision = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Right)
                {
                    move_x = 2;
                }
                if (event.key.code == sf::Keyboard::Left)
                {
                    move_x = -2;
                }
                if (event.key.code == sf::Keyboard::Up)
                {
                    move_y = -2;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    move_y = 2;
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Right)
                {
                    move_x = 0;
                }
                if (event.key.code == sf::Keyboard::Left)
                {
                    move_x = 0;
                }
                if (event.key.code == sf::Keyboard::Up)
                {
                    move_y = 0;
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    move_y = 0;
                }
            }
        }

        if(i%2)
            sprite.move(sf::Vector2f(move_x, 0));
        else
            sprite.move(sf::Vector2f(0, move_y));

        if (move_x > 0)
        {
            sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
        }
        if (move_x < 0)
        {
            sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
        }
        if (move_y < 0)
        {
            sprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
        }
        if (move_y > 0)
        {
            sprite.setTextureRect(sf::IntRect(96, 0, 32, 32));
        }


        sf::FloatRect spriteBoundingBox = sprite.getGlobalBounds();
        if(!departBoundingBox.intersects(spriteBoundingBox) and !started)
        {
            clock.restart();
            started = true;
        }
        else if (started && !arriveBoundingBox.intersects(spriteBoundingBox))
        {
            std::ostringstream out;
            out << float(int(clock.getElapsedTime().asSeconds()*10))/10 << " seconds";
            text.setString(out.str());
        }

        window.clear();
        for(auto & shape: labyrintheShape) 
        {  
            sf::FloatRect boundingBox2 = shape.getGlobalBounds();
            if(spriteBoundingBox.intersects(boundingBox2))
            {
                collision = true;
            }
            window.draw(shape);
        }
        if(collision)
        {
            if(i%2)
                sprite.move(sf::Vector2f(-move_x, 0));
            else
                sprite.move(sf::Vector2f(0, -move_y));
            collision = false;
        }
        window.draw(text);
        window.draw(depart);
        window.draw(arrive);
        window.draw(sprite);
        window.display();
        window.setFramerateLimit(120);

        i++;
    }

    return 0;
}

