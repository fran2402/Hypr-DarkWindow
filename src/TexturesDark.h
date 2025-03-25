#pragma once

#include <string>
#include <format>

#include <hyprland/src/render/shaders/Textures.hpp>

// Replace the old invert function with one that only affects black pixels.
inline static const std::string COLORIZE_BLACK_FUNC = R"glsl(
void colorizeBlack(inout vec4 color) {
    // If the color is nearly black (adjust epsilon as needed)
    if (length(color.rgb) < 0.01) {
        // Set to orange (here: RGB(1.0, 0.65, 0.0) corresponds to #FFA500)
        color.rgb = vec3(1.0, 0.65, 0.0);
    }
}
)glsl";

// Example shader: for CM textures.
inline const std::string TEXFRAGSRCCM_ORANGE = R"glsl(
precision highp float;
varying vec2 v_texcoord;
uniform sampler2D tex;

)glsl" + COLORIZE_BLACK_FUNC + R"glsl(

void main() {
    vec4 pixColor = texture2D(tex, v_texcoord);
    colorizeBlack(pixColor);
    gl_FragColor = pixColor;
})glsl";

// Similar modifications for the RGBA variant.
inline const std::string TEXFRAGSRCRGBA_ORANGE = R"glsl(
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

uniform int applyTint;
uniform vec3 tint;

)glsl" + COLORIZE_BLACK_FUNC + R"glsl(

void main() {

    vec4 pixColor = texture2D(tex, v_texcoord);

    if (discardOpaque == 1 && pixColor[3] * alpha == 1.0)
	    discard;

    if (discardAlpha == 1 && pixColor[3] <= discardAlphaValue)
        discard;

    if (applyTint == 1) {
	    pixColor[0] = pixColor[0] * tint[0];
	    pixColor[1] = pixColor[1] * tint[1];
	    pixColor[2] = pixColor[2] * tint[2];
    }

    colorizeBlack(pixColor);

    if (radius > 0.0) {
    )glsl" +
    ROUNDED_SHADER_FUNC("pixColor") + R"glsl(
    }

    gl_FragColor = pixColor * alpha;
})glsl";

// And for the RGBX variant.
inline const std::string TEXFRAGSRCRGBX_ORANGE = R"glsl(
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

uniform int applyTint;
uniform vec3 tint;

)glsl" + COLORIZE_BLACK_FUNC + R"glsl(

void main() {

    if (discardOpaque == 1 && alpha == 1.0)
	    discard;

    vec4 pixColor = vec4(texture2D(tex, v_texcoord).rgb, 1.0);

    if (applyTint == 1) {
	    pixColor[0] = pixColor[0] * tint[0];
	    pixColor[1] = pixColor[1] * tint[1];
	    pixColor[2] = pixColor[2] * tint[2];
    }

    colorizeBlack(pixColor);

    if (radius > 0.0) {
    )glsl" +
    ROUNDED_SHADER_FUNC("pixColor") + R"glsl(
    }

    gl_FragColor = pixColor * alpha;
})glsl";

// And finally for the external texture variant.
inline const std::string TEXFRAGSRCEXT_ORANGE = R"glsl(
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

uniform int applyTint;
uniform vec3 tint;

)glsl" + COLORIZE_BLACK_FUNC + R"glsl(

void main() {

    vec4 pixColor = texture2D(texture0, v_texcoord);

    if (discardOpaque == 1 && pixColor[3] * alpha == 1.0)
	    discard;

    if (applyTint == 1) {
	    pixColor[0] = pixColor[0] * tint[0];
	    pixColor[1] = pixColor[1] * tint[1];
	    pixColor[2] = pixColor[2] * tint[2];
    }

    colorizeBlack(pixColor);

    if (radius > 0.0) {
    )glsl" +
    ROUNDED_SHADER_FUNC("pixColor") + R"glsl(
    }

    gl_FragColor = pixColor * alpha;
}
)glsl";
