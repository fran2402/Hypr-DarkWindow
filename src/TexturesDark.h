#pragma once

#include <string>
#include <format>

#include <hyprland/src/render/shaders/Textures.hpp>

// Function to replace pure black with red.
inline static const std::string DARK_MODE_FUNC = R"glsl(
void replaceBlackToRed(inout vec4 color) {
    // Check if the color is exactly black (RGB == 0)
    // Using an exact float equality might be fragile;
    // you could add a small threshold if needed.
    if (color.r == 0.0 && color.g == 0.0 && color.b == 0.0) {
        // Replace black with red (#ff0000)
        color.rgb = vec3(1.0, 0.0, 0.0);
    }
}
)glsl";

// CM shader: tint and rounding removed
inline const std::string TEXFRAGSRCCM_DARK = R"glsl(
precision highp float;
varying vec2 v_texcoord;
uniform sampler2D tex;

)glsl" + DARK_MODE_FUNC + R"glsl(

void main() {
    vec4 pixColor = texture2D(tex, v_texcoord);
    replaceBlackToRed(pixColor);
    gl_FragColor = pixColor;
})glsl";

// RGBA shader: tint and rounding removed
inline const std::string TEXFRAGSRCRGBA_DARK = R"glsl(
precision highp float;
varying vec2 v_texcoord; // is in 0-1
uniform sampler2D tex;
uniform float alpha;

uniform vec2 topLeft;
uniform vec2 fullSize;
uniform float radius;
uniform float roundingPower;

uniform int discardOpaque;
uniform int discardAlpha;
uniform float discardAlphaValue;

)glsl" + DARK_MODE_FUNC + R"glsl(

void main() {
    vec4 pixColor = texture2D(tex, v_texcoord);

    if (discardOpaque == 1 && pixColor.a * alpha == 1.0)
	    discard;

    if (discardAlpha == 1 && pixColor.a <= discardAlphaValue)
        discard;

    replaceBlackToRed(pixColor);

    gl_FragColor = pixColor * alpha;
})glsl";

// RGBX shader: tint and rounding removed
inline const std::string TEXFRAGSRCRGBX_DARK = R"glsl(
precision highp float;
varying vec2 v_texcoord;
uniform sampler2D tex;
uniform float alpha;

uniform vec2 topLeft;
uniform vec2 fullSize;
uniform float radius;
uniform float roundingPower;

uniform int discardOpaque;
uniform int discardAlpha;
uniform int discardAlphaValue;

)glsl" + DARK_MODE_FUNC + R"glsl(

void main() {
    if (discardOpaque == 1 && alpha == 1.0)
	    discard;

    vec4 pixColor = vec4(texture2D(tex, v_texcoord).rgb, 1.0);
    replaceBlackToRed(pixColor);

    gl_FragColor = pixColor * alpha;
})glsl";

// EXT shader: tint and rounding removed
inline const std::string TEXFRAGSRCEXT_DARK = R"glsl(
#extension GL_OES_EGL_image_external : require

precision highp float;
varying vec2 v_texcoord;
uniform samplerExternalOES texture0;
uniform float alpha;

uniform vec2 topLeft;
uniform vec2 fullSize;
uniform float radius;
uniform float roundingPower;

uniform int discardOpaque;
uniform int discardAlpha;
uniform int discardAlphaValue;

)glsl" + DARK_MODE_FUNC + R"glsl(

void main() {
    vec4 pixColor = texture2D(texture0, v_texcoord);

    if (discardOpaque == 1 && pixColor.a * alpha == 1.0)
	    discard;

    replaceBlackToRed(pixColor);

    gl_FragColor = pixColor * alpha;
}
)glsl";
