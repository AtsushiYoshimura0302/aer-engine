aer-engine
================

![](./screenshots/aura.png)

About
---------------------------------

An OpenGL 4.3 / C++ 11 rendering engine oriented towards animation.

Features:

* Custom animation model format, SKMA, with a [Blender exporter](https://github.com/tcoppex/io-scene-skma) and a C++ Importer.
* Skeleton animation with GPU Skinning using *Dual Quaternion Blending*.
* Blend shape control (*Morph Targets*).
* Blend tree for sequences and clips processing.

Demos
---------------------------------

<table>
    <tr>
        <td>aura</td>
        <td>A technical demo demonstrating the animation capabilities of the engine, with some rendering techniques (eg. HBAO on Compute Shader).</td>
    </tr>
    <tr>
        <td>cuda_cs_blur</td>
        <td>Performance comparison between a CUDA and a Compute Shader blur kernel.</td>
    </tr>
    <tr>
        <td>gpu_raymarching</td>
        <td>Raymarching on a Fragment Shader.</td>
    </tr>
    <tr>
        <td>hair</td>
        <td>dynamic hair simulation rendered with tesselation and instanciation.</td>
    </tr>
    <tr>
        <td>ik_demo</td>
        <td>A Basic Inverse Kinematic demo.</td>
    </tr>
    <tr>
        <td>marching_cube</td>
        <td>Procedural geometry generation with a marching cube algorithm on the GPU using transform feedback.</td>
    </tr>
</table>

Compilation
---------------------------------

The build was compiled against GCC 4.9.

Compile first the engine, then the demos :
```bash
mkdir build; cd build;
mkdir engine; cd engine
cmake ../../engine -DCMAKE_BUILD_TYPE:STRING=Release
make -j4
cd ..
mkdir demos; cd demos
cmake ../../demos -DCMAKE_BUILD_TYPE:STRING=Release
make -j4
```

Engine dependencies :

<table>
    <tr>
        <td>SFML 2.1</td>
        <td>Used as core window manager.</td>
    </tr>
    <tr>
        <td>Freeimage 3</td>
        <td>Image loader.</td>
    </tr>
    <tr>
        <td>Armadillo 3.9</td>
        <td>Linear algebra library.</td>
    </tr>
    <tr>
        <td>GLM 0.9.6+</td>
        <td>OpenGL Mathematics library.</td>
    </tr>
    <tr>
        <td>GLEW 0.9.0+</td>
        <td>OpenGL wrangler.</td>
    </tr>
    <tr>
        <td>GLSW</td>
        <td>GLSL wrangler (provided).</td>
    </tr>
</table>

*Version number corresponded to the development environment.*

