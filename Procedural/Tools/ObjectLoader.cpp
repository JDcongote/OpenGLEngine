#include "ObjectLoader.h"



ObjectLoader::ObjectLoader()
{
}

//refactor this when moving code to objects
std::vector<GLfloat> ObjectLoader::calculate_normals(const float* points, const int size) {
	std::vector<Vector3f> vec_array(size / 3);
	std::vector<Vector3f> normals;
	std::vector<GLfloat> final_normals(size);

	for (int i = 0; i < size / 3; i++) {
		Vector3f vec = Vector3f(points[i * 3], points[i * 3 + 1], points[i * 3 + 2]);
		vec_array[i] = vec;
	}
	//not the best way but its only for development purposes
	int limit = (size / 3) - 2; //loop run once per triangle, two triangles per face
	for (int i = 0; i < limit; i += 3)
	{
		Vector3f p0 = Vector3f(points[i * 3], points[i * 3 + 1], points[i * 3 + 2]);
		Vector3f p1 = Vector3f(points[i * 3 + 3], points[i * 3 + 4], points[i * 3 + 5]);
		Vector3f p2 = Vector3f(points[i * 3 + 6], points[i * 3 + 7], points[i * 3 + 8]);

		Vector3f edge_1 = (p1 - p2).normalize();
		Vector3f edge_2 = (p2 - p0).normalize();
		Vector3f normal = edge_1 * edge_2;
		normals.push_back(normal);
	}
	int j = 0;
	limit = 9;
	for (int i = 0; i < normals.size(); i++) {
		for (j; j < limit; j += 3) {
			final_normals[j] = normals[i].x * -1;
			final_normals[j + 1] = normals[i].y * -1;
			final_normals[j + 2] = normals[i].z * -1;
		}
		limit += 9;
	}
	return final_normals;
}


bool ObjectLoader::load_mesh(std::string file_name, GLuint* vao, int* point_count) {
	file_name = "Resources/Meshes/" + file_name;
	const aiScene* scene = aiImportFile(file_name.c_str(), aiProcess_CalcTangentSpace);
	const aiMesh* mesh = scene->mMeshes[0];
	*point_count = mesh->mNumVertices;
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	GLfloat *points;
	GLfloat *texcoords;

	GLfloat *normals;
	GLfloat *tangents;
	GLfloat *bitangents;	

	if (mesh->HasPositions()) {
		const int size = (*point_count * 3 * sizeof(GLfloat));
		const int size_tan = (*point_count * 4 * sizeof(GLfloat));
		points = new GLfloat[size];
		texcoords = new GLfloat[size];
		normals = new GLfloat[size];
		tangents = new GLfloat[size_tan];
		bitangents = new GLfloat[size];
		for (int i = 0; i < *point_count; i++)
		{
			const aiVector3D vp = mesh->mVertices[i];
			points[i * 3] = (GLfloat)vp.x;
			points[i * 3 + 1] = (GLfloat)vp.y;
			points[i * 3 + 2] = (GLfloat)vp.z;

			if (mesh->HasTextureCoords(0)) {
				const aiVector3D vt = mesh->mTextureCoords[0][i];
				texcoords[i * 3] = (GLfloat)vt.x;
				texcoords[i * 3 + 1] = (GLfloat)vt.y;
				texcoords[i * 3 + 2] = (GLfloat)vt.z;
			}

			const aiVector3D vn = mesh->mNormals[i];
			normals[i * 3] = (GLfloat)vn.x;
			normals[i * 3 + 1] = (GLfloat)vn.y;
			normals[i * 3 + 2] = (GLfloat)vn.z;
			const aiVector3D tn = mesh->mTangents[i];
			
			const aiVector3D bt = mesh->mTangents[i];
			bitangents[i * 3] = (GLfloat)tn.x;
			bitangents[i * 3 + 1] = (GLfloat)tn.y;
			bitangents[i * 3 + 2] = (GLfloat)tn.z;

			Vector3f curr_tangent = Vector3f(tn.x, tn.y, tn.z);
			Vector3f curr_bitangent = Vector3f(bt.x, bt.y, bt.z);
			Vector3f curr_normal = Vector3f(vn.x, vn.y, vn.z);
			Vector3f final_tangent = curr_tangent - curr_normal * curr_normal.dot(curr_tangent);
			final_tangent = final_tangent.normalize();

			tangents[i * 4] = (GLfloat)final_tangent.x;
			tangents[i * 4 + 1] = (GLfloat)final_tangent.y;
			tangents[i * 4 + 2] = (GLfloat)final_tangent.z;

			float determinant = (curr_normal *curr_tangent).dot(curr_bitangent);
			if (determinant < 0.0f) {
				determinant = -1.0f;
			}
			else {
				determinant = 1.0f;
			}
			tangents[i * 4 + 3] = (GLfloat)determinant;
		}
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, normals, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, texcoords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);		
		
		//tangents = calculate_tangents(normals, points, texcoords, *point_count);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size_tan, tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete[] normals;
		delete[] tangents;
		delete[] texcoords;
		delete[] points;
	}
	aiReleaseImport(scene);
	return true;
}

GLfloat* ObjectLoader::calculate_tangents(GLfloat *normals, GLfloat *points, GLfloat *texcoords, int point_count)
{
	GLfloat *tangents = new GLfloat[point_count* 3 *sizeof(GLfloat)];
	GLfloat *bitangents = new GLfloat[point_count* 3 * sizeof(GLfloat)];
	for (int i = 0; i < point_count; i+=3)
	{
		Vector3f v0 = Vector3f(points[i * 3], points[i * 3 + 1], points[i * 3 + 2]);
		Vector3f v1 = Vector3f(points[i * 3 + 3], points[i * 3 + 4], points[i * 3 + 5]);
		Vector3f v2 = Vector3f(points[i * 3 + 6], points[i * 3 + 7], points[i * 3 + 8]);

		Vector3f st0 = Vector3f(texcoords[i * 3], texcoords[i * 3 + 1], texcoords[i * 3 + 2]);
		Vector3f st1 = Vector3f(texcoords[i * 3 + 3], texcoords[i * 3 + 4], texcoords[i * 3 + 5]);
		Vector3f st2 = Vector3f(texcoords[i * 3 + 6], texcoords[i * 3 + 7], texcoords[i * 3 + 8]);

		Vector3f edge1 = v1 - v0;
		Vector3f edge2 = v2 - v0;

		Vector3f edge_st1 = st1 - st0;
		Vector3f edge_st2 = st2 - st0;

		float r = 1.0f / (edge_st1.x * edge_st2.y - edge_st1.y * edge_st2.x);
		Vector3f tangent = (edge1 * edge_st2.y - edge2 * edge_st1.y)*r;
		Vector3f bitangent = (edge2 * edge_st1.x - edge1 * edge_st2.x)*r;

		tangents[i] = tangent.x;
		tangents[i+1] = tangent.y;
		tangents[i+2] = tangent.z;

		bitangents[i] = bitangent.x;
		bitangents[i + 1] = bitangent.y;
		bitangents[i + 2] = bitangent.z;
	}
	return tangents;
}


ObjectLoader::~ObjectLoader()
{
}
