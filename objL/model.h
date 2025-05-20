#ifndef MODEL_H
#define MODEL_H

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "GL/freeglut.h"
#include "GL/glext.h"

#include "Library/loadpng.h"
#include "Library/process_image.h"
#include "Library/gl_texture.h"

#include "material.h"
#include "face.h"

#include <thread>
#include <chrono> 
#include <math.h>

using namespace std;

class Model {
    public:
    static int count_char(string &str, char ch) {
        int c = 0;
        int length = str.length() - 1;
        for (int i = 0; i < length; i++) {
            if (str[i] == ch)
                c++;
        }
        return c;
    }

    static bool has_double_slash(string &str) {
        int length = str.length() - 2;
        for (int i = 0; i < length; i++) {
            if (str[i] == '/' && str[i + 1] == '/')
                return true;
        }
        return false;
    }

    string prefix;
    vector<Material> materials;
    map<std::string, int> map_material;

    vector<array<float, 3>> sorted_vertices;
    vector<array<float, 3>> vertices;
    vector<array<float, 2>> texcoords;
    vector<array<float, 3>> normals;
    vector<Face> faces;

    GLuint list;
  
    void load_material(const char *filename) {
        string line;
        vector<std::string> lines;
        ifstream in(filename);

        if (!in.is_open()) {
            printf("Cannot load material %s\n", filename);
            return;
        }

        while (!in.eof()) {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();

        Material *m;
        int count_material = 0;
        char str[40];
        string material_name;
        float *a, *d, *s;

        for (std::string &line : lines) {
            if (line[0] == 'n' && line[1] == 'e') {
                sscanf(line.c_str(), "newmtl %s", str);
                material_name = str;
                map_material[material_name] = count_material;
                count_material++;
                a = new float[4]{0.2f, 0.2f, 0.2f, 1.0f};
                d = new float[4]{0.8f, 0.8f, 0.8f, 1.0f};
                s = new float[4]{0.0f, 0.0f, 0.0f, 1.0f};
                materials.push_back(Material(material_name,a, d, s));
                m = &materials[materials.size() - 1];
            } else if (line[0] == 'K') {
                switch (line[1]) {
                case 'a':
                    sscanf(line.c_str(), "Ka %f %f %f", &a[0], &a[1], &a[2]);
                    break;
                case 'd':
                    sscanf(line.c_str(), "Kd %f %f %f", &d[0], &d[1], &d[2]);
                    break;
                case 's':
                    sscanf(line.c_str(), "Ks %f %f %f", &s[0], &s[1], &s[2]);
                    break;
                }
            } else if (line[0] == 'm' && line[1] == 'a') {
                sscanf(line.c_str(), "map_Kd %s", str);
                string file = prefix + str;
                Image img;
                Load_Texture_Swap(&img, file.c_str());
                glGenTextures(1, &(m->texture));
                glBindTexture(GL_TEXTURE_2D, m->texture);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.w, img.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.img);
                glBindTexture(GL_TEXTURE_2D, 0);
                Delete_Image(&img);
            }
        }
    }
  
    void add_face_3v(std::string &line) {
        int v0, v1, v2;
        sscanf(line.c_str(), "f %d %d %d", &v0, &v1, &v2);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        faces.push_back(Face(3, v, NULL));
    }
  
    void add_face_3vt(std::string &line) {
        int v0, v1, v2, t0, t1, t2;
        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
        faces.push_back(Face(3, v, t));
    }
  
    void add_face_3vn(std::string &line) {
        int v0, v1, v2, n;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        faces.push_back(Face(3, v, NULL, n - 1));
    }
  
    void add_face_3vtn(std::string &line) {
        int v0, v1, v2, t0, t1, t2, n;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
        faces.push_back(Face(3, v, t, n - 1));
    }
  
    void add_face_4v(std::string &line) {
        int v0, v1, v2, v3;
        sscanf(line.c_str(), "f %d %d %d %d", &v0, &v1, &v2, &v3);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        faces.push_back(Face(4, v, NULL));
    }

