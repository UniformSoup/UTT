set(SHADER_SOURCES
    triangle.vert
    triangle.frag
)

foreach(SHADER_SOURCE ${SHADER_SOURCES})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_SOURCE}.spv
        COMMAND glslc -o ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_SOURCE}.spv ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_SOURCE}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_SOURCE}
        COMMENT "Compiling Shader: [${SHADER_SOURCE}]"
    )
endforeach()

set(SHADER_OUTPUTS)
foreach(SHADER_SOURCE ${SHADER_SOURCES})
    list(APPEND SHADER_OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_SOURCE}.spv)
endforeach()

add_custom_target(shaders
    COMMENT "Compiling shaders..."
    DEPENDS ${SHADER_OUTPUTS}
)
add_dependencies(${PROJECT_NAME} shaders)