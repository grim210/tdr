#ifndef TDRMAIN_TDRMESH_H
#define TDRMAIN_TDRMESH_H

#include <memory>
#include <vector>

class TDRMesh {
public:
	TDRMesh(void) { };
	virtual ~TDRMesh(void) { };
	static void Delete(std::shared_ptr<TDRMesh> mesh);
	std::shared_ptr<TDRMesh> Load(const char* buff, size_t len);
private:
	std::vector<float> m_colordata;
	std::vector<float> m_indexdata;
	std::vector<float> m_uvdata;
	std::vector<float> m_vertexdata;
};

#endif  /* TDRMAIN_TDRMESH_H */
