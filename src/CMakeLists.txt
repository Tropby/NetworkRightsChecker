# setup your application
add_executable(NetworkRightsChecker 
    nrc.cpp
)

# set ebcpp linking options to the target
target_link_libraries(NetworkRightsChecker ${EBCPP_LIBS})

add_custom_command(
        TARGET NetworkRightsChecker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/../res/style.css
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/style.css)

add_custom_command(
        TARGET NetworkRightsChecker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/../res/sampleConfig.ini
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sampleConfig.ini)