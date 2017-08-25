#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _sprite2_wrap_c_h_
#define _sprite2_wrap_c_h_

#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif // __cplusplus

enum S2_SYM_TYPE
{
	S2_SYM_INVALID = 0,
	S2_SYM_UNKNOWN,

	S2_SYM_IMAGE = 10,
	S2_SYM_SCALE9,
	S2_SYM_ICON,
	S2_SYM_TEXTURE,
	S2_SYM_TEXTBOX,
	S2_SYM_COMPLEX,
	S2_SYM_ANIMATION,
	S2_SYM_ANIM2,
	S2_SYM_PARTICLE3D,
	S2_SYM_PARTICLE2D,
	S2_SYM_SHAPE,
	S2_SYM_MESH,
	S2_SYM_MASK,
	S2_SYM_TRAIL,
	S2_SYM_SKELETON,
	S2_SYM_MODEL,

	S2_SYM_PROXY = 99,
	S2_SYM_ANCHOR = 100,
};

void s2_init();

void s2_on_size(int w, int h);
void s2_get_screen_size(int* w, int* h);

/************************************************************************/
/* symbol                                                               */
/************************************************************************/

void  s2_symbol_draw(const void* actor, float x, float y, float angle, float sx, float sy,
					 float xmin, float ymin, float xmax, float ymax);
void  s2_symbol_draw2(const void* symbol, float x, float y);

void* s2_symbol_query_child(const void* sym, int child_idx, uint32_t child_id, const char* child_name);

int   s2_symbol_get_type_id(const char* name);

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

static const uint32_t S2_DISABLE_DRAW_PARTICLE3D   = 0x00000001;

void  s2_spr_draw(const void* actor, float x, float y, float angle, float sx, float sy,
				  float xmin, float ymin, float xmax, float ymax, int flag, int min_edge);

void  s2_spr_set_pos(void* spr, float x, float y);
void  s2_spr_set_angle(void* spr, float angle);
void  s2_spr_set_scale(void* spr, float sx, float sy);

void  s2_spr_get_pos(const void* spr, float* x, float* y);
float s2_spr_get_angle(const void* spr);
void  s2_spr_get_scale(const void* spr, float* sx, float* sy);

void  s2_spr_set_col_mul(void* spr, uint32_t abgr);
void  s2_spr_set_col_add(void* spr, uint32_t abgr);

void  s2_spr_retain(void* spr);
void  s2_spr_release(void* spr);
int   s2_spr_get_id(void* spr);
int   s2_spr_get_ref_count(void* spr);
const char* s2_spr_get_name(void* spr);

int   s2_spr_get_sym_id(const void* spr);
int   s2_spr_get_sym_type(const void* spr);

void  s2_spr_draw_aabb(const void* spr, float x, float y, float angle, float sx, float sy, const float mat[6]);
bool  s2_spr_point_test(const void* spr, float x, float y);
void* s2_spr_point_query(const void* spr, float x, float y, float mat[6]);

bool  s2_spr_get_force_update(const void* spr);
void  s2_spr_set_force_update(void* spr, bool force);

//////////////////////////////////////////////////////////////////////////
// animation
//////////////////////////////////////////////////////////////////////////

int   s2_spr_get_frame_count(void* spr);
void  s2_spr_anim_set_loop(void* spr, bool loop);

//////////////////////////////////////////////////////////////////////////
// complex
//////////////////////////////////////////////////////////////////////////

bool  s2_spr_has_action(const void* spr, const char* name);

bool  s2_spr_get_scissor(const void* spr, float* xmin, float* ymin, float* xmax, float* ymax);
void  s2_spr_set_scissor(void* spr, float xmin, float ymin, float xmax, float ymax);

//////////////////////////////////////////////////////////////////////////
// textbox
//////////////////////////////////////////////////////////////////////////

void  s2_spr_textbox_reset_time(void* spr);
void  s2_spr_textbox_set_font_color(void* spr, uint32_t abgr);
void  s2_spr_textbox_set_edge_color(void* spr, uint32_t abgr);

//////////////////////////////////////////////////////////////////////////
// anim2
//////////////////////////////////////////////////////////////////////////

void  s2_spr_anim2_set_static_time(void* spr, int time);

//////////////////////////////////////////////////////////////////////////
// particle3d
//////////////////////////////////////////////////////////////////////////

void  s2_spr_p3d_set_local(void* spr, bool local);
void  s2_spr_p3d_set_loop(void* spr, bool loop);
bool  s2_spr_p3d_is_finished(const void* spr);
void  s2_spr_p3d_update(void* spr, float dt);
void  s2_spr_p3d_buffer_draw(float x, float y, float scale);
void  s2_spr_p3d_tick();
void  s2_spr_p3d_gc();

//////////////////////////////////////////////////////////////////////////
// proxy
//////////////////////////////////////////////////////////////////////////

void  s2_spr_proxy_get_children(const void* spr, void* children[], int children_cap, int* count);

//////////////////////////////////////////////////////////////////////////
// dtex
//////////////////////////////////////////////////////////////////////////

void  s2_spr_set_dtex_enable(void* spr, bool enable);
void  s2_spr_set_dtex_force_cached(void* spr, bool cache);
void  s2_spr_set_dtex_force_cached_dirty(void* spr, bool dirty);

