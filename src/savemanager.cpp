#include "SaveManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool SaveManager::saveToFile(
    const std::string &filename,
    const std::vector<PhysicsObject> &objects,
    const std::vector<sf::CircleShape> &balls,
    const std::vector<PhysicsObjectRect> &rectangles,
    const std::vector<sf::RectangleShape> &rectShapes)
{
    json root;

    // --- 1. SAVE CIRCLES ---
    root["circles"] = json::array();
    for (size_t i = 0; i < objects.size(); ++i)
    {
        const auto &obj = objects[i];
        sf::Color col = balls[i].getFillColor();

        json c;
        c["x"] = obj.x;
        c["y"] = obj.y;
        c["vx"] = obj.velocityX;
        c["vy"] = obj.velocityY;
        c["radius"] = obj.radius;
        c["mass"] = obj.mass;
        c["color"] = { col.r, col.g, col.b };

        root["circles"].push_back(c);
    }

    // --- 2. SAVE RECTANGLES (Dynamic & Static Platforms/Walls) ---
    root["rectangles"] = json::array();
    for (size_t i = 0; i < rectangles.size(); ++i)
    {
        const auto &rect = rectangles[i];
        sf::Color col = rectShapes[i].getFillColor();

        json r;
        r["x"] = rect.x;
        r["y"] = rect.y;
        r["vx"] = rect.velocityX;
        r["vy"] = rect.velocityY;
        r["w"] = rect.width;
        r["h"] = rect.height;
        r["angle"] = rect.angle;
        r["angularVel"] = rect.angularVelocity;
        r["mass"] = rect.mass;
        r["isStatic"] = rect.isstatic;
        r["color"] = { col.r, col.g, col.b };

        root["rectangles"].push_back(r);
    }

    // --- 3. WRITE TO DISK ---
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open " << filename << " for saving!\n";
        return false;
    }

    file << root.dump(4); // Indented by 4 spaces
    std::cout << "World saved successfully to " << filename << "!\n";
    return true;
}

bool SaveManager::loadFromFile(
    const std::string &filename,
    std::vector<PhysicsObject> &objects,
    std::vector<sf::CircleShape> &balls,
    std::vector<PhysicsObjectRect> &rectangles,
    std::vector<sf::RectangleShape> &rectShapes)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Save file " << filename << " does not exist!\n";
        return false;
    }

    json root;
    file >> root;

    // Clear current scene completely to load fresh state
    objects.clear();
    balls.clear();
    rectangles.clear();
    rectShapes.clear();

    // --- 1. RESTORE CIRCLES ---
    if (root.contains("circles"))
    {
        for (const auto &c : root["circles"])
        {
            float x = c["x"];
            float y = c["y"];
            float vx = c["vx"];
            float vy = c["vy"];
            float radius = c["radius"];

            // 1. Physics Circle (velY first, then velX as per your constructor)
            objects.emplace_back(x, y, vy, vx, 0.0f, 0.0f, 100.0f, radius);

            // 2. Visual SFML Ball
            balls.emplace_back(radius);
            balls.back().setOrigin({radius, radius});
            balls.back().setPosition({x, y});
            balls.back().setFillColor(sf::Color(
                c["color"][0], c["color"][1], c["color"][2]));
        }
    }

    // --- 2. RESTORE RECTANGLES ---
    if (root.contains("rectangles"))
    {
        for (const auto &r : root["rectangles"])
        {
            float x = r["x"];
            float y = r["y"];
            float vx = r["vx"];
            float vy = r["vy"];
            float w = r["w"];
            float h = r["h"];
            float angle = r["angle"];
            float angularVel = r["angularVel"];
            bool isStatic = r["isStatic"];

            // 1. Physics Rectangle
            rectangles.emplace_back(x, y, vy, vx, w, h, angle, angularVel, isStatic);

            // 2. Visual SFML Rectangle
            rectShapes.emplace_back(sf::Vector2f(w, h));
            rectShapes.back().setOrigin({w / 2.0f, h / 2.0f});
            rectShapes.back().setPosition({x, y});
            rectShapes.back().setRotation(sf::degrees(angle));
            rectShapes.back().setFillColor(sf::Color(
                r["color"][0], r["color"][1], r["color"][2]));
        }
    }

    std::cout << "World loaded successfully from " << filename << "!\n";
    return true;
}