#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

vec4 translateVector(vec4 original, vec3 translate);
vec4 scaleVector(vec4 original, float xs, float ys, float zs);
vec4 rotateVector(vec4 original, vec3 axis, float angle);
mat4 getView(vec3 camera, vec3 target, vec3 up);
mat4 getProjection(float fov, float ratio, float near, float far);

#endif