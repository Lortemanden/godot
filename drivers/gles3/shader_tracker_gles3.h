/**************************************************************************/
/*  shader_tracker_gles3.h                                            */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef SHADER_TRACKER_GLES3_H
#define SHADER_TRACKER_GLES3_H

#include "core/hash_map.h"
#include "core/list.h"
#include "servers/visual_server.h"
#include "shader_compiler_gles3.h"

class String;
class FileAccess;

class ShaderTrackerGLES3 {
	HashMap<String, List<ShaderCompilerGLES3::IdentifierActions>> used_spatial_shaders;
	HashMap<String, List<ShaderCompilerGLES3::IdentifierActions>> used_canvas_shaders;

	FileAccess *fa_spatial;
	FileAccess *fa_canvas;

public:
	ShaderTrackerGLES3();
	void add_shader(VS::ShaderMode mode, const String &shader_path, ShaderCompilerGLES3::IdentifierActions *actions);
	~ShaderTrackerGLES3();

private:
	void _add_spatial_shader(const String &shader_path, ShaderCompilerGLES3::IdentifierActions *actions);

	Vector<Vector<String>> _actions_to_strings(ShaderCompilerGLES3::IdentifierActions *actions);
};

#endif // SHADER_TRACKER_GLES3_H
