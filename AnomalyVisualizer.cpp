#include "AnomalyVisualizer.h"
#include <fstream>
#include <sstream>
#include <math.h>

AnomalyVisualizer::AnomalyVisualizer(const std::string& csvFilename) : currentAnomaly(0) {
    loadAnomalies(csvFilename);
}

void AnomalyVisualizer::loadAnomalies(const std::string& csvFilename) {
    std::ifstream file(csvFilename);
    std::string line;
    
    // Skip header
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Anomaly anomaly;
        std::string waveDataStr;
        
        if (std::getline(iss, anomaly.particle1, ',') &&
            std::getline(iss, anomaly.particle2, ',') &&
            std::getline(iss, anomaly.interactionInfo, ',') &&
            std::getline(iss, waveDataStr)) {
            
            // Parse wave data
            std::istringstream waveStream(waveDataStr);
            float value;
            while (waveStream >> value) {
                anomaly.waveData.push_back(value);
                if (waveStream.peek() == ',')
                    waveStream.ignore();
            }
            
            anomalies.push_back(anomaly);
        }
    }
}

void AnomalyVisualizer::render(sf::RenderWindow& window)
{
    // Clear the window with a white background
    window.clear(sf::Color::White);

    // Render the coil
    renderCoil(window);

    // Render the wave graphs
    renderWaveGraphs(window);

    // Render the text information
    renderText(window);

    // No need to call window.display() here, it will be called in the main loop
}

void AnomalyVisualizer::renderCoil(sf::RenderWindow& window) {
    // OpenGL rendering code for the coil
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);
    
    // Render coil rings
    glColor3f(0.5f, 0.5f, 0.5f);
    for (int i = 0; i < 10; ++i) {
        glPushMatrix();
        glTranslatef(0, i * 0.5f, 0);
        glutSolidTorus(0.1, 1.0, 20, 20);
        glPopMatrix();
    }
}

void AnomalyVisualizer::renderWaveGraphs(sf::RenderWindow& window) {
    if (currentAnomaly >= anomalies.size()) return;
    
    const auto& anomaly = anomalies[currentAnomaly];
    
    // Render normal wave
    sf::VertexArray normalWave(sf::LineStrip, 360);
    for (int i = 0; i < 360; ++i) {
        float x = i * 2.0f;
        float y = 200 + std::sin(i * 0.1f) * 50;
        normalWave[i].position = sf::Vector2f(x, y);
        normalWave[i].color = sf::Color::Blue;
    }
    window.draw(normalWave);
    
    // Render anomalous wave
    sf::VertexArray anomalousWave(sf::LineStrip, anomaly.waveData.size());
    for (size_t i = 0; i < anomaly.waveData.size(); ++i) {
        float x = i * (720.0f / anomaly.waveData.size());
        float y = 400 + anomaly.waveData[i] * 50;
        anomalousWave[i].position = sf::Vector2f(x, y);
        anomalousWave[i].color = sf::Color::Red;
    }
    window.draw(anomalousWave);

    // Highlight breach points
    sf::CircleShape breachPoint(5);
    breachPoint.setFillColor(sf::Color::Yellow);
    for (const auto& point : anomaly.breachPoints) {
        float x = point.first * (720.0f / anomaly.waveData.size());
        float y = 400 + point.second * 50;
        breachPoint.setPosition(x - 5, y - 5);
        window.draw(breachPoint);
    }
}

void AnomalyVisualizer::renderText(sf::RenderWindow& window) {
    if (currentAnomaly >= anomalies.size()) return;
    
    const auto& anomaly = anomalies[currentAnomaly];
    
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf");
    
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    
    std::stringstream ss;
    ss << "Anomaly " << (currentAnomaly + 1) << " of " << anomalies.size() << "\n";
    ss << "Particles: " << anomaly.particle1 << " - " << anomaly.particle2 << "\n";
    ss << "Interaction: " << anomaly.interactionInfo;
    
    text.setString(ss.str());
    text.setPosition(10, 10);
    
    window.draw(text);
}

void AnomalyVisualizer::next() {
    if (currentAnomaly < anomalies.size() - 1)
        ++currentAnomaly;
}

void AnomalyVisualizer::previous() {
    if (currentAnomaly > 0)
        --currentAnomaly;
}
