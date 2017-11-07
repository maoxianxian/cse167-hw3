#include "robot.h"
#include <iostream>
robot::robot(glm::mat4 m,Geometry* antenna,Geometry* body,Geometry* eyeball,Geometry* head,Geometry* limb)
{
	parse("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\sphere.obj");
	state = true;
	count = 0;
	toParent = m*glm::scale(glm::mat4(1.0f),glm::vec3(0.5,0.5,0.5));
	this->antenna = antenna;
	this->body = body;
	this->eyeball = eyeball;
	this->head = head;
	this->limb = limb;
	antennaleftTorobot = new Transform(glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.08, 0.3)));
	antennaleftTorobot->rotate(glm::vec3(1, 0, 0), -(float)M_PI / 1.6);
	antennaleftTorobot->rotate(glm::vec3(0, 0, 1), (float)M_PI / 2);
	antennaleftTorobot->addChild(antenna);
	children.push_back(antennaleftTorobot);

	antennarightTorobot = new Transform(glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.08, 0.3)));
	antennarightTorobot->rotate(glm::vec3(1, 0, 0), -(float)M_PI / 1.6);
	antennarightTorobot->rotate(glm::vec3(0, 0, 1), (float)M_PI / 2);	antennarightTorobot->addChild(antenna);
	antennarightTorobot->translate(3, 0, 0);
	children.push_back(antennarightTorobot);
	
	headTorobot = new Transform(glm::mat4(1.0f));
	headTorobot->scale(0.1, 0.1, 0.1);
	headTorobot->rotate(glm::vec3(1, 0, 0), 3.0f*M_PI / 2);
	headTorobot->translate(1.5f, -5.5f, 0);
	headTorobot->addChild(head);
	children.push_back(headTorobot);
	
	bodyTorobot = new Transform(glm::mat4(1.0f));
	bodyTorobot->scale(0.1, 0.1, 0.1);
	bodyTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	bodyTorobot->translate(1.5f, -3, 0);
	bodyTorobot->addChild(body);
	children.push_back(bodyTorobot);
	
	eyeballTorobot = new Transform(glm::mat4(1.0f));
	eyeballTorobot->scale(0.1, 0.1, 0.1);
	eyeballTorobot->translate(1.5, -1, 2);
	eyeballTorobot->addChild(eyeball);
	children.push_back(eyeballTorobot);
	
	leftarmTorobot = new Transform(glm::mat4(1.0f));
	leftarmTorobot->scale(0.1, 0.09, 0.1);
	leftarmTorobot->rotate(glm::vec3(0, 1, 0), -M_PI / 18);
	leftarmTorobot->translate(0, -3, 0);
	leftarmTorobot->addChild(limb);
	children.push_back(leftarmTorobot);
	
	rightarmTorobot = new Transform(glm::mat4(1.0f));
	rightarmTorobot->scale(0.1, 0.09, 0.1);
	rightarmTorobot->rotate(glm::vec3(0, 0, 1), M_PI / 18);
	rightarmTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	rightarmTorobot->translate(5.1, -3, 0);
	rightarmTorobot->addChild(limb);
	children.push_back(rightarmTorobot);

	
	leftlegTorobot = new Transform(glm::mat4(1.0f));
	leftlegTorobot->scale(0.1, 0.07, 0.1);
	leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 5.0f);
	leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	leftlegTorobot->translate(1.5, -6, 0.5);
	leftlegTorobot->addChild(limb);
	children.push_back(leftlegTorobot);
	
	rightlegTorobot = new Transform(glm::mat4(1.0f));
	rightlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 5.0f);
	rightlegTorobot->scale(0.1, 0.07, 0.1);
	rightlegTorobot->translate(3.5, -6, 0.5);
	rightlegTorobot->addChild(limb);
	children.push_back(rightlegTorobot);
}

robot::~robot()
{
	delete headTorobot;
	delete bodyTorobot;
	delete eyeballTorobot;
	delete leftarmTorobot;
	delete rightarmTorobot;
	delete leftlegTorobot;
	delete rightlegTorobot;
}
void robot::parse(const char* filepath)
{
	FILE* fp;     // file pointer
	GLfloat x, y, z;  // vertex coordinates
	unsigned int i1, i2, i3; // indices
	std::string temps;
	GLfloat r, g, b;  // vertex color
	GLint c1, c2;    // characters read from file
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt
																				   //int i = 0;
	while (!feof(fp))
	{
		c1 = fgetc(fp);

		if (c1 != '#'&&c1 != 13 && c1 != 10)
		{
			c2 = fgetc(fp);

			if ((c1 == 'v') && (c2 == ' '))
			{
				int a = fscanf(fp, "%f %f %f", &x, &y, &z);
				glm::vec3 temp = glm::vec3(x, y, z);
				vertices.push_back(temp);
				if (x > xmax)
				{
					xmax = x;
				}
				if (x < xmin)
				{
					xmin = x;
				}if (y > ymax)
				{
					ymax = y;
				}if (y < ymin)
				{
					ymin = y;
				}if (z > zmax)
				{
					zmax = z;
				}if (z < zmin)
				{
					zmin = z;
				}
			}
			else
			{
				if ((c1 == 'f') && (c2 == ' '))
				{
					fscanf(fp, "%d %s %d %s %d", &i1, &temps, &i2, &temps, &i3);
					indices.push_back(i1 - 1);
					indices.push_back(i2 - 1);
					indices.push_back(i3 - 1);
				}
				else if ((c1 == 'v') && (c2 == 'n') && (fgetc(fp) == ' '))
				{
					fscanf(fp, "%f %f %f", &x, &y, &z);
					glm::vec3 temp = glm::vec3(x, y, z);
					normals.push_back(temp);
				}
			}
		}
		char buffer[128];
		fgets(buffer, 128, fp);
	}
	//center();
	fclose(fp);   // make sure you don't forget to close the file when done
}
void robot::draw(GLuint shaderProgram, glm::mat4 m)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glMultMatrixf(&((m*toParent*glm::scale(glm::mat4(1.0f), glm::vec3(100, 100, 100)))[0][0]));
	//glMultMatrixf(&(glm::mat4(1.0f)[0][0]));
	glBegin(GL_POINTS);
	std::cout << normals.size() <<" "<< vertices.size()<<std::endl;
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		GLfloat length = normals[i].x*normals[i].x + normals[i].y*normals[i].y + normals[i].z*normals[i].z;
		length = sqrt(length);
		GLfloat x = (normals[i].x / length + 1) / 2;
		GLfloat y = (normals[i].y / length + 1) / 2;
		GLfloat z = (normals[i].z / length + 1) / 2;
		glColor3f(x, y, z);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();
	glPopMatrix();

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->draw(shaderProgram, m*toParent);
	}
}

void robot::update()
{
	if (state)
	{
		leftarmTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 180.0f);
		rightarmTorobot->rotate(glm::vec3(1, 0, 0), -M_PI / 180.0f);
		leftlegTorobot->rotate(glm::vec3(1, 0, 0), -M_PI / 180.0f);
		rightlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 180.0f);

	}
	else
	{
		leftarmTorobot->rotate(glm::vec3(1, 0, 0), -M_PI / 180.0f);
		rightarmTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 180.0f);
		leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 180.0f);
		rightlegTorobot->rotate(glm::vec3(1, 0, 0), -M_PI / 180.0f);
	}
	this->toParent = this->toParent*glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.03f));
	count++;
	if (count == 90)
	{
		state = !state;
		count = 0;
	}
}