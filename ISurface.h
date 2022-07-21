#pragma once

struct vertex_t {
	vertex_t() {}
	vertex_t(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0)) {
		position = pos;
		tex_coord = coord;
	}
	void initialize(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0)) {
		position = pos;
		tex_coord = coord;
	}

	Vector2D	position;
	Vector2D	tex_coord;
};

enum FontFlags {
	None,
	Italic = 0x001,
	Underline = 0x002,
	Strikeout = 0x004,
	Symbol = 0x008,
	Antialias = 0x010,
	Gaussianblur = 0x020,
	Rotary = 0x040,
	Dropshadow = 0x080,
	Additive = 0x100,
	Outline = 0x200,
	Custom = 0x400,
	Bitmap = 0x800,
};

class ISurface // ignore this :)
{
public:
	void UnLockCursor()
	{
		typedef void(__thiscall* original_fn)(void*);
		g_Utils.GetVFunc< original_fn >(this, 66)(this);
	}

	void SetDrawingColor(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[15](this, r, g, b, a);
	}

	void play_sound(const char* sound_path) {
		using original_fn = void(__thiscall*)(ISurface*, const char*);
		return (*(original_fn**)this)[82](this, sound_path);
	}

	void SetTextColor(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[25](this, r, g, b, a);
	}

	void draw_polygon(int n, vertex_t* vertice, bool clip_vertices = true) {
		using original_fn = void(__thiscall*)(ISurface*, int, vertex_t*, bool);
		return (*(original_fn**)this)[106](this, n, vertice, clip_vertices);
	}
	void DrawFilledRectangle(int x, int y, int w, int h) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[16](this, x, y, x + w, y + h);
	}
	void SetTexture(int id) {
		using original_fn = void(__thiscall*)(ISurface*, int);
		return (*(original_fn**)this)[38](this, id);
	}
	inline void draw_textured_rectangle(int x, int y, int w, int h) {
		typedef void(__thiscall* original_fn)(void*, int, int, int, int);
		return (*(original_fn**)this)[41](this, x, y, w, h);
	}
	void set_texture_rgba(int id, const unsigned char* rgba, int wide, int tall) {
		using original_fn = void(__thiscall*)(ISurface*, int, const unsigned char*, int, int);
		return (*(original_fn**)this)[37](this, id, rgba, wide, tall);
	}
	int create_new_texture_id(bool procedural = false) {
		using original_fn = int(__thiscall*)(ISurface*, bool);
		return (*(original_fn**)this)[43](this, procedural);
	}
	void DrawRectangle(int x, int y, int w, int h) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[18](this, x, y, x + w, y + h);
	}
	void DrawLine(int x1, int y1, int x2, int y2) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[19](this, x1, y1, x2, y2);
	}
	void DrawTextFont(unsigned long font) {
		using original_fn = void(__thiscall*)(ISurface*, unsigned long);
		return (*(original_fn**)this)[23](this, font);
	}
	void DrawTextPos(int x, int y) {
		using original_fn = void(__thiscall*)(ISurface*, int, int);
		return (*(original_fn**)this)[26](this, x, y);
	}
	void DrawRenderText(const wchar_t* text, int textLen) {
		using original_fn = void(__thiscall*)(ISurface*, const wchar_t*, int, int);
		return (*(original_fn**)this)[28](this, text, textLen, 0);
	}
	unsigned long FontCreate() {
		using original_fn = unsigned int(__thiscall*)(ISurface*);
		return (*(original_fn**)this)[71](this);
	}
	void SetFontGlyph(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags) {
		using original_fn = void(__thiscall*)(ISurface*, unsigned long, const char*, int, int, int, int, int, int, int);
		return (*(original_fn**)this)[72](this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}
	Vector2D GetTextSize(unsigned long font, const wchar_t* text) {
		using original_fn = void(__thiscall*)(ISurface*, unsigned long, const wchar_t*, int&, int&);
		int wide = 0, tall = 0;
		(*(original_fn**)this)[79](this, font, text, wide, tall);
		return {static_cast<float>( wide),static_cast<float>( tall)};
	}

	void set_clip_rect(int x, int y, int w, int h) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[147](this, x, y, w, h);
	}

	void draw_filled_rect_fade(int x, int y, int w, int h, unsigned int alpha1, unsigned int alpha2, bool is_horizontal) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int, unsigned int, unsigned int, bool);
		return (*(original_fn**)this)[123](this, x, y, w, h, alpha1, alpha2, is_horizontal);
	}

	void DrawColoredText(unsigned long font, int x, int y, int red, int green, int blue, int alpha, const char* text) {
		using original_fn = void(__thiscall*)(ISurface*, unsigned long, int, int, int, int, int, int, const char*);
		return (*(original_fn**)this)[163](this, font, x, y, red, green, blue, alpha, text);
	}

	void GetScreenSize(int& width, int& height) {
		using original_fn = void(__thiscall*)(ISurface*, int&, int&);
		return (*(original_fn**)this)[44](this, std::ref(width), std::ref(height));  //width, height
	}

	void DrawTexturedPolygon(int vertex_count, vertex_t* vertex, bool clip_vertices = true) {
		using original_fn = void(__thiscall*)(ISurface*, int, vertex_t*, bool);
		return (*(original_fn**)this)[106](this, vertex_count, vertex, clip_vertices);
	}

	void DrawGradRectdCircle(int x, int y, int radius, int segments) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[103](this, x, y, radius, segments);
	}

	void DrawSetTextureFile(int texture_id, const char* texture_name, int hardware_filter, bool force_reload = 0) {
		using original_fn = void(__thiscall*)(ISurface*, int, const char*, int, bool);
		return (*(original_fn**)this)[36](this, texture_id, texture_name, hardware_filter, force_reload);
	}

	bool is_texture_id_valid(int texture_id) {
		using original_fn = bool(__thiscall*)(ISurface*, int);
		return (*(original_fn**)this)[42](this, texture_id);
	}

	void SurfaceGetCursorPos(int& x, int& y) {
		using original_fn = void(__thiscall*)(ISurface*, int&, int&);
		return (*(original_fn**)this)[100](this, std::ref(x), std::ref(y));  //x, y
	}

	void DrawTexturedRect(int x, int y, int width, int height) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[41](this, x, y, width, height);
	}
};