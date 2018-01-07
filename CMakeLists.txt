# cmake_minimum_required(VERSION <specify CMake version here>)
project(coperC)

set(CMAKE_CXX_STANDARD 11)

add_executable(coperC
        coperC/include/analyzer.h
        coperC/include/assembling.h
        coperC/include/config_file_loader.h
        coperC/include/coper.h
        coperC/include/file_handler.h
        coperC/include/folder_file_validation.h
        coperC/include/interactive_controller.h
        coperC/include/parameter_parser.h
        coperC/src/analyzer.cpp
        coperC/src/assembling.cpp
        coperC/src/config_file_loader.cpp
        coperC/src/coper.cpp
        coperC/src/file_handler.cpp
        coperC/src/folder_file_validation.cpp
        coperC/src/interactive_analyzer.cpp
        coperC/src/main.cpp
        coperC/src/parameter_parser.cpp
        external-includes/cllib/include/cl/core/cl_compile.h
        external-includes/cllib/include/cl/mathex/Bezier.h
        external-includes/cllib/include/cl/mathex/def.h
        external-includes/cllib/include/cl/mathex/RenderAABB.h
        external-includes/cllib/include/cl/mathex/RenderAlgorithm.h
        external-includes/cllib/include/cl/mathex/RenderMath.h
        external-includes/cllib/include/cl/mathex/RenderMatrix3.h
        external-includes/cllib/include/cl/mathex/RenderMatrix4.h
        external-includes/cllib/include/cl/mathex/RenderQuaternion.h
        external-includes/cllib/include/cl/mathex/RenderVector2.h
        external-includes/cllib/include/cl/mathex/RenderVector3.h
        external-includes/cllib/include/cl/mathex/RenderVector4.h
        external-includes/cllib/include/cl/cl_console.h
        external-includes/cllib/include/cl/cl_folder_file.h
        external-includes/cllib/include/cl/cl_hierarchical_structure.h
        external-includes/cllib/include/cl/cl_misc.h
        external-includes/cllib/include/cl/cl_regexp_util.h
        external-includes/cllib/include/cl/cl_types.h
        external-includes/cllib/include/cl/Log.h)
