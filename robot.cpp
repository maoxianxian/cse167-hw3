#include "robot.h"
robot::robot(glm::mat4 m,Geometry* antenna,Geometry* body,Geometry* eyeball,Geometry* head,Geometry* limb)
{
	toParent = m;
	this->antenna = antenna;
	this->body = body;
	this->eyeball = eyeball;
	this->head = head;
	this->limb = limb;
	antennaleftTorobot = new Transform(glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.3, 0.1)));
	antennaleftTorobot->rotate(glm::vec3(0, 0, 1), (float)M_PI*3.0f / 2);
	antennaleftTorobot->addChild(antenna);
	antennaleftTorobot->translate(2, -1, 0);
	children.push_back(antennaleftTorobot);

	antennarightTorobot = new Transform(glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.3, 0.1)));
	antennarightTorobot->rotate(glm::vec3(0, 0, 1), (float)M_PI*3.0f / 2);
	antennarightTorobot->addChild(antenna);
	antennarightTorobot->translate(4, -1, 0);
	children.push_back(antennarightTorobot);

	headTorobot = new Transform(glm::mat4(1.0f));
	headTorobot->scale(0.1, 0.1, 0.1);
	headTorobot->rotate(glm::vec3(1, 0, 0), 3.0f*M_PI / 2);
	headTorobot->translate(3.0f, -10.0f, 0);
	headTorobot->addChild(head);
	children.push_back(headTorobot);

	bodyTorobot = new Transform(glm::mat4(1.0f));
	bodyTorobot->scale(0.1, 0.1, 0.1);
	bodyTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	bodyTorobot->translate(3, 1.3, 0);
	bodyTorobot->addChild(body);
	children.push_back(bodyTorobot);

	eyeballTorobot = new Transform(glm::mat4(1.0f));
	eyeballTorobot->scale(0.1, 0.1, 0.1);
	eyeballTorobot->translate(3, 0, 2);
	eyeballTorobot->addChild(eyeball);
	children.push_back(eyeballTorobot);

	leftarmTorobot = new Transform(glm::mat4(1.0f));
	leftarmTorobot->scale(0.1, 0.09, 0.1);
	leftarmTorobot->translate(3.5, -3, -4);
	leftarmTorobot->addChild(limb);
	children.push_back(leftarmTorobot);

	rightarmTorobot = new Transform(glm::mat4(1.0f));
	rightarmTorobot->scale(0.1, 0.09, 0.1);
	rightarmTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	rightarmTorobot->translate(8, 0.5, 0.5);
	rightarmTorobot->addChild(limb);
	children.push_back(rightarmTorobot);

	leftlegTorobot = new Transform(glm::mat4(1.0f));
	leftlegTorobot->scale(0.1, 0.07, 0.1);
	leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	leftlegTorobot->translate(4.5, -3, 0.5);
	leftlegTorobot->addChild(limb);
	children.push_back(leftarmTorobot);

	rightlegTorobot = new Transform(glm::mat4(1.0f));
	rightlegTorobot->scale(0.1, 0.1, 0.07);
	rightlegTorobot->translate(6.5, -6.2, -2.5);
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
void robot::draw(GLuint shaderProgram, glm::mat4 m)
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->draw(shaderProgram, m);
	}
}
void update()
{

}