#ifndef IMPORTOBJ_HPP
#define IMPORTOBJ_HPP

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "basic_shape.hpp"

/** ImportOBJ is a basic class that facilitates creating BasicShape objects
 *  from  *.obj and *.mtl files generated using a program like Blender.
 *  Exported faces need to be triangulated for this class to work properly.
 **/  
class ImportOBJ{
    public:
        ImportOBJ();
        bool printAll = false;
        /** Returns a new Shape object after loading the .OBJ/.MTL files
          * Only provide the base name (without .OBJ/.MTL extension).  
          * Requires a VAO to be provided that handles the following data:
          * Position (3xfloat values)
          * Surface Normal (3xfloat values)
          * Texture Coordinates (2xfloat values)
          * Color (3xfloat values)
          * Specular Color (3xfloat values)
          *  */
        BasicShape loadFiles(std::string name_without_file_extension, VAOStruct vao);
        bool debugOutput = false;

        int getNumCombined();
        unsigned int getTexture(int index = 0);
        std::vector<unsigned int> getAllTextures();

    private:
        struct CompleteVertex {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 TexCoords;
            glm::vec3 aColor;
            glm::vec3 Color;
            glm::vec3 sColor;
            float opacity = 1.0;
            float texture_index=99.0;
        };

        struct Material {
            std::string material_name;
            glm::vec3 ambient = glm::vec3(1.0);
            glm::vec3 diffuse = glm::vec3(1.0);
            glm::vec3 specular = glm::vec3(1.0);
            float opacity = 1.0;
            int illumination_model = -1;
            bool textured = false;
            int texture_index = 99;
        };

        void readMTLFile(std::string fName);
        void readOBJFile(std::string fName);
        BasicShape genShape(VAOStruct vao);
        void reset();
        void PrintMaterials();
        
        Material *curMat = NULL;

        std::vector<unsigned int> textures;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textCoords;
        std::vector<CompleteVertex> combinedData;
        std::map<std::string,Material> material_map;
    
        void readLineFace(std::string line);
        void readFace(std::string lineSegment);
        float getFloat (std::string line);
        glm::vec3 getVec3(std::string line);
        glm::vec2 getVec2(std::string line);
};



#endif // IMPORTOBJ_HPP
