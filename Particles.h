#ifndef PARTICLES_H
#define PARTICLES_H

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>


// Enum for Particle Types
enum class ParticleType
{
    // Quarks
    QuarkUp,
    QuarkDown,
    QuarkCharm,
    QuarkStrange,
    QuarkTop,
    QuarkBottom,

    // Leptons
    LeptonElectron,
    LeptonMuon,
    LeptonTau,
    LeptonNeutrinoElectron,
    LeptonNeutrinoMuon,
    LeptonNeutrinoTau,
    LeptonPositron,

    // Bosons
    BosonPhoton,
    BosonZ,
    BosonW,
    BosonH,
    BosonGluon,

    // Exotic Particles
    ExoticX,
    ExoticY,
    ExoticZ,

    Unknown
};

// Class to represent a Particle
class Particle
{
public:
    ParticleType type;
    std::string name;
    double mass;
    double charge;
    double energy;
    float x, y, z;   // Position (could be 3D coordinates)
    float amplitude; // Amplitude of the waveform
    float frequency; // Frequency of the waveform

// Add this near the top of the file, with other constant definitions
    const float AMPLITUDE_THRESHOLD = 1.0f;

    Particle(ParticleType type, std::string name, double mass, double charge, double energy,
             float x, float y, float z, float amplitude, float frequency)
        : type(type), name(name), mass(mass), charge(charge), energy(energy), x(x), y(y), z(z),
          amplitude(amplitude), frequency(frequency) {}

    // Function to generate a wave based on particle attributes
    std::vector<float> generateWave() const
    {
        std::vector<float> wave;
        for (int i = 0; i < 360; i++)
        {
            float value = amplitude * sin(frequency * i); // Sinusoidal wave based on frequency and amplitude
            wave.push_back(value);
        }
        return wave;
    }

    static std::vector<float> combineWaves(const std::vector<float>& wave1, const std::vector<float>& wave2);

    
    static std::vector<std::pair<int, float>> checkAmplitudeBreach(const std::vector<float>& wave) {
        std::vector<std::pair<int, float>> breachPoints;
        for (int i = 0; i < wave.size(); ++i) {
            if (std::abs(wave[i]) > 1.0f) {
                breachPoints.push_back({i, wave[i]});
            }
        }
        return breachPoints;
    }

    
    // Function to print detailed particle information
    void printDetails() const
    {
        std::cout << "Particle: " << name << std::endl;
        std::cout << "Type: " << getParticleType() << std::endl;
        std::cout << "Mass: " << mass << " kg" << std::endl;
        std::cout << "Charge: " << charge << " C" << std::endl;
        std::cout << "Energy: " << energy << " J" << std::endl;
        std::cout << "Position: (" << x << ", " << y << ", " << z << ")" << std::endl;
        std::cout << "Amplitude: " << amplitude << std::endl;
        std::cout << "Frequency: " << frequency << " Hz" << std::endl;
        std::cout << std::endl;
    }
    // Get particle type as a string
    std::string getParticleType() const
    {
        switch (type)
        {
        case ParticleType::QuarkUp:
            return "Quark Up";
        case ParticleType::QuarkDown:
            return "Quark Down";
        case ParticleType::QuarkCharm:
            return "Quark Charm";
        case ParticleType::QuarkStrange:
            return "Quark Strange";
        case ParticleType::QuarkTop:
            return "Quark Top";
        case ParticleType::QuarkBottom:
            return "Quark Bottom";

        case ParticleType::LeptonElectron:
            return "Electron";
        case ParticleType::LeptonMuon:
            return "Muon";
        case ParticleType::LeptonTau:
            return "Tau";
        case ParticleType::LeptonNeutrinoElectron:
            return "Electron Neutrino";
        case ParticleType::LeptonNeutrinoMuon:
            return "Muon Neutrino";
        case ParticleType::LeptonNeutrinoTau:
            return "Tau Neutrino";
        case ParticleType::LeptonPositron:
            return "Positron";

        case ParticleType::BosonPhoton:
            return "Photon";
        case ParticleType::BosonZ:
            return "Z Boson";
        case ParticleType::BosonW:
            return "W Boson";
        case ParticleType::BosonH:
            return "Higgs Boson";
        case ParticleType::BosonGluon:
            return "Gluon";

        case ParticleType::ExoticX:
            return "Exotic X";
        case ParticleType::ExoticY:
            return "Exotic Y";
        case ParticleType::ExoticZ:
            return "Exotic Z";

        default:
            return "Unknown Particle";
        }
    }
};


std::vector<float> Particle::combineWaves(const std::vector<float> &wave1, const std::vector<float> &wave2)
{
    // Example of combining the waves by adding them element-wise.
    size_t size = std::min(wave1.size(), wave2.size());
    std::vector<float> combinedWave(size);

    for (size_t i = 0; i < size; ++i)
    {
        combinedWave[i] = wave1[i] + wave2[i]; // Simple addition of waves
    }

    return combinedWave;
}

// Class for Coil interactions
class Coil
{
public:
    std::vector<Particle> particles;

    // Adds particles to the system
    void addParticle(const Particle &p)
    {
        particles.push_back(p);
    }

    // Function to simulate coil interaction
    void interact()
    {
        for (size_t i = 0; i < particles.size(); ++i)
        {
            for (size_t j = i + 1; j < particles.size(); ++j)
            {
                // Interaction logic (energy redirection, amplitude breach, etc.)
                std::vector<float> wave1 = particles[i].generateWave();
                std::vector<float> wave2 = particles[j].generateWave();
                std::vector<float> resultingWave = Particle::combineWaves(wave1, wave2);

                // Check for amplitude breach
                if (!Particle::checkAmplitudeBreach(resultingWave).empty())

                {
                    // Create a new particle or anomaly based on the resulting wave
                    Particle newParticle = createNewParticle(resultingWave);
                    particles.push_back(newParticle);
                    recordAnomaly(resultingWave);
                }
            }
        }
    }

    // Combine the waves generated by two particles (simple addition for now)
    // std::vector<float> combineWaves(const std::vector<float> &wave1, const std::vector<float> &wave2)
    // {
    //     std::vector<float> resultingWave;
    //     size_t size = std::min(wave1.size(), wave2.size());
    //     for (size_t i = 0; i < size; ++i)
    //     {
    //         resultingWave.push_back(wave1[i] + wave2[i]);
    //     }
    //     return resultingWave;
    // }

    // Create a new particle based on the resulting wave
    Particle createNewParticle(const std::vector<float> &wave)
    {
        return Particle(ParticleType::Unknown, "New Anomalous Particle", 1.0e-28, 1.0e-19, 1.0e-13,
                        0.0f, 0.0f, 0.0f, 1.0f, 0.1f);
    }

    // Record anomalies by printing out the wave data and associated particle details
    void recordAnomaly(const std::vector<float> &wave)
    {
        std::cout << "Anomaly detected! Recording wave: ";
        for (float value : wave)
        {
            std::cout << std::fixed << std::setprecision(4) << value << " ";
        }
        std::cout << std::endl;
    }
};

#endif // PARTICLES_H
