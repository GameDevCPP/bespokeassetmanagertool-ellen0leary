//  Press 'T' to select a tile from the left hand side-
//  Press a number from one to the top number of tiles
// 
//  Press 'P' to start painting and left clickon he right hand side of the screen 
// 

#include "TileMap.hpp"
#include <iostream>
#include "json.hpp"
#include <fstream>


int currentTile = -1;
std::string state = "";


int main(){
    sf::RenderWindow window(sf::VideoMode(559, 263), "Bespoke asset manager");
    using nlohmann::json;
    json j;

    std::ifstream inMap("mapData.json");//read in mapData.json
    inMap >> j;
    int newTiles[128]={}; //creat aarray of tiles
    int index = 0;
    //take data from json and add to array
    for(auto i: j["map"]){
        newTiles[index] = i; 
        index++;
    }

    TileMap tileMap; //for our tiles from tile sheet
    TileMap map; //for actual map

    int tiles[4] = {0,1,2,3};
    //left hand side tiles
    if (!tileMap.load("tileset.png", sf::Vector2u(32, 32), tiles, 1, 4))
        return -1;

    //actual map
    if (!map.load("tileset.png", sf::Vector2u(32, 32), newTiles, 16, 8, 45))
                return -1;


    //rectangle that breaks up two parts of screen
    sf::RectangleShape divider;
    divider.setSize(sf::Vector2f(10, 263));
    divider.setFillColor(sf::Color::Red);
    divider.setPosition(35, 0);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed) window.close();

        }

        //changing shapes
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::T)){
            state="Tiles";
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
            state="Painting";
        }

        //check states
        if(state=="Tiles"){
            //get tile user wants to paint
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
                currentTile=0;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
                currentTile=1;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
                currentTile=2;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){
                currentTile=3;
            }
        } else if(state=="Painting"){
            //if the mouse button is clicked and the user has alread slected a tile
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && currentTile>-1){
                sf::Vector2i position = sf::Mouse::getPosition(window); //get the position relative to window
                //if position is on map
                if(position.x>45 && position.x <559  && position.y <263  && position.y>-1){
                    int newx = (position.x-45)/32; //get x tile
                    int newy = position.y/32; //get y tilee
                    newTiles[newx+newy*16] = currentTile; //get position in array
                }
                //redraw the tiles
                if (!map.load("tileset.png", sf::Vector2u(32, 32), newTiles, 16, 8, 45))
                    return -1;
            }
        }

        //draw eventhing
        window.clear();
        window.draw(tileMap);
        window.draw(map);
        window.draw(divider);
        window.display();
    }

    //saves to mapData.json
    std::ofstream outJson("mapData.json");
    json jmap = {
        {"map", newTiles}
    };

    outJson<< jmap<< std::endl;
    outJson.close();
};
