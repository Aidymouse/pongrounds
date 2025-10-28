#version 330 core

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;

out vec4 finalColor;

void main() {
	//vec4 result = texture(tex, pos.xy);
	finalColor = texture(texture0, fragTexCoord);
	return;
}
