#version 430 core

precision highp float;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D imgOutput;

uniform float width;
uniform float height;

void main() {

   


    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec2 coord = (texelCoord / vec2(width, height));
    coord = coord * 2.0 - 1.0;
    


    vec3 rayDirection = vec3(coord.xy, -2.0);
    vec3 rayOrigin = vec3(0.0, 0.0, -2.0);
    vec3 sphereOrigin = vec3(-1.0, 0.0, 1.0);

    vec3 origin = rayOrigin - sphereOrigin;





    //-y up, -z forward
    vec3 lightDir = normalize(vec3(-1, -2, -2));


    float radius = 0.5;
    
    float a = dot(rayDirection, rayDirection);
    float b = 2.0 * dot(origin, rayDirection);
    float c = dot(origin, origin) - radius * radius;

    float discriminant = b * b - 4.0 * a * c;

    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    
    if (discriminant >= 0.0) {

        color = vec4(1.0, 0.27, 0.0, 1.0);

        float t = (-b - sqrt(discriminant)) / (2.0 * a);

        vec3 hit = origin + rayDirection * t;

        vec3 normal = normalize(hit);

        float lightIntensity = max(dot(normal, -lightDir), 0.0);
        color = vec4(lightIntensity * color.xyz, color.w);
    }
    

    
















 
   
    imageStore(imgOutput, texelCoord, color);
}