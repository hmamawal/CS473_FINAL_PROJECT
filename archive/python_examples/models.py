#!/usr/bin/env python3

from panda3d.core import GeomNode, Geom, GeomVertexFormat, GeomVertexData
from panda3d.core import GeomTriangles, GeomVertexWriter, NodePath
from panda3d.core import Vec3

def create_box():
    """Create a simple box model that can be used when no external models are available."""
    format = GeomVertexFormat.getV3n3c4()
    vdata = GeomVertexData('box', format, Geom.UHStatic)
    
    vertex = GeomVertexWriter(vdata, 'vertex')
    normal = GeomVertexWriter(vdata, 'normal')
    color = GeomVertexWriter(vdata, 'color')
    
    # Define the vertices
    # Bottom face
    vertex.addData3f(-1, -1, -1)  # 0
    vertex.addData3f(1, -1, -1)   # 1
    vertex.addData3f(1, 1, -1)    # 2
    vertex.addData3f(-1, 1, -1)   # 3
    
    # Top face
    vertex.addData3f(-1, -1, 1)   # 4
    vertex.addData3f(1, -1, 1)    # 5
    vertex.addData3f(1, 1, 1)     # 6
    vertex.addData3f(-1, 1, 1)    # 7
    
    # Define the normals for each vertex
    # Bottom face - normal points down
    for i in range(4):
        normal.addData3f(0, 0, -1)
    
    # Top face - normal points up
    for i in range(4):
        normal.addData3f(0, 0, 1)
    
    # Define colors for vertices (white)
    for i in range(8):
        color.addData4f(1, 1, 1, 1)
    
    # Create the triangle primitives
    tris = GeomTriangles(Geom.UHStatic)
    
    # Bottom face
    tris.addVertices(0, 1, 2)
    tris.addVertices(0, 2, 3)
    
    # Top face
    tris.addVertices(4, 6, 5)
    tris.addVertices(4, 7, 6)
    
    # Connect the side faces
    # Front face
    tris.addVertices(0, 4, 5)
    tris.addVertices(0, 5, 1)
    
    # Right face
    tris.addVertices(1, 5, 6)
    tris.addVertices(1, 6, 2)
    
    # Back face
    tris.addVertices(2, 6, 7)
    tris.addVertices(2, 7, 3)
    
    # Left face
    tris.addVertices(3, 7, 4)
    tris.addVertices(3, 4, 0)
    
    # Create the geometry
    geom = Geom(vdata)
    geom.addPrimitive(tris)
    
    # Create the node
    node = GeomNode('box')
    node.addGeom(geom)
    
    return NodePath(node)

def create_sphere(radius=1.0, segments=8):
    """Create a simple sphere model."""
    format = GeomVertexFormat.getV3n3c4()
    vdata = GeomVertexData('sphere', format, Geom.UHStatic)
    
    vertex = GeomVertexWriter(vdata, 'vertex')
    normal = GeomVertexWriter(vdata, 'normal')
    color = GeomVertexWriter(vdata, 'color')
    
    # Create vertices for the sphere
    for i in range(segments + 1):
        v = i / segments
        phi = v * math.pi
        
        for j in range(segments * 2 + 1):
            u = j / (segments * 2)
            theta = u * 2.0 * math.pi
            
            x = radius * math.sin(phi) * math.cos(theta)
            y = radius * math.sin(phi) * math.sin(theta)
            z = radius * math.cos(phi)
            
            # Add vertex
            vertex.addData3f(x, y, z)
            
            # Add normal (normalized vertex position)
            normal_vec = Vec3(x, y, z)
            if normal_vec.length() > 0:
                normal_vec.normalize()
                normal.addData3f(normal_vec)
            else:
                normal.addData3f(0, 0, 1)
            
            # Add color (white)
            color.addData4f(1, 1, 1, 1)
    
    # Create triangles
    tris = GeomTriangles(Geom.UHStatic)
    
    # Vertex count per row
    vpr = segments * 2 + 1
    
    for i in range(segments):
        for j in range(segments * 2):
            # Calculate indices
            p1 = i * vpr + j
            p2 = p1 + 1
            p3 = (i + 1) * vpr + j
            p4 = p3 + 1
            
            # Add triangles - make sure indices are valid
            if p1 < vdata.getNumRows() and p2 < vdata.getNumRows() and p4 < vdata.getNumRows():
                tris.addVertices(p1, p2, p4)
            
            if p1 < vdata.getNumRows() and p4 < vdata.getNumRows() and p3 < vdata.getNumRows():
                tris.addVertices(p1, p4, p3)
    
    # Create the geometry
    geom = Geom(vdata)
    geom.addPrimitive(tris)
    
    # Create the node
    node = GeomNode('sphere')
    node.addGeom(geom)
    
    return NodePath(node)

