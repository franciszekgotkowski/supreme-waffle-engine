#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUvPosition;
layout(location = 2) in uint aLineIdx;

// this is defined in text_rendering.h on programs side
const uint MAX_AMOUNT_OF_LINES_IN_SCENE = 1024;

uniform vec2 uOffset[MAX_AMOUNT_OF_LINES_IN_SCENE];
uniform uint uScale[MAX_AMOUNT_OF_LINES_IN_SCENE];
uniform vec4 uColor[MAX_AMOUNT_OF_LINES_IN_SCENE];
uniform bool uShouldDraw[MAX_AMOUNT_OF_LINES_IN_SCENE];

out vec4 vColor;
out vec2 vUvPosition;

void main() {
    gl_Position = vec4(aPosition.x + uOffset[aLineIdx].x, aPosition.y + uOffset[aLineIdx].y, 0.0, 1.0);
    vUvPosition = aUvPosition;
    if (uShouldDraw[aLineIdx] == true) {
        vColor = uColor[aLineIdx];
    } else {
        vColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
