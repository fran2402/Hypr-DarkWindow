#pragma once

#include <string>

inline const std::string CONVERT_BLACK_TO_RED_FUNC = R"glsl(
void convertBlackToRed(inout vec4 color) {
    // If the color is pure black, change it to red.
    if (color.r == 0.0 && color.g == 0.0 && color.b == 0.0) {
        color.rgb = vec3(1.0, 0.0, 0.0);
    }
}
)glsl";

inline const std::string TEXFRAGSRCCM_RED = R"glsl(
precision highp float;
varying vec2 v_texcoord;
uniform sampler2D tex;

)glsl" + CONVERT_BLACK_TO_RED_FUNC + R"glsl(

void main() {
    vec4 pixColor = texture2D(tex, v_texcoord);
    convertBlackToRed(pixColor);
    gl_FragColor = pixColor;
}
)glsl";

inline const std::string TEXFRAGSRCRGBA_RED = R"glsl(
precision highp float;
varying vec2 v_texcoord;
uniform sampler2D tex;
uniform float alpha;

)glsl" + CONVERT_BLACK_TO_RED_FUNC + R"glsl(

void main() {
    vec4 pixColor = texture2D(tex, v_texcoord);
    convertBlackToRed(pixColor);
    gl_FragColor = pixColor * alpha;
}
)glsl";

inline const std::string TEXFRAGSRCRGBX_RED = R"glsl(
precision highp float;
varying vec2 v_texcoord;
uniform sampler2D tex;
uniform float alpha;

)glsl" + CONVERT_BLACK_TO_RED_FUNC + R"glsl(

void main() {
    // Sample the RGB channels and use a fixed alpha of 1.0.
    vec4 pixColor = vec4(texture2D(tex, v_texcoord).rgb, 1.0);
    convertBlackToRed(pixColor);
    gl_FragColor = pixColor * alpha;
}
)glsl";

inline const std::string TEXFRAGSRCEXT_RED = R"glsl(
#extension GL_OES_EGL_image_external : require
precision highp float;
varying vec2 v_texcoord;
uniform samplerExternalOES texture0;
uniform float alpha;

)glsl" + CONVERT_BLACK_TO_RED_FUNC + R"glsl(

void main() {
    vec4 pixColor = texture2D(texture0, v_texcoord);
    convertBlackToRed(pixColor);
    gl_FragColor = pixColor * alpha;
}
)glsl";
