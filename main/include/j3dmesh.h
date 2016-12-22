#ifndef TDRMAIN_J3DMESH_H
#define TDRMAIN_J3DMESH_H

#include <memory>
#include <vector>

class J3DMesh {
public:
	J3DMesh(void) { };
	virtual ~J3DMesh(void) { };
	static void Delete(std::shared_ptr<J3DMesh> mesh);
	std::shared_ptr<J3DMesh> Load(const char* buff, size_t len);
private:
	std::vector<float> m_colordata;
	std::vector<float> m_indexdata;
	std::vector<float> m_uvdata;
	std::vector<float> m_vertexdata;
};

#endif
