#include "shader_tracker_gles3.h"
#include "core/os/dir_access.h"
#include "core/os/file_access.h"
#include "core/os/os.h"
#include "core/ustring.h"

const String SPATIAL_LIST_FILE_NAME = "spatial.shaderlist";
const String CANVAS_LIST_FILE_NAME = "canvas.shaderlist";

ShaderTrackerGLES3::ShaderTrackerGLES3() {
	used_spatial_shaders = HashMap<String, List<ShaderCompilerGLES3::IdentifierActions>>();
	used_canvas_shaders = HashMap<String, List<ShaderCompilerGLES3::IdentifierActions>>();

	//String output_folder = OS::get_singleton()->get_cache_path().plus_file(OS::get_singleton()->get_godot_dir_name()).plus_file("shaders");
	String output_folder = OS::get_singleton()->get_executable_path().get_base_dir();

	fa_spatial = FileAccess::open(output_folder.plus_file(SPATIAL_LIST_FILE_NAME), FileAccess::WRITE);
	ERR_FAIL_COND_MSG(!fa_spatial, "Cannot open file '" + output_folder.plus_file(SPATIAL_LIST_FILE_NAME) + "'. Check user write permissions.");

	fa_canvas = FileAccess::open(output_folder.plus_file(CANVAS_LIST_FILE_NAME), FileAccess::WRITE);
	ERR_FAIL_COND_MSG(!fa_canvas, "Cannot open file '" + output_folder.plus_file(CANVAS_LIST_FILE_NAME) + "'. Check user write permissions.");
}

ShaderTrackerGLES3::~ShaderTrackerGLES3() {
	memdelete(fa_spatial);
	memdelete(fa_canvas);
}

void ShaderTrackerGLES3::add_shader(VS::ShaderMode mode, const String &shader_path, ShaderCompilerGLES3::IdentifierActions *actions) {
	if (mode == VS::ShaderMode::SHADER_SPATIAL) {
		print_line("shader tracker add spatial");

		if (used_spatial_shaders.has(shader_path)) {
			List<ShaderCompilerGLES3::IdentifierActions> permutations = used_spatial_shaders.get(shader_path);

			for (List<ShaderCompilerGLES3::IdentifierActions>::Element *E = permutations.front(); E; E = E->next()) {
				//TODO: it might make more sense to generate a hash from the serialized string once and then to simply compare that. could be faster in the long turn
				/* TODO: add comparison operator to the struct
				if (E->get() == actions) {
					//the shader was already in the list, we are done
					return;
				}
				*/
			}
		}
		_add_spatial_shader(shader_path, actions);
	}
};

void ShaderTrackerGLES3::_add_spatial_shader(const String &shader_path, ShaderCompilerGLES3::IdentifierActions *actions) {
	List<ShaderCompilerGLES3::IdentifierActions> actions_list;
	if (!used_spatial_shaders.has(shader_path)) {
		actions_list = List<ShaderCompilerGLES3::IdentifierActions>();
		used_spatial_shaders.set(shader_path, actions_list);
	} else {
		actions_list = used_spatial_shaders.get(shader_path);
	}

	// again, consider using a hash
	actions_list.push_back(*actions);

	// do this earlier
	Vector<Vector<String>> serialized = _actions_to_strings(actions);

	//fa_spatial->store_csv_line(shader_path);
	fa_spatial->store_string(shader_path + "\n");
	for (unsigned int i = 0; i < serialized.size(); i++) {
		fa_spatial->store_csv_line(serialized[i], ";");
	}
}

Vector<Vector<String>> ShaderTrackerGLES3::_actions_to_strings(ShaderCompilerGLES3::IdentifierActions *actions) {
	Vector<Vector<String>> result;

	//Map<StringName, Pair<int *, int>> render_mode_values;
	Vector<String> render_mode_values;
	for (Map<StringName, Pair<int *, int>>::Element *e = actions->render_mode_values.front(); e; e = e->next()) {
		String tmp = vformat("%s=%d,%d", e->key(), *e->value().first, e->value().second);
		render_mode_values.push_back(tmp);
	}
	result.push_back(render_mode_values);

	//Map<StringName, bool *> render_mode_flags;
	Vector<String> render_mode_flags;
	for (Map<StringName, bool *>::Element *e = actions->render_mode_flags.front(); e; e = e->next()) {
		String tmp = vformat("%s=%d", e->key(), (int)*e->value());
		render_mode_flags.push_back(tmp);
	}
	result.push_back(render_mode_flags);

	//Map<StringName, bool *> usage_flag_pointers;
	Vector<String> usage_flag_pointers;
	for (Map<StringName, bool *>::Element *e = actions->usage_flag_pointers.front(); e; e = e->next()) {
		String tmp = vformat("%s=%d", e->key(), (int)*e->value());
		usage_flag_pointers.push_back(tmp);
	}
	result.push_back(usage_flag_pointers);

	//Map<StringName, bool *> write_flag_pointers;
	Vector<String> write_flag_pointers;
	for (Map<StringName, bool *>::Element *e = actions->write_flag_pointers.front(); e; e = e->next()) {
		String tmp = vformat("%s=%d", e->key(), (int)*e->value());
		write_flag_pointers.push_back(tmp);
	}
	result.push_back(write_flag_pointers);

	//Map<StringName, ShaderLanguage::ShaderNode::Uniform> *uniforms;
	Vector<String> uniforms;
	for (Map<StringName, ShaderLanguage::ShaderNode::Uniform>::Element *e = actions->uniforms->front(); e; e = e->next()) {
		//TODO: do something custom here
	}
	result.push_back(uniforms);

	return result;
}