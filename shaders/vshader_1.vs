#version 410

in vec4						vp;

// model matrices
uniform mat4				mvp_matrix;

layout(location = 1) in		vec2 uv;

// layout(location = 1) in		vec3 v_normal;
// layout(location = 2) in		vec3 v_color;
// layout(location = 4) in		vec2 uv;
// layout(location = 5) in		int transition_flag;

// out vec4					vertex_position;
out vec2					texture_coordinates;
// out vec3					vertex_normal;
// flat out vec3				color;

// flat out int				t_flag; // "flat" means not interpolated.

void main ()
{
	gl_Position = mvp_matrix * vp;

	// set final vertex pos;
	// vertex_position = gl_Position;

	// variables to send to the fragment shader.
	// vertex_normal = v_normal;
	// color = v_color;
	texture_coordinates = uv;
	// t_flag = transition_flag;
}

// #version 410
// in vec4 vp;

// // model matrices
// uniform mat4				identity_matrix;
// uniform mat4				model_recenter_translation_matrix;
// uniform mat4				model_translation_matrix;
// uniform mat4				scaling_matrix;
// uniform mat4				rotation_x_matrix;
// uniform mat4				rotation_y_matrix;
// uniform mat4				rotation_z_matrix;

// // view matrices
// uniform mat4				view_translation_matrix;
// uniform mat4				view_orientation_matrix;

// // projection matrices
// uniform mat4				perspective_projection_matrix;

// layout(location = 1) in		vec3 v_normal;
// layout(location = 2) in		vec3 v_color;
// layout(location = 4) in		vec2 uv;
// layout(location = 5) in		int transition_flag;

// out vec4					vertex_position;
// out vec3					vertex_normal;
// flat out vec3				color;
// out vec2					texture_coordinates;
// flat out int				t_flag; // "flat" means not interpolated.

// void main ()
// {
// 	// set MVP
// 	mat4	model_matrice =
// 		model_translation_matrix * 
// 		rotation_x_matrix * rotation_y_matrix * rotation_z_matrix * 
// 		scaling_matrix * model_recenter_translation_matrix * identity_matrix ;

// 	mat4	view_matrice = view_orientation_matrix * view_translation_matrix;

// 	gl_Position = perspective_projection_matrix * view_matrice * model_matrice * vp;

// 	// set final vertex pos;
// 	vertex_position = gl_Position;

// 	// variables to send to the fragment shader.
// 	vertex_normal = v_normal;
// 	color = v_color;
// 	texture_coordinates = uv;
// 	t_flag = transition_flag;
// }

/*
#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 uv;

uniform mat4 projection_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;

out vec2 texture_coordinates;
out vec3 position_eye;
out vec3 normal_eye;
out vec3 color;

void main () {
    texture_coordinates = uv;
    color = vertex_color;
    position_eye = vec3 (view_mat * model_mat * vec4(vertex_position, 1.0));
    normal_eye = vec3 (view_mat * model_mat * vec4(vertex_normal, 0.0));
    gl_Position = projection_mat * vec4(position_eye, 1.0);
}*/