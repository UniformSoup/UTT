#include <iostream>
#include <Adore/Adore.hpp>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 uv;
};

struct Uniform
{
    glm::mat4 model, view, proj;
};

int main()
{
    std::vector<Vertex> vertices =
    {
        {{-0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{+0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{+0.5f, +0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, +0.5f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
    };

    std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

    Uniform uniform;
    uniform.model = glm::mat4(1.0f);
    uniform.view = glm::mat4(1.0f);
    uniform.proj = glm::mat4(1.0f);

    try
    {
        auto ctx = Adore::Context::create(Adore::API::Vulkan, "UTT");
        auto window = Adore::Window::create(ctx, "UTT Window");
        auto renderer = Adore::Renderer::create(window);
        auto buffer = Adore::VertexBuffer::create(renderer, (void*) vertices.data(), sizeof(Vertex) * vertices.size());
        auto indexbuffer = Adore::IndexBuffer::create(renderer, (void*) indices.data(), sizeof(uint16_t) * indices.size());
        auto ubo = Adore::UniformBuffer::create(renderer, (void*) &uniform, sizeof(Uniform));
        auto sampler = Adore::Sampler::create(renderer, "../../Assets/texture.jpg", Adore::Filter::LINEAR, Adore::Wrap::REPEAT);
        auto shader = Adore::Shader::create(window, 
            {
                {Adore::ShaderType::VERTEX, "Shaders/triangle.vert.spv"},
                {Adore::ShaderType::FRAGMENT, "Shaders/triangle.frag.spv"}
            },
            {
                {
                    {0, 0, offsetof(Vertex, pos), Adore::AttributeFormat::VEC2_FLOAT},
                    {0, 1, offsetof(Vertex, color), Adore::AttributeFormat::VEC3_FLOAT},
                    {0, 2, offsetof(Vertex, uv), Adore::AttributeFormat::VEC2_FLOAT}
                },
                {
                    {0, sizeof(Vertex)}
                },
                {
                    {0, 1, Adore::ShaderType::VERTEX, Adore::ResourceType::BUFFER},
                    {1, 1, Adore::ShaderType::FRAGMENT, Adore::ResourceType::SAMPLER}
                }
            });

        shader->attach(ubo, 0);
        shader->attach(sampler, 1);

        auto clock = std::chrono::steady_clock::now();

        while (window->is_open())
        {
            auto dt = (float) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - clock).count();
            clock = std::chrono::steady_clock::now();
            window->poll();

            uniform.model = glm::rotate(uniform.model, 0.001f * dt, glm::vec3(0.0f, 0.0f, 1.0f));
            ubo->set(&uniform);

            renderer->begin(shader);
                renderer->bind(buffer, 0);
                renderer->bind(indexbuffer);
                renderer->drawIndexed(6);
            renderer->end();
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS; 
}