#ifndef ANOMALY_VISUALIZER_H
#define ANOMALY_VISUALIZER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <GL/glut.h>

struct Anomaly {
    std::string particle1;
    std::string particle2;
    std::string interactionInfo;
    std::vector<float> waveData;
    std::vector<std::pair<int, float>> breachPoints;
};

class AnomalyVisualizer {
public:
    AnomalyVisualizer(const std::string& csvFilename);
    void render(sf::RenderWindow& window);
    void next();
    void previous();

private:
    std::vector<Anomaly> anomalies;
    size_t currentAnomaly;
    void loadAnomalies(const std::string& csvFilename);
    void renderCoil(sf::RenderWindow& window);
    void renderWaveGraphs(sf::RenderWindow& window);
    void renderText(sf::RenderWindow& window);
};

#endif // ANOMALY_VISUALIZER_H
