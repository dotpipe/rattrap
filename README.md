**Particle Simulation Program Overview**

### Introduction
This particle simulation program is designed to model and analyze the interactions between various fundamental particles, such as quarks, leptons, and bosons. It utilizes a coil methodology to simulate and predict collisions, recording new and unexpected outcomes to further our understanding of particle physics.

### The Coil Methodology Explained
The coil methodology is an approach used to track particle interactions by considering their wave-like properties. Each particle possesses characteristics such as mass, charge, spin, velocity, and energy, all of which contribute to its wave function. When two particles interact, their waveforms combine and produce interference patterns, much like ripples on a pond when two stones are thrown in.

The program processes particle interactions by:
1. **Generating wave functions** - Using a particle's attributes to create a representative waveform.
2. **Combining waves** - Overlaying two waveforms to model how particles interact.
3. **Analyzing the results** - Determining if a known or unknown particle emerges from the collision.
4. **Recording new particles** - Storing unique findings in a CSV file for further analysis.

### How the Simulation Works
1. **Initialization**
   - A vector of `Particle` objects is created, each initialized with attributes such as mass, charge, energy, and spin.
   - Example particle types include quarks (up, down, charm, strange, top, bottom), leptons (electron, muon, tau), and bosons (photon, W, Z, Higgs).

2. **Simulation Process**
   - Two particles are randomly selected from the vector.
   - Their wave functions are generated using their unique properties.
   - The wave functions are combined to simulate the interaction.
   - The resulting waveform is analyzed to check for existing or unknown particles.

3. **Data Output**
   - If the collision results in a known particle, the interaction is logged but not stored in the CSV.
   - If an unknown particle is discovered, the program logs detailed data into a CSV file.

### CSV Output Structure
The CSV file records unique interactions and follows this structure:
```
Particle1, Particle2, InteractionType, Mass, Charge, Energy, Velocity, Spin, Radius, Lifetime
```
Example row:
```
QuarkCharm, QuarkStrange, Unknown, 3.5e-27, 3.0e-19, 4.2e-13, 0.85, 1.0, 1020.0, 500.0
```
This format allows easy tracking and future analysis of unexpected particle formations.

### Why It's Interesting
This program provides a glimpse into the complex world of subatomic particles, allowing non-experts to understand how fundamental particles interact and transform. By running multiple iterations, users can discover rare interactions and explore the unknown facets of particle physics without requiring expensive laboratory equipment.

### Key Insights for Users
- **Patterns and Predictions:** Over time, users may notice trends in particle interactions.
- **Learning Opportunity:** The program serves as a practical educational tool to understand concepts like wave-particle duality and quantum behavior.
- **Potential Discoveries:** If enough data is collected, new particles or unexpected behaviors might be identified.

### Conclusion
The particle simulation program is a fascinating blend of theoretical physics and computational modeling. It enables users to simulate, analyze, and record particle interactions with ease, offering both educational and research value. Whether you're a physics enthusiast or a student looking to grasp particle behavior, this program offers a unique and informative experience.

# RatTrap Particle Physics Simulator

## Dependencies

This project requires the following libraries:

- OpenGL
- GLUT (OpenGL Utility Toolkit)
- GLU (OpenGL Utility Library)
- SFML 2.5 or higher (Simple and Fast Multimedia Library)

Make sure to install these dependencies before building the project.

## Building

To build the project, follow these steps:

### Compile

g++ -std=c++17 main26.cpp AnomalyVisualizer.cpp -o particle_visualizer -lGL -lGLU -lglut -lsfml-graphics -lsfml-window -lsfml-system

or

$ mkdir build

$ cd build

$ cmake ..

$ cmake --build .

$ sudo cmake --install .
