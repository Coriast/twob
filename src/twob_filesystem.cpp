#include <twob_filesystem.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "twob_internal.h"
#include <Windows.h>

using namespace twob;
namespace fs = std::filesystem;


std::string Path::get_absolute_path()
{
    char buffer[1024];
    GetModuleFileNameA(NULL, buffer, 1024);
    std::string path(buffer);
    path = path.substr(0, path.rfind("\\"));

	return path;
}

std::string Path::get_absolute_path(const char* filename)
{
    std::string path = Path::get_absolute_path();
    path.append("\\"); path.append(filename);

    return path;
}

void File::load_meshes_from_obj(cstr folder_path, std::vector<Mesh*> &meshes)
{
    std::string obj_str;
    for (const auto& filename : fs::directory_iterator(folder_path))
    {
        if (filename.path().string().find(".obj"))
            obj_str = filename.path().string();
    }

    std::ifstream obj;
    obj.open(obj_str);

    std::stringstream buffer;
    buffer << obj.rdbuf();

    std::string kword;

    //int pos_qty = 0;
    int norms_qty = 0;

    std::vector<vertex> vertices;
    std::vector<u_int> indices;
    std::vector<glm::vec3> normals;
    while (!buffer.eof())
    {
        buffer >> kword;

        // v  Geometric vertices: v x y z
        if (kword == "v")
        {
            glm::vec3 pos;
            buffer >> pos.x >> pos.y >> pos.z;
            vertices.push_back(
                { pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) }
            );
        }
        // vn   Vertex normals: vn dx dy dz
        else if (kword == "vn")
        {
            glm::vec3 norm;
            buffer >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        }
        // f   Face with vertex normals: f v1//n1 v2//n2 .... vn//nn
        else if (kword == "f")
        {
            std::string face;
            int vert_index, norm_index;

            buffer >> face;
            vert_index = std::stoul(face.substr(0, face.find_first_of('/')), nullptr, 0) - 1;
            indices.push_back( vert_index );
            norm_index = std::stoul(face.substr(face.find_last_of('/') + 1), nullptr, 0) - 1;
            vertices[vert_index].normal = normals.at(norm_index);

            buffer >> face;
            vert_index = std::stoul(face.substr(0, face.find_first_of('/')), nullptr, 0) - 1;
            indices.push_back(vert_index);
            norm_index = std::stoul(face.substr(face.find_last_of('/') + 1), nullptr, 0) - 1;
            vertices[vert_index].normal = normals.at(norm_index);

            buffer >> face;
            vert_index = std::stoul(face.substr(0, face.find_first_of('/')), nullptr, 0) - 1;
            indices.push_back(vert_index);
            norm_index = std::stoul(face.substr(face.find_last_of('/') + 1), nullptr, 0) - 1;
            vertices[vert_index].normal = normals.at(norm_index);
        }
        
    }

    meshes.push_back(Internal::app_renderer()->create_mesh(vertices));
    meshes[0]->indices = indices;
    obj.close();
    

}