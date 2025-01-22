#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include "Particles.h"
#include <SFML/Graphics.hpp>
#include "AnomalyVisualizer.h"
#include <thread>

// compile: g++ -std=c++17 main26.cpp AnomalyVisualizer.cpp -o particle_visualizer -lGL -lGLU -lglut -lsfml-graphics -lsfml-window -lsfml-system -fpermissive

// Struct for storing collision information to be written into CSV
struct CollisionInfo
{
    std::string particle1;
    std::string particle2;
    std::string interactionInfo;
    std::vector<float> waveData;
};

// Function to log new collision into CSV
void logNewCollision(const CollisionInfo &collision, const std::string &fileName)
{
    // Open file in append mode
    std::ofstream file(fileName, std::ios::app);
    if (file.is_open())
    {
        // Check if file is empty (for headers)
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0)
        {
            file << "Particle1,Particle2,InteractionInfo,WaveData\n"; // Header Row
        }

        // Write collision data
        file << collision.particle1 << ","
             << collision.particle2 << ","
             << collision.interactionInfo << ",";

        // Write wave data (comma-separated values)
        for (size_t i = 0; i < collision.waveData.size(); ++i)
        {
            file << collision.waveData[i];
            if (i < collision.waveData.size() - 1)
                file << ",";
        }
        file << "\n";
    }
    else
    {
        std::cerr << "Error opening file for logging." << std::endl;
    }
}

// Function to check for new interactions and log them
void checkAndLogInteraction(const Particle &p1, const Particle &p2,
                            std::unordered_set<std::string> &loggedInteractions,
                            const std::string &fileName)
{
    // Create a string identifier for the pair of particles (sorted to ensure consistency)
    std::string interactionID = p1.name + "-" + p2.name;
    if (loggedInteractions.find(interactionID) == loggedInteractions.end())
    {
        // Interaction hasn't been logged before, so we log it
        loggedInteractions.insert(interactionID);

        // Generate the wave from interaction and analyze
        std::vector<float> wave1 = p1.generateWave();
        std::vector<float> wave2 = p2.generateWave();
        std::vector<float> resultingWave = Particle::combineWaves(wave1, wave2);

        // Check for amplitude breach and create collision info
        std::vector<std::pair<int, float>> breachPoints = Particle::checkAmplitudeBreach(resultingWave);
        if (!breachPoints.empty())
        {
            CollisionInfo collision;
            collision.particle1 = p1.name;
            collision.particle2 = p2.name;
            collision.interactionInfo = "Anomaly Detected";
            collision.waveData = resultingWave;

            // Log the new collision
            logNewCollision(collision, fileName);
        }
    }
}

void logCollisionData(float initialEnergy, float finalEnergy, float initialMass, float finalMass, const std::vector<std::pair<int, float>> &breachPoints)
{
    static int collisionCount = 0;
    collisionCount++;

    std::ofstream logFile("collision_log.txt", std::ios::app);
    logFile << "Collision #" << collisionCount << std::endl;
    logFile << "Initial Energy: " << initialEnergy << " GeV" << std::endl;
    logFile << "Final Energy: " << finalEnergy << " GeV" << std::endl;
    logFile << "Energy Difference: " << finalEnergy - initialEnergy << " GeV" << std::endl;
    logFile << "Initial Mass: " << initialMass << " GeV/c^2" << std::endl;
    logFile << "Final Mass: " << finalMass << " GeV/c^2" << std::endl;
    logFile << "Mass Difference: " << finalMass - initialMass << " GeV/c^2" << std::endl;
    logFile << "Amplitude Breach Points:" << std::endl;
    for (const auto &point : breachPoints)
    {
        logFile << "  Index: " << point.first << ", Amplitude: " << point.second << std::endl;
    }
    logFile << std::endl;
    logFile.close();
}

void runVisualization(const std::string &fileName)
{
    int argc = 0;
    char **argv = nullptr;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Particle Interaction Visualizer");
    window.setFramerateLimit(60);

    AnomalyVisualizer visualizer(fileName);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::R)
                    visualizer.previous();
                if (event.key.code == sf::Keyboard::F)
                    visualizer.next();
            }
        }

        window.clear(sf::Color::White);
        visualizer.render(window);
        window.display();
    }
}

