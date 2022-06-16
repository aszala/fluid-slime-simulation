#version 400 core

out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;
  
uniform sampler2D heatmap;

uniform float heatmap_resolution_x;
uniform float heatmap_resolution_y;
uniform float blur_radius = 5;
uniform int blur_axis = 0;

void main() {
    // FragColor = texture(heatmap, TexCoord);

    float x, y, rr = blur_radius*blur_radius, d, w, w0;
    
    vec2 p = TexCoord;
    vec4 col = vec4(0.0, 0.0, 0.0, 0.0);

    w0 = 0.5135 / pow(blur_radius, 0.96);

    // if (blur_axis==0) {
    for (d=1.0/heatmap_resolution_x, x=-blur_radius, p.x+=x*d; x<=blur_radius; x++, p.x+=d) {
        w = w0 * exp((-x*x) / (2.0 * rr));
        col += texture2D(heatmap, p) * w;
    }
    // }
    // if (blur_axis==1) for (d=1.0/ys,y=-r,p.y+=y*d;y<=r;y++,p.y+=d){ w=w0*exp((-y*y)/(2.0*rr)); col+=texture2D(txr,p)*w; }
    
    FragColor = col;
}