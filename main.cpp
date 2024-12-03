#include <iostream>
#include <Adore/Adore.hpp>
#include <thread>

int main()
{
    // ADORE_LOG(INFO, "This is INFO!!!");
    // ADORE_LOG(WARN, "This is a warning!!!");
    // ADORE_LOG(ERROR, "This is an error!!!");
    // Adore::AdoreException("This is an exception");

    auto ctx = Adore::Context::create(Adore::API::Vulkan, "UTT");
    auto window = Adore::Window::create(ctx, "UTT Window");

    return EXIT_SUCCESS; 
}