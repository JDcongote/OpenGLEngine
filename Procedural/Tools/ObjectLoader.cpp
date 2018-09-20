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
		points = new GLfloat[size];
		for (int i = 0; i < *point_count; i++)
		{
			const aiVector3D vp = mesh->mVertices[i];
			points[i * 3] = (GLfloat)vp.x;
			points[i * 3 + 1] = (GLfloat)vp.y;
			points[i * 3 + 2] = (GLfloat)vp.z;
		}
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		if (mesh->HasNormals()) {
			
		}
		else {
			std::vector<GLfloat> normals = calculate_normals(points, *point_count);
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);
			//delete[] &normals;
		}
		if (mesh->HasTangentsAndBitangents()) {
			const int size = (*point_count * 3 * sizeof(GLfloat));
			const int size_4 = (*point_count * 4 * sizeof(GLfloat));
			tangents = new GLfloat[size_4];
			bitangents = new GLfloat[size];
			normals = new GLfloat[size];
			Vector3f curr_normal;
			Vector3f curr_tangent;
			Vector3f curr_bitangent;
			for (int i = 0; i < *point_count; i++)
			{
				const aiVector3D tangent = mesh->mTangents[i];
				const aiVector3D bitangent = mesh->mBitangents[i];
				const aiVector3D normal = mesh->mNormals[i];

				curr_tangent = Vector3f(tangent.x, tangent.y, tangent.z);
				curr_bitangent = Vector3f(bitangent.x, bitangent.y, bitangent.z);
				curr_normal = Vector3f(normal.x, normal.y, normal.z);

				Vector3f final_tangent = curr_tangent - curr_normal * curr_normal.dot(curr_tangent);
				final_tangent = final_tangent.normalize();

				bitangents[i * 3] = (GLfloat)bitangent.x;
				bitangents[i * 3 + 1] = (GLfloat)bitangent.y;
				bitangents[i * 3 + 2] = (GLfloat)bitangent.z;

				normals[i * 3] = (GLfloat)normal.x;
				normals[i * 3 + 1] = (GLfloat)normal.y;
				normals[i * 3 + 2] = (GLfloat)normal.z;

				tangents[i * 3] = (GLfloat)curr_tangent.x;
				tangents[i * 3 + 1] = (GLfloat)curr_tangent.y;
				tangents[i * 3 + 2] = (GLfloat)curr_tangent.z;

				float determinant = (curr_normal *curr_tangent).dot(curr_bitangent);
				if (determinant < 0.0f) {
					determinant = -1.0f;
				}
				else {
					determinant = 1.0f;
				}
				tangents[i * 3 + 3] = (GLfloat)determinant;

			}
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, size, normals, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);
			delete[] normals;

			GLuint vbo2;
			glGenBuffers(1, &vbo2);
			glBindBuffer(GL_ARRAY_BUFFER, vbo2);
			glBufferData(GL_ARRAY_BUFFER, size, tangents, GL_STATIC_DRAW);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(3);
			delete[] tangents;			
			delete[] bitangents;
		}
		delete[] points;
	}
	
	if (mesh->HasTextureCoords(0)) {
		const int size = (*point_count * 3 * sizeof(GLfloat));
		texcoords = new GLfloat[size];
		for (int i = 0; i < *point_count; i++)
		{
			const aiVector3D vp = mesh->mTextureCoords[0][i];
			texcoords[i * 3] = (GLfloat)vp.x;
			texcoords[i * 3 + 1] = (GLfloat)vp.y;
			texcoords[i * 3 + 2] = (GLfloat)vp.z;
		}
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, texcoords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);
		delete[] texcoords;
	}
	aiReleaseImport(scene);
	return true;
}

ObjectLoader::~ObjectLoader()
{
}
