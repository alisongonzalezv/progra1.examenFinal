cmake_minimum_required(VERSION 3.25)
project(PizzaBuilder CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(PizzaBuilder
        main.cpp
        PizzaWindow.cpp
        PizzaWindow.h
        PizzaWindow.ui
        PizzaWindow.qrc
        Headers/PizzaBuilder.h
        Headers/IngredientSelector.h
        Headers/CheeseRepository.h
        Headers/MeatsRepository.h
        Headers/HerbsRepository.h
        Headers/TomatoesRepository.h
        Headers/DataLoader.h
        Classes/PizzaBuilder.cpp
        Classes/IngredientRepository.cpp
        Classes/CheeseRepository.cpp
        Classes/MeatsRepository.cpp
        Classes/HerbsRepository.cpp
        Classes/TomatoesRepository.cpp
        Classes/DataLoader.cpp
)

target_include_directories(PizzaBuilder PRIVATE Headers)
target_link_libraries(PizzaBuilder PRIVATE Qt6::Widgets)

add_custom_command(TARGET PizzaBuilder POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Data $<TARGET_FILE_DIR:PizzaBuilder>/Data)
