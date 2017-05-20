#include "particle.h"
#include "build_options.h"
#include "debug_draw.h"
#include "imgui.h"
#include <algorithm>

static Particle particles[PARTICLE_NUM];
static sf::Vertex particleVertex[PARTICLE_NUM];
static int particleIndex = 0;

void particles_tick(sf::Vector2f camera) {
    for (int i = 0; i < PARTICLE_NUM; ++i) {
        if (!particles[i].active) continue;

        // Move & accelerate.
        particles[i].position[0] += particles[i].velocityVec[0];
        particles[i].position[1] += particles[i].velocityVec[1];
        particles[i].velocityVec[0] += particles[i].accelerationVec[0];
        particles[i].velocityVec[1] += particles[i].accelerationVec[1];

        // Kill due to lifetime.
        if (particles[i].lifetime-- < 0) {
            particles[i].active = false;
        }

        // Kill due to offscreen
        else if (particles[i].position[0] < camera.x ||
            particles[i].position[0] > camera.x + WINDOW_WIDTH ||
            particles[i].position[1] < camera.y ||
            particles[i].position[1] > camera.y + WINDOW_HEIGHT) {
            particles[i].active = false;
        }
    }
}

void particles_draw(sf::RenderWindow &window, sf::Vector2f camera) {
    int vertexIndex = 0;
    for (int i = 0; i < PARTICLE_NUM; ++i) {
        if (particles[i].active) {
            particleVertex[vertexIndex].position.x = particles[i].position[0] - camera.x;
            particleVertex[vertexIndex].position.y = particles[i].position[1] - camera.y;
            particleVertex[vertexIndex].color = sf::Color(
                particles[i].colour[0],
                particles[i].colour[1],
                particles[i].colour[2],
                particles[i].colour[3]);
            vertexIndex++;
        }
    }

    window.draw(particleVertex, vertexIndex, sf::PrimitiveType::Points);

    if (edit && ImGui::CollapsingHeader("particles")) {
        ImGui::Text("Particles on screen: %i", vertexIndex);
    }
}

void add_particle(Particle *part) {
    particles[particleIndex] = *part;
    particles[particleIndex].active = true;
    particleIndex = (particleIndex + 1) % PARTICLE_NUM;
}

void add_particle(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f acceleration, sf::Color colour, int lifetime) {
    particles[particleIndex].position[0] = position.x;
    particles[particleIndex].position[1] = position.y;
    particles[particleIndex].velocityVec[0] = velocity.x;
    particles[particleIndex].velocityVec[1] = velocity.y;
    particles[particleIndex].accelerationVec[0] = acceleration.x;
    particles[particleIndex].accelerationVec[1] = acceleration.y;
    particles[particleIndex].colourInt = colour.toInteger();
    particles[particleIndex].lifetime = lifetime;
    particles[particleIndex].active = true;
    particleIndex = (particleIndex + 1) % PARTICLE_NUM;
}

/*

ParticleSystem particleSystem;

ParticleSystem::ParticleSystem() {
    index = 0;
    gravity = sf::Vector2f(0.f, GRAVITY);
}

void ParticleSystem::draw(sf::RenderWindow &window, sf::Vector2f camera) {
    int activeCount = 0;
    for (unsigned int i = 0; i < PARTICLE_NUM; ++i) {
        Particle &p = parts[i];
        if (!p.active) continue;
        ++activeCount;
        shape.setOrigin(shape.getSize() / 2.f);
        shape.setPosition(p.position - camera);
        shape.setFillColor(p.colour);
        window.draw(shape);
    }
    
    if (edit && ImGui::CollapsingHeader("Particles")) {
        ImGui::Text("particles on screen: %i", activeCount);
    }
}

void ParticleSystem::add_particle(Particle part) {
    parts[index] = part;
    parts[index].active = true;
    index = (index + 1) % PARTICLE_NUM;
}

void ParticleSystem::tick(sf::Vector2f camera) {
    for (unsigned int i = 0; i < PARTICLE_NUM; ++i) {
        if (!parts[i].active) continue;
        parts[i].position += parts[i].velocity;
        parts[i].velocity += parts[i].gravity;
        if (--parts[i].lifetime < 0)
            parts[i].active = false;
        if (parts[i].lifetime < 255)
            parts[i].colour.a = parts[i].lifetime;
        if (parts[i].position.x < camera.x || parts[i].position.x > camera.x + WINDOW_WIDTH || parts[i].position.y < camera.y || parts[i].position.y > camera.y + WINDOW_HEIGHT)
            parts[i].active = false;
    }
}
*/