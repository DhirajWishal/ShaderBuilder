# ShaderBuilder

C++ based SPIR-V generator.

## Why Shader Builder?

Well as of now, we can either hardcode the shader code or use external files to load the shaders to the application. This comes with some problems because it's less flexible compared to
what we can achieve from building the shader from within the application. Shader Builder allows you to do exactly that; to build the shaders from the application itself and export to
GLSL, HLSL, MSL or any other compatible high-level language or straight up provide the SPIR-V binary to the graphics API.

## How can I use this?

Before we start, make sure you have a C++ compiler and CMake. Once the basic dependencies are down, start by cloning and initializing the submodules.

```bash
git clone https://github.com/DhirajWishal/ShaderBuilder {SOURCE}
cd {SOURCE}
git submodule update --init
```

Once it's cloned and initialized you can either build the library using CMake and link the `ShaderBuilder` library file, or add this to your CMake project as a subdirectory.

Note that you set the include directory to `{SOURCE}/Include` and you're good to go!

## Where can I get started?

Take a look at the `{SOURCE}/Example` directory and use it as a starting point to get started.

## Licensing

This repository is licensed under MIT.
