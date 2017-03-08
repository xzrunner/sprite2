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

void s2_init();

void s2_on_size(int w, int h);

/************************************************************************/
/* symbol                                                               */
/************************************************************************/

void  s2_symbol_draw(const void* actor, float x, float y, float angle, float sx, float sy,
					 float xmin, float ymin, float xmax, float ymax);

/************************************************************************/
/* sprite                                                               */
/************************************************************************/

void  s2_spr_draw(const void* spr, float x, float y, float angle, float sx, float sy,
				  float xmin, float ymin, float xmax, float ymax);
void  s2_spr_update(void* spr);

void* s2_spr_fetch_child(const void* spr, const void* actor, const char* name);
void* s2_spr_fetch_child_by_index(const void* spr, const void* actor, int idx);
void* s2_spr_fetch_child_no_proxy(const void* spr, const char* name);

void  s2_spr_set_pos(void* spr, float x, float y);
void  s2_spr_set_angle(void* spr, float angle);
void  s2_spr_set_scale(void* spr, float sx, float sy);

void  s2_spr_get_pos(const void* spr, float* x, float* y);
void  s2_spr_get_angle(const void* spr, float* angle);
void  s2_spr_get_scale(const void* spr, float* sx, float* sy);

void  s2_spr_retain(void* spr);
void  s2_spr_release(void* spr);
int   s2_spr_get_id(void* spr);
int   s2_spr_get_ref_count(void* spr);
const char* s2_spr_get_name(void* spr);

int   s2_spr_get_sym_id(void* spr);
int   s2_spr_get_sym_type(void* spr);

bool  s2_spr_get_editable(void* spr);
void  s2_spr_set_editable(void* spr, bool editable);

bool  s2_spr_get_force_up_frame(void* spr);
void  s2_spr_set_force_up_frame(void* spr, bool force);

void  s2_spr_set_frame(void* spr, int frame);
void  s2_spr_set_action(void* spr, const char* action);

int   s2_spr_get_frame(void* spr);
int   s2_spr_get_frame_count(void* spr);
int   s2_spr_get_component_count(void* spr);

void  s2_spr_set_filter(void* spr, int mode);

const char* s2_spr_get_text(void* spr);
void  s2_spr_set_text(void* spr, const char* text);

void  s2_spr_get_aabb(const void* spr, float aabb[4]);
void  s2_spr_draw_aabb(const void* spr, float x, float y, float angle, float sx, float sy, const float mat[6]);
bool  s2_spr_point_test(const void* spr, float x, float y);
void* s2_spr_point_query(const void* spr, float x, float y, float mat[6]);

bool  s2_spr_has_action(const void* spr, const char* name);

bool  s2_spr_get_text_size(const void* spr, float* w, float* h);

bool  s2_spr_get_scissor(const void* spr, float* x, float* y, float* w, float* h);
bool  s2_spr_set_scissor(void* spr, float x, float y, float w, float h);

void  s2_spr_scale9_resize(void* spr, int w, int h);

void  s2_spr_set_dtex_enable(void* spr, bool enable);
void  s2_spr_set_dtex_force_cached(void* spr, bool cache);
void  s2_spr_set_dtex_force_cached_dirty(void* spr, bool dirty);

/************************************************************************/
/* actor                                                                */
/************************************************************************/

void  s2_actor_draw(const void* actor, float x, float y, float angle, float sx, float sy,
					float xmin, float ymin, float xmax, float ymax);

void* s2_point_query_actor(const void* parent_actor, float x, float y, float mat[6]);

void* s2_get_actor(const void* parent_actor, void* child_spr);
int   s2_get_actor_count();

void  s2_actor_set_proxy(void* actor, void* spr);

void* s2_actor_get_spr(void* actor);
void  s2_actor_set_text(void* actor, const char* text);
void  s2_actor_print_path(void* actor);

void  s2_actor_set_pos(void* actor, float x, float y);
void  s2_actor_get_pos(void* actor, float* x, float* y);
void  s2_actor_set_angle(void* actor, float angle);
float s2_actor_get_angle(void* actor);
void  s2_actor_set_scale(void* actor, float sx, float sy);
void  s2_actor_get_scale(void* actor, float* sx, float* sy);

void  s2_actor_get_world_pos(void* actor, float* x, float* y);

void* s2_actor_get_parent(void* actor);

bool  s2_actor_get_visible(void* actor);
void  s2_actor_set_visible(void* actor, bool visible);

uint32_t s2_actor_get_col_mul(void* actor);
void     s2_actor_set_col_mul(void* actor, uint32_t abgr);
uint32_t s2_actor_get_col_add(void* actor);
void     s2_actor_set_col_add(void* actor, uint32_t abgr);
void     s2_actor_get_col_map(void* actor, uint32_t* rmap, uint32_t* gmap, uint32_t* bmap);
void     s2_actor_set_col_map(void* actor, uint32_t rmap, uint32_t gmap, uint32_t bmap);

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

#endif // _sprite2_wrap_c_h_

#ifdef __cplusplus
}
#endif