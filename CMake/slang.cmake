function(compile_slang_file_to_spirv slang_shader_path output_directory)
    cmake_path(GET slang_shader_path FILENAME slang_shader_name)
    cmake_path(GET slang_shader_path STEM slang_shader_stem)

    make_directory(${CMAKE_CURRENT_BINARY_DIR}/${output_directory})
    add_custom_target(${slang_shader_name}_compile ALL
        COMMAND slangc ${CMAKE_CURRENT_SOURCE_DIR}/${slang_shader_path} -target spirv -o ${CMAKE_CURRENT_BINARY_DIR}/${output_directory}/${slang_shader_stem}.spv
    )
endfunction()