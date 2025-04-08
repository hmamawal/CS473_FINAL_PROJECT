#include "import_object.hpp"
#include "../utilities/build_shapes.hpp"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


ImportOBJ::ImportOBJ() {
}

BasicShape ImportOBJ::loadFiles(std::string baseName, VAOStruct vao) {
    this->reset();
    std::string matName = baseName + ".mtl";
    std::string objName = baseName + ".obj";
    this->readMTLFile(matName);
    std::cout<<"Read MTL File:"<<matName<< std::endl;
    this->readOBJFile(objName);
    std::cout<<"Read OBJ File"<<objName<<std::endl;

    return this->genShape(vao);
}

unsigned int ImportOBJ::getTexture(int index) {
    unsigned int texture = 99;
   try {
    texture = this->textures[index];
    return texture;
   } catch (std::out_of_range &e) {
    std::cerr<<"Error getting texture "<<index <<std::endl;
    return 99;
   }
}

std::vector<unsigned int> ImportOBJ::getAllTextures() {
    return this->textures;
}

void ImportOBJ::readMTLFile(std::string fName) {
    std::ifstream infile(fName.c_str());
    if (infile.fail()) {
        std::cout << "ERROR: File " << fName << " does not exist.\n";
        return;
    }

    std::string curLine;
    //int matCount = 0;   // Index of next material to be added
    std::string current_material_name;
    while (std::getline(infile, curLine)) {
        Material* current_material = NULL;
        std::string linePrefix;
        std::istringstream iss(curLine, std::istringstream::in);
        iss >> linePrefix;


        if (linePrefix == "newmtl") {
            current_material_name = curLine.substr(7);
            Material new_mat;
            new_mat.material_name = current_material_name;
            this->material_map.insert(std::pair<std::string, 
                                      Material>(current_material_name, new_mat));
        }
        //Ambient Color
        else if (linePrefix == "Ka") {
            current_material = &(this->material_map.find(current_material_name)->second);
            glm::vec3 color = getVec3(curLine);
            current_material->ambient = color;
        }

        // Diffuse color
        else if (linePrefix == "Kd") {
            current_material = &(this->material_map.find(current_material_name)->second);
            glm::vec3 color = getVec3(curLine);
            current_material->diffuse = color;
        }

        //Specular Color
        else if (linePrefix == "Ks") {
            current_material = &(this->material_map.find(current_material_name)->second);
            glm::vec3 color = getVec3(curLine);
            current_material->specular = color;
        }

        //Opacity
        else if (linePrefix == "d") {
            current_material = &(this->material_map.find(current_material_name)->second);
            float opacity_setting = getFloat(curLine);
            current_material->opacity = opacity_setting;
        }

        //Texture
        else if (linePrefix == "map_Kd") {
            current_material = &(this->material_map.find(current_material_name)->second);
            this->textures.push_back(GetTexture(curLine.substr(7),true));
            current_material->textured = true;
            current_material->texture_index = textures.size()-1;
        }
    }
}

std::string GetVecStr (glm::vec3 v) {
        std::string s{};
        s += std::to_string(v.r) + ",";
        s += std::to_string(v.g) + ",";
        s += std::to_string(v.b);
        return s;
}

void ImportOBJ::PrintMaterials() {
    std::map<std::string,Material>::iterator i;

    for (i = this->material_map.begin(); i != this->material_map.end(); i++) {
        std::string name = i->first;
        Material material = i->second;
        std::cout<<"Material: "<<name<<std::endl;
        std::cout<<"  ambient: " << GetVecStr(material.ambient)<<std::endl;
        std::cout<<"  diffuse: " << GetVecStr(material.diffuse)<<std::endl;
        std::cout<<"  specular: " << GetVecStr(material.specular)<<std::endl;
        std::cout<<"  texture_index: " << material.texture_index << std::endl;
        std::cout<<"  textured? " << material.textured << std::endl;
    }

}

