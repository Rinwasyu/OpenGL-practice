extern crate glu_sys;
extern crate glfw;

use glu_sys::*;
use glfw::Context;

fn main() {
	let mut glfw = glfw::init(glfw::FAIL_ON_ERRORS).unwrap();
	
	glfw.window_hint(glfw::WindowHint::ContextVersion(1, 0));
	
	let (mut window, _events) = glfw.create_window(800, 800, "Hello OpenGL 1.0", glfw::WindowMode::Windowed)
		.expect("Failed to create window");
	
	window.make_current();
	
	unsafe {
		glMatrixMode(glu_sys::GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(50.0, 1.0, 0.1, 10.0);
		glMatrixMode(glu_sys::GL_MODELVIEW);
	}
	
	let vertices: [f32; 12] = [
		0.0, 0.57735, 0.0, 1.0,
		-0.5, -0.28868, 0.0, 1.0,
		0.5, -0.28868, 0.0, 1.0
	];
	
	let color: [f32; 4] = [1.0, 1.0, 1.0, 1.0];
	let triangle_call_list;
	
	unsafe {
		triangle_call_list = glGenLists(1);
		glNewList(triangle_call_list, GL_COMPILE);
		glBegin(GL_TRIANGLES);
		glMaterialfv(GL_FRONT, GL_AMBIENT, &color[0]);
		for i in 0..3 {
			glVertex4fv(&vertices[4*i]);
		}
		glEnd();
		glEndList();
		
		glClearColor(0.0, 0.0, 0.0, 1.0);
	}
	
	while !window.should_close() {
		unsafe {
			glClear(GL_COLOR_BUFFER_BIT);
			glLoadIdentity();
			glTranslated(0.0, 0.0, -2.0);
			glRotated(glfw.get_time()*180.0/3.14, 0.0, 0.0, -1.0);
			glCallList(triangle_call_list);
		}
		window.swap_buffers();
		glfw.poll_events();
	}
}