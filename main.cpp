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
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    try
    {
        auto ctx = Adore::Context::create(Adore::API::Vulkan, "UTT");
        auto window = Adore::Window::create(ctx, "UTT Window");
        auto renderer = Adore::Renderer::create(window);
        auto buffer = Adore::Buffer::create(renderer, Adore::Buffer::Usage::Vertex, 
                                            (void*) vertices.data(), sizeof(Vertex) * vertices.size());
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
            if (std::chrono::steady_clock::now() - t1 > std::chrono::milliseconds(300))
            {
                for (auto& v : vertices)
                    v.color = glm::vec3(v.color.z, v.color.x, v.color.y);

                buffer = Adore::Buffer::create(renderer, Adore::Buffer::Usage::Vertex, 
                            (void*) vertices.data(), sizeof(Vertex) * vertices.size());
                t1 = std::chrono::steady_clock::now();
            }

            renderer->begin(shader);
                renderer->bind(buffer, 0);
                renderer->draw(3);
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