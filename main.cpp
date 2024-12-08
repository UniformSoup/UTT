#include <iostream>
#include <Adore/Adore.hpp>
#include <thread>

int main()
{
    // ADORE_LOG(INFO, "This is INFO!!!");
    // ADORE_LOG(WARN, "This is a warning!!!");
    // ADORE_LOG(ERROR, "This is an error!!!");
    // Adore::AdoreException("This is an exception");

    try
    {
        auto ctx = Adore::Context::create(Adore::API::Vulkan, "UTT");
        auto window = Adore::Window::create(ctx, "UTT Window");
        auto renderer = Adore::Renderer::create(window);
        auto shader = Adore::Shader::create(renderer, 
        {
            {Adore::Shader::Type::VERTEX, "Shaders/triangle.vert.spv"},
            {Adore::Shader::Type::FRAGMENT, "Shaders/triangle.frag.spv"}
        });

        while (window->is_open())
        {
            window->poll();
            renderer->render(shader);
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS; 
}