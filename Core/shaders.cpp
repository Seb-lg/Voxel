//  Created on 10/03/21 by
//  ███████╗ ██████╗ ██████╗ ██╗ █████╗  ██████╗
//  ╚══███╔╝██╔═══██╗██╔══██╗██║██╔══██╗██╔════╝
//    ███╔╝ ██║   ██║██║  ██║██║███████║██║
//   ███╔╝  ██║   ██║██║  ██║██║██╔══██║██║
//  ███████╗╚██████╔╝██████╔╝██║██║  ██║╚██████╗
//  ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝
// Inspired from StackDanny's answer at
// https://en.sfml-dev.org/forums/index.php?topic=25019.0
#include "Core.hpp"


sf::Sprite Core::applyShaders(sf::RenderTexture &rawGameTexture) {
    //apply luminescence
    shader_states.shader = &luminescence_shader;
    luminescence_render.clear();
    luminescence_render.draw(sf::Sprite(rawGameTexture.getTexture()), shader_states);
    luminescence_render.display();

    //apply two pass gaussian blur n times to simulate gaussian blur.
    shader_states.shader = &blur_shader;
    for (int i = 0; i < blur_renders.size(); ++i) {
        blur_shader.setUniform("blur_radius", BLUR_RADIUS_VALUES[i]);

        blur_renders[i].clear();
        blur_renders[i].draw(sf::Sprite(luminescence_render.getTexture()));
        blur_renders[i].display();

        //vertical blur
        blur_shader.setUniform("blur_direction", sf::Glsl::Vec2(1.0, 0.0));
        blur_renders[i].draw(sf::Sprite(blur_renders[i].getTexture()), shader_states);
        blur_renders[i].display();

        //horizontal blur
        blur_shader.setUniform("blur_direction", sf::Glsl::Vec2(0.0, 1.0));
        blur_renders[i].draw(sf::Sprite(blur_renders[i].getTexture()), shader_states);
        blur_renders[i].display();
    }

    //load blur_renders[0] into assemble_render so we can add the other blurs ontop of it
    shader_states.shader = &multiply_shader;
    multiply_shader.setUniform("multiply", blur_weight);
    assemble_render.clear();
    assemble_render.draw(sf::Sprite(blur_renders[0].getTexture()), shader_states);
    assemble_render.display();

    //adding the rest ontop creating a final blur
    shader_states.shader = &assemble_shader;
    assemble_shader.setUniform("add_weight", blur_weight);
    for (int i = 1; i < blur_renders.size(); ++i) {
        assemble_shader.setUniform("add_texture", blur_renders[i].getTexture());
        assemble_render.draw(sf::Sprite(assemble_render.getTexture()), shader_states);
        assemble_render.display();
    }

    //final result; scene + blur
    assemble_shader.setUniform("add_weight", 1.0f);
    assemble_shader.setUniform("add_texture", assemble_render.getTexture());
    assemble_render.draw(sf::Sprite(rawGameTexture.getTexture()), shader_states);
    assemble_render.display();

    return sf::Sprite(assemble_render.getTexture());
}


void Core::loadShaders() {
    luminescence_shader.loadFromFile("./Shaders/luminescence.frag", sf::Shader::Fragment);
    luminescence_shader.setUniform("texture", sf::Shader::CurrentTexture);
    luminescence_shader.setUniform("threshold", 0.4f);

    blur_shader.loadFromFile("./Shaders/boxblur.frag", sf::Shader::Fragment);
    blur_shader.setUniform("texture", sf::Shader::CurrentTexture);
    blur_shader.setUniform("texture_inverse", 1.0f / width);

    assemble_shader.loadFromFile("./Shaders/assemble.frag", sf::Shader::Fragment);
    assemble_shader.setUniform("texture", sf::Shader::CurrentTexture);

    multiply_shader.loadFromFile("./Shaders/multiply.frag", sf::Shader::Fragment);
    multiply_shader.setUniform("texture", sf::Shader::CurrentTexture);

    context_settings.antialiasingLevel = 2;

    rawGameTexture.create(width, height, context_settings);
    luminescence_render.create(width, height, context_settings);
    assemble_render.create(width, height, context_settings);

    // Addding multiple boxblurs with different radii looks really nice! in this case 4 layers
    for (int i = 0; i < blur_renders.size(); ++i) {
        blur_renders[i].create(width, height, context_settings);
    }
    blur_weight = blur_renders.empty() ? 0.0 : 1.0 / blur_renders.size();

    // From https://coding-experiments.blogspot.com/2010/06/pixelation.html
    pixelate_shader.loadFromFile("./Shaders/pixelateHex.vert", sf::Shader::Fragment);
    pixelate_shader.setUniform("tex", sf::Shader::CurrentTexture);
    // pixelate_shader.setUniform("resolution", 2048);
}
