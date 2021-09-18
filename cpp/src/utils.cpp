#include <utils/utils.h> 

GLfloat mapScreenValuesFromNormalToGPU(GLfloat number, GLfloat inMax) {
	return ((number * 2) / inMax) - 1;
}