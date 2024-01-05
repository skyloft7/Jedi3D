#version 430 core

precision highp float;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;

uniform float width;
uniform float height;

struct Sphere {
    vec3 pos;
    vec4 color;
    float radius;
};

void main() {

   


    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec2 coord = (texelCoord / vec2(width, height));
    coord = coord * 2.0 - 1.0;
    


    //Here's the problem. Even if an object is behind another, it might get drawn on top of it just because of the ordering in the array.
    //We need to sort the hits by closeness to camera and pick the smallest
    Sphere[] spheres = Sphere[2](
        Sphere(vec3(-1.0, 0.0, 1.0), vec4(1.0, 0.27, 0.0, 1.0), 0.5),
        Sphere(vec3(-1.0, 0.0, 0.5), vec4(0.2, 0.57, 1.0, 1.0), 0.9)

    );





    vec3 rayDirection = vec3(coord.xy, -2.0);
    vec3 lightDir = normalize(vec3(-1, -2, -2));
    vec3 rayOrigin = vec3(0.0, 0.0, -2.0);


    vec4 color;

    for (int i = 0; i < 2; i++) {
        Sphere sphere = spheres[i];

        vec3 origin = rayOrigin - sphere.pos;



        float a = dot(rayDirection, rayDirection);
        float b = 2.0 * dot(origin, rayDirection);
        float c = dot(origin, origin) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4.0 * a * c;

        color = vec4(1.0, 1.0, 1.0, 1.0);

        if (discriminant >= 0.0) {

            color = sphere.color;

            float t = (-b - sqrt(discriminant)) / (2.0 * a);

            vec3 hit = origin + rayDirection * t;

            vec3 normal = normalize(hit);

            float lightIntensity = max(dot(normal, -lightDir), 0.0);
            color = vec4(lightIntensity * color.xyz, color.w);



        }

        imageStore(imgOutput, texelCoord, color);

        
        if(discriminant >= 0.0)
            return;


    }

}