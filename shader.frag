#version 410 core

in vec2 pos;

out vec4 FragColor;

uniform vec2 center_z;
uniform float zoom;

uniform float max_iters;

uniform float screen_ratio; // height over width

dvec2 f(dvec2 z, dvec2 c) {
    dvec2 res;

    res.x = z.x*z.x - z.y*z.y + c.x;
    res.y = 2*z.x*z.y + c.y;

    return res;
}

float log_star(float x, int n) {
    float r = x;

    for(int i = 0; i < n; i++)
        r = log(r);

    return r;
}

double c_norm(dvec2 c) {
    return c.x*c.x + c.y*c.y;
}

void main()
{
    dvec2 c = dvec2(pos) * dvec2(double(zoom), double(zoom * screen_ratio)) + dvec2(center_z);

    dvec2 val = vec2(0.0, 0.0);

    float stop_iter = 0.0;
    bool part = true;

    for(float i = 0.0f; i < max_iters; i += 1.0f) {
        val = f(val, c);

        if(c_norm(val) > 4.0) {
            stop_iter = i;
            part = false;
            break;
        }
    }

    if(part)
	   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    else {
        FragColor.w = 1.0f;

        //float v = 1.50f;

        vec3 inner = vec3(0.0f, 0.0f, 0.6f);
        vec3 middle = vec3(0.0f, 0.5f, 0.1f);
        vec3 outer = vec3(0.4f, 0.0f, 0.0f);

        if(stop_iter < 10.0f) {
            stop_iter = 10.0f;

            FragColor = vec4(1.0f * outer / max(1.0, float(c_norm(c - dvec2(-0.5, 0.0)))), 1.0f);
        }
        else {
            float lmax = log_star(float(max_iters), 3);
            //float n_iter = tan(v * (stop_iter - max_iters * 0.5f) / (max_iters * 0.5f)) / (2.0f * tan(v)) + 0.5f;
            float n_iter = (lmax - log_star(stop_iter, 3)) / lmax;

            //float l_zoom = -log(zoom) * 0.1f;
            //float f = abs((l_zoom - floor(l_zoom)) * 2.0f - 1.0f);

            if(n_iter < 0.5f) {
                FragColor.xyz = inner  * (1.0f - 2.0f * n_iter) + middle * 2.0f * n_iter;
            }
            else {
                FragColor.xyz = middle * (1.0f - 2.0f * (n_iter - 0.5f)) + outer * 2.0f * (n_iter - 0.5f);
            }

            //FragColor.xyz = FragColor.xyz / max(1.0f, float(c_norm(val)));
        }
    }
}