def create_cylinder(radius=1.0, height=2.0, segments=16):
    """Create a simple cylinder model."""
    format = GeomVertexFormat.getV3n3c4()
    vdata = GeomVertexData('cylinder', format, Geom.UHStatic)
    
    vertex = GeomVertexWriter(vdata, 'vertex')
    normal = GeomVertexWriter(vdata, 'normal')
    color = GeomVertexWriter(vdata, 'color')
    
    # Create top and bottom center vertices
    vertex.addData3f(0, 0, height/2)  # Top center
    normal.addData3f(0, 0, 1)
    color.addData4f(1, 1, 1, 1)
    
    vertex.addData3f(0, 0, -height/2)  # Bottom center
    normal.addData3f(0, 0, -1)
    color.addData4f(1, 1, 1, 1)
    
    # Create vertices for the top circle
    for i in range(segments):
        angle = i * 2.0 * 3.1415926535897931 / segments
        x = radius * math.cos(angle)
        y = radius * math.sin(angle)
        
        # Top circle vertex
        vertex.addData3f(x, y, height/2)
        normal.addData3f(0, 0, 1)  # Normal points up
        color.addData4f(1, 1, 1, 1)
    
    # Create vertices for the bottom circle
    for i in range(segments):
        angle = i * 2.0 * 3.1415926535897931 / segments
        x = radius * math.cos(angle)
        y = radius * math.sin(angle)
        
        # Bottom circle vertex
        vertex.addData3f(x, y, -height/2)
        normal.addData3f(0, 0, -1)  # Normal points down
        color.addData4f(1, 1, 1, 1)
    
    # Create vertices for the side
    for i in range(segments):
        angle = i * 2.0 * 3.1415926535897931 / segments
        x = radius * math.cos(angle)
        y = radius * math.sin(angle)
        nx = math.cos(angle)
        ny = math.sin(angle)
        
        # Side vertices (top and bottom of each quad)
        vertex.addData3f(x, y, height/2)
        normal.addData3f(nx, ny, 0)  # Normal points outward
        color.addData4f(1, 1, 1, 1)
        
        vertex.addData3f(x, y, -height/2)
        normal.addData3f(nx, ny, 0)  # Normal points outward
        color.addData4f(1, 1, 1, 1)
    
    # Create triangles
    tris = GeomTriangles(Geom.UHStatic)
    
    # Top face triangles
    for i in range(segments):
        tris.addVertices(0, 2 + i, 2 + ((i + 1) % segments))
    
    # Bottom face triangles
    for i in range(segments):
        tris.addVertices(1, 2 + segments + ((i + 1) % segments), 2 + segments + i)
    
    # Side triangles
    for i in range(segments):
        v1 = 2 + 2*segments + 2*i
        v2 = v1 + 1
        v3 = 2 + 2*segments + 2*((i + 1) % segments)
        v4 = v3 + 1
        
        tris.addVertices(v1, v3, v4)
        tris.addVertices(v1, v4, v2)
    
    # Create the geometry
    geom = Geom(vdata)
    geom.addPrimitive(tris)
    
    # Create the node
    node = GeomNode('cylinder')
    node.addGeom(geom)
    
    return NodePath(node)

# Import math for sphere and cylinder creation
import math