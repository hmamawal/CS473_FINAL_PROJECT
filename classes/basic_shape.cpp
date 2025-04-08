#include "basic_shape.hpp"

BasicShape::BasicShape()
{
    this->number_vertices = 0;
    this->vbo = 0;
    this->primitive = GL_TRIANGLES;
    this->ebo = 0;
    this->ebo_number_indices = 0;
    this->ebo_primitive = GL_LINE_LOOP;
}

void BasicShape::Initialize(VAOStruct vao, float* vertices, int vertices_bytes, int num_vertices, GLuint prim)
{
    this->vao = vao;
    this->number_vertices = num_vertices;
    this->primitive = prim;
    glGenBuffers(1,&(this->vbo));
    glBindBuffer(GL_ARRAY_BUFFER,this->vbo);
    glBufferData(GL_ARRAY_BUFFER,vertices_bytes,vertices,GL_STATIC_DRAW);
    
    // Calculate bounds based on vertex data
    // Determine the stride based on the VAO attributes
    int stride = 0;
    for (const auto& attr : vao.attributes) {
        stride += attr.number_per_vertex * sizeof(float); // Using number_per_vertex instead of size
    }
    
    // Only calculate bounds if we have vertices
    if (num_vertices > 0 && vertices != nullptr) {
        CalculateBounds(vertices, num_vertices, stride);
    }
}

void BasicShape::InitializeEBO(unsigned int *ebo_data, int ebo_bytes, 
                            int num_indices, int primitive)
{
    this->ebo_number_indices = num_indices;
    this->ebo_primitive = primitive;
    glGenBuffers(1,&(this->ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,ebo_bytes,ebo_data,GL_STATIC_DRAW);
}

void BasicShape::Draw (Shader shader)
{
    glUseProgram(shader.ID);
    this->Draw();

}

void BasicShape::DrawEBO (float line_width)
{
    if (this->ebo == 0)
    {
        std::cout<<"Attempted to draw ebo "<<this->ebo<<std::endl;
        return;
    }
    BindVAO(this->vao, this->vbo);
    glLineWidth(line_width);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ebo);
    glDrawElements(this->ebo_primitive,this->ebo_number_indices,GL_UNSIGNED_INT,0);
    glLineWidth(1.0);
}

void BasicShape::Draw ()
{
    //Assumes the shader has already been set (more efficient)
    BindVAO(this->vao,this->vbo,GL_ARRAY_BUFFER);
    glDrawArrays(this->primitive,0,this->number_vertices);

}

void BasicShape::DeallocateShape()
{
    glDeleteBuffers(1,&(this->vbo));
    if (this->ebo > 0)
        glDeleteBuffers(1,&(this->ebo));
}

unsigned int BasicShape::GetVBO() {
    return this->vbo;
}

void BasicShape::CalculateBounds(float* vertices, int num_vertices, int stride) {
    // Reset bounds to extreme values
    min_bounds = glm::vec3(std::numeric_limits<float>::max());
    max_bounds = glm::vec3(-std::numeric_limits<float>::max());
    
    // Calculate stride in floats (assuming stride is given in bytes)
    int stride_floats = stride / sizeof(float);
    
    // Iterate through all vertices and find min/max
    for (int i = 0; i < num_vertices; i++) {
        // Get position (assuming first 3 values are x,y,z)
        glm::vec3 pos(
            vertices[i * stride_floats],
            vertices[i * stride_floats + 1],
            vertices[i * stride_floats + 2]
        );
        
        // Update bounds
        min_bounds.x = std::min(min_bounds.x, pos.x);
        min_bounds.y = std::min(min_bounds.y, pos.y);
        min_bounds.z = std::min(min_bounds.z, pos.z);
        
        max_bounds.x = std::max(max_bounds.x, pos.x);
        max_bounds.y = std::max(max_bounds.y, pos.y);
        max_bounds.z = std::max(max_bounds.z, pos.z);
    }
    
    // Calculate dimensions
    dimensions = max_bounds - min_bounds;
}

glm::vec3 BasicShape::GetDimensions() const {
    return dimensions;
}

glm::vec3 BasicShape::GetMinBounds() const {
    return min_bounds;
}

glm::vec3 BasicShape::GetMaxBounds() const {
    return max_bounds;
}