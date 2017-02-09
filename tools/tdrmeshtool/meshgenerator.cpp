#include "meshgenerator.h"

static const float cube_verts[] = {
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

static const float cube_colors[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

MeshGenerator::MeshGenerator(void)
{
    txml::XMLElement* scene;
    txml::XMLElement* mesh;
    txml::XMLElement* shaders;
    txml::XMLElement* shader[2];
    txml::XMLElement* colors;
    txml::XMLElement* vertices;

    std::vector<float> bdata;
    std::string bname;
    int count;

    m_doc = std::unique_ptr<txml::XMLDocument>(new txml::XMLDocument());

    /* the root element is the 'scene' tag. */
    scene = m_doc->NewElement("scene");

    /* Under that is the mesh tag, of which there can be multiple. */
    mesh = m_doc->NewElement("mesh");
    mesh->SetAttribute("name", "cube");

    /* the vertices tag */
    count = sizeof(cube_verts) / sizeof(float);
    vertices = m_doc->NewElement("vertices");
    vertices->SetAttribute("count", count);
    vertices->SetAttribute("per", "3");
    vertices->InsertFirstChild(m_doc->NewText("cube.verts.bin"));

    /* the UVs tag */
    count = sizeof(cube_colors) / sizeof(float);
    colors = m_doc->NewElement("colors");
    colors->SetAttribute("count", count);
    colors->SetAttribute("per", 3);
    colors->InsertFirstChild(m_doc->NewText("cube.colors.bin"));

    shaders = m_doc->NewElement("shaders");

    /* set up the shader elements */
    shader[0] = m_doc->NewElement("shader");
    shader[0]->SetAttribute("type", "vertex");
    shader[0]->InsertFirstChild(m_doc->NewText("vshader.vs"));

    shader[1] = m_doc->NewElement("shader");
    shader[1]->SetAttribute("type", "fragment");
    shader[1]->InsertFirstChild(m_doc->NewText("fshader.fs"));

    /* insert the tags into the mesh.  There is nothing nested
    * below the mesh tage as of now. */
    shaders->InsertEndChild(shader[0]);
    shaders->InsertEndChild(shader[1]);

    mesh->InsertEndChild(vertices);
    mesh->InsertEndChild(colors);
    mesh->InsertEndChild(colors);
    mesh->InsertEndChild(shaders);

    /* insert the mesh into the scene, and the scene into the document. */
    scene->InsertEndChild(mesh);
    m_doc->InsertEndChild(scene);

    /* now to save the binary data to the names referenced in the mesh. */
    bdata.clear();
    bname = "cube.verts.bin";
    for (unsigned int i = 0; i < sizeof(cube_verts) / sizeof(float); i++) {
        bdata.push_back(cube_verts[i]);
    }

    /* push it into the data structures in the generator for later dumping. */
    m_bdata.push_back(bdata);
    m_bnames.push_back(bname);

    /* and again for the UVs. */
    bdata.clear();
    bname = "cube.colors.bin";
    for (unsigned int i = 0; i < sizeof(cube_colors) / sizeof(float); i++) {
        bdata.push_back(cube_colors[i]);
    }

    m_bdata.push_back(bdata);
    m_bnames.push_back(bname);
}

MeshGenerator::~MeshGenerator(void) { }

void MeshGenerator::dump(void)
{
    char* buff = nullptr;
    std::fstream out;

    txml::XMLPrinter* p = new txml::XMLPrinter();
    m_doc->Print(p);

    out.open("scene.xml", std::fstream::out);
    out << p->CStr();
    out.close();

    /* XXX: Note that the data is always saved in the same format as
    * the current architecture.  I don't think this would be a huge
    * deal to correct later.  A char[4] to hold 32-bit floats and then
    * re-arrange.  It'll take a little longer to export and import on
    * a different architecture, but would greatly improve speed on the
    * target. */
    for (unsigned int i = 0; i < m_bdata.size(); i++) {
        buff = reinterpret_cast<char*>(m_bdata[i].data());
        out.open(m_bnames[i].c_str(), std::fstream::out |
            std::fstream::binary);
        out.write(buff, sizeof(float) * m_bdata[i].size());
        out.close();
    }

    delete(p);
}
