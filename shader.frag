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

vec3 inner = vec3(0.0f, 0.0f, 0.6f);
        vec3 middle = vec3(0.0f, 0.5f, 0.1f);
        vec3 outer = vec3(0.0f, 0.0f, 0.0f);

vec3 color_from_iter(float stop_iter)
{
    float lmax = log_star(float(max_iters), 3);
            float n_iter = (lmax - log_star(stop_iter, 3)) / lmax;

            if(n_iter < 0.5f) {
                return (inner * (1.0f - 2.0f * n_iter) + middle * 2.0f * n_iter);
            }
            else {
                return (middle * (1.0f - 2.0f * (n_iter - 0.5f)) + outer * 2.0f * (n_iter - 0.5f));
            }
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

        if(stop_iter < 10.0f) {
            stop_iter = 10.0f;

            FragColor = vec4(1.0f * outer / max(1.0, float(c_norm(c - dvec2(-0.5, 0.0)))), 1.0f);
        }
        else {

	    vec3 color0 = color_from_iter(stop_iter - 1.0);
	    vec3 color1 = color_from_iter(stop_iter);
            vec3 color2 = color_from_iter(stop_iter + 1.0);

	    float nsmooth = stop_iter + 1 - log_star(float(c_norm(val)), 2) / log(2);

	    float di = nsmooth - stop_iter;

	    if(di > 0.0)
                FragColor.xyz = (1.0 - di) * color1 + di * color2;
	    else
		FragColor.xyz = (1.0 + di) * color1 - di * color0;
        }
    }
}
