#version 330 core

in float hight;
in vec3 col;
in vec3 nor;
in vec3 eyeDir;
in vec4 shadowCoord;

out vec4 colour;

uniform vec3 lightDir;
uniform sampler2DShadow shadowMap;

//BIAS to reduce shadow acne
const float BIAS=0.0005;

void main() {
        //change the shadow coordinates. They have to be in [0,1] for texture samlping
        vec3 shadowChangedCoord = shadowCoord.xyz/shadowCoord.w;
        shadowChangedCoord = shadowChangedCoord*0.5 + 0.5;

        //cosinus of the light direction and the normal vector
        float cosLightNormal = max(0., dot(lightDir, nor));

        //visibility equals 0.3 if the fragment is in the shadow and 1 if not (if it is on the back of the face, it is in the shadow).
        float visibility = texture(shadowMap, vec3(shadowChangedCoord.xy, shadowChangedCoord.z-BIAS));
        visibility = visibility * clamp(cosLightNormal - 0.1, 0., 1.) * 0.7 + 0.3;

        vec3 specular = 0.5 * vec3(1., 1., 1.) * pow(clamp( dot( eyeDir, reflect(lightDir, nor)), 0., 1.), 4.);

        if (visibility <= 0.3)
        {
                specular *= 0.2;
        }

        //output colour
        colour = vec4(visibility * ( //shadow
        col * (cosLightNormal + 0.2) + //ambiant and difuse
        specular),
        1.); //no transparency
}