/** Loads .OBJ file into the ImportOBJ data structures */
void ImportOBJ::readOBJFile(std::string fName) {
    std::ifstream infile(fName.c_str());
    if (infile.fail()) {
        std::cout << "ERROR: File " << fName << " does not exist.\n";
        return;
    }

    std::string curLine;

    // Push placeholder vec3s to our data structures
    // Allows index numbers to directly align with vertex# or
    // normal# in the OBJ file (OBJ vertex numbers start at 1)
    this->vertices.push_back(glm::vec3(-1.0, -1.0, -1.0));
    this->normals.push_back(glm::vec3(-1.0, -1.0, -1.0));
    this->textCoords.push_back(glm::vec2(-1.0, -1.0));


    while (std::getline(infile, curLine)) {
        
        // .OBJ lines are prefixed to indicate what information they contain
        // Designate an input string stream to capture it
        std::string linePrefix;
        std::istringstream iss(curLine, std::istringstream::in);
        iss >> linePrefix;

        // Standard vertex coordinate
        if (linePrefix == "v") {
            this->vertices.push_back(this->getVec3(curLine));
        }

        // Vertex normal
        else if (linePrefix == "vn") {
            this->normals.push_back(this->getVec3(curLine));
        }

        // Vertex texture
        else if (linePrefix == "vt") {
            this->textCoords.push_back(this->getVec2(curLine));
        }

        // Changes the material being used
        else if (linePrefix == "usemtl") {
            std::string matName = curLine.substr(7);
            this->curMat = &(this->material_map.find(matName)->second);
        }

        // Face
        else if (linePrefix == "f") {
            this->readLineFace(curLine);
        }
    }

    if (debugOutput) {
        std::cout << fName << " file parsed.\n";
        std::cout << vertices.size() << " vertices parsed.\n";
        std::cout << normals.size() << " normals parsed.\n";
        std::cout << textCoords.size() << " texture coordinates parsed.\n";
        std::cout << combinedData.size() << " combined points.\n";

        infile.close();
    }
}

int ImportOBJ::getNumCombined() {
    return this->combinedData.size();
}

/** Generates a BasicShape from stored vertices and texture coordinates. */
BasicShape ImportOBJ::genShape(VAOStruct vao) {
    BasicShape new_shape;
    
    new_shape.Initialize(vao,(float*)&this->combinedData[0],
                         this->combinedData.size()*sizeof(CompleteVertex),
                         this->combinedData.size(),GL_TRIANGLES);
    
    // Calculate model bounds
    this->CalculateModelBounds(new_shape);

    return new_shape;
}

void ImportOBJ::CalculateModelBounds(BasicShape &shape) {
    // Reset bounds to extreme values
    glm::vec3 min_bounds(std::numeric_limits<float>::max());
    glm::vec3 max_bounds(-std::numeric_limits<float>::max());
    
    // Iterate through all vertices in combinedData
    for (const auto &vertex : combinedData) {
        min_bounds.x = std::min(min_bounds.x, vertex.Position.x);
        min_bounds.y = std::min(min_bounds.y, vertex.Position.y);
        min_bounds.z = std::min(min_bounds.z, vertex.Position.z);
        
        max_bounds.x = std::max(max_bounds.x, vertex.Position.x);
        max_bounds.y = std::max(max_bounds.y, vertex.Position.y);
        max_bounds.z = std::max(max_bounds.z, vertex.Position.z);
    }
    
    // If there are valid vertices, create a float array and call CalculateBounds on the shape
    if (!combinedData.empty()) {
        // Create temporary vertex array with just positions
        std::vector<float> positions;
        for (const auto &vertex : combinedData) {
            positions.push_back(vertex.Position.x);
            positions.push_back(vertex.Position.y);
            positions.push_back(vertex.Position.z);
        }
        
        // Calculate bounds
        shape.CalculateBounds(positions.data(), positions.size() / 3, 3 * sizeof(float));
        
        if (debugOutput) {
            glm::vec3 dimensions = shape.GetDimensions();
            std::cout << "Model dimensions: " 
                     << dimensions.x << " x " 
                     << dimensions.y << " x " 
                     << dimensions.z << std::endl;
        }
    }
}