/************************************************************************/
/* actor                                                                */
/************************************************************************/

void  s2_actor_retain(void* actor);
void  s2_actor_release(void* actor);

void  s2_actor_draw(const void* actor, float x, float y, float angle, float sx, float sy,
					float xmin, float ymin, float xmax, float ymax);
void  s2_actor_update(void* actor, bool force);

void  s2_actor_msg_start(void* actor, bool force);
void  s2_actor_msg_stop(void* actor, bool force);
void  s2_actor_msg_clear(void* actor, bool force);

void  s2_actor_set_frame(void* actor, int frame);
int   s2_actor_get_frame(void* actor);

int   s2_actor_get_component_count(void* actor);

void* s2_actor_fetch_child(const void* actor, const char* name);
void* s2_actor_fetch_child_by_index(const void* actor, int idx);

/**
 *  @return
 *    0 ok, -1 no child with name, -2 child isn't anchor.
 */
int   s2_actor_mount(const void* actor, const char* name, const void* anchor);

void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]);

void* s2_actor_create(const void* parent_actor, void* child_spr);
int   s2_actor_get_count();

void* s2_actor_get_spr(void* actor);
void  s2_actor_get_aabb(const void* actor, float aabb[4]);

void  s2_actor_set_pos(void* actor, float x, float y);
void  s2_actor_get_pos(const void* actor, float* x, float* y);
void  s2_actor_set_angle(void* actor, float angle);
float s2_actor_get_angle(const void* actor);
void  s2_actor_set_scale(void* actor, float sx, float sy);
void  s2_actor_get_scale(const void* actor, float* sx, float* sy);

void  s2_actor_get_world_pos(const void* actor, float* x, float* y);
float s2_actor_get_world_angle(const void* actor);
void  s2_actor_get_world_scale(const void* actor, float* sx, float* sy);

void* s2_actor_get_parent(void* actor);

bool  s2_actor_get_visible(void* actor);
void  s2_actor_set_visible(void* actor, bool visible);
bool  s2_actor_get_editable(void* actor);
void  s2_actor_set_editable(void* actor, bool editable);

uint32_t s2_actor_get_col_mul(void* actor);
void     s2_actor_set_col_mul(void* actor, uint32_t abgr);
uint32_t s2_actor_get_col_add(void* actor);
void     s2_actor_set_col_add(void* actor, uint32_t abgr);
void     s2_actor_get_col_map(void* actor, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap);
void     s2_actor_set_col_map(void* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap);

void  s2_actor_set_filter(void* actor, int mode);

//////////////////////////////////////////////////////////////////////////
// complex
//////////////////////////////////////////////////////////////////////////

void  s2_actor_set_action(void* spr, const char* action);

//////////////////////////////////////////////////////////////////////////
// text
//////////////////////////////////////////////////////////////////////////

const char* s2_actor_get_text(void* actor);
void  s2_actor_set_text(void* actor, const char* text);

bool  s2_actor_get_text_size(const void* actor, float* w, float* h);

//////////////////////////////////////////////////////////////////////////
// anchor
//////////////////////////////////////////////////////////////////////////

void* s2_actor_get_anchor_real_for_fetch(void* actor);
void  s2_actor_anchor_deconnect(void* actor);

//////////////////////////////////////////////////////////////////////////
// scale9
//////////////////////////////////////////////////////////////////////////

void  s2_actor_scale9_resize(void* actor, int w, int h);

/************************************************************************/
/* rt                                                                   */
/************************************************************************/

struct s2_region {
	float xmin, ymin;
	float xmax, ymax;
};

void* s2_rt_fetch();
void  s2_rt_return(void* rt);
void  s2_rt_draw_from(void* rt, const struct s2_region* dst, const struct s2_region* src, int src_tex_id);
void  s2_rt_draw_to(void* rt, const struct s2_region* dst, const struct s2_region* src);
int   s2_rt_get_texid(void* rt);

/************************************************************************/
/* camera                                                               */
/************************************************************************/

void* s2_cam_create();
void  s2_cam_release(void* cam);
void  s2_cam_bind(void* cam);
void  s2_cam_set(void* cam, float x, float y, float scale);
void  s2_cam_get(const void* cam, float* x, float* y, float* scale);
void  s2_cam_screen2project(const void* cam, int src_x, int src_y, float* dst_x, float* dst_y);

/************************************************************************/
/* rvg                                                                  */
/************************************************************************/

void s2_rvg_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void s2_rvg_set_line_width(float width);
void s2_rvg_draw_line(const float* vertices, int count);
void s2_rvg_draw_rect(bool filling, float x, float y, float w, float h);
void s2_rvg_draw_circle(bool filling, float cx, float cy, float radius, int segments);

/************************************************************************/
/* other                                                                */
/************************************************************************/

enum S2_PIXEL_TYPE
{
	RGBA = 0,
	ARGB,
	ABGR,
	BGRA
};

uint32_t s2_trans_color(uint32_t src, enum S2_PIXEL_TYPE src_type, enum S2_PIXEL_TYPE dst_type);

void s2_set_viewport(float x, float y, float w, float h);

#endif // _sprite2_wrap_c_h_

#ifdef __cplusplus
}
#endif