int main()
{
    // Create the particles
    std::vector<Particle> particles = {
        Particle(ParticleType::QuarkUp, "Up Quark", 2.0e-27, 1.6e-19, 2.5e-13, 1.0f, 1.5f, 0.5f, 0.6f, 500.0f),
        Particle(ParticleType::QuarkDown, "Down Quark", 3.0e-27, -1.6e-19, 2.2e-13, 0.5f, 1.0f, 0.6f, 0.4f, 550.0f),
        Particle(ParticleType::QuarkCharm, "Charm Quark", 1.3e-27, 2.0e-19, 2.0e-13, 0.8f, 0.5f, 0.7f, 0.5f, 500.0f),
        Particle(ParticleType::QuarkStrange, "Strange Quark", 9.5e-30, -1.0e-19, 1.6e-13, 0.9f, 0.5f, 0.4f, 0.6f, 550.0f),
        Particle(ParticleType::QuarkTop, "Top Quark", 1.8e-25, 1.602e-19, 2.0e-13, 0.1f, 0.7f, 0.3f, 0.8f, 700.0f),
        Particle(ParticleType::QuarkBottom, "Bottom Quark", 4.2e-28, -1.602e-19, 1.7e-13, 0.2f, 0.6f, 0.5f, 0.7f, 600.0f),
        Particle(ParticleType::LeptonElectron, "Electron", 9.1e-31, -1.602e-19, 1.0e-13, 0.99f, 0.8f, 0.2f, 0.9f, 1000.0f),
        Particle(ParticleType::LeptonMuon, "Muon", 1.88e-28, -1.602e-19, 2.0e-13, 0.3f, 0.5f, 0.6f, 0.4f, 1000.0f),
        Particle(ParticleType::LeptonTau, "Tau", 3.2e-27, -1.602e-19, 1.5e-13, 0.6f, 0.6f, 0.4f, 0.7f, 1200.0f),
        Particle(ParticleType::LeptonNeutrinoElectron, "Electron Neutrino", 1.0e-35, 0.0, 1.0e-14, 0.1f, 0.1f, 0.1f, 0.1f, 2000.0f),
        Particle(ParticleType::LeptonNeutrinoMuon, "Muon Neutrino", 1.0e-35, 0.0, 1.0e-14, 0.2f, 0.2f, 0.2f, 0.2f, 2100.0f),
        Particle(ParticleType::LeptonNeutrinoTau, "Tau Neutrino", 1.0e-35, 0.0, 1.0e-14, 0.3f, 0.3f, 0.3f, 0.3f, 2200.0f),
        Particle(ParticleType::LeptonPositron, "Positron", 9.1e-31, 1.602e-19, 1.0e-13, 0.95f, 0.75f, 0.25f, 0.85f, 1050.0f),
        Particle(ParticleType::BosonPhoton, "Photon", 0.0, 0.0, 0.0, 0.9f, 1.0f, 0.1f, 1.0f, 1500.0f),
        Particle(ParticleType::BosonZ, "Z Boson", 9.1e-26, 0, 2.5e-13, 0.7f, 1.0f, 0.2f, 0.9f, 1100.0f),
        Particle(ParticleType::BosonW, "W Boson", 8.0e-26, 1.602e-19, 3.0e-13, 0.6f, 1.0f, 0.3f, 0.8f, 1150.0f),
        Particle(ParticleType::BosonH, "Higgs Boson", 1.0e-25, 0, 3.0e-13, 0.5f, 1.2f, 0.4f, 0.7f, 1250.0f),
        Particle(ParticleType::BosonGluon, "Gluon", 0.0, 0.0, 1.0e-13, 0.4f, 0.4f, 0.4f, 0.5f, 1300.0f),
        Particle(ParticleType::ExoticX, "Exotic X", 2.0e-24, 2.0e-19, 4.0e-13, 0.8f, 0.2f, 0.8f, 0.6f, 1400.0f),
        Particle(ParticleType::ExoticY, "Exotic Y", 2.5e-24, -2.0e-19, 4.5e-13, 0.2f, 0.8f, 0.8f, 0.7f, 1450.0f),
        Particle(ParticleType::ExoticZ, "Exotic Z", 3.0e-24, 0.0, 5.0e-13, 0.8f, 0.8f, 0.2f, 0.8f, 1500.0f)
    };

    // Set to track logged interactions
    // Set to track logged interactions
    std::unordered_set<std::string> loggedInteractions;

    // File name to store collisions
    const std::string fileName = "collisions.csv";

    // Loop through all pairs of particles and check for new interactions
    for (size_t i = 0; i < particles.size(); ++i)
    {
        for (size_t j = i + 1; j < particles.size(); ++j)
        {
            checkAndLogInteraction(particles[i], particles[j], loggedInteractions, fileName);
        }
    }

    std::cout << "Collision logging completed." << std::endl;

    // Start the visualization in a separate thread
    std::thread visualizationThread(runVisualization, fileName);

    // Wait for the visualization thread to finish
    visualizationThread.join();

    return 0;
}