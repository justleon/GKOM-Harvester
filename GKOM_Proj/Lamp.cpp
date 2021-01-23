#include "headers/Lamp.h"

Lamp::Lamp(GLfloat size, Transformation transMat)
	: ObjectPrimitive(transMat), size(size)
{
	if (size <= 0.0f)
		std::cout << "Dimension can't be <= 0!" << std::endl;
	else
	{
		this->size = size;
		initObject();
	}
}

void Lamp::draw(ShaderProgram shader)
{
	useModel();

	shader.Use();
	shader.setUniformMat4("model", modelMat);

	vertexArray->Bind();
	indexBuffer->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	vertexArray->Unbind();
	indexBuffer->Unbind();
}

std::unique_ptr<VertexBuffer> Lamp::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[] = {
		// coordinates														// color			// texture
		0.0f,		0.0f,	halfSize,						0.0f, 0.0f, 1.0f,	1.0f,  1.0f, //sphere north pole

		0.0f,		0.5f * halfSize,	0.866f * halfSize,					0.0f, 0.0f, 1.0f,	1.0f,  0.0f, //second ring above center
		0.25f * halfSize,	0.433f * halfSize,  0.866f * halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		0.433f * halfSize,	0.25f * halfSize,  0.866f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		0.5f * halfSize,	0.0f,  0.866f * halfSize,						0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		0.433f * halfSize,	-0.25f * halfSize,	0.866f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		0.25f * halfSize,	-0.433f * halfSize,	0.866f * halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		0.0f,	-0.5f * halfSize,	0.866f * halfSize,						1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-0.25f * halfSize,	-0.433f * halfSize,	0.866f * halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		-0.433f * halfSize, -0.25f * halfSize,  0.866f * halfSize,			1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-0.5f * halfSize,	0.0f,	0.866f * halfSize,						1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		-0.433f * halfSize,	0.25f * halfSize,	0.866f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		-0.25f * halfSize,  0.433f * halfSize,  0.866f * halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  0.0f,

		0.0f,		0.866f * halfSize,	0.5f * halfSize,					0.0f, 0.0f, 1.0f,	1.0f,  1.0f, //first ring above center
		0.433f * halfSize,	0.75f * halfSize,  0.5f * halfSize,				1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		0.75f * halfSize,	0.433f * halfSize,  0.5f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		0.866f * halfSize,	0.0f,  0.5f * halfSize,							0.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		0.75f * halfSize,	-0.433f * halfSize,	0.5f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		0.433f * halfSize,	-0.75f * halfSize,	0.5f * halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		0.0f,	-0.866f * halfSize,	0.5f * halfSize,						1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		-0.433f * halfSize,	-0.75f * halfSize,	0.5f * halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		-0.75f * halfSize, -0.433f * halfSize,  0.5f * halfSize,			1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		-0.866f * halfSize,	0.0f,	0.5f * halfSize,						1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		-0.75f * halfSize,	0.433f * halfSize,	0.5f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		-0.433f * halfSize,  0.75f * halfSize,  0.5f * halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  1.0f,

		0.0f,		halfSize,	0.0f,										0.0f, 0.0f, 1.0f,	1.0f,  0.0f, // center ring
		0.5f * halfSize,	0.866f * halfSize,  0.0f,						1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		0.866f * halfSize,	0.5f * halfSize,  0.0f,							0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		halfSize,	0.0f,  0.0f,											0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		0.866f * halfSize,	-0.5f * halfSize,	0.0f,						0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		0.5f * halfSize,	-0.866f * halfSize,	0.0f,						0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		0.0f,	-halfSize,	0.0f,											1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-0.5f * halfSize,	-0.866f * halfSize,	0.0f,						1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		-0.866f * halfSize, -0.5f * halfSize,  0.0f,						1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-halfSize,	0.0f,	0.0f,											1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		-0.866f * halfSize,	0.5f * halfSize,	0.0f,						0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		-0.5f * halfSize,  0.866f * halfSize,  0.0f,						0.0f, 1.0f, 0.0f,	0.0f,  0.0f,

		0.0f,		0.866f * halfSize,	-0.5f * halfSize,					0.0f, 0.0f, 1.0f,	1.0f,  1.0f, //first ring below center
		0.433f * halfSize,	0.75f * halfSize,  -0.5f * halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		0.75f * halfSize,	0.433f * halfSize,  -0.5f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		0.866f * halfSize,	0.0f,  -0.5f * halfSize,						0.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		0.75f * halfSize,	-0.433f * halfSize,	-0.5f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		0.433f * halfSize,	-0.75f * halfSize,	-0.5f * halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		0.0f,	-0.866f * halfSize,	-0.5f * halfSize,						1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		-0.433f * halfSize,	-0.75f * halfSize,	-0.5f * halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		-0.75f * halfSize, -0.433f * halfSize,  -0.5f * halfSize,			1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		-0.866f * halfSize,	0.0f,	-0.5f * halfSize,						1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		-0.75f * halfSize,	0.433f * halfSize,	-0.5f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		-0.433f * halfSize,  0.75f * halfSize,  -0.5f * halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  1.0f,

		0.0f,		0.5f * halfSize,	-0.866f * halfSize,					0.0f, 0.0f, 1.0f,	1.0f,  0.0f, //second ring below center
		0.25f * halfSize,	0.433f * halfSize,  -0.866f * halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		0.433f * halfSize,	0.25f * halfSize,  -0.866f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		0.5f * halfSize,	0.0f,  -0.866f * halfSize,						0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		0.433f * halfSize,	-0.25f * halfSize,	-0.866f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		0.25f * halfSize,	-0.433f * halfSize,	-0.866f * halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		0.0f,	-0.5f * halfSize,	-0.866f * halfSize,						1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-0.25f * halfSize,	-0.433f * halfSize,	-0.866f * halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		-0.433f * halfSize, -0.25f * halfSize,  -0.866f * halfSize,			1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-0.5f * halfSize,	0.0f,	-0.866f * halfSize,						1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		-0.433f * halfSize,	0.25f * halfSize,	-0.866f * halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		-0.25f * halfSize,  0.433f * halfSize,  -0.866f * halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  0.0f,

		0.0f,		0.0f,	-halfSize,						0.0f, 0.0f, 1.0f,	1.0f,  1.0f, //sphere south pole
	};

	return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
}

