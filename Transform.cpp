#include "Transform.h";
Transform::Transform(glm::mat4 m)
{
	toParent = m;
}
void Transform::draw(GLuint shaderProgram, glm::mat4 m)
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->draw(shaderProgram,m*toParent);
	}
}
void Transform::translate(float x, float y, float z)
{
	this->toParent = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Transform::rotate(glm::vec3 axis, float deg)
{
	this->toParent = glm::rotate(glm::mat4(1.0f), deg, axis)*this->toParent;
}

void Transform::scale(float x, float y, float z)
{
	this->toParent = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}
void Transform::addChild(Node* child)
{
	children.push_back(child);
}
void Transform::removeChild(Node * child)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i] == child)
		{
			children.erase(children.begin() + i);
			return;
		}
	}
}