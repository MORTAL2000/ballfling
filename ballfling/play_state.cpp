#include "play_state.h"
#include "world.h"
#include "terrain_entity.h"
#include "util.h"
#include "ball_entity.h"
#include "gate.h"
#include "build_options.h"
#include "gui.h"
#include "options_state.h"
#include "win_state.h"
#include "pause_state.h"
#include <string>
#include "imgui.h"
#include "debug_draw.h"

std::string level_num_to_filename(int levelNum) {
    return std::string("data/lvl" + util::to_string(levelNum) + ".png");
}

StatePlay::StatePlay(World *world, int levelNum) : State(world) {
    this->levelNum = levelNum;
    filename = level_num_to_filename(levelNum);

    //levelColour = sf::Color::Red;
    levelColour = sf::Color(99, 155, 255);

    gui.RemoveAll();
    world->remove_entity(ENTITY_TAG_ALL);

    // create terrain
    terrain = new EntityTerrain(2.f, filename, levelColour);
    if (terrain->error) {
        return;
    }
    world->add_entity(terrain);

    // add ball & centre camera
    player = new EntityBall(terrain->playerSpawn, sf::Vector2f(), levelColour);
    world->add_entity(player);
    sf::Vector2f screenSize((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
    world->camera = (player->position - screenSize / 2.f);

    // add gate
    gate = new Gate(sf::Vector2f(650.f, 750.f), 25.f, 64.f, 2.f);
    world->add_entity(gate);

    completed = false;
}

void StatePlay::on_event(sf::Event &event) {
    if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::P)) {
        State::push_state(new StatePause(world));
    }
}

void StatePlay::on_tick() {
    if (terrain->error) State::pop_state(); 
    if (completed) {
        if (!world->is_paused()) world->toggle_pause(); // pause world
        State::push_state(new StateWin(world, levelNum));
    }
}

void StatePlay::on_draw(sf::RenderWindow &window) {
    if (edit && ImGui::CollapsingHeader("Gates")) {
        ImGui::DragFloat("x", &gate->position.x);
        ImGui::DragFloat("y", &gate->position.y);
        ImGui::DragFloat("angle", &gate->angle);
        ImGui::DragFloat("strength", &gate->strength, .002f, 0.f, 16.f);
    }
}

void StatePlay::on_draw_ui(sf::RenderWindow &window) {
    hud.draw(window, world->camera, levelColour);
}

void StatePlay::on_gain_focus() {
    gui.RemoveAll();
    if (world->is_paused()) world->toggle_pause();
}

void StatePlay::on_lose_focus() {
}

void StatePlay::on_notify(Event event, void *data) {
    if (event == EVENT_BALL_HIT_FINISH) {
        completed = true;
    }
}