std::unique_ptr<IndexBuffer> Lamp::initIndices()
{
	GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 5,
			0, 5, 6,
			0, 6, 7,
			0, 7, 8,
			0, 8, 9,
			0, 9, 10,
			0, 10, 11,
			0, 11, 12,
			0, 1, 12,

			13, 1, 2,
			14, 2, 3,
			15, 3, 4,
			16, 4, 5,
			17, 5, 6,
			18, 6, 7,
			19, 7, 8,
			20, 8, 9,
			21, 9, 10,
			22, 10, 11,
			23, 11, 12,
			24, 1, 12,

			2, 13, 14,
			3, 14, 15,
			4, 15, 16,
			5, 16, 17,
			6, 17, 18,
			7, 18, 19,
			8, 19, 20,
			9, 20, 21,
			10, 21, 22,
			11, 22, 23,
			12, 23, 24,
			1, 24, 13,

			25,26,13,
			26,27,14,
			27,28,15,
			28,29,16,
			29,30,17,
			30,31,18,
			31,32,19,
			32,33,20,
			33,34,21,
			34,35,22,
			35,36,23,
			36,25,24,

			13,14,26,
			14,15,27,
			15,16,28,
			16,17,29,
			17,18,30,
			18,19,31,
			19,20,32,
			20,21,33,
			21,22,34,
			22,23,35,
			23,24,36,
			24,13,25,

			25,26,37,
			26,27,38,
			27,28,39,
			28,29,40,
			29,30,41,
			30,31,42,
			31,32,43,
			32,33,44,
			33,34,45,
			34,35,46,
			35,36,47,
			36,25,48,

			37,38,26,
			38,39,27,
			39,40,28,
			40,41,29,
			41,42,30,
			42,43,31,
			43,44,32,
			44,45,33,
			45,46,34,
			46,47,35,
			47,48,36,
			48,37,25,

			37,38,49,
			38,39,50,
			39,40,51,
			40,41,52,
			41,42,53,
			42,43,54,
			43,44,55,
			44,45,56,
			45,46,57,
			46,47,58,
			47,48,59,
			48,37,60,

			49,50,38,
			50,51,39,
			51,52,40,
			52,53,41,
			53,54,42,
			54,55,43,
			55,56,44,
			56,57,45,
			57,58,46,
			58,59,47,
			59,60,48,
			60,49,37,

			49,50,61,
			50,51,61,
			51,52,61,
			52,53,61,
			53,54,61,
			54,55,61,
			55,56,61,
			56,57,61,
			57,58,61,
			58,59,61,
			59,60,61,
			60,49,61
	};

	return std::make_unique<IndexBuffer>(indices, sizeof(indices));
}