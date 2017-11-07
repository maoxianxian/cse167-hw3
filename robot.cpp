#include "robot.h"
robot::robot(glm::mat4 m,Geometry* antenna,Geometry* body,Geometry* eyeball,Geometry* head,Geometry* limb)
{
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
	leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	leftlegTorobot->translate(1.5, -6, 0.5);
	leftlegTorobot->addChild(limb);
	children.push_back(leftlegTorobot);
	
	rightlegTorobot = new Transform(glm::mat4(1.0f));
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

void robot::draw(GLuint shaderProgram, glm::mat4 m)
{
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