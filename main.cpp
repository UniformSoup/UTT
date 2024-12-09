#include <iostream>
#include <Adore/Adore.hpp>
#include <thread>
#include <glm/glm.hpp>

#include <chrono>

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;
};

int main()
{
    // ADORE_LOG(INFO, "This is INFO!!!");
    // ADORE_LOG(WARN, "This is a warning!!!");
    // ADORE_LOG(ERROR, "This is an error!!!");
    // Adore::AdoreException("This is an exception");

    std::vector<Vertex> vertices =
    {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}}
    };

    std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

    try
    {
        auto ctx = Adore::Context::create(Adore::API::Vulkan, "UTT");
        auto window = Adore::Window::create(ctx, "UTT Window");
        auto renderer = Adore::Renderer::create(window);
        auto buffer = Adore::VertexBuffer::create(renderer, (void*) vertices.data(), sizeof(Vertex) * vertices.size());
        auto indexbuffer = Adore::IndexBuffer::create(renderer, (void*) indices.data(), sizeof(uint16_t) * indices.size());
        auto shader = Adore::Shader::create(window, 
            {
                {Adore::Shader::Type::VERTEX, "Shaders/triangle.vert.spv"},
                {Adore::Shader::Type::FRAGMENT, "Shaders/triangle.frag.spv"}
            },
            {
                {
                    {0, 0, offsetof(Vertex, pos), Adore::Shader::InputDescriptor::Format::VEC2_FLOAT},
                    {0, 1, offsetof(Vertex, color), Adore::Shader::InputDescriptor::Format::VEC3_FLOAT}
                },
                {
                    {0, sizeof(Vertex)}
                }
            });

        auto t1 = std::chrono::steady_clock::now();
        while (window->is_open())
        {
            window->poll();
            if (std::chrono::steady_clock::now() - t1 > std::chrono::milliseconds(50))
            {
                for (auto& v : vertices)
                    v.color = glm::vec3(v.color.z, v.color.x, v.color.y);

                buffer = Adore::VertexBuffer::create(renderer, (void*) vertices.data(), sizeof(Vertex) * vertices.size());
                t1 = std::chrono::steady_clock::now();
            }

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