    void add_face_4vt(std::string &line) {
        int v0, v1, v2, v3, t0, t1, t2, t3;
        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2, &v3, &t3);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
        faces.push_back(Face(4, v, t));
    }

    void add_face_4vn(std::string &line) {
        int v0, v1, v2, v3, n;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n, &v3, &n);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        faces.push_back(Face(4, v, NULL, n - 1));
    }

    void add_face_4vtn(std::string &line) {
        int v0, v1, v2, v3, t0, t1, t2, t3, n;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n, &v3,
                &t3, &n);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
        faces.push_back(Face(4, v, t, n - 1));
    }
  
    public:
    float pos_x, pos_y, pos_z;

    float min_x, min_y, min_z;
    float max_x, max_y, max_z;

    void load(const char *filename) {
        string tmp = filename;
        prefix = "";
        int n = tmp.find_last_of('/') + 1;
        if (n > 0)
            prefix = tmp.substr(0, n);

        string line;
        vector<std::string> lines;
        ifstream in(filename);
        if (!in.is_open()) {
            printf("Cannot load model %s\n", filename);
            return;
        }

        while (!in.eof()) {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();

        float a, b, c;
        char str[40];

        pos_x = pos_y = 0.0f;

        float sum_x = 0.0f, sum_y = 0.0f, sum_z = 0.0f;

        for (std::string &line : lines) {
            if (line[0] == 'v') {
                if (line[1] == ' ') {
                    sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
                    if (a > 0.0f)
                        sum_x += a;
                    else
                        sum_x -= a;
                    if (b > 0.0f)
                        sum_y += b;
                    else
                        sum_y -= b;
                    if (c > 0.0f)
                        sum_z += c;
                    else
                        sum_z -= c;
                    pos_x += a;
                    pos_y += b;

                    vertices.push_back({a, b, c});

                } else if (line[1] == 't') {
                    sscanf(line.c_str(), "vt %f %f", &a, &b);
                    texcoords.push_back({a, b});
                } else {
                    sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
                    normals.push_back({a, b, c});
                }
            } else if (line[0] == 'f') {
                int edge = count_char(line, ' ');
                int count_slash = count_char(line, '/');
                if (count_slash == 0) {
                    if (edge == 3)
                        add_face_3v(line);
                    else
                        add_face_4v(line);
                } else if (count_slash == edge) {
                    if (edge == 3)
                        add_face_3vt(line);
                    else
                        add_face_4vt(line);
                } else if (count_slash == edge * 2) {
                    if (has_double_slash(line)) {
                        if (edge == 3)
                            add_face_3vn(line);
                        else
                            add_face_4vn(line);
                    } else {
                        if (edge == 3)
                            add_face_3vtn(line);
                        else
                            add_face_4vtn(line);
                    }
                }
            } else if (line[0] == 'm' && line[1] == 't') {
                sscanf(line.c_str(), "mtllib %s", &str);
                string file = prefix + str;
                load_material(file.c_str());
            } else if (line[0] == 'u' && line[1] == 's') {
                sscanf(line.c_str(), "usemtl %s", &str);
                string material = str;
                if (map_material.find(material) != map_material.end())
                    faces.push_back(Face(-1, NULL, NULL, map_material[material]));
            }
        }

        bool has_texcoord = false;
int current_material = -1;

list = glGenLists(1);
glNewList(list, GL_COMPILE);
for (Face &face : faces) {
    if (face.edge == -1) {  // Material change
        current_material = face.normal;
        if (current_material >= 0 && current_material < materials.size()) {
            const Material &mat = materials[current_material];
            
            // Set material properties
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat.ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat.specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 50.0f);
            
            // Handle texture
            has_texcoord = (mat.texture != 0);
            if (has_texcoord) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, mat.texture);
            } else {
                glDisable(GL_TEXTURE_2D);
            }
        }
        continue;
    }

    // Set normal if available
    // if (face.normal != -1 && face.normal < normals.size()) {
        glNormal3fv(normals[face.normal].data());
    // }

    // Draw the face
    glBegin(GL_POLYGON);
    for (int i = 0; i < face.edge; i++) {
        if (has_texcoord && face.texcoords != nullptr && i < face.edge) {
            int tex_idx = face.texcoords[i];
            if (tex_idx >= 0 && tex_idx < texcoords.size()) {
                glTexCoord2fv(texcoords[tex_idx].data());
            }
        }
        if (face.vertices[i] >= 0 && face.vertices[i] < vertices.size()) {
            glVertex3fv(vertices[face.vertices[i]].data());
        }
    }
    glEnd();
}
glDisable(GL_TEXTURE_2D); // Clean up
glEndList();

        printf("Model: %s\n", filename);
        printf("Vertices: %d\n", vertices.size());
        printf("Texcoords: %d\n", texcoords.size());
        printf("Normals: %d\n", normals.size());
        printf("Faces: %d\n", faces.size());
        printf("Materials: %d\n", materials.size());

        sum_x /= vertices.size();
        sum_y /= vertices.size();
        sum_z /= vertices.size();
        pos_x /= vertices.size();
        pos_x = -pos_x;
        pos_y /= vertices.size();
        pos_y = -pos_y;
        pos_z = -sqrt(sum_x * sum_x + sum_y * sum_y + sum_z * sum_z) * 15;

        printf("Pos_X: %f\n", pos_x);
        printf("Pos_Y: %f\n", pos_y);
        printf("Pos_Z: %f\n", pos_z);

        // for (size_t i = 0; i < vertices.size(); ++i) {
        //     printf("%f %f %f\n", vertices[i][0], vertices[i][1], vertices[i][2]);
        // }

        min_x = min_y = min_z = FLT_MAX;
        max_x = max_y = max_z = -FLT_MAX;

        for (const auto& v : vertices) {
            if (v[0] < min_x) min_x = v[0];
            if (v[1] < min_y) min_y = v[1];
            if (v[2] < min_z) min_z = v[2];
            if (v[0] > max_x) max_x = v[0];
            if (v[1] > max_y) max_y = v[1];
            if (v[2] > max_z) max_z = v[2];
        }

        for (Material &material : materials) {
            delete material.ambient;
            delete material.diffuse;
            delete material.specular;
        }
    }

    void unload()
    {
        for (Material &material : materials) {
            glDeleteTextures(1, &(material.texture));
        }
        materials.clear();
        map_material.clear();
        sorted_vertices.clear();
        vertices.clear();
        texcoords.clear();
        normals.clear();
        faces.clear();
    }

    void draw() { glCallList(list); }

    bool check_collision(float x, float y, float z,
                     float model_translate_x,
                     float model_translate_y,
                     float model_translate_z,
                     float model_scale = 1.0f) {
    // Transform local bounding box to world space
    float world_min_x = model_translate_x + min_x * model_scale;
    float world_max_x = model_translate_x + max_x * model_scale;
    float world_min_y = model_translate_y + min_y * model_scale;
    float world_max_y = model_translate_y + max_y * model_scale;
    float world_min_z = model_translate_z + min_z * model_scale;
    float world_max_z = model_translate_z + max_z * model_scale;

    // Now compare with world-space coordinates
    return x >= world_min_x && x <= world_max_x &&
           y >= world_min_y && y <= world_max_y &&
           z >= world_min_z && z <= world_max_z;
}

};
#endif