/** Clears all internal data structures */
void ImportOBJ::reset() {
    this->vertices.clear();
    this->normals.clear();
    this->textCoords.clear();
    this->combinedData.clear();
    this->material_map.clear();
    this->textures.clear();
}

/** Only works with faces broken down into triangles */
void ImportOBJ::readLineFace(std::string line) {
    int firstVertexStart = 2;
    int secondVertexStart = line.find(" ", firstVertexStart) + 1;
    int thirdVertexStart = line.find(" ", secondVertexStart) + 1;
    this->readFace(line.substr(firstVertexStart, secondVertexStart - 1));
    this->readFace(line.substr(secondVertexStart, thirdVertexStart - 1));
    this->readFace(line.substr(thirdVertexStart));
    
}

// Given a string in the format:
// xx/yy/zz returns a ivec3 of (xx, yy, zz)
void ImportOBJ::readFace(std::string lineSegment) {
    // std::cout<<"Line: "<<lineSegment<<std::endl;
    int indexY = lineSegment.find("/", 0) + 1;
    int indexZ = lineSegment.find("/", indexY) + 1;
    int x = strtol(lineSegment.substr(0, indexY - 1).c_str(), NULL, 10);
    int y = strtol(lineSegment.substr(indexY, indexZ - 1).c_str(), NULL, 10);
    int z = strtol(lineSegment.substr(indexZ).c_str(), NULL, 10);
    // std::cout<<"  Line Parsed: "<<x<<","<<y<<","<<z<<std::endl;
    CompleteVertex newVert;
    newVert.Position = this->vertices.at(x);
    newVert.TexCoords = this->textCoords.at(y);
    newVert.Normal = this->normals.at(z);
    newVert.aColor = this->curMat->ambient;
    newVert.Color = this->curMat->diffuse;
    newVert.sColor = this->curMat->specular;
    newVert.opacity = this->curMat->opacity;
    if (this->curMat->textured) {
        newVert.texture_index = 1.0f*this->curMat->texture_index;
    }
    this->combinedData.push_back(newVert);

}

float ImportOBJ::getFloat (std::string line) {
    int indexD = line.find(" ",0)+1;
    return strtof(line.substr(indexD,line.find(".",0)+4).c_str(),NULL);
}

glm::vec3 ImportOBJ::getVec3(std::string line) {
    // Line format, where _ is a space and XX designates the info on the line
    // mtllib = file name for materials
    // v = vertex coordinate
    // vn = vertex normal
    // vt = vertex texture coordinate
    // <x>, <y>, <z> are the x, y, and z coordinates

    // XX_<x>_<y>_<z>

    // Finds index of start of x, y, and z coordinates
    int indexX = line.find(" ", 0) + 1;
    int indexY = line.find(" ", indexX) + 1;
    int indexZ = line.find(" ", indexY) + 1;
    float x = strtof(line.substr(indexX, indexY-1).c_str(), NULL);
    float y = strtof(line.substr(indexY, indexZ-1).c_str(), NULL);
    float z = strtof(line.substr(indexZ).c_str(), NULL);
    return glm::vec3(x, y, z);
}

glm::vec2 ImportOBJ::getVec2(std::string line) {
    // As getVec3, except only gets x and y
    // Used primarily for vt coordinates
    int indexX = line.find(" ", 0) + 1;
    int indexY = line.find(" ", indexX) + 1;
    float x = strtof(line.substr(indexX, indexY-1).c_str(), NULL);
    float y = strtof(line.substr(indexY).c_str(), NULL);
    return glm::vec2(x, y);
}



