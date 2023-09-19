# twob Framework
Consiste de uma abstração de APIs gráficas (OpenGL, Vulkan, DirectX) em uma única interface, para facilitação no desenvolvimento de aplicações 3D, com criação e manipulação de janelas utilizando GLFW. 

![image](https://github.com/Coriast/twob/assets/38512684/22768f6d-c838-41f1-9de4-d31f48b4b547)
## Objetivo
O objetivo do desenvolvimento do framework é fornecer suporte para futuros projetos na área de computação gráfica.

### Demonstração 
Inicia uma janela com um cubo vermelho em um plano 3D
```cpp
#include <twob.h>
using namespace twob;

Cluster demo;

void startup()
{
  demo.init();
}

void render()
{
  demo.load_primitive("cube", Polygon::CUBE)->meshes[0]->color = Color::red();
  demo.render();
}

int main()
{
  Config config;
  config.name = "Demo";
  config.on_startup = startup;
  config.on_render = render;

  App::run(&config);
}
```
## Instalação
Para rodar esse projeto localmente, siga os seguintes passos:
1. Clone esse repositório:
```bash
git clone <repository-url>
```

2. Navegue até o diretório desse projeto:
```bash
cd <project-directory>
```

3. Instale as dependências utilizando vcpkg:

Opcional: Para integração das dependências com Visual Studio
```bash
vcpkg integrate install
```
GLFW
```bash
vcpkg install glfw3 --triplet x64-windows
```
GLM
```bash
vcpkg install glm --triplet x64-windows
```
GLM
```bash
vcpkg install glad --triplet x64-windows
```

4. Build com o CMake

```bash
mkdir build && cd build
cmake ../
cmake --build .
```

## Documentação
Em desenvolvimento


